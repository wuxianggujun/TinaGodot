/**************************************************************************/
/*  flow_graph_editor_plugin.h                                            */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/

#ifndef FLOW_GRAPH_EDITOR_PLUGIN_H
#define FLOW_GRAPH_EDITOR_PLUGIN_H

#include "editor/plugins/editor_plugin.h"

class FlowGraphEditor;

class FlowGraphEditorPlugin : public EditorPlugin {
	GDCLASS(FlowGraphEditorPlugin, EditorPlugin);

private:
	FlowGraphEditor *flow_graph_editor = nullptr;

public:
	virtual String get_plugin_name() const override { return "FlowGraph"; }
	virtual bool has_main_screen() const override { return true; }
	virtual void make_visible(bool p_visible) override;
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;

	FlowGraphEditorPlugin();
	~FlowGraphEditorPlugin();
};

#endif // FLOW_GRAPH_EDITOR_PLUGIN_H
