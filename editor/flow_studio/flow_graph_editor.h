/**************************************************************************/
/*  flow_graph_editor.h                                                   */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/

#ifndef FLOW_GRAPH_EDITOR_H
#define FLOW_GRAPH_EDITOR_H

#include "editor/flow_studio/core/flow_graph.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/graph_edit.h"
#include "scene/gui/panel_container.h"
#include "scene/gui/split_container.h"
#include "scene/gui/tree.h"

class FlowGraphNode;

class FlowGraphEditor : public VBoxContainer {
	GDCLASS(FlowGraphEditor, VBoxContainer);

private:
	// UI Components
	HBoxContainer *toolbar = nullptr;
	Button *btn_new = nullptr;
	Button *btn_open = nullptr;
	Button *btn_save = nullptr;
	Button *btn_run = nullptr;
	Button *btn_stop = nullptr;
	
	HSplitContainer *main_split = nullptr;
	PanelContainer *node_library_panel = nullptr;
	Tree *node_library_tree = nullptr;
	GraphEdit *graph_edit = nullptr;
	
	// Data
	Ref<FlowGraph> current_graph;
	HashMap<int, FlowGraphNode *> graph_nodes; // node_id -> GraphNode
	
	// Node creation
	String dragging_node_type;
	
	void _setup_ui();
	void _setup_toolbar();
	void _setup_node_library();
	void _populate_node_library();
	void _setup_graph_edit();
	
	// Toolbar callbacks
	void _on_new_pressed();
	void _on_open_pressed();
	void _on_save_pressed();
	void _on_run_pressed();
	void _on_stop_pressed();
	
	// Node library callbacks
	void _on_node_library_item_selected();
	void _on_node_library_item_activated();
	
	// Graph edit callbacks
	void _on_connection_request(const StringName &p_from, int p_from_port, const StringName &p_to, int p_to_port);
	void _on_disconnection_request(const StringName &p_from, int p_from_port, const StringName &p_to, int p_to_port);
	void _on_popup_request(const Vector2 &p_position);
	void _on_node_selected(Node *p_node);
	void _on_delete_nodes_request(const TypedArray<StringName> &p_nodes);
	
	// Node management
	void _create_graph_node(const String &p_node_type, const Vector2 &p_position);
	void _update_graph_view();
	void _clear_graph_view();
	
	// File operations
	void _new_graph();
	void _load_graph(const String &p_path);
	void _save_graph(const String &p_path);

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_graph(const Ref<FlowGraph> &p_graph);
	Ref<FlowGraph> get_graph() const { return current_graph; }
	
	FlowGraphEditor();
	~FlowGraphEditor();
};

#endif // FLOW_GRAPH_EDITOR_H
