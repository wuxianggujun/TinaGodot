/**************************************************************************/
/*  material_editor_plugin.cpp                                            */
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

#include "material_editor_plugin.h"

#include "core/config/project_settings.h"
#include "editor/editor_node.h"
#include "editor/editor_string_names.h"
#include "editor/editor_undo_redo_manager.h"
#include "editor/settings/editor_settings.h"
#include "editor/themes/editor_scale.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/color_rect.h"
#include "scene/gui/label.h"
#include "scene/gui/subviewport_container.h"
#include "scene/main/viewport.h"
#include "scene/resources/canvas_item_material.h"
#include "scene/resources/particle_process_material.h"

// 3D classes removed in 2D Lite version

Ref<ShaderMaterial> MaterialEditor::make_shader_material(const Ref<Material> &p_from, bool p_copy_params) {
	ERR_FAIL_COND_V(p_from.is_null(), Ref<ShaderMaterial>());
	ERR_FAIL_COND_V(!p_from->_is_initialized(), Ref<ShaderMaterial>());

	Ref<ShaderMaterial> smat;
	smat.instantiate();

	Ref<Shader> shader;
	shader.instantiate();

	String code = RS::get_singleton()->shader_get_code(p_from->get_shader_rid());
	shader->set_code(code);
	smat->set_shader(shader);

	if (p_copy_params) {
		List<PropertyInfo> params;
		RS::get_singleton()->get_shader_parameter_list(p_from->get_shader_rid(), &params);

		for (const PropertyInfo &E : params) {
			Variant value = RS::get_singleton()->material_get_param(p_from->get_rid(), E.name);
			smat->set_shader_parameter(E.name, value);
		}
	}

	smat->set_render_priority(p_from->get_render_priority());
	smat->set_local_to_scene(p_from->is_local_to_scene());
	smat->set_name(p_from->get_name());
	return smat;
}

void MaterialEditor::gui_input(const Ref<InputEvent> &p_event) {
	// 2D Lite: 3D 材质旋转交互已移除
}

void MaterialEditor::_update_theme_item_cache() {
	Control::_update_theme_item_cache();
	theme_cache.checkerboard = get_editor_theme_icon(SNAME("Checkerboard"));
}

void MaterialEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_THEME_CHANGED: {
			error_label->add_theme_color_override(SceneStringName(font_color), get_theme_color(SNAME("error_color"), EditorStringName(Editor)));
		} break;

		case NOTIFICATION_DRAW: {
			if (!is_unsupported_shader_mode) {
				Size2 size = get_size();
				draw_texture_rect(theme_cache.checkerboard, Rect2(Point2(), size), true);
			}
		} break;
	}
}

void MaterialEditor::edit(Ref<Material> p_material) {
	material = p_material;

	is_unsupported_shader_mode = false;
	if (material.is_valid()) {
		Shader::Mode mode = p_material->get_shader_mode();
		if (mode == Shader::MODE_CANVAS_ITEM) {
			layout_error->hide();
			layout_2d->show();
			rect_instance->set_material(material);
			vc->hide();
		} else {
			// 不支持的着色器模式 (仅支持 2D Canvas)
			layout_error->show();
			layout_2d->hide();
			is_unsupported_shader_mode = true;
			vc->hide();
		}
	} else {
		hide();
	}
}

MaterialEditor::MaterialEditor() {
	set_custom_minimum_size(Size2(1, 150) * EDSCALE);

	// Canvas item

	vc_2d = memnew(SubViewportContainer);
	vc_2d->set_stretch(true);
	add_child(vc_2d);
	vc_2d->set_anchors_and_offsets_preset(PRESET_FULL_RECT);

	viewport_2d = memnew(SubViewport);
	vc_2d->add_child(viewport_2d);
	viewport_2d->set_disable_input(true);
	viewport_2d->set_transparent_background(true);

	layout_2d = memnew(HBoxContainer);
	layout_2d->set_alignment(BoxContainer::ALIGNMENT_CENTER);
	viewport_2d->add_child(layout_2d);
	layout_2d->set_anchors_and_offsets_preset(PRESET_FULL_RECT);

	rect_instance = memnew(ColorRect);
	layout_2d->add_child(rect_instance);
	rect_instance->set_custom_minimum_size(Size2(150, 150) * EDSCALE);

	layout_2d->set_visible(false);

	layout_error = memnew(VBoxContainer);
	layout_error->set_alignment(BoxContainer::ALIGNMENT_CENTER);
	layout_error->set_anchors_and_offsets_preset(PRESET_FULL_RECT);

	error_label = memnew(Label);
	error_label->set_focus_mode(FOCUS_ACCESSIBILITY);
	error_label->set_text(TTR("Preview is not available for this shader mode."));
	error_label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
	error_label->set_vertical_alignment(VERTICAL_ALIGNMENT_CENTER);
	error_label->set_autowrap_mode(TextServer::AUTOWRAP_WORD_SMART);

	layout_error->add_child(error_label);
	layout_error->hide();
	add_child(layout_error);
}

///////////////////////

bool EditorInspectorPluginMaterial::can_handle(Object *p_object) {
	Material *material = Object::cast_to<Material>(p_object);
	if (!material) {
		return false;
	}
	Shader::Mode mode = material->get_shader_mode();
	return mode == Shader::MODE_SPATIAL || mode == Shader::MODE_CANVAS_ITEM;
}

void EditorInspectorPluginMaterial::parse_begin(Object *p_object) {
	Material *material = Object::cast_to<Material>(p_object);
	if (!material) {
		return;
	}
	Ref<Material> m(material);

	MaterialEditor *editor = memnew(MaterialEditor);
	editor->edit(m);
	add_custom_control(editor);
}

void EditorInspectorPluginMaterial::_undo_redo_inspector_callback(Object *p_undo_redo, Object *p_edited, const String &p_property, const Variant &p_new_value) {
	EditorUndoRedoManager *undo_redo = Object::cast_to<EditorUndoRedoManager>(p_undo_redo);
	ERR_FAIL_NULL(undo_redo);

	// For BaseMaterial3D, if a roughness or metallic textures is being assigned to an empty slot,
	// set the respective metallic or roughness factor to 1.0 as a convenience feature
	BaseMaterial3D *base_material = Object::cast_to<StandardMaterial3D>(p_edited);
	if (base_material) {
		Texture2D *texture = Object::cast_to<Texture2D>(p_new_value);
		if (texture) {
			if (p_property == "roughness_texture") {
				if (base_material->get_texture(StandardMaterial3D::TEXTURE_ROUGHNESS).is_null()) {
					undo_redo->add_do_property(p_edited, "roughness", 1.0);

					bool valid = false;
					Variant value = p_edited->get("roughness", &valid);
					if (valid) {
						undo_redo->add_undo_property(p_edited, "roughness", value);
					}
				}
			} else if (p_property == "metallic_texture") {
				if (base_material->get_texture(StandardMaterial3D::TEXTURE_METALLIC).is_null()) {
					undo_redo->add_do_property(p_edited, "metallic", 1.0);

					bool valid = false;
					Variant value = p_edited->get("metallic", &valid);
					if (valid) {
						undo_redo->add_undo_property(p_edited, "metallic", value);
					}
				}
			}
		}
	}
}

EditorInspectorPluginMaterial::EditorInspectorPluginMaterial() {
	EditorNode::get_editor_data().add_undo_redo_inspector_hook_callback(callable_mp(this, &EditorInspectorPluginMaterial::_undo_redo_inspector_callback));
}

MaterialEditorPlugin::MaterialEditorPlugin() {
	Ref<EditorInspectorPluginMaterial> plugin;
	plugin.instantiate();
	add_inspector_plugin(plugin);
}

String ParticleProcessMaterialConversionPlugin::converts_to() const {
	return "ShaderMaterial";
}

bool ParticleProcessMaterialConversionPlugin::handles(const Ref<Resource> &p_resource) const {
	Ref<ParticleProcessMaterial> mat = p_resource;
	return mat.is_valid();
}

Ref<Resource> ParticleProcessMaterialConversionPlugin::convert(const Ref<Resource> &p_resource) const {
	return MaterialEditor::make_shader_material(p_resource);
}

String CanvasItemMaterialConversionPlugin::converts_to() const {
	return "ShaderMaterial";
}

bool CanvasItemMaterialConversionPlugin::handles(const Ref<Resource> &p_resource) const {
	Ref<CanvasItemMaterial> mat = p_resource;
	return mat.is_valid();
}

Ref<Resource> CanvasItemMaterialConversionPlugin::convert(const Ref<Resource> &p_resource) const {
	return MaterialEditor::make_shader_material(p_resource);
}
