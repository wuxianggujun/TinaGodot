/**************************************************************************/
/*  flow_graph_editor_plugin.cpp                                          */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/

#include "flow_graph_editor_plugin.h"
#include "flow_graph_editor.h"
#include "editor/editor_interface.h"

FlowGraphEditorPlugin::FlowGraphEditorPlugin() {
	flow_graph_editor = memnew(FlowGraphEditor);
	flow_graph_editor->hide();
	EditorInterface::get_singleton()->get_editor_main_screen()->add_child(flow_graph_editor);
}

FlowGraphEditorPlugin::~FlowGraphEditorPlugin() {
	if (flow_graph_editor) {
		memdelete(flow_graph_editor);
	}
}

void FlowGraphEditorPlugin::make_visible(bool p_visible) {
	if (flow_graph_editor) {
		flow_graph_editor->set_visible(p_visible);
	}
}

void FlowGraphEditorPlugin::edit(Object *p_object) {
	// TODO: Handle editing FlowGraph resources
}

bool FlowGraphEditorPlugin::handles(Object *p_object) const {
	// TODO: Return true if object is a FlowGraph resource
	return false;
}
