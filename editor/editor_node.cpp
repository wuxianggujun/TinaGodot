/**************************************************************************/
/*  editor_node.cpp                                                       */
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

#include "editor_node.h"

#include "core/config/project_settings.h"
#include "core/extension/gdextension_manager.h"
#include "core/input/input.h"
#include "core/io/config_file.h"
#include "core/io/file_access.h"
#include "core/io/image.h"
#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"
#include "core/object/class_db.h"
#include "core/os/keyboard.h"
#include "core/os/os.h"
#include "core/os/time.h"
#include "core/string/print_string.h"
#include "core/string/translation_server.h"
#include "core/version.h"
#include "editor/editor_string_names.h"
#include "editor/inspector/editor_context_menu_plugin.h"
#include "main/main.h"
#include "scene/2d/node_2d.h"
#include "scene/animation/animation_tree.h"
#include "scene/gui/color_picker.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/menu_bar.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/panel.h"
#include "scene/gui/popup.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/split_container.h"
#include "scene/gui/tab_container.h"
#include "scene/main/timer.h"
#include "scene/main/window.h"
#include "scene/property_utils.h"
#include "scene/resources/image_texture.h"
#include "scene/resources/packed_scene.h"
#include "scene/resources/portable_compressed_texture.h"
#include "scene/theme/theme_db.h"
#include "servers/display/display_server.h"
#include "servers/navigation_2d/navigation_server_2d.h"
#include "servers/rendering/rendering_server.h"

#include "editor/animation/animation_player_editor_plugin.h"
#include "editor/asset_library/asset_library_editor_plugin.h"
#include "editor/audio/audio_stream_preview.h"
#include "editor/audio/editor_audio_buses.h"
#include "editor/debugger/debugger_editor_plugin.h"
#include "editor/debugger/editor_debugger_node.h"
#include "editor/debugger/script_editor_debugger.h"
#include "editor/doc/editor_help.h"
#include "editor/docks/editor_dock_manager.h"
#include "editor/docks/filesystem_dock.h"
#include "editor/docks/history_dock.h"
#include "editor/docks/import_dock.h"
#include "editor/docks/inspector_dock.h"
#include "editor/docks/node_dock.h"
#include "editor/docks/scene_tree_dock.h"
#include "editor/editor_data.h"
#include "editor/editor_interface.h"
#include "editor/editor_log.h"
#include "editor/editor_main_screen.h"
#include "editor/editor_undo_redo_manager.h"
#include "editor/export/dedicated_server_export_plugin.h"
#include "editor/export/editor_export.h"
#include "editor/export/export_template_manager.h"
#include "editor/export/gdextension_export_plugin.h"
#include "editor/export/project_export.h"
#include "editor/export/project_zip_packer.h"
#include "editor/export/register_exporters.h"
#include "editor/export/shader_baker_export_plugin.h"
#include "editor/file_system/dependency_editor.h"
#include "editor/file_system/editor_paths.h"
#include "editor/gui/editor_about.h"
#include "editor/gui/editor_bottom_panel.h"
#include "editor/gui/editor_file_dialog.h"
#include "editor/gui/editor_quick_open_dialog.h"
#include "editor/gui/editor_title_bar.h"
#include "editor/gui/editor_toaster.h"
#include "editor/gui/progress_dialog.h"
#include "editor/gui/window_wrapper.h"
#include "editor/import/audio_stream_import_settings.h"
#include "editor/import/dynamic_font_import_settings.h"
#include "editor/import/fbx_importer_manager.h"
#include "editor/import/resource_importer_bitmask.h"
#include "editor/import/resource_importer_bmfont.h"
#include "editor/import/resource_importer_csv_translation.h"
#include "editor/import/resource_importer_dynamic_font.h"
#include "editor/import/resource_importer_image.h"
#include "editor/import/resource_importer_imagefont.h"
#include "editor/import/resource_importer_layered_texture.h"
#include "editor/import/resource_importer_shader_file.h"
#include "editor/import/resource_importer_svg.h"
#include "editor/import/resource_importer_texture.h"
#include "editor/import/resource_importer_texture_atlas.h"
#include "editor/import/resource_importer_wav.h"
#include "editor/inspector/editor_inspector.h"
#include "editor/inspector/editor_preview_plugins.h"
#include "editor/inspector/editor_properties.h"
#include "editor/inspector/editor_property_name_processor.h"
#include "editor/inspector/editor_resource_picker.h"
#include "editor/inspector/editor_resource_preview.h"
#include "editor/inspector/multi_node_edit.h"
#include "editor/plugins/editor_plugin.h"
#include "editor/plugins/editor_resource_conversion_plugin.h"
#include "editor/plugins/plugin_config_dialog.h"
#include "editor/project_upgrade/project_upgrade_tool.h"
#include "editor/run/editor_run.h"
#include "editor/run/editor_run_bar.h"
#include "editor/run/game_view_plugin.h"
// 3D editor plugins removed in 2D Lite version
// 

#ifdef ANDROID_ENABLED
#include "editor/gui/touch_actions_panel.h"
#endif // ANDROID_ENABLED

#include <cstdlib>

EditorNode *EditorNode::singleton = nullptr;

static const String EDITOR_NODE_CONFIG_SECTION = "EditorNode";

static const String REMOVE_ANDROID_BUILD_TEMPLATE_MESSAGE = TTRC("The Android build template is already installed in this project and it won't be overwritten.\nRemove the \"%s\" directory manually before attempting this operation again.");
static const String INSTALL_ANDROID_BUILD_TEMPLATE_MESSAGE = TTRC("This will set up your project for gradle Android builds by installing the source template to \"%s\".\nNote that in order to make gradle builds instead of using pre-built APKs, the \"Use Gradle Build\" option should be enabled in the Android export preset.");

bool EditorProgress::step(const String &p_state, int p_step, bool p_force_refresh) {
	if (!force_background && Thread::is_main_thread()) {
		return EditorNode::progress_task_step(task, p_state, p_step, p_force_refresh);
	} else {
		EditorNode::progress_task_step_bg(task, p_step);
		return false;
	}
}

EditorProgress::EditorProgress(const String &p_task, const String &p_label, int p_amount, bool p_can_cancel, bool p_force_background) {
	if (!p_force_background && Thread::is_main_thread()) {
		EditorNode::progress_add_task(p_task, p_label, p_amount, p_can_cancel);
	} else {
		EditorNode::progress_add_task_bg(p_task, p_label, p_amount);
	}
	task = p_task;
	force_background = p_force_background;
}

EditorProgress::~EditorProgress() {
	if (!force_background && Thread::is_main_thread()) {
		EditorNode::progress_end_task(task);
	} else {
		EditorNode::progress_end_task_bg(task);
	}
}

void EditorNode::disambiguate_filenames(const Vector<String> p_full_paths, Vector<String> &r_filenames) {
	ERR_FAIL_COND_MSG(p_full_paths.size() != r_filenames.size(), vformat("disambiguate_filenames requires two string vectors of same length (%d != %d).", p_full_paths.size(), r_filenames.size()));

	// Keep track of a list of "index sets," i.e. sets of indices
	// within disambiguated_scene_names which contain the same name.
	Vector<RBSet<int>> index_sets;
	HashMap<String, int> scene_name_to_set_index;
	for (int i = 0; i < r_filenames.size(); i++) {
		const String &scene_name = r_filenames[i];
		if (!scene_name_to_set_index.has(scene_name)) {
			index_sets.append(RBSet<int>());
			scene_name_to_set_index.insert(r_filenames[i], index_sets.size() - 1);
		}
		index_sets.write[scene_name_to_set_index[scene_name]].insert(i);
	}

	// For each index set with a size > 1, we need to disambiguate.
	for (int i = 0; i < index_sets.size(); i++) {
		RBSet<int> iset = index_sets[i];
		while (iset.size() > 1) {
			// Append the parent folder to each scene name.
			for (const int &E : iset) {
				int set_idx = E;
				String scene_name = r_filenames[set_idx];
				String full_path = p_full_paths[set_idx];

				// Get rid of file extensions and res:// prefixes.
				scene_name = scene_name.get_basename();
				if (full_path.begins_with("res://")) {
					full_path = full_path.substr(6);
				}
				full_path = full_path.get_basename();

				// Normalize trailing slashes when normalizing directory names.
				scene_name = scene_name.trim_suffix("/");
				full_path = full_path.trim_suffix("/");

				int scene_name_size = scene_name.size();
				int full_path_size = full_path.size();
				int difference = full_path_size - scene_name_size;

				// Find just the parent folder of the current path and append it.
				// If the current name is foo.tscn, and the full path is /some/folder/foo.tscn
				// then slash_idx is the second '/', so that we select just "folder", and
				// append that to yield "folder/foo.tscn".
				if (difference > 0) {
					String parent = full_path.substr(0, difference);
					int slash_idx = parent.rfind_char('/');
					slash_idx = parent.rfind_char('/', slash_idx - 1);
					parent = (slash_idx >= 0 && parent.length() > 1) ? parent.substr(slash_idx + 1) : parent;
					r_filenames.write[set_idx] = parent + r_filenames[set_idx];
				}
			}

			// Loop back through scene names and remove non-ambiguous names.
			bool can_proceed = false;
			RBSet<int>::Element *E = iset.front();
			while (E) {
				String scene_name = r_filenames[E->get()];
				bool duplicate_found = false;
				for (const int &F : iset) {
					if (E->get() == F) {
						continue;
					}
					const String &other_scene_name = r_filenames[F];
					if (other_scene_name == scene_name) {
						duplicate_found = true;
						break;
					}
				}

				RBSet<int>::Element *to_erase = duplicate_found ? nullptr : E;

				// We need to check that we could actually append anymore names
				// if we wanted to for disambiguation. If we can't, then we have
				// to abort even with ambiguous names. We clean the full path
				// and the scene name first to remove extensions so that this
				// comparison actually works.
				String path = p_full_paths[E->get()];

				// Get rid of file extensions and res:// prefixes.
				scene_name = scene_name.get_basename();
				if (path.begins_with("res://")) {
					path = path.substr(6);
				}
				path = path.get_basename();

				// Normalize trailing slashes when normalizing directory names.
				scene_name = scene_name.trim_suffix("/");
				path = path.trim_suffix("/");

				// We can proceed if the full path is longer than the scene name,
				// meaning that there is at least one more parent folder we can
				// tack onto the name.
				can_proceed = can_proceed || (path.size() - scene_name.size()) >= 1;

				E = E->next();
				if (to_erase) {
					iset.erase(to_erase);
				}
			}

			if (!can_proceed) {
				break;
			}
		}
	}
}

void EditorNode::_version_control_menu_option(int p_idx) {
	switch (vcs_actions_menu->get_item_id(p_idx)) {
		case VCS_METADATA: {
			VersionControlEditorPlugin::get_singleton()->popup_vcs_metadata_dialog();
		} break;
		case VCS_SETTINGS: {
			VersionControlEditorPlugin::get_singleton()->popup_vcs_set_up_dialog(gui_base);
		} break;
	}
}

void EditorNode::_update_title() {
	const String appname = GLOBAL_GET("application/config/name");
	String title = (appname.is_empty() ? TTR("Unnamed Project") : appname);
	const String edited = editor_data.get_edited_scene_root() ? editor_data.get_edited_scene_root()->get_scene_file_path() : String();
	if (!edited.is_empty()) {
		// Display the edited scene name before the program name so that it can be seen in the OS task bar.
		title = vformat("%s - %s", edited.get_file(), title);
	}
	if (unsaved_cache) {
		// Display the "modified" mark before anything else so that it can always be seen in the OS task bar.
		title = vformat("(*) %s", title);
	}
	DisplayServer::get_singleton()->window_set_title(title + String(" - ") + GODOT_VERSION_NAME);
	if (project_title) {
		project_title->set_text(title);
	}
}

void EditorNode::_update_unsaved_cache() {
	bool is_unsaved = EditorUndoRedoManager::get_singleton()->is_history_unsaved(EditorUndoRedoManager::GLOBAL_HISTORY) ||
			EditorUndoRedoManager::get_singleton()->is_history_unsaved(editor_data.get_current_edited_scene_history_id());

	if (unsaved_cache != is_unsaved) {
		unsaved_cache = is_unsaved;
		_update_title();
	}
}

void EditorNode::input(const Ref<InputEvent> &p_event) {
	// EditorNode::get_singleton()->set_process_input is set to true in ProgressDialog
	// only when the progress dialog is visible.
	// We need to discard all key events to disable all shortcuts while the progress
	// dialog is displayed, simulating an exclusive popup. Mouse events are
	// captured by a full-screen container in front of the EditorNode in ProgressDialog,
	// allowing interaction with the actual dialog where a Cancel button may be visible.
	Ref<InputEventKey> k = p_event;
	if (k.is_valid()) {
		get_tree()->get_root()->set_input_as_handled();
	}
}

void EditorNode::shortcut_input(const Ref<InputEvent> &p_event) {
	ERR_FAIL_COND(p_event.is_null());

	Ref<InputEventKey> k = p_event;
	if ((k.is_valid() && k->is_pressed() && !k->is_echo()) || Object::cast_to<InputEventShortcut>(*p_event)) {
		bool is_handled = true;
		if (ED_IS_SHORTCUT("editor/filter_files", p_event)) {
			FileSystemDock::get_singleton()->focus_on_filter();
		} else if (ED_IS_SHORTCUT("editor/editor_2d", p_event)) {
			editor_main_screen->select(EditorMainScreen::EDITOR_2D);
		} else if (ED_IS_SHORTCUT("editor/editor_3d", p_event)) {
			editor_main_screen->select(EditorMainScreen::EDITOR_3D);
		} else if (ED_IS_SHORTCUT("editor/editor_script", p_event)) {
			editor_main_screen->select(EditorMainScreen::EDITOR_SCRIPT);
		} else if (ED_IS_SHORTCUT("editor/editor_game", p_event)) {
			editor_main_screen->select(EditorMainScreen::EDITOR_GAME);
		} else if (ED_IS_SHORTCUT("editor/editor_help", p_event)) {
			emit_signal(SNAME("request_help_search"), "");
		} else if (ED_IS_SHORTCUT("editor/editor_assetlib", p_event) && AssetLibraryEditorPlugin::is_available()) {
			editor_main_screen->select(EditorMainScreen::EDITOR_ASSETLIB);
		} else if (ED_IS_SHORTCUT("editor/editor_next", p_event)) {
			editor_main_screen->select_next();
		} else if (ED_IS_SHORTCUT("editor/editor_prev", p_event)) {
			editor_main_screen->select_prev();
		} else if (ED_IS_SHORTCUT("editor/command_palette", p_event)) {
			_open_command_palette();
		} else if (ED_IS_SHORTCUT("editor/toggle_last_opened_bottom_panel", p_event)) {
			bottom_panel->toggle_last_opened_bottom_panel();
		} else {
			is_handled = false;
		}

		if (is_handled) {
			get_tree()->get_root()->set_input_as_handled();
		}
	}
}

void EditorNode::_update_vsync_mode() {
	const DisplayServer::VSyncMode window_vsync_mode = DisplayServer::VSyncMode(int(EDITOR_GET("interface/editor/vsync_mode")));
	DisplayServer::get_singleton()->window_set_vsync_mode(window_vsync_mode);
}

void EditorNode::_update_from_settings() {
	if (!is_inside_tree()) {
		return;
	}
	_update_title();

	int current_filter = GLOBAL_GET("rendering/textures/canvas_textures/default_texture_filter");
	if (current_filter != scene_root->get_default_canvas_item_texture_filter()) {
		Viewport::DefaultCanvasItemTextureFilter tf = (Viewport::DefaultCanvasItemTextureFilter)current_filter;
		scene_root->set_default_canvas_item_texture_filter(tf);
	}
	int current_repeat = GLOBAL_GET("rendering/textures/canvas_textures/default_texture_repeat");
	if (current_repeat != scene_root->get_default_canvas_item_texture_repeat()) {
		Viewport::DefaultCanvasItemTextureRepeat tr = (Viewport::DefaultCanvasItemTextureRepeat)current_repeat;
		scene_root->set_default_canvas_item_texture_repeat(tr);
	}
	String current_fallback_locale = GLOBAL_GET("internationalization/locale/fallback");
	if (current_fallback_locale != TranslationServer::get_singleton()->get_fallback_locale()) {
		TranslationServer::get_singleton()->set_fallback_locale(current_fallback_locale);
		scene_root->propagate_notification(Control::NOTIFICATION_LAYOUT_DIRECTION_CHANGED);
	}

	RS::DOFBokehShape dof_shape = RS::DOFBokehShape(int(GLOBAL_GET("rendering/camera/depth_of_field/depth_of_field_bokeh_shape")));
	RS::get_singleton()->camera_attributes_set_dof_blur_bokeh_shape(dof_shape);
	RS::DOFBlurQuality dof_quality = RS::DOFBlurQuality(int(GLOBAL_GET("rendering/camera/depth_of_field/depth_of_field_bokeh_quality")));
	bool dof_jitter = GLOBAL_GET("rendering/camera/depth_of_field/depth_of_field_use_jitter");
	RS::get_singleton()->camera_attributes_set_dof_blur_quality(dof_quality, dof_jitter);
	RS::get_singleton()->environment_set_ssao_quality(RS::EnvironmentSSAOQuality(int(GLOBAL_GET("rendering/environment/ssao/quality"))), GLOBAL_GET("rendering/environment/ssao/half_size"), GLOBAL_GET("rendering/environment/ssao/adaptive_target"), GLOBAL_GET("rendering/environment/ssao/blur_passes"), GLOBAL_GET("rendering/environment/ssao/fadeout_from"), GLOBAL_GET("rendering/environment/ssao/fadeout_to"));
	RS::get_singleton()->screen_space_roughness_limiter_set_active(GLOBAL_GET("rendering/anti_aliasing/screen_space_roughness_limiter/enabled"), GLOBAL_GET("rendering/anti_aliasing/screen_space_roughness_limiter/amount"), GLOBAL_GET("rendering/anti_aliasing/screen_space_roughness_limiter/limit"));
	bool glow_bicubic = int(GLOBAL_GET("rendering/environment/glow/upscale_mode")) > 0;
	RS::get_singleton()->environment_set_ssil_quality(RS::EnvironmentSSILQuality(int(GLOBAL_GET("rendering/environment/ssil/quality"))), GLOBAL_GET("rendering/environment/ssil/half_size"), GLOBAL_GET("rendering/environment/ssil/adaptive_target"), GLOBAL_GET("rendering/environment/ssil/blur_passes"), GLOBAL_GET("rendering/environment/ssil/fadeout_from"), GLOBAL_GET("rendering/environment/ssil/fadeout_to"));
	RS::get_singleton()->environment_glow_set_use_bicubic_upscale(glow_bicubic);
	RS::EnvironmentSSRRoughnessQuality ssr_roughness_quality = RS::EnvironmentSSRRoughnessQuality(int(GLOBAL_GET("rendering/environment/screen_space_reflection/roughness_quality")));
	RS::get_singleton()->environment_set_ssr_roughness_quality(ssr_roughness_quality);
	RS::SubSurfaceScatteringQuality sss_quality = RS::SubSurfaceScatteringQuality(int(GLOBAL_GET("rendering/environment/subsurface_scattering/subsurface_scattering_quality")));
	RS::get_singleton()->sub_surface_scattering_set_quality(sss_quality);
	float sss_scale = GLOBAL_GET("rendering/environment/subsurface_scattering/subsurface_scattering_scale");
	float sss_depth_scale = GLOBAL_GET("rendering/environment/subsurface_scattering/subsurface_scattering_depth_scale");
	RS::get_singleton()->sub_surface_scattering_set_scale(sss_scale, sss_depth_scale);

	uint32_t directional_shadow_size = GLOBAL_GET("rendering/lights_and_shadows/directional_shadow/size");
	uint32_t directional_shadow_16_bits = GLOBAL_GET("rendering/lights_and_shadows/directional_shadow/16_bits");
	RS::get_singleton()->directional_shadow_atlas_set_size(directional_shadow_size, directional_shadow_16_bits);

	RS::ShadowQuality shadows_quality = RS::ShadowQuality(int(GLOBAL_GET("rendering/lights_and_shadows/positional_shadow/soft_shadow_filter_quality")));
	RS::get_singleton()->positional_soft_shadow_filter_set_quality(shadows_quality);
	RS::ShadowQuality directional_shadow_quality = RS::ShadowQuality(int(GLOBAL_GET("rendering/lights_and_shadows/directional_shadow/soft_shadow_filter_quality")));
	RS::get_singleton()->directional_soft_shadow_filter_set_quality(directional_shadow_quality);
	float probe_update_speed = GLOBAL_GET("rendering/lightmapping/probe_capture/update_speed");
	RS::get_singleton()->lightmap_set_probe_capture_update_speed(probe_update_speed);
	RS::EnvironmentSDFGIFramesToConverge frames_to_converge = RS::EnvironmentSDFGIFramesToConverge(int(GLOBAL_GET("rendering/global_illumination/sdfgi/frames_to_converge")));
	RS::get_singleton()->environment_set_sdfgi_frames_to_converge(frames_to_converge);
	RS::EnvironmentSDFGIRayCount ray_count = RS::EnvironmentSDFGIRayCount(int(GLOBAL_GET("rendering/global_illumination/sdfgi/probe_ray_count")));
	RS::get_singleton()->environment_set_sdfgi_ray_count(ray_count);
	RS::VoxelGIQuality voxel_gi_quality = RS::VoxelGIQuality(int(GLOBAL_GET("rendering/global_illumination/voxel_gi/quality")));
	RS::get_singleton()->voxel_gi_set_quality(voxel_gi_quality);
	RS::get_singleton()->environment_set_volumetric_fog_volume_size(GLOBAL_GET("rendering/environment/volumetric_fog/volume_size"), GLOBAL_GET("rendering/environment/volumetric_fog/volume_depth"));
	RS::get_singleton()->environment_set_volumetric_fog_filter_active(bool(GLOBAL_GET("rendering/environment/volumetric_fog/use_filter")));
	RS::get_singleton()->canvas_set_shadow_texture_size(GLOBAL_GET("rendering/2d/shadow_atlas/size"));

	bool use_half_res_gi = GLOBAL_GET("rendering/global_illumination/gi/use_half_resolution");
	RS::get_singleton()->gi_set_use_half_resolution(use_half_res_gi);

	bool snap_2d_transforms = GLOBAL_GET("rendering/2d/snap/snap_2d_transforms_to_pixel");
	scene_root->set_snap_2d_transforms_to_pixel(snap_2d_transforms);
	bool snap_2d_vertices = GLOBAL_GET("rendering/2d/snap/snap_2d_vertices_to_pixel");
	scene_root->set_snap_2d_vertices_to_pixel(snap_2d_vertices);

	Viewport::SDFOversize sdf_oversize = Viewport::SDFOversize(int(GLOBAL_GET("rendering/2d/sdf/oversize")));
	scene_root->set_sdf_oversize(sdf_oversize);
	Viewport::SDFScale sdf_scale = Viewport::SDFScale(int(GLOBAL_GET("rendering/2d/sdf/scale")));
	scene_root->set_sdf_scale(sdf_scale);

	Viewport::MSAA msaa = Viewport::MSAA(int(GLOBAL_GET("rendering/anti_aliasing/quality/msaa_2d")));
	scene_root->set_msaa_2d(msaa);

	// 2D doesn't use a dedicated SubViewport like 3D does, so we apply it on the root viewport instead.
	bool use_debanding = GLOBAL_GET("rendering/anti_aliasing/quality/use_debanding");
	scene_root->set_use_debanding(use_debanding);
	get_viewport()->set_use_debanding(use_debanding);

	bool use_hdr_2d = GLOBAL_GET("rendering/viewport/hdr_2d");
	scene_root->set_use_hdr_2d(use_hdr_2d);
	get_viewport()->set_use_hdr_2d(use_hdr_2d);

	float mesh_lod_threshold = GLOBAL_GET("rendering/mesh_lod/lod_change/threshold_pixels");
	scene_root->set_mesh_lod_threshold(mesh_lod_threshold);

	RS::get_singleton()->decals_set_filter(RS::DecalFilter(int(GLOBAL_GET("rendering/textures/decals/filter"))));
	RS::get_singleton()->light_projectors_set_filter(RS::LightProjectorFilter(int(GLOBAL_GET("rendering/textures/light_projectors/filter"))));
	RS::get_singleton()->lightmaps_set_bicubic_filter(GLOBAL_GET("rendering/lightmapping/lightmap_gi/use_bicubic_filter"));
	RS::get_singleton()->material_set_use_debanding(GLOBAL_GET("rendering/anti_aliasing/quality/use_debanding"));

	SceneTree *tree = get_tree();
	tree->set_debug_collisions_color(GLOBAL_GET("debug/shapes/collision/shape_color"));
	tree->set_debug_collision_contact_color(GLOBAL_GET("debug/shapes/collision/contact_color"));

	ResourceImporterTexture::get_singleton()->update_imports();

	_update_translations();

#ifdef DEBUG_ENABLED
	NavigationServer2D::get_singleton()->set_debug_navigation_edge_connection_color(GLOBAL_GET("debug/shapes/navigation/2d/edge_connection_color"));
	NavigationServer2D::get_singleton()->set_debug_navigation_geometry_edge_color(GLOBAL_GET("debug/shapes/navigation/2d/geometry_edge_color"));
	NavigationServer2D::get_singleton()->set_debug_navigation_geometry_face_color(GLOBAL_GET("debug/shapes/navigation/2d/geometry_face_color"));
	NavigationServer2D::get_singleton()->set_debug_navigation_geometry_edge_disabled_color(GLOBAL_GET("debug/shapes/navigation/2d/geometry_edge_disabled_color"));
	NavigationServer2D::get_singleton()->set_debug_navigation_geometry_face_disabled_color(GLOBAL_GET("debug/shapes/navigation/2d/geometry_face_disabled_color"));
	NavigationServer2D::get_singleton()->set_debug_navigation_enable_edge_connections(GLOBAL_GET("debug/shapes/navigation/2d/enable_edge_connections"));
	NavigationServer2D::get_singleton()->set_debug_navigation_enable_edge_lines(GLOBAL_GET("debug/shapes/navigation/2d/enable_edge_lines"));
	NavigationServer2D::get_singleton()->set_debug_navigation_enable_geometry_face_random_color(GLOBAL_GET("debug/shapes/navigation/2d/enable_geometry_face_random_color"));

#endif // DEBUG_ENABLED
}

void EditorNode::_gdextensions_reloaded() {
	// In case the developer is inspecting an object that will be changed by the reload.
	InspectorDock::get_inspector_singleton()->update_tree();

	// Reload script editor to revalidate GDScript if classes are added or removed.
	ScriptEditor::get_singleton()->reload_scripts(true);

	// Regenerate documentation without using script documentation cache since that would
	// revert doc changes during this session.
	EditorHelp::generate_doc(true, false);
}

void EditorNode::_update_translations() {
	Ref<TranslationDomain> main = TranslationServer::get_singleton()->get_main_domain();

	main->clear();
	TranslationServer::get_singleton()->load_translations();

	if (main->is_enabled()) {
		// Check for the exact locale.
		// `get_potential_translations("zh_CN")` could return translations for "zh".
		if (main->get_loaded_locales().has(main->get_locale_override())) {
			// The set of translation resources for the current locale changed.
			const HashSet<Ref<Translation>> translations = main->get_potential_translations(main->get_locale_override());
			if (translations != tracked_translations) {
				_translation_resources_changed();
			}
		} else {
			// Translations for the current preview locale is removed.
			main->set_enabled(false);
			main->set_locale_override(String());
			_translation_resources_changed();
		}
	}
}

void EditorNode::_translation_resources_changed() {
	for (const Ref<Translation> &E : tracked_translations) {
		E->disconnect_changed(callable_mp(this, &EditorNode::_queue_translation_notification));
	}
	tracked_translations.clear();

	const Ref<TranslationDomain> main = TranslationServer::get_singleton()->get_main_domain();
	if (main->is_enabled()) {
		const HashSet<Ref<Translation>> translations = main->get_potential_translations(main->get_locale_override());
		tracked_translations.reserve(translations.size());
		for (const Ref<Translation> &translation : translations) {
			translation->connect_changed(callable_mp(this, &EditorNode::_queue_translation_notification));
			tracked_translations.insert(translation);
		}
	}

	_queue_translation_notification();
	emit_signal(SNAME("preview_locale_changed"));
}

void EditorNode::_queue_translation_notification() {
	if (pending_translation_notification) {
		return;
	}
	pending_translation_notification = true;
	callable_mp(this, &EditorNode::_propagate_translation_notification).call_deferred();
}

void EditorNode::_propagate_translation_notification() {
	pending_translation_notification = false;
	scene_root->propagate_notification(NOTIFICATION_TRANSLATION_CHANGED);
}

void EditorNode::_update_theme(bool p_skip_creation) {
	if (!p_skip_creation) {
		theme = EditorThemeManager::generate_theme(theme);
		DisplayServer::set_early_window_clear_color_override(true, theme->get_color(SNAME("background"), EditorStringName(Editor)));
	}

	Vector<Ref<Theme>> editor_themes;
	editor_themes.push_back(theme);
	editor_themes.push_back(ThemeDB::get_singleton()->get_default_theme());

	ThemeContext *node_tc = ThemeDB::get_singleton()->get_theme_context(this);
	if (node_tc) {
		node_tc->set_themes(editor_themes);
	} else {
		ThemeDB::get_singleton()->create_theme_context(this, editor_themes);
	}

	Window *window = get_window();
	if (window) {
		ThemeContext *window_tc = ThemeDB::get_singleton()->get_theme_context(window);
		if (window_tc) {
			window_tc->set_themes(editor_themes);
		} else {
			ThemeDB::get_singleton()->create_theme_context(window, editor_themes);
		}
	}

	if (CanvasItemEditor::get_singleton()->get_theme_preview() == CanvasItemEditor::THEME_PREVIEW_EDITOR) {
		update_preview_themes(CanvasItemEditor::THEME_PREVIEW_EDITOR);
	}

	// Update styles.
	{
		bool global_menu = !bool(EDITOR_GET("interface/editor/use_embedded_menu")) && NativeMenu::get_singleton()->has_feature(NativeMenu::FEATURE_GLOBAL_MENU);
		bool dark_mode = DisplayServer::get_singleton()->is_dark_mode_supported() && DisplayServer::get_singleton()->is_dark_mode();

		gui_base->add_theme_style_override(SceneStringName(panel), theme->get_stylebox(SNAME("Background"), EditorStringName(EditorStyles)));
		main_vbox->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT, Control::PRESET_MODE_MINSIZE, theme->get_constant(SNAME("window_border_margin"), EditorStringName(Editor)));
		main_vbox->add_theme_constant_override("separation", theme->get_constant(SNAME("top_bar_separation"), EditorStringName(Editor)));

		if (main_menu_button != nullptr) {
			main_menu_button->set_button_icon(theme->get_icon(SNAME("TripleBar"), EditorStringName(EditorIcons)));
		}

		editor_main_screen->add_theme_style_override(SceneStringName(panel), theme->get_stylebox(SNAME("Content"), EditorStringName(EditorStyles)));
		bottom_panel->add_theme_style_override(SceneStringName(panel), theme->get_stylebox(SNAME("BottomPanel"), EditorStringName(EditorStyles)));
		distraction_free->set_button_icon(theme->get_icon(SNAME("DistractionFree"), EditorStringName(EditorIcons)));
		distraction_free->add_theme_style_override(SceneStringName(pressed), theme->get_stylebox(CoreStringName(normal), "FlatMenuButton"));

		help_menu->set_item_icon(help_menu->get_item_index(HELP_SEARCH), _get_editor_theme_native_menu_icon(SNAME("HelpSearch"), global_menu, dark_mode));
		help_menu->set_item_icon(help_menu->get_item_index(HELP_COPY_SYSTEM_INFO), _get_editor_theme_native_menu_icon(SNAME("ActionCopy"), global_menu, dark_mode));
		help_menu->set_item_icon(help_menu->get_item_index(HELP_ABOUT), _get_editor_theme_native_menu_icon(SNAME("Godot"), global_menu, dark_mode));
		help_menu->set_item_icon(help_menu->get_item_index(HELP_SUPPORT_GODOT_DEVELOPMENT), _get_editor_theme_native_menu_icon(SNAME("Heart"), global_menu, dark_mode));

		_update_renderer_color();
	}

	Ref<Texture2D> thumbnail_icon = gui_base->get_theme_icon(SNAME("file_thumbnail"), SNAME("FileDialog"));
	default_thumbnail.instantiate();
	default_thumbnail->set_image(thumbnail_icon->get_image());

	editor_dock_manager->update_tab_styles();
	editor_dock_manager->update_docks_menu();
	editor_dock_manager->set_tab_icon_max_width(theme->get_constant(SNAME("class_icon_size"), EditorStringName(Editor)));
#ifdef ANDROID_ENABLED
	DisplayServer::get_singleton()->window_set_color(theme->get_color(SNAME("background"), EditorStringName(Editor)));
#endif
}

Ref<Texture2D> EditorNode::_get_editor_theme_native_menu_icon(const StringName &p_name, bool p_global_menu, bool p_dark_mode) const {
	if (!p_global_menu) {
		return theme->get_icon(p_name, SNAME("EditorIcons"));
	}
	if (p_dark_mode && theme->has_icon(String(p_name) + "Dark", SNAME("EditorIcons"))) {
		return theme->get_icon(String(p_name) + "Dark", SNAME("EditorIcons"));
	} else if (!p_dark_mode && theme->has_icon(String(p_name) + "Light", SNAME("EditorIcons"))) {
		return theme->get_icon(String(p_name) + "Light", SNAME("EditorIcons"));
	}
	return theme->get_icon(p_name, SNAME("EditorIcons"));
}

void EditorNode::update_preview_themes(int p_mode) {
	if (!scene_root->is_inside_tree()) {
		return; // Too early.
	}

	Vector<Ref<Theme>> preview_themes;

	switch (p_mode) {
		case CanvasItemEditor::THEME_PREVIEW_PROJECT:
			preview_themes.push_back(ThemeDB::get_singleton()->get_project_theme());
			break;

		case CanvasItemEditor::THEME_PREVIEW_EDITOR:
			preview_themes.push_back(get_editor_theme());
			break;

		default:
			break;
	}

	preview_themes.push_back(ThemeDB::get_singleton()->get_default_theme());

	ThemeContext *preview_context = ThemeDB::get_singleton()->get_theme_context(scene_root);
	if (preview_context) {
		preview_context->set_themes(preview_themes);
	} else {
		ThemeDB::get_singleton()->create_theme_context(scene_root, preview_themes);
	}
}

bool EditorNode::_is_project_data_missing() {
	Ref<DirAccess> da = DirAccess::create(DirAccess::ACCESS_RESOURCES);
	const String project_data_dir = EditorPaths::get_singleton()->get_project_data_dir();
	if (!da->dir_exists(project_data_dir)) {
		return true;
	}

	String project_data_gdignore_file_path = project_data_dir.path_join(".gdignore");
	if (!FileAccess::exists(project_data_gdignore_file_path)) {
		Ref<FileAccess> f = FileAccess::open(project_data_gdignore_file_path, FileAccess::WRITE);
		if (f.is_valid()) {
			f->store_line("");
		} else {
			ERR_PRINT("Failed to create file " + project_data_gdignore_file_path.quote() + ".");
		}
	}

	String uid_cache = ResourceUID::get_singleton()->get_cache_file();
	if (!da->file_exists(uid_cache)) {
		Error err = ResourceUID::get_singleton()->save_to_cache();
		if (err != OK) {
			ERR_PRINT("Failed to create file " + uid_cache.quote() + ".");
		}
	}

	const String dirs[] = {
		EditorPaths::get_singleton()->get_project_settings_dir(),
		ProjectSettings::get_singleton()->get_imported_files_path()
	};
	for (const String &dir : dirs) {
		if (!da->dir_exists(dir)) {
			return true;
		}
	}
	return false;
}

void EditorNode::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_TRANSLATION_CHANGED: {
			_update_title();
			callable_mp(this, &EditorNode::_titlebar_resized).call_deferred();
		} break;

		case NOTIFICATION_POSTINITIALIZE: {
			EditorHelp::generate_doc();
#if defined(MODULE_GDSCRIPT_ENABLED) || defined(MODULE_MONO_ENABLED)
			EditorHelpHighlighter::create_singleton();
#endif
		} break;

		case NOTIFICATION_PROCESS: {
			if (editor_data.is_scene_changed(-1)) {
				scene_tabs->update_scene_tabs();
			}

			// Update the animation frame of the update spinner.
			uint64_t frame = Engine::get_singleton()->get_frames_drawn();
			uint64_t tick = OS::get_singleton()->get_ticks_msec();

			if (frame != update_spinner_step_frame && (tick - update_spinner_step_msec) > (1000 / 8)) {
				update_spinner_step++;
				if (update_spinner_step >= 8) {
					update_spinner_step = 0;
				}

				update_spinner_step_msec = tick;
				update_spinner_step_frame = frame + 1;

				// Update the icon itself only when the spinner is visible.
				if (_should_display_update_spinner()) {
					update_spinner->set_button_icon(theme->get_icon("Progress" + itos(update_spinner_step + 1), EditorStringName(EditorIcons)));
				}
			}

			editor_selection->update();

			ResourceImporterTexture::get_singleton()->update_imports();

			if (requested_first_scan) {
				requested_first_scan = false;

				OS::get_singleton()->benchmark_begin_measure("Editor", "First Scan");

				EditorFileSystem::get_singleton()->connect("filesystem_changed", callable_mp(this, &EditorNode::_execute_upgrades), CONNECT_ONE_SHOT);
				EditorFileSystem::get_singleton()->scan();
			}

			if (settings_overrides_changed) {
				EditorSettings::get_singleton()->notify_changes();
				EditorSettings::get_singleton()->emit_signal(SNAME("settings_changed"));
				settings_overrides_changed = false;
			}
		} break;

		case NOTIFICATION_ENTER_TREE: {
			get_tree()->set_disable_node_threading(true); // No node threading while running editor.

			Engine::get_singleton()->set_editor_hint(true);

			Window *window = get_window();
			if (window) {
				// Handle macOS fullscreen and extend-to-title changes.
				window->connect("titlebar_changed", callable_mp(this, &EditorNode::_titlebar_resized));
			}

			// Theme has already been created in the constructor, so we can skip that step.
			_update_theme(true);

			OS::get_singleton()->set_low_processor_usage_mode_sleep_usec(int(EDITOR_GET("interface/editor/low_processor_mode_sleep_usec")));

#ifndef PHYSICS_2D_DISABLED
		PhysicsServer2D::get_singleton()->set_active(false);
#endif // PHYSICS_2D_DISABLED

		// No scripting by default if in editor (except for tool).
		ScriptServer::set_scripting_enabled(false);

		if (!DisplayServer::get_singleton()->is_touchscreen_available()) {
			// Only if no touchscreen ui hint, disable emulation just in case.
			Input::get_singleton()->set_emulate_touch_from_mouse(false);
		}
		if (DisplayServer::get_singleton()->has_feature(DisplayServer::FEATURE_CUSTOM_CURSOR_SHAPE)) {
			DisplayServer::get_singleton()->cursor_set_custom_image(Ref<Resource>());
		}
	}

	SceneState::set_disable_placeholders(true);
	ResourceLoader::clear_translation_remaps(); // Using no remaps if in editor.
	ResourceLoader::set_create_missing_resources_if_class_unavailable(true);

	EditorPropertyNameProcessor *epnp = memnew(EditorPropertyNameProcessor);
	add_child(epnp);

	EditorUndoRedoManager::get_singleton()->connect("version_changed", callable_mp(this, &EditorNode::_update_undo_redo_allowed));
	EditorUndoRedoManager::get_singleton()->connect("version_changed", callable_mp(this, &EditorNode::_update_unsaved_cache));
	EditorUndoRedoManager::get_singleton()->connect("history_changed", callable_mp(this, &EditorNode::_update_undo_redo_allowed));
	EditorUndoRedoManager::get_singleton()->connect("history_changed", callable_mp(this, &EditorNode::_update_unsaved_cache));
	ProjectSettings::get_singleton()->connect("settings_changed", callable_mp(this, &EditorNode::_update_from_settings));
	GDExtensionManager::get_singleton()->connect("extensions_reloaded", callable_mp(this, &EditorNode::_gdextensions_reloaded));

	TranslationServer::get_singleton()->get_main_domain()->set_enabled(false);
	// Load settings.
	if (!EditorSettings::get_singleton()) {
		EditorSettings::create();
	}

	ED_SHORTCUT("editor/lock_selected_nodes", TTRC("Lock Selected Node(s)"), KeyModifierMask::CMD_OR_CTRL | Key::L);
	ED_SHORTCUT("editor/unlock_selected_nodes", TTRC("Unlock Selected Node(s)"), KeyModifierMask::CMD_OR_CTRL | KeyModifierMask::SHIFT | Key::L);
	ED_SHORTCUT("editor/group_selected_nodes", TTRC("Group Selected Node(s)"), KeyModifierMask::CMD_OR_CTRL | Key::G);
	ED_SHORTCUT("editor/ungroup_selected_nodes", TTRC("Ungroup Selected Node(s)"), KeyModifierMask::CMD_OR_CTRL | KeyModifierMask::SHIFT | Key::G);

	FileAccess::set_backup_save(EDITOR_GET("filesystem/on_save/safe_save_on_backup_then_rename"));

	_update_vsync_mode();

	// Warm up the project upgrade tool as early as possible.
	project_upgrade_tool = memnew(ProjectUpgradeTool);
	run_project_upgrade_tool = EditorSettings::get_singleton()->get_project_metadata(project_upgrade_tool->META_PROJECT_UPGRADE_TOOL, project_upgrade_tool->META_RUN_ON_RESTART, false);
	if (run_project_upgrade_tool) {
		project_upgrade_tool->begin_upgrade();
	}

	{
		bool agile_input_event_flushing = EDITOR_GET("input/buffering/agile_event_flushing");
		bool use_accumulated_input = EDITOR_GET("input/buffering/use_accumulated_input");

		Input::get_singleton()->set_agile_input_event_flushing(agile_input_event_flushing);
		Input::get_singleton()->set_use_accumulated_input(use_accumulated_input);
	}

	{
		int display_scale = EDITOR_GET("interface/editor/display_scale");

		switch (display_scale) {
			case 0:
				// Try applying a suitable display scale automatically.
				EditorScale::set_scale(EditorSettings::get_auto_display_scale());
				break;
			case 1:
				EditorScale::set_scale(0.75);
				break;
			case 2:
				EditorScale::set_scale(1.0);
				break;
			case 3:
				EditorScale::set_scale(1.25);
				break;
			case 4:
				EditorScale::set_scale(1.5);
				break;
			case 5:
				EditorScale::set_scale(1.75);
				break;
			case 6:
				EditorScale::set_scale(2.0);
				break;
			default:
				EditorScale::set_scale(EDITOR_GET("interface/editor/custom_display_scale"));
				break;
		}
	}

	// Define a minimum window size to prevent UI elements from overlapping or being cut off.
	Window *w = Object::cast_to<Window>(SceneTree::get_singleton()->get_root());
	if (w) {
		const Size2 minimum_size = Size2(1024, 600) * EDSCALE;
		w->set_min_size(minimum_size); // Calling it this early doesn't sync the property with DS.
		DisplayServer::get_singleton()->window_set_min_size(minimum_size);
	}

	FileDialog::set_default_show_hidden_files(EDITOR_GET("filesystem/file_dialog/show_hidden_files"));
	FileDialog::set_default_display_mode(EDITOR_GET("filesystem/file_dialog/display_mode"));
	EditorFileDialog::set_default_show_hidden_files(EDITOR_GET("filesystem/file_dialog/show_hidden_files"));
	EditorFileDialog::set_default_display_mode((EditorFileDialog::DisplayMode)EDITOR_GET("filesystem/file_dialog/display_mode").operator int());

	int swap_cancel_ok = EDITOR_GET("interface/editor/accept_dialog_cancel_ok_buttons");
	if (swap_cancel_ok != 0) { // 0 is auto, set in register_scene based on DisplayServer.
		// Swap on means OK first.
		AcceptDialog::set_swap_cancel_ok(swap_cancel_ok == 2);
	}

	int ed_root_dir = EDITOR_GET("interface/editor/ui_layout_direction");
	Control::set_root_layout_direction(ed_root_dir);
	Window::set_root_layout_direction(ed_root_dir);

	ResourceLoader::set_abort_on_missing_resources(false);
	ResourceLoader::set_error_notify_func(&EditorNode::add_io_error);
	ResourceLoader::set_dependency_error_notify_func(&EditorNode::_dependency_error_report);

	SceneState::set_instantiation_warning_notify_func([](const String &p_warning) {
		add_io_warning(p_warning);
		callable_mp(EditorInterface::get_singleton(), &EditorInterface::mark_scene_as_unsaved).call_deferred();
	});

	{
		// Register importers at the beginning, so dialogs are created with the right extensions.
		Ref<ResourceImporterTexture> import_texture = memnew(ResourceImporterTexture(true));
		ResourceFormatImporter::get_singleton()->add_importer(import_texture);

		Ref<ResourceImporterLayeredTexture> import_cubemap;
		import_cubemap.instantiate();
		import_cubemap->set_mode(ResourceImporterLayeredTexture::MODE_CUBEMAP);
		ResourceFormatImporter::get_singleton()->add_importer(import_cubemap);

		Ref<ResourceImporterLayeredTexture> import_array;
		import_array.instantiate();
		import_array->set_mode(ResourceImporterLayeredTexture::MODE_2D_ARRAY);
		ResourceFormatImporter::get_singleton()->add_importer(import_array);

		Ref<ResourceImporterLayeredTexture> import_cubemap_array;
		import_cubemap_array.instantiate();
		import_cubemap_array->set_mode(ResourceImporterLayeredTexture::MODE_CUBEMAP_ARRAY);
		ResourceFormatImporter::get_singleton()->add_importer(import_cubemap_array);

		Ref<ResourceImporterLayeredTexture> import_3d = memnew(ResourceImporterLayeredTexture(true));
		import_3d->set_mode(ResourceImporterLayeredTexture::MODE_3D);
		ResourceFormatImporter::get_singleton()->add_importer(import_3d);

		Ref<ResourceImporterImage> import_image;
		import_image.instantiate();
		ResourceFormatImporter::get_singleton()->add_importer(import_image);

		Ref<ResourceImporterSVG> import_svg;
		import_svg.instantiate();
		ResourceFormatImporter::get_singleton()->add_importer(import_svg);

		Ref<ResourceImporterTextureAtlas> import_texture_atlas;
		import_texture_atlas.instantiate();
		ResourceFormatImporter::get_singleton()->add_importer(import_texture_atlas);

		Ref<ResourceImporterDynamicFont> import_font_data_dynamic;
		import_font_data_dynamic.instantiate();
		ResourceFormatImporter::get_singleton()->add_importer(import_font_data_dynamic);

		Ref<ResourceImporterBMFont> import_font_data_bmfont;
		import_font_data_bmfont.instantiate();
		ResourceFormatImporter::get_singleton()->add_importer(import_font_data_bmfont);

		Ref<ResourceImporterImageFont> import_font_data_image;
		import_font_data_image.instantiate();
		ResourceFormatImporter::get_singleton()->add_importer(import_font_data_image);

		Ref<ResourceImporterCSVTranslation> import_csv_translation;
		import_csv_translation.instantiate();
		ResourceFormatImporter::get_singleton()->add_importer(import_csv_translation);

		Ref<ResourceImporterWAV> import_wav;
		import_wav.instantiate();
		ResourceFormatImporter::get_singleton()->add_importer(import_wav);

		Ref<ResourceImporterShaderFile> import_shader_file;
		import_shader_file.instantiate();
		ResourceFormatImporter::get_singleton()->add_importer(import_shader_file);

		Ref<ResourceImporterBitMap> import_bitmap;
		import_bitmap.instantiate();
		ResourceFormatImporter::get_singleton()->add_importer(import_bitmap);
	}

	{
		Ref<EditorInspectorDefaultPlugin> eidp;
		eidp.instantiate();
		EditorInspector::add_inspector_plugin(eidp);

		Ref<EditorInspectorVisualShaderModePlugin> smp;
		smp.instantiate();
		EditorInspector::add_inspector_plugin(smp);

		Ref<EditorInspectorParticleProcessMaterialPlugin> ppm;
		ppm.instantiate();
		EditorInspector::add_inspector_plugin(ppm);
	}

	editor_selection = memnew(EditorSelection);

	EditorFileSystem *efs = memnew(EditorFileSystem);
	add_child(efs);

	EditorContextMenuPluginManager::create();

	// Used for previews.
	FileDialog::set_get_icon_callback(callable_mp_static(_file_dialog_get_icon));
	FileDialog::set_get_thumbnail_callback(callable_mp_static(_file_dialog_get_thumbnail));
	FileDialog::register_func = _file_dialog_register;
	FileDialog::unregister_func = _file_dialog_unregister;

	EditorFileDialog::get_icon_func = _file_dialog_get_icon;
	EditorFileDialog::register_func = _editor_file_dialog_register;
	EditorFileDialog::unregister_func = _editor_file_dialog_unregister;

	editor_export = memnew(EditorExport);
	add_child(editor_export);

	// Exporters might need the theme.
	EditorThemeManager::initialize();
	theme = EditorThemeManager::generate_theme();
	DisplayServer::set_early_window_clear_color_override(true, theme->get_color(SNAME("background"), EditorStringName(Editor)));

	register_exporters();

	ED_SHORTCUT("canvas_item_editor/pan_view", TTRC("Pan View"), Key::SPACE);

	const Vector<String> textfile_ext = ((String)(EDITOR_GET("docks/filesystem/textfile_extensions"))).split(",", false);
	for (const String &E : textfile_ext) {
		textfile_extensions.insert(E);
	}
	const Vector<String> other_file_ext = ((String)(EDITOR_GET("docks/filesystem/other_file_extensions"))).split(",", false);
	for (const String &E : other_file_ext) {
		other_file_extensions.insert(E);
	}

	resource_preview = memnew(EditorResourcePreview);
	add_child(resource_preview);
	progress_dialog = memnew(ProgressDialog);
	add_child(progress_dialog);
	progress_dialog->connect(SceneStringName(visibility_changed), callable_mp(this, &EditorNode::_progress_dialog_visibility_changed));

	gui_base = memnew(Panel);
	add_child(gui_base);

	// Take up all screen.
	gui_base->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	gui_base->set_anchor(SIDE_RIGHT, Control::ANCHOR_END);
	gui_base->set_anchor(SIDE_BOTTOM, Control::ANCHOR_END);
	gui_base->set_end(Point2(0, 0));

	main_vbox = memnew(VBoxContainer);

#ifdef ANDROID_ENABLED
	base_vbox = memnew(VBoxContainer);
	base_vbox->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT, Control::PRESET_MODE_MINSIZE, theme->get_constant(SNAME("window_border_margin"), EditorStringName(Editor)));

	title_bar = memnew(EditorTitleBar);
	base_vbox->add_child(title_bar);

	main_hbox = memnew(HBoxContainer);
	main_hbox->add_child(main_vbox);
	main_vbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	main_hbox->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	base_vbox->add_child(main_hbox);

	_touch_actions_panel_mode_changed();

	gui_base->add_child(base_vbox);
#else
	gui_base->add_child(main_vbox);

	title_bar = memnew(EditorTitleBar);
	main_vbox->add_child(title_bar);
#endif

	left_l_hsplit = memnew(DockSplitContainer);
	left_l_hsplit->set_name("DockHSplitLeftL");
	main_vbox->add_child(left_l_hsplit);

	left_l_hsplit->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	left_l_vsplit = memnew(DockSplitContainer);
	left_l_vsplit->set_name("DockVSplitLeftL");
	left_l_vsplit->set_vertical(true);
	left_l_hsplit->add_child(left_l_vsplit);

	TabContainer *dock_slot[EditorDockManager::DOCK_SLOT_MAX];
	dock_slot[EditorDockManager::DOCK_SLOT_LEFT_UL] = memnew(TabContainer);
	dock_slot[EditorDockManager::DOCK_SLOT_LEFT_UL]->set_name("DockSlotLeftUL");
	left_l_vsplit->add_child(dock_slot[EditorDockManager::DOCK_SLOT_LEFT_UL]);
	dock_slot[EditorDockManager::DOCK_SLOT_LEFT_BL] = memnew(TabContainer);
	dock_slot[EditorDockManager::DOCK_SLOT_LEFT_BL]->set_name("DockSlotLeftBL");
	left_l_vsplit->add_child(dock_slot[EditorDockManager::DOCK_SLOT_LEFT_BL]);

	left_r_hsplit = memnew(DockSplitContainer);
	left_r_hsplit->set_name("DockHSplitLeftR");
	left_l_hsplit->add_child(left_r_hsplit);
	left_r_vsplit = memnew(DockSplitContainer);
	left_r_vsplit->set_name("DockVSplitLeftR");
	left_r_vsplit->set_vertical(true);
	left_r_hsplit->add_child(left_r_vsplit);
	dock_slot[EditorDockManager::DOCK_SLOT_LEFT_UR] = memnew(TabContainer);
	dock_slot[EditorDockManager::DOCK_SLOT_LEFT_UR]->set_name("DockSlotLeftUR");
	left_r_vsplit->add_child(dock_slot[EditorDockManager::DOCK_SLOT_LEFT_UR]);
	dock_slot[EditorDockManager::DOCK_SLOT_LEFT_BR] = memnew(TabContainer);
	dock_slot[EditorDockManager::DOCK_SLOT_LEFT_BR]->set_name("DockSlotLeftBR");
	left_r_vsplit->add_child(dock_slot[EditorDockManager::DOCK_SLOT_LEFT_BR]);

	main_hsplit = memnew(DockSplitContainer);
	main_hsplit->set_name("DockHSplitMain");
	left_r_hsplit->add_child(main_hsplit);
	VBoxContainer *center_vb = memnew(VBoxContainer);
	main_hsplit->add_child(center_vb);

	center_vb->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	center_split = memnew(DockSplitContainer);
	center_split->set_name("DockVSplitCenter");
	center_split->set_vertical(true);
	center_split->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	center_split->set_collapsed(true);
	center_vb->add_child(center_split);

	right_hsplit = memnew(DockSplitContainer);
	right_hsplit->set_name("DockHSplitRight");
	main_hsplit->add_child(right_hsplit);

	right_l_vsplit = memnew(DockSplitContainer);
	right_l_vsplit->set_name("DockVSplitRightL");
	right_l_vsplit->set_vertical(true);
	right_hsplit->add_child(right_l_vsplit);
	dock_slot[EditorDockManager::DOCK_SLOT_RIGHT_UL] = memnew(TabContainer);
	dock_slot[EditorDockManager::DOCK_SLOT_RIGHT_UL]->set_name("DockSlotRightUL");
	right_l_vsplit->add_child(dock_slot[EditorDockManager::DOCK_SLOT_RIGHT_UL]);
	dock_slot[EditorDockManager::DOCK_SLOT_RIGHT_BL] = memnew(TabContainer);
	dock_slot[EditorDockManager::DOCK_SLOT_RIGHT_BL]->set_name("DockSlotRightBL");
	right_l_vsplit->add_child(dock_slot[EditorDockManager::DOCK_SLOT_RIGHT_BL]);

	right_r_vsplit = memnew(DockSplitContainer);
	right_r_vsplit->set_name("DockVSplitRightR");
	right_r_vsplit->set_vertical(true);
	right_hsplit->add_child(right_r_vsplit);
	dock_slot[EditorDockManager::DOCK_SLOT_RIGHT_UR] = memnew(TabContainer);
	dock_slot[EditorDockManager::DOCK_SLOT_RIGHT_UR]->set_name("DockSlotRightUR");
	right_r_vsplit->add_child(dock_slot[EditorDockManager::DOCK_SLOT_RIGHT_UR]);
	dock_slot[EditorDockManager::DOCK_SLOT_RIGHT_BR] = memnew(TabContainer);
	dock_slot[EditorDockManager::DOCK_SLOT_RIGHT_BR]->set_name("DockSlotRightBR");
	right_r_vsplit->add_child(dock_slot[EditorDockManager::DOCK_SLOT_RIGHT_BR]);

	editor_dock_manager = memnew(EditorDockManager);

	// Save the splits for easier access.
	editor_dock_manager->add_vsplit(left_l_vsplit);
	editor_dock_manager->add_vsplit(left_r_vsplit);
	editor_dock_manager->add_vsplit(right_l_vsplit);
	editor_dock_manager->add_vsplit(right_r_vsplit);

	editor_dock_manager->add_hsplit(left_l_hsplit);
	editor_dock_manager->add_hsplit(left_r_hsplit);
	editor_dock_manager->add_hsplit(main_hsplit);
	editor_dock_manager->add_hsplit(right_hsplit);

	for (int i = 0; i < EditorDockManager::DOCK_SLOT_MAX; i++) {
		editor_dock_manager->register_dock_slot((EditorDockManager::DockSlot)i, dock_slot[i]);
	}

	editor_layout_save_delay_timer = memnew(Timer);
	add_child(editor_layout_save_delay_timer);
	editor_layout_save_delay_timer->set_wait_time(0.5);
	editor_layout_save_delay_timer->set_one_shot(true);
	editor_layout_save_delay_timer->connect("timeout", callable_mp(this, &EditorNode::_save_editor_layout));

	scan_changes_timer = memnew(Timer);
	scan_changes_timer->set_wait_time(0.5);
	scan_changes_timer->set_autostart(EDITOR_GET("interface/editor/import_resources_when_unfocused"));
	scan_changes_timer->connect("timeout", callable_mp(EditorFileSystem::get_singleton(), &EditorFileSystem::scan_changes));
	add_child(scan_changes_timer);

	top_split = memnew(VSplitContainer);
	center_split->add_child(top_split);
	top_split->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	top_split->set_collapsed(true);

	VBoxContainer *srt = memnew(VBoxContainer);
	srt->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	srt->add_theme_constant_override("separation", 0);
	top_split->add_child(srt);

	scene_tabs = memnew(EditorSceneTabs);
	srt->add_child(scene_tabs);
	scene_tabs->connect("tab_changed", callable_mp(this, &EditorNode::_set_current_scene));
	scene_tabs->connect("tab_closed", callable_mp(this, &EditorNode::_scene_tab_closed));

	distraction_free = memnew(Button);
	distraction_free->set_theme_type_variation("FlatMenuButton");
	ED_SHORTCUT_AND_COMMAND("editor/distraction_free_mode", TTRC("Distraction Free Mode"), KeyModifierMask::CTRL | KeyModifierMask::SHIFT | Key::F11);
	ED_SHORTCUT_OVERRIDE("editor/distraction_free_mode", "macos", KeyModifierMask::META | KeyModifierMask::SHIFT | Key::D);
	ED_SHORTCUT_AND_COMMAND("editor/toggle_last_opened_bottom_panel", TTRC("Toggle Last Opened Bottom Panel"), KeyModifierMask::CMD_OR_CTRL | Key::J);
	distraction_free->set_shortcut(ED_GET_SHORTCUT("editor/distraction_free_mode"));
	distraction_free->set_tooltip_text(TTRC("Toggle distraction-free mode."));
	distraction_free->set_toggle_mode(true);
	scene_tabs->add_extra_button(distraction_free);
	distraction_free->connect(SceneStringName(pressed), callable_mp(this, &EditorNode::_toggle_distraction_free_mode));

	editor_main_screen = memnew(EditorMainScreen);
	editor_main_screen->set_custom_minimum_size(Size2(0, 80) * EDSCALE);
	editor_main_screen->set_draw_behind_parent(true);
	srt->add_child(editor_main_screen);
	editor_main_screen->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	scene_root = memnew(SubViewport);
	scene_root->set_auto_translate_mode(AUTO_TRANSLATE_MODE_ALWAYS);
	scene_root->set_translation_domain(StringName());
	scene_root->set_embedding_subwindows(true);
	// 2D Lite: 全局禁用 3D，无需调用 SubViewport::set_disable_3d。
	scene_root->set_disable_input(true);
	scene_root->set_as_audio_listener_2d(true);

	bool global_menu = !bool(EDITOR_GET("interface/editor/use_embedded_menu")) && NativeMenu::get_singleton()->has_feature(NativeMenu::FEATURE_GLOBAL_MENU);
	bool dark_mode = DisplayServer::get_singleton()->is_dark_mode_supported() && DisplayServer::get_singleton()->is_dark_mode();
	bool can_expand = bool(EDITOR_GET("interface/editor/expand_to_title")) && DisplayServer::get_singleton()->has_feature(DisplayServer::FEATURE_EXTEND_TO_TITLE);

	if (can_expand) {
		// Add spacer to avoid other controls under window minimize/maximize/close buttons (left side).
		left_menu_spacer = memnew(Control);
		left_menu_spacer->set_mouse_filter(Control::MOUSE_FILTER_PASS);
		title_bar->add_child(left_menu_spacer);
	}

	_update_main_menu_type();

	file_menu = memnew(PopupMenu);
	_add_to_main_menu(TTRC("Scene"), file_menu);

	accept = memnew(AcceptDialog);
	accept->set_autowrap(true);
	accept->set_min_size(Vector2i(600, 0));
	accept->set_unparent_when_invisible(true);

	save_accept = memnew(AcceptDialog);
	save_accept->set_unparent_when_invisible(true);
	save_accept->connect(SceneStringName(confirmed), callable_mp(this, &EditorNode::_menu_option).bind((int)MenuOptions::SCENE_SAVE_AS_SCENE));

	project_export = memnew(ProjectExportDialog);
	gui_base->add_child(project_export);

	dependency_error = memnew(DependencyErrorDialog);
	gui_base->add_child(dependency_error);

	dependency_fixer = memnew(DependencyEditor);
	gui_base->add_child(dependency_fixer);

	editor_settings_dialog = memnew(EditorSettingsDialog);
	gui_base->add_child(editor_settings_dialog);

	project_settings_editor = memnew(ProjectSettingsEditor(&editor_data));
	gui_base->add_child(project_settings_editor);

	audio_stream_import_settings = memnew(AudioStreamImportSettingsDialog);
	gui_base->add_child(audio_stream_import_settings);

	fontdata_import_settings = memnew(DynamicFontImportSettingsDialog);
	gui_base->add_child(fontdata_import_settings);

	export_template_manager = memnew(ExportTemplateManager);
	gui_base->add_child(export_template_manager);

	feature_profile_manager = memnew(EditorFeatureProfileManager);
	gui_base->add_child(feature_profile_manager);

	build_profile_manager = memnew(EditorBuildProfileManager);
	gui_base->add_child(build_profile_manager);

	about = memnew(EditorAbout);
	gui_base->add_child(about);
	feature_profile_manager->connect("current_feature_profile_changed", callable_mp(this, &EditorNode::_feature_profile_changed));

#if !defined(ANDROID_ENABLED) && !defined(WEB_ENABLED)
	fbx_importer_manager = memnew(FBXImporterManager);
	gui_base->add_child(fbx_importer_manager);
#endif

	warning = memnew(AcceptDialog);
	warning->set_unparent_when_invisible(true);
	warning->add_button(TTR("Copy Text"), true, "copy");
	warning->connect("custom_action", callable_mp(this, &EditorNode::_copy_warning));

	ED_SHORTCUT("editor/next_tab", TTRC("Next Scene Tab"), KeyModifierMask::CTRL + Key::TAB);
	ED_SHORTCUT("editor/prev_tab", TTRC("Previous Scene Tab"), KeyModifierMask::CTRL + KeyModifierMask::SHIFT + Key::TAB);
	ED_SHORTCUT("editor/filter_files", TTRC("Focus FileSystem Filter"), KeyModifierMask::CMD_OR_CTRL + KeyModifierMask::ALT + Key::P);

	command_palette = EditorCommandPalette::get_singleton();
	command_palette->set_title(TTR("Command Palette"));
	gui_base->add_child(command_palette);

	file_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/new_scene", TTRC("New Scene"), KeyModifierMask::CMD_OR_CTRL + Key::N), SCENE_NEW_SCENE);
	file_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/new_inherited_scene", TTRC("New Inherited Scene..."), KeyModifierMask::CMD_OR_CTRL + KeyModifierMask::SHIFT + Key::N), SCENE_NEW_INHERITED_SCENE);
	file_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/open_scene", TTRC("Open Scene..."), KeyModifierMask::CMD_OR_CTRL + Key::O), SCENE_OPEN_SCENE);
	file_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/reopen_closed_scene", TTRC("Reopen Closed Scene"), KeyModifierMask::CMD_OR_CTRL + KeyModifierMask::SHIFT + Key::T), SCENE_OPEN_PREV);

	recent_scenes = memnew(PopupMenu);
	recent_scenes->set_auto_translate_mode(AUTO_TRANSLATE_MODE_DISABLED);
	file_menu->add_submenu_node_item(TTRC("Open Recent"), recent_scenes, SCENE_OPEN_RECENT);
	recent_scenes->connect(SceneStringName(id_pressed), callable_mp(this, &EditorNode::_open_recent_scene));

	file_menu->add_separator();
	file_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/save_scene", TTRC("Save Scene"), KeyModifierMask::CMD_OR_CTRL + Key::S), SCENE_SAVE_SCENE);
	file_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/save_scene_as", TTRC("Save Scene As..."), KeyModifierMask::CMD_OR_CTRL + KeyModifierMask::SHIFT + Key::S), SCENE_SAVE_AS_SCENE);
	file_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/save_all_scenes", TTRC("Save All Scenes"), KeyModifierMask::CMD_OR_CTRL + KeyModifierMask::SHIFT + KeyModifierMask::ALT + Key::S), SCENE_SAVE_ALL_SCENES);

	file_menu->add_separator();

	file_menu->add_shortcut(ED_SHORTCUT_ARRAY_AND_COMMAND("editor/quick_open", TTRC("Quick Open..."), { int32_t(KeyModifierMask::SHIFT + KeyModifierMask::ALT + Key::O), int32_t(KeyModifierMask::CMD_OR_CTRL + Key::P) }), SCENE_QUICK_OPEN);
	ED_SHORTCUT_OVERRIDE_ARRAY("editor/quick_open", "macos", { int32_t(KeyModifierMask::META + KeyModifierMask::CTRL + Key::O), int32_t(KeyModifierMask::CMD_OR_CTRL + Key::P) });
	file_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/quick_open_scene", TTRC("Quick Open Scene..."), KeyModifierMask::CMD_OR_CTRL + KeyModifierMask::SHIFT + Key::O), SCENE_QUICK_OPEN_SCENE);
	file_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/quick_open_script", TTRC("Quick Open Script..."), KeyModifierMask::CMD_OR_CTRL + KeyModifierMask::ALT + Key::O), SCENE_QUICK_OPEN_SCRIPT);

	file_menu->add_separator();
	export_as_menu = memnew(PopupMenu);
	file_menu->add_submenu_node_item(TTRC("Export As..."), export_as_menu);
	export_as_menu->add_shortcut(ED_SHORTCUT("editor/export_as_mesh_library", TTRC("MeshLibrary...")), FILE_EXPORT_MESH_LIBRARY);
	export_as_menu->connect("index_pressed", callable_mp(this, &EditorNode::_export_as_menu_option));

	file_menu->add_separator();
	file_menu->add_shortcut(ED_GET_SHORTCUT("ui_undo"), SCENE_UNDO, false, true);
	file_menu->add_shortcut(ED_GET_SHORTCUT("ui_redo"), SCENE_REDO, false, true);

	file_menu->add_separator();
	file_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/reload_saved_scene", TTRC("Reload Saved Scene")), SCENE_RELOAD_SAVED_SCENE);
	file_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/close_scene", TTRC("Close Scene"), KeyModifierMask::CMD_OR_CTRL + KeyModifierMask::SHIFT + Key::W), SCENE_CLOSE);
	file_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/close_all_scenes", TTRC("Close All Scenes")), SCENE_CLOSE_ALL);
	ED_SHORTCUT_OVERRIDE("editor/close_scene", "macos", KeyModifierMask::CMD_OR_CTRL + Key::W);

	if (!global_menu || !OS::get_singleton()->has_feature("macos")) {
		// On macOS  "Quit" and "About" options are in the "app" menu.
		file_menu->add_separator();
		file_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/file_quit", TTRC("Quit"), KeyModifierMask::CMD_OR_CTRL + Key::Q), SCENE_QUIT, true);
	}

	ED_SHORTCUT_AND_COMMAND("editor/editor_settings", TTRC("Editor Settings..."));
	ED_SHORTCUT_OVERRIDE("editor/editor_settings", "macos", KeyModifierMask::META + Key::COMMA);
#ifdef MACOS_ENABLED
	if (global_menu && NativeMenu::get_singleton()->has_system_menu(NativeMenu::APPLICATION_MENU_ID)) {
		apple_menu = memnew(PopupMenu);
		apple_menu->set_system_menu(NativeMenu::APPLICATION_MENU_ID);
		main_menu_bar->add_child(apple_menu);

		apple_menu->add_shortcut(ED_GET_SHORTCUT("editor/editor_settings"), EDITOR_OPEN_SETTINGS);
		apple_menu->add_separator();
		apple_menu->connect(SceneStringName(id_pressed), callable_mp(this, &EditorNode::_menu_option));
	}
#endif

	project_menu = memnew(PopupMenu);
	_add_to_main_menu(TTRC("Project"), project_menu);

	project_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/project_settings", TTRC("Project Settings..."), Key::NONE, TTRC("Project Settings")), PROJECT_OPEN_SETTINGS);
	project_menu->connect(SceneStringName(id_pressed), callable_mp(this, &EditorNode::_menu_option));

	ED_SHORTCUT_AND_COMMAND("editor/find_in_files", TTRC("Find in Files..."), KeyModifierMask::CMD_OR_CTRL | KeyModifierMask::SHIFT | Key::F);
	project_menu->add_shortcut(ED_GET_SHORTCUT("editor/find_in_files"), PROJECT_FIND_IN_FILES);

	project_menu->add_separator();
	project_menu->add_item(TTRC("Version Control"), PROJECT_VERSION_CONTROL);

	project_menu->add_separator();
	project_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/export", TTRC("Export..."), Key::NONE, TTRC("Export")), PROJECT_EXPORT);
	project_menu->add_item(TTRC("Pack Project as ZIP..."), PROJECT_PACK_AS_ZIP);
#ifndef ANDROID_ENABLED
	project_menu->add_item(TTRC("Install Android Build Template..."), PROJECT_INSTALL_ANDROID_SOURCE);
	project_menu->add_item(TTRC("Open User Data Folder"), PROJECT_OPEN_USER_DATA_FOLDER);
#endif

	project_menu->add_separator();

	tool_menu = memnew(PopupMenu);
	tool_menu->connect("index_pressed", callable_mp(this, &EditorNode::_tool_menu_option));
	project_menu->add_submenu_node_item(TTRC("Tools"), tool_menu);
	tool_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/orphan_resource_explorer", TTRC("Orphan Resource Explorer...")), TOOLS_ORPHAN_RESOURCES);
	tool_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/engine_compilation_configuration_editor", TTRC("Engine Compilation Configuration Editor...")), TOOLS_BUILD_PROFILE_MANAGER);
	tool_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/upgrade_project", TTRC("Upgrade Project Files...")), TOOLS_PROJECT_UPGRADE);

	project_menu->add_separator();
	project_menu->add_shortcut(ED_SHORTCUT("editor/reload_current_project", TTRC("Reload Current Project")), PROJECT_RELOAD_CURRENT_PROJECT);
	ED_SHORTCUT_AND_COMMAND("editor/quit_to_project_list", TTRC("Quit to Project List"), KeyModifierMask::CTRL + KeyModifierMask::SHIFT + Key::Q);
	ED_SHORTCUT_OVERRIDE("editor/quit_to_project_list", "macos", KeyModifierMask::META + KeyModifierMask::CTRL + KeyModifierMask::ALT + Key::Q);
	project_menu->add_shortcut(ED_GET_SHORTCUT("editor/quit_to_project_list"), PROJECT_QUIT_TO_PROJECT_MANAGER, true);

	// Spacer to center 2D / 3D / Script buttons.
	left_spacer = memnew(HBoxContainer);
	left_spacer->set_mouse_filter(Control::MOUSE_FILTER_PASS);
	left_spacer->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	title_bar->add_child(left_spacer);

	if (can_expand && global_menu) {
		project_title = memnew(Label);
		project_title->add_theme_font_override(SceneStringName(font), theme->get_font(SNAME("bold"), EditorStringName(EditorFonts)));
		project_title->add_theme_font_size_override(SceneStringName(font_size), theme->get_font_size(SNAME("bold_size"), EditorStringName(EditorFonts)));
		project_title->set_text_overrun_behavior(TextServer::OVERRUN_TRIM_ELLIPSIS);
		project_title->set_vertical_alignment(VERTICAL_ALIGNMENT_CENTER);
		project_title->set_h_size_flags(Control::SIZE_EXPAND_FILL);
		project_title->set_mouse_filter(Control::MOUSE_FILTER_PASS);
		left_spacer->add_child(project_title);
	}

	HBoxContainer *main_editor_button_hb = memnew(HBoxContainer);
	main_editor_button_hb->set_mouse_filter(Control::MOUSE_FILTER_STOP);
	main_editor_button_hb->set_name("EditorMainScreenButtons");
	editor_main_screen->set_button_container(main_editor_button_hb);
	title_bar->add_child(main_editor_button_hb);
	title_bar->set_center_control(main_editor_button_hb);

	// Options are added and handled by DebuggerEditorPlugin.
	debug_menu = memnew(PopupMenu);
	_add_to_main_menu(TTRC("Debug"), debug_menu);

	settings_menu = memnew(PopupMenu);
	_add_to_main_menu(TTRC("Editor"), settings_menu);

#ifdef MACOS_ENABLED
	if (!global_menu) {
		settings_menu->add_shortcut(ED_GET_SHORTCUT("editor/editor_settings"), EDITOR_OPEN_SETTINGS);
	}
#else
	settings_menu->add_shortcut(ED_GET_SHORTCUT("editor/editor_settings"), EDITOR_OPEN_SETTINGS);
#endif
	settings_menu->add_shortcut(ED_SHORTCUT("editor/command_palette", TTRC("Command Palette..."), KeyModifierMask::CMD_OR_CTRL | KeyModifierMask::SHIFT | Key::P), EDITOR_COMMAND_PALETTE);
	settings_menu->add_separator();

	settings_menu->add_submenu_node_item(TTRC("Editor Docks"), editor_dock_manager->get_docks_menu());

	editor_layouts = memnew(PopupMenu);
	settings_menu->add_submenu_node_item(TTRC("Editor Layout"), editor_layouts);
	editor_layouts->connect(SceneStringName(id_pressed), callable_mp(this, &EditorNode::_layout_menu_option));
	settings_menu->add_separator();

	ED_SHORTCUT_AND_COMMAND("editor/take_screenshot", TTRC("Take Screenshot"), KeyModifierMask::CTRL | Key::F12);
	ED_SHORTCUT_OVERRIDE("editor/take_screenshot", "macos", KeyModifierMask::META | Key::F12);
	settings_menu->add_shortcut(ED_GET_SHORTCUT("editor/take_screenshot"), EDITOR_TAKE_SCREENSHOT);

	settings_menu->set_item_tooltip(-1, TTRC("Screenshots are stored in the user data folder (\"user://\")."));

	ED_SHORTCUT_AND_COMMAND("editor/fullscreen_mode", TTRC("Toggle Fullscreen"), KeyModifierMask::SHIFT | Key::F11);
	ED_SHORTCUT_OVERRIDE("editor/fullscreen_mode", "macos", KeyModifierMask::META | KeyModifierMask::CTRL | Key::F);
	settings_menu->add_shortcut(ED_GET_SHORTCUT("editor/fullscreen_mode"), EDITOR_TOGGLE_FULLSCREEN);
	settings_menu->add_separator();

#ifndef ANDROID_ENABLED
	if (EditorPaths::get_singleton()->get_data_dir() == EditorPaths::get_singleton()->get_config_dir()) {
		// Configuration and data folders are located in the same place.
		settings_menu->add_item(TTRC("Open Editor Data/Settings Folder"), EDITOR_OPEN_DATA_FOLDER);
	} else {
		// Separate configuration and data folders.
		settings_menu->add_item(TTRC("Open Editor Data Folder"), EDITOR_OPEN_DATA_FOLDER);
		settings_menu->add_item(TTRC("Open Editor Settings Folder"), EDITOR_OPEN_CONFIG_FOLDER);
	}
	settings_menu->add_separator();
#endif

	settings_menu->add_item(TTRC("Manage Editor Features..."), EDITOR_MANAGE_FEATURE_PROFILES);
	settings_menu->add_item(TTRC("Manage Export Templates..."), EDITOR_MANAGE_EXPORT_TEMPLATES);
#if !defined(ANDROID_ENABLED) && !defined(WEB_ENABLED)
	settings_menu->add_item(TTRC("Configure FBX Importer..."), EDITOR_CONFIGURE_FBX_IMPORTER);
#endif

	help_menu = memnew(PopupMenu);
	if (global_menu && NativeMenu::get_singleton()->has_system_menu(NativeMenu::HELP_MENU_ID)) {
		help_menu->set_system_menu(NativeMenu::HELP_MENU_ID);
	}
	_add_to_main_menu(TTRC("Help"), help_menu);

	help_menu->connect(SceneStringName(id_pressed), callable_mp(this, &EditorNode::_menu_option));

	ED_SHORTCUT_AND_COMMAND("editor/editor_help", TTRC("Search Help..."), Key::F1);
	ED_SHORTCUT_OVERRIDE("editor/editor_help", "macos", KeyModifierMask::ALT | Key::SPACE);
	help_menu->add_icon_shortcut(_get_editor_theme_native_menu_icon(SNAME("HelpSearch"), global_menu, dark_mode), ED_GET_SHORTCUT("editor/editor_help"), HELP_SEARCH);
	help_menu->add_separator();
	help_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/online_docs", TTRC("Online Documentation")), HELP_DOCS);
	help_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/forum", TTRC("Forum")), HELP_FORUM);
	help_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/community", TTRC("Community")), HELP_COMMUNITY);
	help_menu->add_separator();
	help_menu->add_icon_shortcut(_get_editor_theme_native_menu_icon(SNAME("ActionCopy"), global_menu, dark_mode), ED_SHORTCUT_AND_COMMAND("editor/copy_system_info", TTRC("Copy System Info")), HELP_COPY_SYSTEM_INFO);
	help_menu->set_item_tooltip(-1, TTR("Copies the system info as a single-line text into the clipboard."));
	help_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/report_a_bug", TTRC("Report a Bug")), HELP_REPORT_A_BUG);
	help_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/suggest_a_feature", TTRC("Suggest a Feature")), HELP_SUGGEST_A_FEATURE);
	help_menu->add_shortcut(ED_SHORTCUT_AND_COMMAND("editor/send_docs_feedback", TTRC("Send Docs Feedback")), HELP_SEND_DOCS_FEEDBACK);
	help_menu->add_separator();
	if (!global_menu || !OS::get_singleton()->has_feature("macos")) {
		// On macOS  "Quit" and "About" options are in the "app" menu.
		help_menu->add_icon_shortcut(_get_editor_theme_native_menu_icon(SNAME("Godot"), global_menu, dark_mode), ED_SHORTCUT_AND_COMMAND("editor/about", TTRC("About Godot...")), HELP_ABOUT);
	}
	help_menu->add_icon_shortcut(_get_editor_theme_native_menu_icon(SNAME("Heart"), global_menu, dark_mode), ED_SHORTCUT_AND_COMMAND("editor/support_development", TTRC("Support Godot Development")), HELP_SUPPORT_GODOT_DEVELOPMENT);

	// Spacer to center 2D / 3D / Script buttons.
	right_spacer = memnew(Control);
	right_spacer->set_mouse_filter(Control::MOUSE_FILTER_PASS);
	right_spacer->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	title_bar->add_child(right_spacer);

	project_run_bar = memnew(EditorRunBar);
	project_run_bar->set_mouse_filter(Control::MOUSE_FILTER_STOP);
	title_bar->add_child(project_run_bar);
	project_run_bar->connect("play_pressed", callable_mp(this, &EditorNode::_project_run_started));
	project_run_bar->connect("stop_pressed", callable_mp(this, &EditorNode::_project_run_stopped));

	right_menu_hb = memnew(HBoxContainer);
	right_menu_hb->set_mouse_filter(Control::MOUSE_FILTER_STOP);
	title_bar->add_child(right_menu_hb);

	renderer = memnew(OptionButton);
	renderer->set_visible(true);
	renderer->set_flat(true);
	renderer->set_theme_type_variation("TopBarOptionButton");
	renderer->set_fit_to_longest_item(false);
	renderer->set_focus_mode(Control::FOCUS_ACCESSIBILITY);
	renderer->set_auto_translate_mode(AUTO_TRANSLATE_MODE_DISABLED);
	renderer->set_tooltip_auto_translate_mode(AUTO_TRANSLATE_MODE_ALWAYS);
	renderer->set_tooltip_text(TTRC("Choose a rendering method.\n\nNotes:\n- On mobile platforms, the Mobile rendering method is used if Forward+ is selected here.\n- On the web platform, the Compatibility rendering method is always used."));
	renderer->set_accessibility_name(TTRC("Rendering Method"));

	right_menu_hb->add_child(renderer);

	if (can_expand) {
		// Add spacer to avoid other controls under the window minimize/maximize/close buttons (right side).
		right_menu_spacer = memnew(Control);
		right_menu_spacer->set_mouse_filter(Control::MOUSE_FILTER_PASS);
		title_bar->add_child(right_menu_spacer);
	}

	String current_renderer_ps = GLOBAL_GET("rendering/renderer/rendering_method");
	current_renderer_ps = current_renderer_ps.to_lower();
	String current_renderer_os = OS::get_singleton()->get_current_rendering_method().to_lower();

	// Add the renderers name to the UI.
	if (current_renderer_ps == current_renderer_os) {
		renderer->connect(SceneStringName(item_selected), callable_mp(this, &EditorNode::_renderer_selected));
		// As we are doing string comparisons, keep in standard case to prevent problems with capitals
		// "vulkan" in particular uses lowercase "v" in the code, and uppercase in the UI.
		PackedStringArray renderers = ProjectSettings::get_singleton()->get_custom_property_info().get(StringName("rendering/renderer/rendering_method")).hint_string.split(",", false);
		for (int i = 0; i < renderers.size(); i++) {
			String rendering_method = renderers[i];
			if (rendering_method == "dummy") {
				continue;
			}
			_add_renderer_entry(rendering_method, false);
			renderer->set_item_metadata(i, rendering_method);
			// Lowercase for standard comparison.
			rendering_method = rendering_method.to_lower();
			if (current_renderer_ps == rendering_method) {
				renderer->select(i);
				renderer_current = i;
			}
		}
	} else {
		// It's an CLI-overridden rendering method.
		_add_renderer_entry(current_renderer_os, true);
		renderer->set_item_metadata(0, current_renderer_os);
		renderer->select(0);
		renderer_current = 0;
	}
	_update_renderer_color();

	video_restart_dialog = memnew(ConfirmationDialog);
	video_restart_dialog->set_ok_button_text(TTR("Save & Restart"));
	video_restart_dialog->connect(SceneStringName(confirmed), callable_mp(this, &EditorNode::_set_renderer_name_save_and_restart));
	gui_base->add_child(video_restart_dialog);

	progress_hb = memnew(BackgroundProgress);

	layout_dialog = memnew(EditorLayoutsDialog);
	gui_base->add_child(layout_dialog);
	layout_dialog->set_hide_on_ok(false);
	layout_dialog->set_size(Size2(225, 270) * EDSCALE);
	layout_dialog->connect("name_confirmed", callable_mp(this, &EditorNode::_dialog_action));

	update_spinner = memnew(MenuButton);
	right_menu_hb->add_child(update_spinner);
	update_spinner->set_button_icon(theme->get_icon(SNAME("Progress1"), EditorStringName(EditorIcons)));
	update_spinner->get_popup()->connect(SceneStringName(id_pressed), callable_mp(this, &EditorNode::_menu_option));
	update_spinner->set_accessibility_name(TTRC("Update Mode"));
	PopupMenu *p = update_spinner->get_popup();
	p->add_radio_check_item(TTRC("Update Continuously"), SPINNER_UPDATE_CONTINUOUSLY);
	p->add_radio_check_item(TTRC("Update When Changed"), SPINNER_UPDATE_WHEN_CHANGED);
	p->add_separator();
	p->add_item(TTRC("Hide Update Spinner"), SPINNER_UPDATE_SPINNER_HIDE);
	_update_update_spinner();

	// Instantiate and place editor docks.

	memnew(SceneTreeDock(scene_root, editor_selection, editor_data));
	editor_dock_manager->add_dock(SceneTreeDock::get_singleton());

	memnew(ImportDock);
	editor_dock_manager->add_dock(ImportDock::get_singleton());

	FileSystemDock *filesystem_dock = memnew(FileSystemDock);
	filesystem_dock->connect("inherit", callable_mp(this, &EditorNode::_inherit_request));
	filesystem_dock->connect("instantiate", callable_mp(this, &EditorNode::_instantiate_request));
	filesystem_dock->connect("display_mode_changed", callable_mp(this, &EditorNode::_save_editor_layout));
	get_project_settings()->connect_filesystem_dock_signals(filesystem_dock);
	editor_dock_manager->add_dock(filesystem_dock);

	memnew(InspectorDock(editor_data));
	editor_dock_manager->add_dock(InspectorDock::get_singleton());

	memnew(NodeDock);
	editor_dock_manager->add_dock(NodeDock::get_singleton());

	history_dock = memnew(HistoryDock);
	editor_dock_manager->add_dock(history_dock);

	// Add some offsets to left_r and main hsplits to make LEFT_R and RIGHT_L docks wider than minsize.
	left_r_hsplit->set_split_offset(270 * EDSCALE);
	main_hsplit->set_split_offset(-270 * EDSCALE);

	// Define corresponding default layout.

	const String docks_section = "docks";
	default_layout.instantiate();
	// Dock numbers are based on DockSlot enum value + 1.
	default_layout->set_value(docks_section, "dock_3", "Scene,Import");
	default_layout->set_value(docks_section, "dock_4", "FileSystem");
	default_layout->set_value(docks_section, "dock_5", "Inspector,Node,History");

	int hsplits[] = { 0, 270, -270, 0 };
	DEV_ASSERT((int)std_size(hsplits) == editor_dock_manager->get_hsplit_count());
	for (int i = 0; i < editor_dock_manager->get_hsplit_count(); i++) {
		default_layout->set_value(docks_section, "dock_hsplit_" + itos(i + 1), hsplits[i]);
	}
	for (int i = 0; i < editor_dock_manager->get_vsplit_count(); i++) {
		default_layout->set_value(docks_section, "dock_split_" + itos(i + 1), 0);
	}

	_update_layouts_menu();

	// Bottom panels.

	bottom_panel = memnew(EditorBottomPanel);
	center_split->add_child(bottom_panel);
	center_split->set_dragger_visibility(SplitContainer::DRAGGER_HIDDEN);

	log = memnew(EditorLog);
	Button *output_button = bottom_panel->add_item(TTRC("Output"), log, ED_SHORTCUT_AND_COMMAND("bottom_panels/toggle_output_bottom_panel", TTRC("Toggle Output Bottom Panel"), KeyModifierMask::ALT | Key::O));
	log->set_tool_button(output_button);

	center_split->connect(SceneStringName(resized), callable_mp(this, &EditorNode::_vp_resized));

	native_shader_source_visualizer = memnew(EditorNativeShaderSourceVisualizer);
	gui_base->add_child(native_shader_source_visualizer);

	orphan_resources = memnew(OrphanResourcesDialog);
	gui_base->add_child(orphan_resources);

	confirmation = memnew(ConfirmationDialog);
	confirmation_button = confirmation->add_button(TTRC("Don't Save"), DisplayServer::get_singleton()->get_swap_cancel_ok(), "discard");
	gui_base->add_child(confirmation);
	confirmation->set_min_size(Vector2(450.0 * EDSCALE, 0));
	confirmation->connect(SceneStringName(confirmed), callable_mp(this, &EditorNode::_menu_confirm_current));
	confirmation->connect("custom_action", callable_mp(this, &EditorNode::_discard_changes));
	confirmation->connect("canceled", callable_mp(this, &EditorNode::_cancel_confirmation));

	save_confirmation = memnew(ConfirmationDialog);
	save_confirmation->add_button(TTRC("Don't Save"), DisplayServer::get_singleton()->get_swap_cancel_ok(), "discard");
	gui_base->add_child(save_confirmation);
	save_confirmation->set_min_size(Vector2(450.0 * EDSCALE, 0));
	save_confirmation->connect(SceneStringName(confirmed), callable_mp(this, &EditorNode::_menu_confirm_current));
	save_confirmation->connect("custom_action", callable_mp(this, &EditorNode::_discard_changes));
	save_confirmation->connect("canceled", callable_mp(this, &EditorNode::_cancel_close_scene_tab));
	save_confirmation->connect("about_to_popup", callable_mp(this, &EditorNode::_prepare_save_confirmation_popup));

	gradle_build_manage_templates = memnew(ConfirmationDialog);
	gradle_build_manage_templates->set_text(TTR("Android build template is missing, please install relevant templates."));
	gradle_build_manage_templates->set_ok_button_text(TTR("Manage Templates"));
	gradle_build_manage_templates->add_button(TTR("Install from file"))->connect(SceneStringName(pressed), callable_mp(this, &EditorNode::_android_install_build_template));
	gradle_build_manage_templates->connect(SceneStringName(confirmed), callable_mp(this, &EditorNode::_menu_option).bind(EDITOR_MANAGE_EXPORT_TEMPLATES));
	gui_base->add_child(gradle_build_manage_templates);

	file_android_build_source = memnew(EditorFileDialog);
	file_android_build_source->set_title(TTR("Select Android sources file"));
	file_android_build_source->set_access(EditorFileDialog::ACCESS_FILESYSTEM);
	file_android_build_source->set_file_mode(EditorFileDialog::FILE_MODE_OPEN_FILE);
	file_android_build_source->add_filter("*.zip");
	file_android_build_source->connect("file_selected", callable_mp(this, &EditorNode::_android_build_source_selected));
	gui_base->add_child(file_android_build_source);

	{
		VBoxContainer *vbox = memnew(VBoxContainer);
		install_android_build_template_message = memnew(Label);
		install_android_build_template_message->set_focus_mode(Control::FOCUS_ACCESSIBILITY);
		install_android_build_template_message->set_autowrap_mode(TextServer::AUTOWRAP_WORD_SMART);
		install_android_build_template_message->set_custom_minimum_size(Size2(300 * EDSCALE, 1));
		vbox->add_child(install_android_build_template_message);

		choose_android_export_profile = memnew(OptionButton);
		choose_android_export_profile->connect(SceneStringName(item_selected), callable_mp(this, &EditorNode::_android_export_preset_selected));
		vbox->add_child(choose_android_export_profile);

		install_android_build_template = memnew(ConfirmationDialog);
		install_android_build_template->set_ok_button_text(TTR("Install"));
		install_android_build_template->connect(SceneStringName(confirmed), callable_mp(this, &EditorNode::_menu_confirm_current));
		install_android_build_template->add_child(vbox);
		install_android_build_template->set_min_size(Vector2(500.0 * EDSCALE, 0));
		gui_base->add_child(install_android_build_template);
	}

	remove_android_build_template = memnew(ConfirmationDialog);
	remove_android_build_template->set_ok_button_text(TTR("Show in File Manager"));
	remove_android_build_template->connect(SceneStringName(confirmed), callable_mp(this, &EditorNode::_android_explore_build_templates));
	gui_base->add_child(remove_android_build_template);

	file_templates = memnew(EditorFileDialog);
	file_templates->set_title(TTR("Import Templates From ZIP File"));

	gui_base->add_child(file_templates);
	file_templates->set_file_mode(EditorFileDialog::FILE_MODE_OPEN_FILE);
	file_templates->set_access(EditorFileDialog::ACCESS_FILESYSTEM);
	file_templates->clear_filters();
	file_templates->add_filter("*.tpz", TTR("Template Package"));

	file = memnew(EditorFileDialog);
	gui_base->add_child(file);
	file->set_current_dir("res://");
	file->set_transient_to_focused(true);

	file_export_lib = memnew(EditorFileDialog);
	file_export_lib->set_title(TTR("Export Library"));
	file_export_lib->set_file_mode(EditorFileDialog::FILE_MODE_SAVE_FILE);
	file_export_lib->connect("file_selected", callable_mp(this, &EditorNode::_dialog_action));
	file_export_lib->add_option(TTR("Merge With Existing"), Vector<String>(), true);
	file_export_lib->add_option(TTR("Apply MeshInstance Transforms"), Vector<String>(), false);
	gui_base->add_child(file_export_lib);

	file_pack_zip = memnew(EditorFileDialog);
	file_pack_zip->connect("file_selected", callable_mp(this, &EditorNode::_dialog_action));
	file_pack_zip->set_file_mode(EditorFileDialog::FILE_MODE_SAVE_FILE);
	file_pack_zip->set_access(EditorFileDialog::ACCESS_FILESYSTEM);
	file_pack_zip->add_filter("*.zip", "ZIP Archive");
	file_pack_zip->set_title(TTR("Pack Project as ZIP..."));
	gui_base->add_child(file_pack_zip);

	file_menu->connect(SceneStringName(id_pressed), callable_mp(this, &EditorNode::_menu_option));
	file_menu->connect("about_to_popup", callable_mp(this, &EditorNode::_update_file_menu_opened));

	settings_menu->connect(SceneStringName(id_pressed), callable_mp(this, &EditorNode::_menu_option));

	file->connect("file_selected", callable_mp(this, &EditorNode::_dialog_action));
	file_templates->connect("file_selected", callable_mp(this, &EditorNode::_dialog_action));

	audio_preview_gen = memnew(AudioStreamPreviewGenerator);
	add_child(audio_preview_gen);

	add_editor_plugin(memnew(DebuggerEditorPlugin(debug_menu)));

	disk_changed = memnew(ConfirmationDialog);
	{
		disk_changed->set_title(TTR("Files have been modified outside Godot"));

		VBoxContainer *vbc = memnew(VBoxContainer);
		disk_changed->add_child(vbc);

		Label *dl = memnew(Label);
		dl->set_text(TTR("The following files are newer on disk:"));
		vbc->add_child(dl);

		disk_changed_list = memnew(Tree);
		disk_changed_list->set_accessibility_name(TTRC("The following files are newer on disk:"));
		vbc->add_child(disk_changed_list);
		disk_changed_list->set_v_size_flags(Control::SIZE_EXPAND_FILL);

		Label *what_action_label = memnew(Label);
		what_action_label->set_text(TTR("What action should be taken?"));
		vbc->add_child(what_action_label);

		disk_changed->connect(SceneStringName(confirmed), callable_mp(this, &EditorNode::_reload_modified_scenes));
		disk_changed->connect(SceneStringName(confirmed), callable_mp(this, &EditorNode::_reload_project_settings));
		disk_changed->set_ok_button_text(TTR("Reload from disk"));

		disk_changed->add_button(TTR("Ignore external changes"), !DisplayServer::get_singleton()->get_swap_cancel_ok(), "resave");
		disk_changed->connect("custom_action", callable_mp(this, &EditorNode::_resave_externally_modified_scenes));
	}

	gui_base->add_child(disk_changed);

	project_data_missing = memnew(ConfirmationDialog);
	project_data_missing->set_text(TTRC("Project data folder (.godot) is missing. Please restart editor."));
	project_data_missing->connect(SceneStringName(confirmed), callable_mp(this, &EditorNode::restart_editor).bind(false));
	project_data_missing->set_ok_button_text(TTRC("Restart"));

	gui_base->add_child(project_data_missing);

	add_editor_plugin(memnew(AnimationPlayerEditorPlugin));
	add_editor_plugin(memnew(AnimationTrackKeyEditEditorPlugin));
	add_editor_plugin(memnew(AnimationMarkerKeyEditEditorPlugin));
	add_editor_plugin(memnew(CanvasItemEditorPlugin));
	add_editor_plugin(memnew(ScriptEditorPlugin));

	if (!Engine::get_singleton()->is_recovery_mode_hint()) {
		add_editor_plugin(get_game_view_plugin());
	}

	EditorAudioBuses *audio_bus_editor = EditorAudioBuses::register_editor();

	ScriptTextEditor::register_editor(); // Register one for text scripts.
	TextEditor::register_editor();

	if (AssetLibraryEditorPlugin::is_available()) {
		add_editor_plugin(memnew(AssetLibraryEditorPlugin));
	} else {
		print_verbose("Asset Library not available (due to using Web editor, or SSL support disabled).");
	}

	// More visually meaningful to have this later.
	bottom_panel->move_item_to_end(AnimationPlayerEditor::get_singleton());

	add_editor_plugin(VersionControlEditorPlugin::get_singleton());

	vcs_actions_menu = VersionControlEditorPlugin::get_singleton()->get_version_control_actions_panel();
	vcs_actions_menu->connect("index_pressed", callable_mp(this, &EditorNode::_version_control_menu_option));
	vcs_actions_menu->add_item(TTRC("Create/Override Version Control Metadata..."), VCS_METADATA);
	vcs_actions_menu->add_item(TTRC("Version Control Settings..."), VCS_SETTINGS);
	project_menu->set_item_submenu_node(project_menu->get_item_index(PROJECT_VERSION_CONTROL), vcs_actions_menu);

	add_editor_plugin(memnew(AudioBusesEditorPlugin(audio_bus_editor)));

	for (int i = 0; i < EditorPlugins::get_plugin_count(); i++) {
		add_editor_plugin(EditorPlugins::create(i));
	}

	for (const StringName &extension_class_name : GDExtensionEditorPlugins::get_extension_classes()) {
		add_extension_editor_plugin(extension_class_name);
	}
	GDExtensionEditorPlugins::editor_node_add_plugin = &EditorNode::add_extension_editor_plugin;
	GDExtensionEditorPlugins::editor_node_remove_plugin = &EditorNode::remove_extension_editor_plugin;

	for (int i = 0; i < plugin_init_callback_count; i++) {
		plugin_init_callbacks[i]();
	}

	resource_preview->add_preview_generator(Ref<EditorTexturePreviewPlugin>(memnew(EditorTexturePreviewPlugin)));
	resource_preview->add_preview_generator(Ref<EditorImagePreviewPlugin>(memnew(EditorImagePreviewPlugin)));
	resource_preview->add_preview_generator(Ref<EditorPackedScenePreviewPlugin>(memnew(EditorPackedScenePreviewPlugin)));
	resource_preview->add_preview_generator(Ref<EditorMaterialPreviewPlugin>(memnew(EditorMaterialPreviewPlugin)));
	resource_preview->add_preview_generator(Ref<EditorScriptPreviewPlugin>(memnew(EditorScriptPreviewPlugin)));
	resource_preview->add_preview_generator(Ref<EditorAudioStreamPreviewPlugin>(memnew(EditorAudioStreamPreviewPlugin)));
	resource_preview->add_preview_generator(Ref<EditorMeshPreviewPlugin>(memnew(EditorMeshPreviewPlugin)));
	resource_preview->add_preview_generator(Ref<EditorBitmapPreviewPlugin>(memnew(EditorBitmapPreviewPlugin)));
	resource_preview->add_preview_generator(Ref<EditorFontPreviewPlugin>(memnew(EditorFontPreviewPlugin)));
	resource_preview->add_preview_generator(Ref<EditorGradientPreviewPlugin>(memnew(EditorGradientPreviewPlugin)));

	{
		Ref<CanvasItemMaterialConversionPlugin> canvas_item_mat_convert;
		canvas_item_mat_convert.instantiate();
		resource_conversion_plugins.push_back(canvas_item_mat_convert);

		Ref<ParticleProcessMaterialConversionPlugin> particles_mat_convert;
		particles_mat_convert.instantiate();
		resource_conversion_plugins.push_back(particles_mat_convert);

		Ref<VisualShaderConversionPlugin> vshader_convert;
		vshader_convert.instantiate();
		resource_conversion_plugins.push_back(vshader_convert);
	}

	update_spinner_step_msec = OS::get_singleton()->get_ticks_msec();
	update_spinner_step_frame = Engine::get_singleton()->get_frames_drawn();

	editor_plugins_over = memnew(EditorPluginList);
	editor_plugins_force_over = memnew(EditorPluginList);
	editor_plugins_force_input_forwarding = memnew(EditorPluginList);

	Ref<GDExtensionExportPlugin> gdextension_export_plugin;
	gdextension_export_plugin.instantiate();

	EditorExport::get_singleton()->add_export_plugin(gdextension_export_plugin);

	Ref<DedicatedServerExportPlugin> dedicated_server_export_plugin;
	dedicated_server_export_plugin.instantiate();

	EditorExport::get_singleton()->add_export_plugin(dedicated_server_export_plugin);

	Ref<ShaderBakerExportPlugin> shader_baker_export_plugin;
	shader_baker_export_plugin.instantiate();

#ifdef VULKAN_ENABLED
	Ref<ShaderBakerExportPluginPlatformVulkan> shader_baker_export_plugin_platform_vulkan;
	shader_baker_export_plugin_platform_vulkan.instantiate();
	shader_baker_export_plugin->add_platform(shader_baker_export_plugin_platform_vulkan);
#endif

#ifdef D3D12_ENABLED
	Ref<ShaderBakerExportPluginPlatformD3D12> shader_baker_export_plugin_platform_d3d12;
	shader_baker_export_plugin_platform_d3d12.instantiate();
	shader_baker_export_plugin->add_platform(shader_baker_export_plugin_platform_d3d12);
#endif

#ifdef METAL_ENABLED
	Ref<ShaderBakerExportPluginPlatformMetal> shader_baker_export_plugin_platform_metal;
	shader_baker_export_plugin_platform_metal.instantiate();
	shader_baker_export_plugin->add_platform(shader_baker_export_plugin_platform_metal);
#endif

	EditorExport::get_singleton()->add_export_plugin(shader_baker_export_plugin);

	Ref<PackedSceneEditorTranslationParserPlugin> packed_scene_translation_parser_plugin;
	packed_scene_translation_parser_plugin.instantiate();
	EditorTranslationParser::get_singleton()->add_parser(packed_scene_translation_parser_plugin, EditorTranslationParser::STANDARD);

	_edit_current();
	saving_resource = Ref<Resource>();

	set_process(true);

	open_imported = memnew(ConfirmationDialog);
	open_imported->set_ok_button_text(TTR("Open Anyway"));
	new_inherited_button = open_imported->add_button(TTR("New Inherited"), !DisplayServer::get_singleton()->get_swap_cancel_ok(), "inherit");
	open_imported->connect(SceneStringName(confirmed), callable_mp(this, &EditorNode::_open_imported));
	open_imported->connect("custom_action", callable_mp(this, &EditorNode::_inherit_imported));
	gui_base->add_child(open_imported);

	quick_open_dialog = memnew(EditorQuickOpenDialog);
	gui_base->add_child(quick_open_dialog);

	quick_open_color_palette = memnew(EditorQuickOpenDialog);
	gui_base->add_child(quick_open_color_palette);

	_update_recent_scenes();

	set_process_shortcut_input(true);

	load_errors = memnew(RichTextLabel);
	load_error_dialog = memnew(AcceptDialog);
	load_error_dialog->set_unparent_when_invisible(true);
	load_error_dialog->add_child(load_errors);
	load_error_dialog->set_title(TTR("Load Errors"));
	load_error_dialog->connect(SceneStringName(visibility_changed), callable_mp(this, &EditorNode::_load_error_dialog_visibility_changed));

	execute_outputs = memnew(RichTextLabel);
	execute_outputs->set_selection_enabled(true);
	execute_outputs->set_context_menu_enabled(true);
	execute_output_dialog = memnew(AcceptDialog);
	execute_output_dialog->set_unparent_when_invisible(true);
	execute_output_dialog->add_child(execute_outputs);
	execute_output_dialog->set_title("");

	EditorFileSystem::get_singleton()->connect("sources_changed", callable_mp(this, &EditorNode::_sources_changed));
	EditorFileSystem::get_singleton()->connect("filesystem_changed", callable_mp(this, &EditorNode::_fs_changed));
	EditorFileSystem::get_singleton()->connect("resources_reimporting", callable_mp(this, &EditorNode::_resources_reimporting));
	EditorFileSystem::get_singleton()->connect("resources_reimported", callable_mp(this, &EditorNode::_resources_reimported));
	EditorFileSystem::get_singleton()->connect("resources_reload", callable_mp(this, &EditorNode::_resources_changed));

	_build_icon_type_cache();

	pick_main_scene = memnew(ConfirmationDialog);
	gui_base->add_child(pick_main_scene);
	pick_main_scene->set_ok_button_text(TTR("Select"));
	pick_main_scene->connect(SceneStringName(confirmed), callable_mp(this, &EditorNode::_menu_option).bind(SETTINGS_PICK_MAIN_SCENE));
	select_current_scene_button = pick_main_scene->add_button(TTR("Select Current"), true, "select_current");
	pick_main_scene->connect("custom_action", callable_mp(this, &EditorNode::_pick_main_scene_custom_action));

	open_project_settings = memnew(ConfirmationDialog);
	gui_base->add_child(open_project_settings);
	open_project_settings->set_ok_button_text(TTRC("Open Project Settings"));
	open_project_settings->connect(SceneStringName(confirmed), callable_mp(this, &EditorNode::_menu_option).bind(PROJECT_OPEN_SETTINGS));

	for (int i = 0; i < _init_callbacks.size(); i++) {
		_init_callbacks[i]();
	}

	editor_data.add_edited_scene(-1);
	editor_data.set_edited_scene(0);
	scene_tabs->update_scene_tabs();

	ImportDock::get_singleton()->initialize_import_options();

	FileAccess::set_file_close_fail_notify_callback(_file_access_close_error_notify);

	print_handler.printfunc = _print_handler;
	print_handler.userdata = this;
	add_print_handler(&print_handler);

	ResourceSaver::set_save_callback(_resource_saved);
	ResourceLoader::set_load_callback(_resource_loaded);

	// Use the Ctrl modifier so F2 can be used to rename nodes in the scene tree dock.
	ED_SHORTCUT_AND_COMMAND("editor/editor_2d", TTRC("Open 2D Editor"), KeyModifierMask::CTRL | Key::F1);
	ED_SHORTCUT_AND_COMMAND("editor/editor_3d", TTRC("Open 3D Editor"), KeyModifierMask::CTRL | Key::F2);
	ED_SHORTCUT_AND_COMMAND("editor/editor_script", TTRC("Open Script Editor"), KeyModifierMask::CTRL | Key::F3);
	ED_SHORTCUT_AND_COMMAND("editor/editor_game", TTRC("Open Game View"), KeyModifierMask::CTRL | Key::F4);
	ED_SHORTCUT_AND_COMMAND("editor/editor_assetlib", TTRC("Open Asset Library"), KeyModifierMask::CTRL | Key::F5);

	ED_SHORTCUT_OVERRIDE("editor/editor_2d", "macos", KeyModifierMask::META | KeyModifierMask::CTRL | Key::KEY_1);
	ED_SHORTCUT_OVERRIDE("editor/editor_3d", "macos", KeyModifierMask::META | KeyModifierMask::CTRL | Key::KEY_2);
	ED_SHORTCUT_OVERRIDE("editor/editor_script", "macos", KeyModifierMask::META | KeyModifierMask::CTRL | Key::KEY_3);
	ED_SHORTCUT_OVERRIDE("editor/editor_game", "macos", KeyModifierMask::META | KeyModifierMask::CTRL | Key::KEY_4);
	ED_SHORTCUT_OVERRIDE("editor/editor_assetlib", "macos", KeyModifierMask::META | KeyModifierMask::CTRL | Key::KEY_5);

	ED_SHORTCUT_AND_COMMAND("editor/editor_next", TTRC("Open the next Editor"));
	ED_SHORTCUT_AND_COMMAND("editor/editor_prev", TTRC("Open the previous Editor"));

	screenshot_timer = memnew(Timer);
	screenshot_timer->set_one_shot(true);
	screenshot_timer->set_wait_time(settings_menu->get_submenu_popup_delay() + 0.1f);
	screenshot_timer->connect("timeout", callable_mp(this, &EditorNode::_request_screenshot));
	add_child(screenshot_timer);
	screenshot_timer->set_owner(get_owner());

	// Adjust spacers to center 2D / 3D / Script buttons.
	if (main_menu_button != nullptr) {
		int max_w = MAX(project_run_bar->get_minimum_size().x + right_menu_hb->get_minimum_size().x, main_menu_button->get_minimum_size().x);
		left_spacer->set_custom_minimum_size(Size2(MAX(0, max_w - main_menu_button->get_minimum_size().x), 0));
		right_spacer->set_custom_minimum_size(Size2(MAX(0, max_w - project_run_bar->get_minimum_size().x - right_menu_hb->get_minimum_size().x), 0));
	} else {
		int max_w = MAX(project_run_bar->get_minimum_size().x + right_menu_hb->get_minimum_size().x, main_menu_bar->get_minimum_size().x);
		left_spacer->set_custom_minimum_size(Size2(MAX(0, max_w - main_menu_bar->get_minimum_size().x), 0));
		right_spacer->set_custom_minimum_size(Size2(MAX(0, max_w - project_run_bar->get_minimum_size().x - right_menu_hb->get_minimum_size().x), 0));
	}
	// Extend menu bar to window title.
	if (can_expand) {
		DisplayServer::get_singleton()->process_events();
		DisplayServer::get_singleton()->window_set_flag(DisplayServer::WINDOW_FLAG_EXTEND_TO_TITLE, true, DisplayServer::MAIN_WINDOW_ID);
		title_bar->set_can_move_window(true);
	}

	{
		const String exec = OS::get_singleton()->get_executable_path();
		const String old_exec = EditorSettings::get_singleton()->get_project_metadata("editor_metadata", "executable_path", "");
		// Save editor executable path for third-party tools.
		if (exec != old_exec) {
			EditorSettings::get_singleton()->set_project_metadata("editor_metadata", "executable_path", exec);
		}
	}

	follow_system_theme = EDITOR_GET("interface/theme/follow_system_theme");
	use_system_accent_color = EDITOR_GET("interface/theme/use_system_accent_color");
	system_theme_timer = memnew(Timer);
	system_theme_timer->set_wait_time(1.0);
	system_theme_timer->connect("timeout", callable_mp(this, &EditorNode::_check_system_theme_changed));
	add_child(system_theme_timer);
	system_theme_timer->set_owner(get_owner());
	system_theme_timer->set_autostart(true);
}

EditorNode::~EditorNode() {
	EditorInspector::cleanup_plugins();
	EditorTranslationParser::get_singleton()->clean_parsers();
	EditorContextMenuPluginManager::cleanup();

	remove_print_handler(&print_handler);
	EditorHelp::cleanup_doc();
#if defined(MODULE_GDSCRIPT_ENABLED) || defined(MODULE_MONO_ENABLED)
	EditorHelpHighlighter::free_singleton();
#endif
	memdelete(editor_selection);
	memdelete(editor_plugins_over);
	memdelete(editor_plugins_force_over);
	memdelete(editor_plugins_force_input_forwarding);
	memdelete(progress_hb);
	memdelete(project_upgrade_tool);
	memdelete(editor_dock_manager);

	EditorSettings::destroy();
	EditorThemeManager::finalize();

	GDExtensionEditorPlugins::editor_node_add_plugin = nullptr;
	GDExtensionEditorPlugins::editor_node_remove_plugin = nullptr;

	FileDialog::register_func = nullptr;
	FileDialog::unregister_func = nullptr;

	EditorFileDialog::get_icon_func = nullptr;
	EditorFileDialog::register_func = nullptr;
	EditorFileDialog::unregister_func = nullptr;

	file_dialogs.clear();
	editor_file_dialogs.clear();

	singleton = nullptr;
}

/*
 * EDITOR PLUGIN LIST
 */

void EditorPluginList::make_visible(bool p_visible) {
	for (int i = 0; i < plugins_list.size(); i++) {
		plugins_list[i]->make_visible(p_visible);
	}
}

void EditorPluginList::edit(Object *p_object) {
	for (int i = 0; i < plugins_list.size(); i++) {
		plugins_list[i]->edit(p_object);
	}
}

bool EditorPluginList::forward_gui_input(const Ref<InputEvent> &p_event) {
	bool discard = false;

	for (int i = 0; i < plugins_list.size(); i++) {
		if (plugins_list[i]->forward_canvas_gui_input(p_event)) {
			discard = true;
		}
	}

	return discard;
}

void EditorPluginList::forward_canvas_draw_over_viewport(Control *p_overlay) {
	for (int i = 0; i < plugins_list.size(); i++) {
		plugins_list[i]->forward_canvas_draw_over_viewport(p_overlay);
	}
}

void EditorPluginList::forward_canvas_force_draw_over_viewport(Control *p_overlay) {
	for (int i = 0; i < plugins_list.size(); i++) {
		plugins_list[i]->forward_canvas_force_draw_over_viewport(p_overlay);
	}
}

void EditorPluginList::add_plugin(EditorPlugin *p_plugin) {
	ERR_FAIL_COND(plugins_list.has(p_plugin));
	plugins_list.push_back(p_plugin);
}

void EditorPluginList::remove_plugin(EditorPlugin *p_plugin) {
	plugins_list.erase(p_plugin);
}

bool EditorPluginList::is_empty() {
	return plugins_list.is_empty();
}

void EditorPluginList::clear() {
	plugins_list.clear();
}
