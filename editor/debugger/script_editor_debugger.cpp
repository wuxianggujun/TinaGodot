/**************************************************************************/
/*  script_editor_debugger.cpp                                            */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "script_editor_debugger.h"

#include "core/debugger/debugger_marshalls.h"
#include "core/debugger/remote_debugger.h"
#include "core/string/ustring.h"
#include "core/version.h"
#include "editor/debugger/editor_debugger_plugin.h"
#include "editor/debugger/editor_expression_evaluator.h"
#include "editor/debugger/editor_performance_profiler.h"
#include "editor/debugger/editor_profiler.h"
#include "editor/debugger/editor_visual_profiler.h"
#include "editor/docks/filesystem_dock.h"
#include "editor/docks/inspector_dock.h"
#include "editor/editor_log.h"
#include "editor/editor_node.h"
#include "editor/editor_string_names.h"
#include "editor/file_system/editor_file_system.h"
#include "editor/gui/editor_file_dialog.h"
#include "editor/gui/editor_toaster.h"
#include "editor/inspector/editor_property_name_processor.h"

				}

				if (is_breaked() && can_request_idle_draw) {
					_put_msg("servers:draw", Array());
					can_request_idle_draw = false;
				}
			}

			const uint64_t until = OS::get_singleton()->get_ticks_msec() + 20;

			while (peer.is_valid() && peer->has_message()) {
				Array arr = peer->get_message();
				if (arr.size() != 3 || arr[0].get_type() != Variant::STRING || arr[1].get_type() != Variant::INT || arr[2].get_type() != Variant::ARRAY) {
					_stop_and_notify();
					ERR_FAIL_MSG("Invalid message format received from peer");
				}

				_parse_message(arr[0], arr[1], arr[2]);

				if (OS::get_singleton()->get_ticks_msec() > until) {
					break;
				}
			}
			if (!is_session_active()) {
				_stop_and_notify();
				break;
			};
		} break;
	}
}

void ScriptEditorDebugger::_clear_execution() {
	TreeItem *ti = stack_dump->get_selected();
	if (!ti) {
		return;
	}

	Dictionary d = ti->get_metadata(0);

	stack_script = ResourceLoader::load(d["file"]);
	emit_signal(SNAME("clear_execution"), stack_script);
	stack_script.unref();
	stack_dump->clear();
	inspector->clear_stack_variables();
}

void ScriptEditorDebugger::_set_breakpoint(const String &p_file, const int &p_line, const bool &p_enabled) {
	Ref<Script> scr = ResourceLoader::load(p_file);
	emit_signal(SNAME("set_breakpoint"), scr, p_line - 1, p_enabled);
	scr.unref();
}

void ScriptEditorDebugger::_clear_breakpoints() {
	emit_signal(SNAME("clear_breakpoints"));
}

void ScriptEditorDebugger::_breakpoint_tree_clicked() {
	TreeItem *selected = breakpoints_tree->get_selected();
	if (selected->has_meta("line")) {
		emit_signal(SNAME("breakpoint_selected"), selected->get_parent()->get_text(0), int(selected->get_meta("line")));
	}
}

String ScriptEditorDebugger::_format_frame_text(const ScriptLanguage::StackInfo *info) {
	String text = info->file.get_file() + ":" + itos(info->line) + " @ " + info->func;
	if (!text.ends_with(")")) {
		text += "()";
	}
	return text;
}

void ScriptEditorDebugger::start(Ref<RemoteDebuggerPeer> p_peer) {
	_clear_errors_list();
	stop();

	profiler->set_enabled(true, true);
	visual_profiler->set_enabled(true);

	peer = p_peer;
	ERR_FAIL_COND(p_peer.is_null());

	performance_profiler->reset();

	set_process(true);
	camera_override = CameraOverride::OVERRIDE_NONE;

	_set_reason_text(TTR("Debug session started."), MESSAGE_SUCCESS);
	_update_buttons_state();

	Array quit_keys = DebuggerMarshalls::serialize_key_shortcut(ED_GET_SHORTCUT("editor/stop_running_project"));
	_put_msg("scene:setup_scene", quit_keys);

	if (EditorSettings::get_singleton()->get_project_metadata("debug_options", "autostart_profiler", false)) {
		profiler->set_profiling(true);
	}

	if (EditorSettings::get_singleton()->get_project_metadata("debug_options", "autostart_visual_profiler", false)) {
		visual_profiler->set_profiling(true);
	}
}

void ScriptEditorDebugger::_update_buttons_state() {
	const bool active = is_session_active();
	const bool has_editor_tree = active && editor_remote_tree && editor_remote_tree->get_selected();
	vmem_refresh->set_disabled(!active);
	step->set_disabled(!active || !is_breaked() || !is_debuggable());
	next->set_disabled(!active || !is_breaked() || !is_debuggable());
	copy->set_disabled(!active || !is_breaked());
	docontinue->set_disabled(!active || !is_breaked());
	dobreak->set_disabled(!active || is_breaked());
	le_clear->set_disabled(!active);
	le_set->set_disabled(!has_editor_tree);

	thread_list_updating = true;
	LocalVector<ThreadDebugged *> threadss;
	for (KeyValue<uint64_t, ThreadDebugged> &I : threads_debugged) {
		threadss.push_back(&I.value);
	}
	threads->set_disabled(threadss.is_empty());

	threadss.sort_custom<ThreadSort>();
	threads->clear();
	int32_t selected_index = -1;
	for (uint32_t i = 0; i < threadss.size(); i++) {
		if (debugging_thread_id == threadss[i]->thread_id) {
			selected_index = i;
		}
		threads->add_item(threadss[i]->name);
		threads->set_item_metadata(threads->get_item_count() - 1, threadss[i]->thread_id);
	}
	if (selected_index != -1) {
		threads->select(selected_index);
	}

	thread_list_updating = false;
}

void ScriptEditorDebugger::_stop_and_notify() {
	stop();
	emit_signal(SNAME("stopped"));
	_set_reason_text(TTR("Debug session closed."), MESSAGE_WARNING);
}

void ScriptEditorDebugger::stop() {
	set_process(false);
	threads_debugged.clear();
	debugging_thread_id = Thread::UNASSIGNED_ID;
	remote_pid = 0;
	_clear_execution();

	inspector->clear_cache();

	if (peer.is_valid()) {
		peer->close();
		peer.unref();
		reason->set_text("");
		reason->set_tooltip_text("");
		reason->set_custom_minimum_size(Size2(0, 0));
	}

	node_path_cache.clear();
	res_path_cache.clear();
	profiler_signature.clear();

	profiler->set_enabled(false, false);
	profiler->set_profiling(false);

	visual_profiler->set_enabled(false);
	visual_profiler->set_profiling(false);

	inspector->edit(nullptr);
	_update_buttons_state();
}

void ScriptEditorDebugger::_profiler_activate(bool p_enable, int p_type) {
	Array msg_data = { p_enable };
	switch (p_type) {
		case PROFILER_VISUAL:
			_put_msg("profiler:visual", msg_data);
			break;
		case PROFILER_SCRIPTS_SERVERS:
			if (p_enable) {
				// Clear old script signatures. (should we move all this into the profiler?)
				profiler_signature.clear();
				// Add max funcs options to request.
				int max_funcs = EDITOR_GET("debugger/profiler_frame_max_functions");
				bool include_native = EDITOR_GET("debugger/profile_native_calls");
				Array opts = { CLAMP(max_funcs, 16, 512), include_native };
				msg_data.push_back(opts);
			}
			_put_msg("profiler:servers", msg_data);
			break;
		default:
			ERR_FAIL_MSG("Invalid profiler type");
	}
}

void ScriptEditorDebugger::_profiler_seeked() {
	if (is_breaked()) {
		return;
	}
	debug_break();
}

void ScriptEditorDebugger::_stack_dump_frame_selected() {
	emit_signal(SNAME("stack_frame_selected"));

	int frame = get_stack_script_frame();

	if (!request_stack_dump(frame)) {
		inspector->edit(nullptr);
	}
}

void ScriptEditorDebugger::_export_csv() {
	file_dialog->set_file_mode(EditorFileDialog::FILE_MODE_SAVE_FILE);
	file_dialog->set_access(EditorFileDialog::ACCESS_FILESYSTEM);
	file_dialog_purpose = SAVE_MONITORS_CSV;
	file_dialog->popup_file_dialog();
}

String ScriptEditorDebugger::get_var_value(const String &p_var) const {
	if (!is_breaked()) {
		return String();
	}
	return inspector->get_stack_variable(p_var);
}

void ScriptEditorDebugger::_resources_reimported(const PackedStringArray &p_resources) {
	Array msg = { p_resources };
	_put_msg("scene:reload_cached_files", msg);
}

int ScriptEditorDebugger::_get_node_path_cache(const NodePath &p_path) {
	const int *r = node_path_cache.getptr(p_path);
	if (r) {
		return *r;
	}

	last_path_id++;

	node_path_cache[p_path] = last_path_id;
	Array msg = { p_path, last_path_id };
	_put_msg("scene:live_node_path", msg);

	return last_path_id;
}

int ScriptEditorDebugger::_get_res_path_cache(const String &p_path) {
	HashMap<String, int>::Iterator E = res_path_cache.find(p_path);

	if (E) {
		return E->value;
	}

	last_path_id++;

	res_path_cache[p_path] = last_path_id;
	Array msg = { p_path, last_path_id };
	_put_msg("scene:live_res_path", msg);

	return last_path_id;
}

void ScriptEditorDebugger::_method_changed(Object *p_base, const StringName &p_name, const Variant **p_args, int p_argcount) {
	if (!p_base || !live_debug || !is_session_active() || !EditorNode::get_singleton()->get_edited_scene()) {
		return;
	}

	Node *node = Object::cast_to<Node>(p_base);

	for (int i = 0; i < p_argcount; i++) {
		//no pointers, sorry
		if (p_args[i]->get_type() == Variant::OBJECT || p_args[i]->get_type() == Variant::RID) {
			return;
		}
	}

	if (node) {
		NodePath path = EditorNode::get_singleton()->get_edited_scene()->get_path_to(node);
		int pathid = _get_node_path_cache(path);

		Array msg = { pathid, p_name };
		for (int i = 0; i < p_argcount; i++) {
			//no pointers, sorry
			msg.push_back(*p_args[i]);
		}
		_put_msg("scene:live_node_call", msg);

		return;
	}

	Resource *res = Object::cast_to<Resource>(p_base);

	if (res && !res->get_path().is_empty()) {
		String respath = res->get_path();
		int pathid = _get_res_path_cache(respath);

		Array msg = { pathid, p_name };
		for (int i = 0; i < p_argcount; i++) {
			//no pointers, sorry
			msg.push_back(*p_args[i]);
		}
		_put_msg("scene:live_res_call", msg);

		return;
	}
}

void ScriptEditorDebugger::_property_changed(Object *p_base, const StringName &p_property, const Variant &p_value) {
	if (!p_base || !live_debug || !EditorNode::get_singleton()->get_edited_scene()) {
		return;
	}

	Node *node = Object::cast_to<Node>(p_base);

	if (node) {
		NodePath path = EditorNode::get_singleton()->get_edited_scene()->get_path_to(node);
		int pathid = _get_node_path_cache(path);

		if (p_value.is_ref_counted()) {
			Ref<Resource> res = p_value;
			if (res.is_valid() && !res->get_path().is_empty()) {
				Array msg = { pathid, p_property, res->get_path() };
				_put_msg("scene:live_node_prop_res", msg);
			}
		} else {
			Array msg = { pathid, p_property, p_value };
			_put_msg("scene:live_node_prop", msg);
		}

		return;
	}

	Resource *res = Object::cast_to<Resource>(p_base);

	if (res && !res->get_path().is_empty()) {
		String respath = res->get_path();
		int pathid = _get_res_path_cache(respath);

		if (p_value.is_ref_counted()) {
			Ref<Resource> res2 = p_value;
			if (res2.is_valid() && !res2->get_path().is_empty()) {
				Array msg = { pathid, p_property, res2->get_path() };
				_put_msg("scene:live_res_prop_res", msg);
			}
		} else {
			Array msg = { pathid, p_property, p_value };
			_put_msg("scene:live_res_prop", msg);
		}

		return;
	}
}

bool ScriptEditorDebugger::is_move_to_foreground() const {
	return move_to_foreground;
}

void ScriptEditorDebugger::set_move_to_foreground(const bool &p_move_to_foreground) {
	move_to_foreground = p_move_to_foreground;
}

String ScriptEditorDebugger::get_stack_script_file() const {
	TreeItem *ti = stack_dump->get_selected();
	if (!ti) {
		return "";
	}
	Dictionary d = ti->get_metadata(0);
	return d["file"];
}

int ScriptEditorDebugger::get_stack_script_line() const {
	TreeItem *ti = stack_dump->get_selected();
	if (!ti) {
		return -1;
	}
	Dictionary d = ti->get_metadata(0);
	return d["line"];
}

int ScriptEditorDebugger::get_stack_script_frame() const {
	TreeItem *ti = stack_dump->get_selected();
	if (!ti) {
		return -1;
	}
	Dictionary d = ti->get_metadata(0);
	return d["frame"];
}

bool ScriptEditorDebugger::request_stack_dump(const int &p_frame) {
	ERR_FAIL_COND_V(!is_session_active() || p_frame < 0, false);

	Array msg = { p_frame };
	_put_msg("get_stack_frame_vars", msg, debugging_thread_id);
	return true;
}

void ScriptEditorDebugger::set_live_debugging(bool p_enable) {
	live_debug = p_enable;
}

void ScriptEditorDebugger::_live_edit_set() {
	if (!is_session_active() || !editor_remote_tree) {
		return;
	}

	TreeItem *ti = editor_remote_tree->get_selected();
	if (!ti) {
		return;
	}

	String path;

	while (ti) {
		String lp = ti->get_text(0);
		path = "/" + lp + path;
		ti = ti->get_parent();
	}

	NodePath np = path;

	EditorNode::get_editor_data().set_edited_scene_live_edit_root(np);

	update_live_edit_root();
}

void ScriptEditorDebugger::_live_edit_clear() {
	NodePath np = NodePath("/root");
	EditorNode::get_editor_data().set_edited_scene_live_edit_root(np);

	update_live_edit_root();
}

void ScriptEditorDebugger::update_live_edit_root() {
	NodePath np = EditorNode::get_editor_data().get_edited_scene_live_edit_root();

	Array msg = { np };
	if (EditorNode::get_singleton()->get_edited_scene()) {
		msg.push_back(EditorNode::get_singleton()->get_edited_scene()->get_scene_file_path());
	} else {
		msg.push_back("");
	}
	_put_msg("scene:live_set_root", msg);
	live_edit_root->set_text(String(np));
}

void ScriptEditorDebugger::live_debug_create_node(const NodePath &p_parent, const String &p_type, const String &p_name) {
	if (live_debug) {
		Array msg = { p_parent, p_type, p_name };
		_put_msg("scene:live_create_node", msg);
	}
}

void ScriptEditorDebugger::live_debug_instantiate_node(const NodePath &p_parent, const String &p_path, const String &p_name) {
	if (live_debug) {
		Array msg = { p_parent, p_path, p_name };
		_put_msg("scene:live_instantiate_node", msg);
	}
}

void ScriptEditorDebugger::live_debug_remove_node(const NodePath &p_at) {
	if (live_debug) {
		Array msg = { p_at };
		_put_msg("scene:live_remove_node", msg);
	}
}

void ScriptEditorDebugger::live_debug_remove_and_keep_node(const NodePath &p_at, ObjectID p_keep_id) {
	if (live_debug) {
		Array msg = { p_at, p_keep_id };
		_put_msg("scene:live_remove_and_keep_node", msg);
	}
}

void ScriptEditorDebugger::live_debug_restore_node(ObjectID p_id, const NodePath &p_at, int p_at_pos) {
	if (live_debug) {
		Array msg = { p_id, p_at, p_at_pos };
		_put_msg("scene:live_restore_node", msg);
	}
}

void ScriptEditorDebugger::live_debug_duplicate_node(const NodePath &p_at, const String &p_new_name) {
	if (live_debug) {
		Array msg = { p_at, p_new_name };
		_put_msg("scene:live_duplicate_node", msg);
	}
}

void ScriptEditorDebugger::live_debug_reparent_node(const NodePath &p_at, const NodePath &p_new_place, const String &p_new_name, int p_at_pos) {
	if (live_debug) {
		Array msg = { p_at, p_new_place, p_new_name, p_at_pos };
		_put_msg("scene:live_reparent_node", msg);
	}
}

bool ScriptEditorDebugger::get_debug_mute_audio() const {
	return debug_mute_audio;
}

void ScriptEditorDebugger::_send_debug_mute_audio_msg(bool p_mute) {
	Array msg = { p_mute };
	_put_msg("scene:debug_mute_audio", msg);
}

void ScriptEditorDebugger::set_debug_mute_audio(bool p_mute) {
	// Send the message if we want to mute the audio or if it isn't muted already due to a break.
	if (p_mute || !audio_muted_on_break) {
		_send_debug_mute_audio_msg(p_mute);
	}
	debug_mute_audio = p_mute;
}

void ScriptEditorDebugger::_mute_audio_on_break(bool p_mute) {
	// Send the message if we want to mute the audio on a break or if it isn't muted already.
	if (p_mute || !debug_mute_audio) {
		_send_debug_mute_audio_msg(p_mute);
	}
	audio_muted_on_break = p_mute;
}

CameraOverride ScriptEditorDebugger::get_camera_override() const {
	return camera_override;
}

void ScriptEditorDebugger::set_camera_override(CameraOverride p_override) {
	Array msg = {
		p_override != CameraOverride::OVERRIDE_NONE,
		p_override == CameraOverride::OVERRIDE_EDITORS
	};
	_put_msg("scene:override_cameras", msg);

	camera_override = p_override;
}

void ScriptEditorDebugger::set_breakpoint(const String &p_path, int p_line, bool p_enabled) {
	Array msg = { p_path, p_line, p_enabled };
	_put_msg("breakpoint", msg, debugging_thread_id != Thread::UNASSIGNED_ID ? debugging_thread_id : Thread::MAIN_ID);

	TreeItem *path_item = breakpoints_tree->search_item_text(p_path);
	if (path_item == nullptr) {
		if (!p_enabled) {
			return;
		}
		path_item = breakpoints_tree->create_item();
		path_item->set_text(0, p_path);
	}

	int idx = 0;
	TreeItem *breakpoint_item;
	for (breakpoint_item = path_item->get_first_child(); breakpoint_item; breakpoint_item = breakpoint_item->get_next()) {
		if ((int)breakpoint_item->get_meta("line") < p_line) {
			idx++;
			continue;
		}

		if ((int)breakpoint_item->get_meta("line") == p_line) {
			break;
		}
	}

	if (breakpoint_item == nullptr) {
		if (!p_enabled) {
			return;
		}
		breakpoint_item = breakpoints_tree->create_item(path_item, idx);
		breakpoint_item->set_meta("line", p_line);
		breakpoint_item->set_text(0, vformat(TTR("Line %d"), p_line));
		return;
	}

	if (!p_enabled) {
		path_item->remove_child(breakpoint_item);
		if (path_item->get_first_child() == nullptr) {
			breakpoints_tree->get_root()->remove_child(path_item);
		}
	}
}

void ScriptEditorDebugger::reload_all_scripts() {
	_put_msg("reload_all_scripts", Array(), debugging_thread_id != Thread::UNASSIGNED_ID ? debugging_thread_id : Thread::MAIN_ID);
}

void ScriptEditorDebugger::reload_scripts(const Vector<String> &p_script_paths) {
	_put_msg("reload_scripts", Variant(p_script_paths).operator Array(), debugging_thread_id != Thread::UNASSIGNED_ID ? debugging_thread_id : Thread::MAIN_ID);
}

bool ScriptEditorDebugger::is_skip_breakpoints() const {
	return skip_breakpoints_value;
}

bool ScriptEditorDebugger::is_ignore_error_breaks() const {
	return ignore_error_breaks_value;
}

void ScriptEditorDebugger::_error_activated() {
	TreeItem *selected = error_tree->get_selected();

	if (!selected) {
		return;
	}

	TreeItem *ci = selected->get_first_child();
	if (ci) {
		selected->set_collapsed(!selected->is_collapsed());
	}
}

void ScriptEditorDebugger::_error_selected() {
	TreeItem *selected = error_tree->get_selected();

	if (!selected) {
		return;
	}

	Array meta = selected->get_metadata(0);
	if (meta.is_empty()) {
		return;
	}

	emit_signal(SNAME("error_selected"), String(meta[0]), int(meta[1]));
}

void ScriptEditorDebugger::_expand_errors_list() {
	TreeItem *root = error_tree->get_root();
	if (!root) {
		return;
	}

	TreeItem *item = root->get_first_child();
	while (item) {
		item->set_collapsed(false);
		item = item->get_next();
	}
}

void ScriptEditorDebugger::_collapse_errors_list() {
	TreeItem *root = error_tree->get_root();
	if (!root) {
		return;
	}

	TreeItem *item = root->get_first_child();
	while (item) {
		item->set_collapsed(true);
		item = item->get_next();
	}
}

void ScriptEditorDebugger::_vmem_item_activated() {
	TreeItem *selected = vmem_tree->get_selected();
	if (!selected) {
		return;
	}
	const String path = selected->get_text(0);
	if (path.is_empty() || !FileAccess::exists(path)) {
		return;
	}
	FileSystemDock::get_singleton()->navigate_to_path(path);
}

void ScriptEditorDebugger::_clear_errors_list() {
	error_tree->clear();
	error_count = 0;
	warning_count = 0;
	emit_signal(SNAME("errors_cleared"));
	update_tabs();

	expand_all_button->set_disabled(true);
	collapse_all_button->set_disabled(true);
	clear_button->set_disabled(true);
}

void ScriptEditorDebugger::_breakpoints_item_rmb_selected(const Vector2 &p_pos, MouseButton p_button) {
	if (p_button != MouseButton::RIGHT) {
		return;
	}

	breakpoints_menu->clear();
	breakpoints_menu->set_size(Size2(1, 1));

	const TreeItem *selected = breakpoints_tree->get_selected();
	String file = selected->get_text(0);
	if (selected->has_meta("line")) {
		breakpoints_menu->add_icon_item(get_editor_theme_icon(SNAME("Remove")), TTR("Delete Breakpoint"), ACTION_DELETE_BREAKPOINT);
		file = selected->get_parent()->get_text(0);
	}
	breakpoints_menu->add_icon_item(get_editor_theme_icon(SNAME("Remove")), TTR("Delete All Breakpoints in:") + " " + file, ACTION_DELETE_BREAKPOINTS_IN_FILE);
	breakpoints_menu->add_icon_item(get_editor_theme_icon(SNAME("Remove")), TTR("Delete All Breakpoints"), ACTION_DELETE_ALL_BREAKPOINTS);

	breakpoints_menu->set_position(get_screen_position() + get_local_mouse_position());
	breakpoints_menu->popup();
}

// Right click on specific file(s) or folder(s).
void ScriptEditorDebugger::_error_tree_item_rmb_selected(const Vector2 &p_pos, MouseButton p_button) {
	if (p_button != MouseButton::RIGHT) {
		return;
	}

	item_menu->clear();
	item_menu->reset_size();

	if (error_tree->is_anything_selected()) {
		item_menu->add_icon_item(get_editor_theme_icon(SNAME("ActionCopy")), TTR("Copy Error"), ACTION_COPY_ERROR);
		item_menu->add_icon_item(get_editor_theme_icon(SNAME("ExternalLink")), TTR("Open C++ Source on GitHub"), ACTION_OPEN_SOURCE);
	}

	if (item_menu->get_item_count() > 0) {
		item_menu->set_position(error_tree->get_screen_position() + p_pos);
		item_menu->popup();
	}
}

void ScriptEditorDebugger::_item_menu_id_pressed(int p_option) {
	switch (p_option) {
		case ACTION_COPY_ERROR: {
			TreeItem *ti = error_tree->get_selected();
			while (ti->get_parent() != error_tree->get_root()) {
				ti = ti->get_parent();
			}

			String type;

			if (ti->has_meta("_is_warning")) {
				type = "W ";
			} else if (ti->has_meta("_is_error")) {
				type = "E ";
			}

			String text = ti->get_text(0) + "   ";
			int rpad_len = text.length();

			text = type + text + ti->get_text(1) + "\n";
			TreeItem *ci = ti->get_first_child();
			while (ci) {
				text += "  " + ci->get_text(0).rpad(rpad_len) + ci->get_text(1) + "\n";
				ci = ci->get_next();
			}

			DisplayServer::get_singleton()->clipboard_set(text);
		} break;

		case ACTION_OPEN_SOURCE: {
			TreeItem *ti = error_tree->get_selected();
			while (ti->get_parent() != error_tree->get_root()) {
				ti = ti->get_parent();
			}

			// Find the child with the "C++ Source".
			// It's not at a fixed position as "C++ Error" may come first.
			TreeItem *ci = ti->get_first_child();
			const String cpp_source = "<" + TTR("C++ Source") + ">";
			while (ci) {
				if (ci->get_text(0) == cpp_source) {
					break;
				}
				ci = ci->get_next();
			}

			if (!ci) {
				WARN_PRINT_ED("No C++ source reference is available for this error.");
				return;
			}

			// Parse back the `file:line @ method()` string.
			const Vector<String> file_line_number = ci->get_text(1).get_slicec('@', 0).strip_edges().split(":");
			ERR_FAIL_COND_MSG(file_line_number.size() < 2, "Incorrect C++ source stack trace file:line format (please report).");
			const String &file = file_line_number[0];
			const int line_number = file_line_number[1].to_int();

			// Construct a GitHub repository URL and open it in the user's default web browser.
			// If the commit hash is available, use it for greater accuracy. Otherwise fall back to tagged release.
			String git_ref = String(GODOT_VERSION_HASH).is_empty() ? String(GODOT_VERSION_NUMBER) + "-stable" : String(GODOT_VERSION_HASH);
			OS::get_singleton()->shell_open(vformat("https://github.com/godotengine/godot/blob/%s/%s#L%d",
					git_ref, file, line_number));
		} break;
		case ACTION_DELETE_BREAKPOINT: {
			const TreeItem *selected = breakpoints_tree->get_selected();
			_set_breakpoint(selected->get_parent()->get_text(0), selected->get_meta("line"), false);
		} break;
		case ACTION_DELETE_BREAKPOINTS_IN_FILE: {
			TreeItem *file_item = breakpoints_tree->get_selected();
			if (file_item->has_meta("line")) {
				file_item = file_item->get_parent();
			}

			// Store first else we will be removing as we loop.
			List<int> lines;
			for (TreeItem *breakpoint_item = file_item->get_first_child(); breakpoint_item; breakpoint_item = breakpoint_item->get_next()) {
				lines.push_back(breakpoint_item->get_meta("line"));
			}

			for (const int &line : lines) {
				_set_breakpoint(file_item->get_text(0), line, false);
			}
		} break;
		case ACTION_DELETE_ALL_BREAKPOINTS: {
			_clear_breakpoints();
		} break;
	}
}

void ScriptEditorDebugger::_tab_changed(int p_tab) {
	if (tabs->get_tab_title(p_tab) == TTR("Video RAM")) {
		// "Video RAM" tab was clicked, refresh the data it's displaying when entering the tab.
		_video_mem_request();
	}
}

void ScriptEditorDebugger::_bind_methods() {
	ClassDB::bind_method(D_METHOD("live_debug_create_node"), &ScriptEditorDebugger::live_debug_create_node);
	ClassDB::bind_method(D_METHOD("live_debug_instantiate_node"), &ScriptEditorDebugger::live_debug_instantiate_node);
	ClassDB::bind_method(D_METHOD("live_debug_remove_node"), &ScriptEditorDebugger::live_debug_remove_node);
	ClassDB::bind_method(D_METHOD("live_debug_remove_and_keep_node"), &ScriptEditorDebugger::live_debug_remove_and_keep_node);
	ClassDB::bind_method(D_METHOD("live_debug_restore_node"), &ScriptEditorDebugger::live_debug_restore_node);
	ClassDB::bind_method(D_METHOD("live_debug_duplicate_node"), &ScriptEditorDebugger::live_debug_duplicate_node);
	ClassDB::bind_method(D_METHOD("live_debug_reparent_node"), &ScriptEditorDebugger::live_debug_reparent_node);
	ClassDB::bind_method(D_METHOD("update_remote_object", "id", "property", "value", "field"), &ScriptEditorDebugger::update_remote_object);

	ADD_SIGNAL(MethodInfo("started"));
	ADD_SIGNAL(MethodInfo("stopped"));
	ADD_SIGNAL(MethodInfo("stop_requested"));
	ADD_SIGNAL(MethodInfo("stack_frame_selected", PropertyInfo(Variant::INT, "frame")));
	ADD_SIGNAL(MethodInfo("error_selected", PropertyInfo(Variant::INT, "error")));
	ADD_SIGNAL(MethodInfo("breakpoint_selected", PropertyInfo("script"), PropertyInfo(Variant::INT, "line")));
	ADD_SIGNAL(MethodInfo("set_execution", PropertyInfo("script"), PropertyInfo(Variant::INT, "line")));
	ADD_SIGNAL(MethodInfo("clear_execution", PropertyInfo("script")));
	ADD_SIGNAL(MethodInfo("breaked", PropertyInfo(Variant::BOOL, "reallydid"), PropertyInfo(Variant::BOOL, "can_debug"), PropertyInfo(Variant::STRING, "reason"), PropertyInfo(Variant::BOOL, "has_stackdump")));
	ADD_SIGNAL(MethodInfo("remote_objects_requested", PropertyInfo(Variant::ARRAY, "ids")));
	ADD_SIGNAL(MethodInfo("remote_objects_updated", PropertyInfo(Variant::OBJECT, "remote_objects")));
	ADD_SIGNAL(MethodInfo("remote_object_property_updated", PropertyInfo(Variant::INT, "id"), PropertyInfo(Variant::STRING, "property")));
	ADD_SIGNAL(MethodInfo("remote_window_title_changed", PropertyInfo(Variant::STRING, "title")));
	ADD_SIGNAL(MethodInfo("remote_tree_updated"));
	ADD_SIGNAL(MethodInfo("remote_tree_select_requested", PropertyInfo(Variant::ARRAY, "ids")));
	ADD_SIGNAL(MethodInfo("remote_tree_clear_selection_requested"));
	ADD_SIGNAL(MethodInfo("output", PropertyInfo(Variant::STRING, "msg"), PropertyInfo(Variant::INT, "level")));
	ADD_SIGNAL(MethodInfo("stack_dump", PropertyInfo(Variant::ARRAY, "stack_dump")));
	ADD_SIGNAL(MethodInfo("stack_frame_vars", PropertyInfo(Variant::INT, "num_vars")));
	ADD_SIGNAL(MethodInfo("stack_frame_var", PropertyInfo(Variant::ARRAY, "data")));
	ADD_SIGNAL(MethodInfo("debug_data", PropertyInfo(Variant::STRING, "msg"), PropertyInfo(Variant::ARRAY, "data")));
	ADD_SIGNAL(MethodInfo("set_breakpoint", PropertyInfo("script"), PropertyInfo(Variant::INT, "line"), PropertyInfo(Variant::BOOL, "enabled")));
	ADD_SIGNAL(MethodInfo("clear_breakpoints"));
	ADD_SIGNAL(MethodInfo("errors_cleared"));
	ADD_SIGNAL(MethodInfo("embed_shortcut_requested", PropertyInfo(Variant::INT, "embed_shortcut_action")));
}

void ScriptEditorDebugger::add_debugger_tab(Control *p_control) {
	tabs->add_child(p_control);
}

void ScriptEditorDebugger::remove_debugger_tab(Control *p_control) {
	int idx = tabs->get_tab_idx_from_control(p_control);
	ERR_FAIL_COND(idx < 0);
	p_control->queue_free();
}

int ScriptEditorDebugger::get_current_debugger_tab() const {
	return tabs->get_current_tab();
}

void ScriptEditorDebugger::switch_to_debugger(int p_debugger_tab_idx) {
	tabs->set_current_tab(p_debugger_tab_idx);
}

void ScriptEditorDebugger::send_message(const String &p_message, const Array &p_args) {
	_put_msg(p_message, p_args);
}

void ScriptEditorDebugger::toggle_profiler(const String &p_profiler, bool p_enable, const Array &p_data) {
	Array msg_data = { p_enable, p_data };
	_put_msg("profiler:" + p_profiler, msg_data);
}

ScriptEditorDebugger::ScriptEditorDebugger() {
	if (unlikely(parse_message_handlers.is_empty())) {
		_init_parse_message_handlers();
	}

	tabs = memnew(TabContainer);
	add_child(tabs);
	tabs->connect("tab_changed", callable_mp(this, &ScriptEditorDebugger::_tab_changed));

	InspectorDock::get_inspector_singleton()->connect("object_id_selected", callable_mp(this, &ScriptEditorDebugger::_remote_object_selected));
	EditorFileSystem::get_singleton()->connect("resources_reimported", callable_mp(this, &ScriptEditorDebugger::_resources_reimported));

	{ //debugger
		VBoxContainer *vbc = memnew(VBoxContainer);
		vbc->set_name(TTR("Stack Trace"));
		Control *dbg = vbc;

		HBoxContainer *hbc = memnew(HBoxContainer);
		vbc->add_child(hbc);

		reason = memnew(RichTextLabel);
		reason->set_focus_mode(FOCUS_ACCESSIBILITY);
		reason->set_selection_enabled(true);
		reason->set_context_menu_enabled(true);
		reason->set_h_size_flags(SIZE_EXPAND_FILL);
		reason->set_v_size_flags(SIZE_SHRINK_CENTER);
		reason->connect(SceneStringName(resized), callable_mp(this, &ScriptEditorDebugger::_update_reason_content_height));
		hbc->add_child(reason);

		hbc->add_child(memnew(VSeparator));

		skip_breakpoints = memnew(Button);
		skip_breakpoints->set_theme_type_variation(SceneStringName(FlatButton));
		hbc->add_child(skip_breakpoints);
		skip_breakpoints->set_tooltip_text(TTR("Skip Breakpoints"));
		skip_breakpoints->connect(SceneStringName(pressed), callable_mp(this, &ScriptEditorDebugger::debug_skip_breakpoints));

		ignore_error_breaks = memnew(Button);
		ignore_error_breaks->set_theme_type_variation(SceneStringName(FlatButton));
		ignore_error_breaks->set_tooltip_text(TTR("Ignore Error Breaks"));
		hbc->add_child(ignore_error_breaks);
		ignore_error_breaks->connect(SceneStringName(pressed), callable_mp(this, &ScriptEditorDebugger::debug_ignore_error_breaks));

		hbc->add_child(memnew(VSeparator));

		copy = memnew(Button);
		copy->set_theme_type_variation(SceneStringName(FlatButton));
		hbc->add_child(copy);
		copy->set_tooltip_text(TTR("Copy Error"));
		copy->connect(SceneStringName(pressed), callable_mp(this, &ScriptEditorDebugger::debug_copy));

		hbc->add_child(memnew(VSeparator));

		step = memnew(Button);
		step->set_theme_type_variation(SceneStringName(FlatButton));
		hbc->add_child(step);
		step->set_tooltip_text(TTR("Step Into"));
		step->set_shortcut(ED_GET_SHORTCUT("debugger/step_into"));
		step->connect(SceneStringName(pressed), callable_mp(this, &ScriptEditorDebugger::debug_step));

		next = memnew(Button);
		next->set_theme_type_variation(SceneStringName(FlatButton));
		hbc->add_child(next);
		next->set_tooltip_text(TTR("Step Over"));
		next->set_shortcut(ED_GET_SHORTCUT("debugger/step_over"));
		next->connect(SceneStringName(pressed), callable_mp(this, &ScriptEditorDebugger::debug_next));

		hbc->add_child(memnew(VSeparator));

		dobreak = memnew(Button);
		dobreak->set_theme_type_variation(SceneStringName(FlatButton));
		hbc->add_child(dobreak);
		dobreak->set_tooltip_text(TTR("Break"));
		dobreak->set_shortcut(ED_GET_SHORTCUT("debugger/break"));
		dobreak->connect(SceneStringName(pressed), callable_mp(this, &ScriptEditorDebugger::debug_break));

		docontinue = memnew(Button);
		docontinue->set_theme_type_variation(SceneStringName(FlatButton));
		hbc->add_child(docontinue);
		docontinue->set_tooltip_text(TTR("Continue"));
		docontinue->set_shortcut(ED_GET_SHORTCUT("debugger/continue"));
		docontinue->connect(SceneStringName(pressed), callable_mp(this, &ScriptEditorDebugger::debug_continue));

		HSplitContainer *parent_sc = memnew(HSplitContainer);
		vbc->add_child(parent_sc);
		parent_sc->set_v_size_flags(SIZE_EXPAND_FILL);
		parent_sc->set_split_offset(500 * EDSCALE);

		HSplitContainer *sc = memnew(HSplitContainer);
		sc->set_v_size_flags(SIZE_EXPAND_FILL);
		sc->set_h_size_flags(SIZE_EXPAND_FILL);
		parent_sc->add_child(sc);

		VBoxContainer *stack_vb = memnew(VBoxContainer);
		stack_vb->set_h_size_flags(SIZE_EXPAND_FILL);
		sc->add_child(stack_vb);
		HBoxContainer *thread_hb = memnew(HBoxContainer);
		stack_vb->add_child(thread_hb);
		thread_hb->add_child(memnew(Label(TTR("Thread:"))));
		threads = memnew(OptionButton);
		thread_hb->add_child(threads);
		threads->set_auto_translate_mode(AUTO_TRANSLATE_MODE_DISABLED);
		threads->set_h_size_flags(SIZE_EXPAND_FILL);
		threads->connect(SceneStringName(item_selected), callable_mp(this, &ScriptEditorDebugger::_select_thread));

		stack_dump = memnew(Tree);
		stack_dump->set_custom_minimum_size(Size2(150, 0) * EDSCALE);
		stack_dump->set_allow_reselect(true);
		stack_dump->set_columns(1);
		stack_dump->set_column_titles_visible(true);
		stack_dump->set_column_title(0, TTR("Stack Frames"));
		stack_dump->set_hide_root(true);
		stack_dump->set_v_size_flags(SIZE_EXPAND_FILL);
		stack_dump->set_theme_type_variation("TreeSecondary");
		stack_dump->connect("cell_selected", callable_mp(this, &ScriptEditorDebugger::_stack_dump_frame_selected));
		stack_vb->add_child(stack_dump);

		VBoxContainer *inspector_vbox = memnew(VBoxContainer);
		inspector_vbox->set_custom_minimum_size(Size2(200, 0) * EDSCALE);
		inspector_vbox->set_h_size_flags(SIZE_EXPAND_FILL);
		sc->add_child(inspector_vbox);

		HBoxContainer *tools_hb = memnew(HBoxContainer);
		inspector_vbox->add_child(tools_hb);

		search = memnew(LineEdit);
		search->set_h_size_flags(Control::SIZE_EXPAND_FILL);
		search->set_placeholder(TTR("Filter Stack Variables"));
		search->set_accessibility_name(TTRC("Filter Stack Variables"));
		search->set_clear_button_enabled(true);
		tools_hb->add_child(search);

		inspector = memnew(EditorDebuggerInspector);
		inspector->set_h_size_flags(SIZE_EXPAND_FILL);
		inspector->set_v_size_flags(SIZE_EXPAND_FILL);
		inspector->set_property_name_style(EditorPropertyNameProcessor::STYLE_RAW);
		inspector->set_read_only(true);
		inspector->connect("object_selected", callable_mp(this, &ScriptEditorDebugger::_remote_object_selected));
		inspector->connect("objects_edited", callable_mp(this, &ScriptEditorDebugger::_remote_objects_edited));
		inspector->connect("object_property_updated", callable_mp(this, &ScriptEditorDebugger::_remote_object_property_updated));
		inspector->register_text_enter(search);
		inspector->set_use_filter(true);
		inspector_vbox->add_child(inspector);

		breakpoints_tree = memnew(Tree);
		breakpoints_tree->set_custom_minimum_size(Size2(100, 0) * EDSCALE);
		breakpoints_tree->set_h_size_flags(SIZE_EXPAND_FILL);
		breakpoints_tree->set_column_titles_visible(true);
		breakpoints_tree->set_column_title(0, TTR("Breakpoints"));
		breakpoints_tree->set_allow_reselect(true);
		breakpoints_tree->set_allow_rmb_select(true);
		breakpoints_tree->set_hide_root(true);
		breakpoints_tree->set_theme_type_variation("TreeSecondary");
		breakpoints_tree->connect("item_mouse_selected", callable_mp(this, &ScriptEditorDebugger::_breakpoints_item_rmb_selected));
		breakpoints_tree->create_item();

		parent_sc->add_child(breakpoints_tree);
		tabs->add_child(dbg);

		breakpoints_menu = memnew(PopupMenu);
		breakpoints_menu->connect(SceneStringName(id_pressed), callable_mp(this, &ScriptEditorDebugger::_item_menu_id_pressed));
		breakpoints_tree->add_child(breakpoints_menu);
	}

	{ //errors
		errors_tab = memnew(VBoxContainer);
		errors_tab->set_name(TTR("Errors"));

		HBoxContainer *error_hbox = memnew(HBoxContainer);
		errors_tab->add_child(error_hbox);

		expand_all_button = memnew(Button);
		expand_all_button->set_text(TTR("Expand All"));
		expand_all_button->set_disabled(true);
		expand_all_button->connect(SceneStringName(pressed), callable_mp(this, &ScriptEditorDebugger::_expand_errors_list));
		error_hbox->add_child(expand_all_button);

		collapse_all_button = memnew(Button);
		collapse_all_button->set_text(TTR("Collapse All"));
		collapse_all_button->set_disabled(true);
		collapse_all_button->connect(SceneStringName(pressed), callable_mp(this, &ScriptEditorDebugger::_collapse_errors_list));
		error_hbox->add_child(collapse_all_button);

		Control *space = memnew(Control);
		space->set_h_size_flags(SIZE_EXPAND_FILL);
		error_hbox->add_child(space);

		clear_button = memnew(Button);
		clear_button->set_text(TTR("Clear"));
		clear_button->set_h_size_flags(0);
		clear_button->set_disabled(true);
		clear_button->connect(SceneStringName(pressed), callable_mp(this, &ScriptEditorDebugger::_clear_errors_list));
		error_hbox->add_child(clear_button);

		error_tree = memnew(Tree);
		error_tree->set_columns(2);

		error_tree->set_column_expand(0, false);
		error_tree->set_column_custom_minimum_width(0, 140);
		error_tree->set_column_clip_content(0, true);

		error_tree->set_column_expand(1, true);
		error_tree->set_column_clip_content(1, true);

		error_tree->set_select_mode(Tree::SELECT_ROW);
		error_tree->set_hide_root(true);
		error_tree->set_v_size_flags(SIZE_EXPAND_FILL);
		error_tree->set_allow_rmb_select(true);
		error_tree->set_allow_reselect(true);
		error_tree->set_theme_type_variation("TreeSecondary");
		error_tree->connect("item_mouse_selected", callable_mp(this, &ScriptEditorDebugger::_error_tree_item_rmb_selected));
		errors_tab->add_child(error_tree);

		item_menu = memnew(PopupMenu);
		item_menu->connect(SceneStringName(id_pressed), callable_mp(this, &ScriptEditorDebugger::_item_menu_id_pressed));
		error_tree->add_child(item_menu);

		tabs->add_child(errors_tab);
	}

	{ // File dialog
		file_dialog = memnew(EditorFileDialog);
		file_dialog->connect("file_selected", callable_mp(this, &ScriptEditorDebugger::_file_selected));
		add_child(file_dialog);
	}

	{ // Expression evaluator
		expression_evaluator = memnew(EditorExpressionEvaluator);
		expression_evaluator->set_name(TTR("Evaluator"));
		expression_evaluator->set_editor_debugger(this);
		tabs->add_child(expression_evaluator);
	}

	{ //profiler
		profiler = memnew(EditorProfiler);
		profiler->set_name(TTR("Profiler"));
		tabs->add_child(profiler);
		profiler->connect("enable_profiling", callable_mp(this, &ScriptEditorDebugger::_profiler_activate).bind(PROFILER_SCRIPTS_SERVERS));
		profiler->connect("break_request", callable_mp(this, &ScriptEditorDebugger::_profiler_seeked));
	}

	{ //frame profiler
		visual_profiler = memnew(EditorVisualProfiler);
		visual_profiler->set_name(TTR("Visual Profiler"));
		tabs->add_child(visual_profiler);
		visual_profiler->connect("enable_profiling", callable_mp(this, &ScriptEditorDebugger::_profiler_activate).bind(PROFILER_VISUAL));
	}

	{ //monitors
		performance_profiler = memnew(EditorPerformanceProfiler);
		tabs->add_child(performance_profiler);
	}

	{ //vmem inspect
		VBoxContainer *vmem_vb = memnew(VBoxContainer);
		HBoxContainer *vmem_hb = memnew(HBoxContainer);

		Label *vmlb = memnew(Label(TTRC("List of Video Memory Usage by Resource:")));
		vmlb->set_theme_type_variation("HeaderSmall");
		vmem_hb->add_child(vmlb);

		{ // Add notice icon.
			vmem_notice_icon = memnew(TextureRect);
			vmem_notice_icon->set_stretch_mode(TextureRect::STRETCH_KEEP_CENTERED);
			vmem_notice_icon->set_h_size_flags(SIZE_SHRINK_CENTER);
			vmem_notice_icon->set_visible(true);
			vmem_notice_icon->set_tooltip_text(TTR(R"(Notice:
This tool only reports memory allocations tracked by the engine.
Therefore, total VRAM usage is inaccurate compared to what the Monitors tab or external tools can report.
Instead, use the monitors tab to obtain more precise VRAM usage.

- Buffer Memory (e.g. GPUParticles) is not tracked.
- Meshes are not tracked in the Compatibility renderer.)"));
			vmem_hb->add_child(vmem_notice_icon);
		}

		{ // Add some space to move the rest of the controls to the right.
			Control *space = memnew(Control);
			space->set_h_size_flags(SIZE_EXPAND_FILL);
			vmem_hb->add_child(space);
		}

		vmem_hb->add_child(memnew(Label(TTR("Total:") + " ")));
		vmem_total = memnew(LineEdit);
		vmem_total->set_editable(false);
		vmem_total->set_accessibility_name(TTRC("Video RAM Total"));
		vmem_total->set_custom_minimum_size(Size2(100, 0) * EDSCALE);
		vmem_hb->add_child(vmem_total);
		vmem_refresh = memnew(Button);
		vmem_refresh->set_accessibility_name(TTRC("Refresh Video RAM"));
		vmem_refresh->set_theme_type_variation(SceneStringName(FlatButton));
		vmem_hb->add_child(vmem_refresh);
		vmem_export = memnew(Button);
		vmem_export->set_theme_type_variation(SceneStringName(FlatButton));
		vmem_export->set_tooltip_text(TTR("Export list to a CSV file"));
		vmem_hb->add_child(vmem_export);
		vmem_vb->add_child(vmem_hb);
		vmem_refresh->connect(SceneStringName(pressed), callable_mp(this, &ScriptEditorDebugger::_video_mem_request));
		vmem_export->connect(SceneStringName(pressed), callable_mp(this, &ScriptEditorDebugger::_video_mem_export));

		VBoxContainer *vmmc = memnew(VBoxContainer);
		vmem_tree = memnew(Tree);
		vmem_tree->set_v_size_flags(SIZE_EXPAND_FILL);
		vmem_tree->set_h_size_flags(SIZE_EXPAND_FILL);
		vmmc->add_child(vmem_tree);
		vmmc->set_v_size_flags(SIZE_EXPAND_FILL);
		vmem_vb->add_child(vmmc);

		vmem_vb->set_name(TTR("Video RAM"));
		vmem_tree->set_columns(4);
		vmem_tree->set_column_titles_visible(true);
		vmem_tree->set_column_title(0, TTR("Resource Path"));
		vmem_tree->set_column_expand(0, true);
		vmem_tree->set_column_expand(1, false);
		vmem_tree->set_column_title(1, TTR("Type"));
		vmem_tree->set_column_custom_minimum_width(1, 100 * EDSCALE);
		vmem_tree->set_column_expand(2, false);
		vmem_tree->set_column_title(2, TTR("Format"));
		vmem_tree->set_column_custom_minimum_width(2, 150 * EDSCALE);
		vmem_tree->set_column_expand(3, false);
		vmem_tree->set_column_title(3, TTR("Usage"));
		vmem_tree->set_column_custom_minimum_width(3, 80 * EDSCALE);
		vmem_tree->set_hide_root(true);
		vmem_tree->connect("item_activated", callable_mp(this, &ScriptEditorDebugger::_vmem_item_activated));

		tabs->add_child(vmem_vb);
	}

	{ // misc
		VBoxContainer *misc = memnew(VBoxContainer);
		misc->set_name(TTR("Misc"));
		tabs->add_child(misc);

		GridContainer *info_left = memnew(GridContainer);
		info_left->set_columns(2);
		misc->add_child(info_left);
		clicked_ctrl = memnew(LineEdit);
		clicked_ctrl->set_editable(false);
		clicked_ctrl->set_accessibility_name(TTRC("Clicked Control:"));
		clicked_ctrl->set_h_size_flags(SIZE_EXPAND_FILL);
		info_left->add_child(memnew(Label(TTR("Clicked Control:"))));
		info_left->add_child(clicked_ctrl);
		clicked_ctrl_type = memnew(LineEdit);
		clicked_ctrl_type->set_editable(false);
		clicked_ctrl_type->set_accessibility_name(TTRC("Clicked Control Type:"));
		info_left->add_child(memnew(Label(TTR("Clicked Control Type:"))));
		info_left->add_child(clicked_ctrl_type);

		scene_tree = memnew(SceneDebuggerTree);
		live_edit_root = memnew(LineEdit);
		live_edit_root->set_editable(false);
		live_edit_root->set_h_size_flags(SIZE_EXPAND_FILL);
		live_edit_root->set_accessibility_name(TTRC("Live Edit Root:"));

		{
			HBoxContainer *lehb = memnew(HBoxContainer);
			Label *l = memnew(Label(TTR("Live Edit Root:")));
			info_left->add_child(l);
			lehb->add_child(live_edit_root);
			le_set = memnew(Button(TTR("Set From Tree")));
			lehb->add_child(le_set);
			le_clear = memnew(Button(TTR("Clear")));
			lehb->add_child(le_clear);
			info_left->add_child(lehb);
		}

		misc->add_child(memnew(VSeparator));

		HBoxContainer *buttons = memnew(HBoxContainer);

		export_csv = memnew(Button(TTR("Export measures as CSV")));
		export_csv->connect(SceneStringName(pressed), callable_mp(this, &ScriptEditorDebugger::_export_csv));
		buttons->add_child(export_csv);

		misc->add_child(buttons);
	}

	msgdialog = memnew(AcceptDialog);
	add_child(msgdialog);

	camera_override = CameraOverride::OVERRIDE_NONE;
	error_count = 0;
	warning_count = 0;
	_update_buttons_state();
}

ScriptEditorDebugger::~ScriptEditorDebugger() {
	if (peer.is_valid()) {
		peer->close();
		peer.unref();
	}
	memdelete(scene_tree);
}
