/**************************************************************************/
/*  flow_graph_editor.cpp                                                 */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/

#include "flow_graph_editor.h"
#include "flow_graph_node.h"
#include "editor/flow_studio/core/flow_connection.h"
#include "editor/flow_studio/core/flow_node.h"
#include "editor/themes/editor_scale.h"
#include "scene/gui/label.h"
#include "scene/gui/separator.h"

FlowGraphEditor::FlowGraphEditor() {
	set_v_size_flags(SIZE_EXPAND_FILL);
	set_h_size_flags(SIZE_EXPAND_FILL);
	
	_setup_ui();
	_new_graph();
}

FlowGraphEditor::~FlowGraphEditor() {
}

void FlowGraphEditor::_bind_methods() {
}

void FlowGraphEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			_populate_node_library();
		} break;
	}
}

void FlowGraphEditor::_setup_ui() {
	// Toolbar
	_setup_toolbar();
	
	// Main split container
	main_split = memnew(HSplitContainer);
	main_split->set_v_size_flags(SIZE_EXPAND_FILL);
	add_child(main_split);
	
	// Node library (left panel)
	_setup_node_library();
	
	// Graph edit (main area)
	_setup_graph_edit();
}

void FlowGraphEditor::_setup_toolbar() {
	toolbar = memnew(HBoxContainer);
	add_child(toolbar);
	
	btn_new = memnew(Button);
	btn_new->set_text(TTR("New"));
	btn_new->set_tooltip_text(TTR("Create a new flow graph"));
	btn_new->connect("pressed", callable_mp(this, &FlowGraphEditor::_on_new_pressed));
	toolbar->add_child(btn_new);
	
	btn_open = memnew(Button);
	btn_open->set_text(TTR("Open"));
	btn_open->set_tooltip_text(TTR("Open an existing flow graph"));
	btn_open->connect("pressed", callable_mp(this, &FlowGraphEditor::_on_open_pressed));
	toolbar->add_child(btn_open);
	
	btn_save = memnew(Button);
	btn_save->set_text(TTR("Save"));
	btn_save->set_tooltip_text(TTR("Save the current flow graph"));
	btn_save->connect("pressed", callable_mp(this, &FlowGraphEditor::_on_save_pressed));
	toolbar->add_child(btn_save);
	
	toolbar->add_child(memnew(VSeparator));
	
	btn_run = memnew(Button);
	btn_run->set_text(TTR("Run"));
	btn_run->set_tooltip_text(TTR("Run the flow graph"));
	btn_run->connect("pressed", callable_mp(this, &FlowGraphEditor::_on_run_pressed));
	toolbar->add_child(btn_run);
	
	btn_stop = memnew(Button);
	btn_stop->set_text(TTR("Stop"));
	btn_stop->set_tooltip_text(TTR("Stop execution"));
	btn_stop->set_disabled(true);
	btn_stop->connect("pressed", callable_mp(this, &FlowGraphEditor::_on_stop_pressed));
	toolbar->add_child(btn_stop);
}

void FlowGraphEditor::_setup_node_library() {
	node_library_panel = memnew(PanelContainer);
	node_library_panel->set_custom_minimum_size(Size2(200, 0) * EDSCALE);
	main_split->add_child(node_library_panel);
	
	VBoxContainer *library_vbox = memnew(VBoxContainer);
	node_library_panel->add_child(library_vbox);
	
	Label *library_label = memnew(Label);
	library_label->set_text(TTR("Node Library"));
	library_vbox->add_child(library_label);
	
	node_library_tree = memnew(Tree);
	node_library_tree->set_v_size_flags(SIZE_EXPAND_FILL);
	node_library_tree->set_hide_root(true);
	node_library_tree->connect("item_selected", callable_mp(this, &FlowGraphEditor::_on_node_library_item_selected));
	node_library_tree->connect("item_activated", callable_mp(this, &FlowGraphEditor::_on_node_library_item_activated));
	library_vbox->add_child(node_library_tree);
}

void FlowGraphEditor::_populate_node_library() {
	node_library_tree->clear();
	TreeItem *root = node_library_tree->create_item();
	
	// Event nodes
	TreeItem *event_category = node_library_tree->create_item(root);
	event_category->set_text(0, TTR("Event"));
	event_category->set_selectable(0, false);
	
	TreeItem *start_node = node_library_tree->create_item(event_category);
	start_node->set_text(0, TTR("Start"));
	start_node->set_metadata(0, "event.start");
	
	// I/O nodes
	TreeItem *io_category = node_library_tree->create_item(root);
	io_category->set_text(0, TTR("Input/Output"));
	io_category->set_selectable(0, false);
	
	TreeItem *print_node = node_library_tree->create_item(io_category);
	print_node->set_text(0, TTR("Print"));
	print_node->set_metadata(0, "io.print");
	
	// Data nodes
	TreeItem *data_category = node_library_tree->create_item(root);
	data_category->set_text(0, TTR("Data"));
	data_category->set_selectable(0, false);
	
	TreeItem *constant_node = node_library_tree->create_item(data_category);
	constant_node->set_text(0, TTR("Constant"));
	constant_node->set_metadata(0, "data.constant");
	
	TreeItem *variable_node = node_library_tree->create_item(data_category);
	variable_node->set_text(0, TTR("Variable"));
	variable_node->set_metadata(0, "data.variable");
	
	// Math nodes
	TreeItem *math_category = node_library_tree->create_item(root);
	math_category->set_text(0, TTR("Math"));
	math_category->set_selectable(0, false);
	
	TreeItem *add_node = node_library_tree->create_item(math_category);
	add_node->set_text(0, TTR("Add"));
	add_node->set_metadata(0, "math.add");
	
	// Control flow nodes
	TreeItem *control_category = node_library_tree->create_item(root);
	control_category->set_text(0, TTR("Control Flow"));
	control_category->set_selectable(0, false);
	
	TreeItem *if_node = node_library_tree->create_item(control_category);
	if_node->set_text(0, TTR("If/Else"));
	if_node->set_metadata(0, "control.if_else");
}

void FlowGraphEditor::_setup_graph_edit() {
	graph_edit = memnew(GraphEdit);
	graph_edit->set_v_size_flags(SIZE_EXPAND_FILL);
	graph_edit->set_h_size_flags(SIZE_EXPAND_FILL);
	graph_edit->set_right_disconnects(true);
	graph_edit->set_show_zoom_label(true);
	
	graph_edit->connect("connection_request", callable_mp(this, &FlowGraphEditor::_on_connection_request));
	graph_edit->connect("disconnection_request", callable_mp(this, &FlowGraphEditor::_on_disconnection_request));
	graph_edit->connect("popup_request", callable_mp(this, &FlowGraphEditor::_on_popup_request));
	graph_edit->connect("delete_nodes_request", callable_mp(this, &FlowGraphEditor::_on_delete_nodes_request));
	
	main_split->add_child(graph_edit);
}

// Toolbar callbacks
void FlowGraphEditor::_on_new_pressed() {
	_new_graph();
}

void FlowGraphEditor::_on_open_pressed() {
	// TODO: Implement file dialog
	print_line("Open graph - not implemented yet");
}

void FlowGraphEditor::_on_save_pressed() {
	// TODO: Implement file dialog
	print_line("Save graph - not implemented yet");
}

void FlowGraphEditor::_on_run_pressed() {
	// TODO: Implement interpreter execution
	print_line("Run graph - not implemented yet");
	btn_run->set_disabled(true);
	btn_stop->set_disabled(false);
}

void FlowGraphEditor::_on_stop_pressed() {
	// TODO: Stop interpreter
	print_line("Stop graph");
	btn_run->set_disabled(false);
	btn_stop->set_disabled(true);
}

// Node library callbacks
void FlowGraphEditor::_on_node_library_item_selected() {
	TreeItem *selected = node_library_tree->get_selected();
	if (selected && selected->has_meta("0")) {
		dragging_node_type = selected->get_metadata(0);
	}
}

void FlowGraphEditor::_on_node_library_item_activated() {
	TreeItem *selected = node_library_tree->get_selected();
	if (selected && selected->has_meta("0")) {
		String node_type = selected->get_metadata(0);
		Vector2 center = graph_edit->get_scroll_offset() + graph_edit->get_size() / 2;
		_create_graph_node(node_type, center / graph_edit->get_zoom());
	}
}

// Graph edit callbacks
void FlowGraphEditor::_on_connection_request(const StringName &p_from, int p_from_port, const StringName &p_to, int p_to_port) {
	// Extract node IDs from names (format: "node_123")
	String from_str = p_from;
	String to_str = p_to;
	
	int from_id = from_str.get_slice("_", 1).to_int();
	int to_id = to_str.get_slice("_", 1).to_int();
	
	// Get the nodes
	Ref<FlowNode> from_node = current_graph->get_node(from_id);
	Ref<FlowNode> to_node = current_graph->get_node(to_id);
	
	if (from_node.is_null() || to_node.is_null()) {
		return;
	}
	
	// Get port names from GraphNode
	FlowGraphNode *from_gnode = graph_nodes[from_id];
	FlowGraphNode *to_gnode = graph_nodes[to_id];
	
	String from_port_name = from_gnode->get_output_port_name(p_from_port);
	String to_port_name = to_gnode->get_input_port_name(p_to_port);
	
	// Create connection in data model
	Ref<FlowConnection> conn;
	conn.instantiate();
	conn->set_from_node(from_id);
	conn->set_from_port(from_port_name);
	conn->set_to_node(to_id);
	conn->set_to_port(to_port_name);
	current_graph->add_connection(conn);
	
	// Create visual connection
	graph_edit->connect_node(p_from, p_from_port, p_to, p_to_port);
	
	print_line("Connected: " + from_str + ":" + from_port_name + " -> " + to_str + ":" + to_port_name);
}

void FlowGraphEditor::_on_disconnection_request(const StringName &p_from, int p_from_port, const StringName &p_to, int p_to_port) {
	graph_edit->disconnect_node(p_from, p_from_port, p_to, p_to_port);
	
	// TODO: Remove from data model
	print_line("Disconnected");
}

void FlowGraphEditor::_on_popup_request(const Vector2 &p_position) {
	// TODO: Show context menu for creating nodes
	print_line("Popup at: ", p_position);
}

void FlowGraphEditor::_on_node_selected(Node *p_node) {
	// TODO: Show node properties in inspector
}

void FlowGraphEditor::_on_delete_nodes_request(const TypedArray<StringName> &p_nodes) {
	for (int i = 0; i < p_nodes.size(); i++) {
		StringName node_name = p_nodes[i];
		String name_str = node_name;
		int node_id = name_str.get_slice("_", 1).to_int();
		
		// Remove from data model
		current_graph->remove_node(node_id);
		
		// Remove from visual graph
		Node *gnode = graph_edit->get_node_or_null(NodePath(node_name));
		if (gnode) {
			graph_edit->remove_child(gnode);
			gnode->queue_free();
		}
		
		graph_nodes.erase(node_id);
	}
}

// Node management
void FlowGraphEditor::_create_graph_node(const String &p_node_type, const Vector2 &p_position) {
	// Create data node
	Ref<FlowNode> node;
	node.instantiate();
	node->set_type(p_node_type);
	node->set_position(p_position);
	
	// Add ports based on node type
	// TODO: This should be done by a node factory
	if (p_node_type == "event.start") {
		Ref<FlowPort> exec_out;
		exec_out.instantiate();
		exec_out->set_name("execute");
		exec_out->set_type(FlowPort::TYPE_EXECUTION);
		node->add_output_port(exec_out);
	} else if (p_node_type == "io.print") {
		Ref<FlowPort> exec_in;
		exec_in.instantiate();
		exec_in->set_name("execute");
		exec_in->set_type(FlowPort::TYPE_EXECUTION);
		node->add_input_port(exec_in);
		
		Ref<FlowPort> msg_in;
		msg_in.instantiate();
		msg_in->set_name("message");
		msg_in->set_type(FlowPort::TYPE_ANY);
		node->add_input_port(msg_in);
		
		Ref<FlowPort> exec_out;
		exec_out.instantiate();
		exec_out->set_name("execute");
		exec_out->set_type(FlowPort::TYPE_EXECUTION);
		node->add_output_port(exec_out);
	}
	// Add more node types...
	
	int node_id = current_graph->add_node(node);
	
	// Create visual node
	FlowGraphNode *gnode = memnew(FlowGraphNode);
	gnode->set_flow_node(node);
	gnode->set_name("node_" + itos(node_id));
	gnode->set_position_offset(p_position);
	
	graph_edit->add_child(gnode);
	graph_nodes[node_id] = gnode;
	
	print_line("Created node: ", p_node_type, " at ", p_position);
}

void FlowGraphEditor::_update_graph_view() {
	_clear_graph_view();
	
	if (current_graph.is_null()) {
		return;
	}
	
	// Create visual nodes
	Array nodes = current_graph->get_all_nodes();
	for (int i = 0; i < nodes.size(); i++) {
		Ref<FlowNode> node = nodes[i];
		if (node.is_null()) {
			continue;
		}
		
		FlowGraphNode *gnode = memnew(FlowGraphNode);
		gnode->set_flow_node(node);
		gnode->set_name("node_" + itos(node->get_id()));
		gnode->set_position_offset(node->get_position());
		
		graph_edit->add_child(gnode);
		graph_nodes[node->get_id()] = gnode;
	}
	
	// Create visual connections
	Array graph_connections = current_graph->get_all_connections();
	for (int i = 0; i < graph_connections.size(); i++) {
		Ref<FlowConnection> conn = graph_connections[i];
		if (conn.is_null()) {
			continue;
		}
		
		String from_name = "node_" + itos(conn->get_from_node());
		String to_name = "node_" + itos(conn->get_to_node());
		
		// Find port indices
		FlowGraphNode *from_gnode = graph_nodes[conn->get_from_node()];
		FlowGraphNode *to_gnode = graph_nodes[conn->get_to_node()];
		
		if (from_gnode && to_gnode) {
			int from_port = from_gnode->get_output_port_index(conn->get_from_port());
			int to_port = to_gnode->get_input_port_index(conn->get_to_port());
			
			if (from_port >= 0 && to_port >= 0) {
				graph_edit->connect_node(from_name, from_port, to_name, to_port);
			}
		}
	}
}

void FlowGraphEditor::_clear_graph_view() {
	// Remove all graph nodes
	for (KeyValue<int, FlowGraphNode *> &E : graph_nodes) {
		if (E.value) {
			graph_edit->remove_child(E.value);
			E.value->queue_free();
		}
	}
	graph_nodes.clear();
	
	// Clear all connections
	graph_edit->clear_connections();
}

// File operations
void FlowGraphEditor::_new_graph() {
	current_graph.instantiate();
	_update_graph_view();
	print_line("New graph created");
}

void FlowGraphEditor::_load_graph(const String &p_path) {
	// TODO: Implement
}

void FlowGraphEditor::_save_graph(const String &p_path) {
	// TODO: Implement
}

void FlowGraphEditor::set_graph(const Ref<FlowGraph> &p_graph) {
	current_graph = p_graph;
	_update_graph_view();
}
