/**************************************************************************/
/*  flow_node_library_dock.cpp                                            */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/

#include "flow_node_library_dock.h"

#include "core/input/shortcut.h"
#include "scene/gui/box_container.h"
#include "scene/gui/label.h"

FlowNodeLibraryDock *FlowNodeLibraryDock::singleton = nullptr;

FlowNodeLibraryDock::FlowNodeLibraryDock() {
	singleton = this;
	set_name(TTR("FlowNodes"));
	set_title(TTR("Node Library"));
	set_icon_name("NodeWarning"); // 使用一个临时图标，后续可以自定义
	set_default_slot(EditorDockManager::DOCK_SLOT_LEFT_UR); // 与 Scene 和 Import 在同一位置
	
	// Create a VBoxContainer to hold the content
	VBoxContainer *vbox = memnew(VBoxContainer);
	vbox->set_v_size_flags(SIZE_EXPAND_FILL);
	add_child(vbox);
	
	// Node tree
	node_tree = memnew(Tree);
	node_tree->set_v_size_flags(SIZE_EXPAND_FILL);
	node_tree->set_hide_root(true);
	node_tree->set_allow_reselect(true);
	node_tree->connect("item_selected", callable_mp(this, &FlowNodeLibraryDock::_on_node_selected));
	node_tree->connect("item_activated", callable_mp(this, &FlowNodeLibraryDock::_on_node_activated));
	vbox->add_child(node_tree);
	
	_populate_nodes();
}

FlowNodeLibraryDock::~FlowNodeLibraryDock() {
	singleton = nullptr;
}

void FlowNodeLibraryDock::_bind_methods() {
}

void FlowNodeLibraryDock::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			// Additional setup if needed
		} break;
	}
}

void FlowNodeLibraryDock::_populate_nodes() {
	node_tree->clear();
	TreeItem *root = node_tree->create_item();
	
	// Event nodes
	TreeItem *event_category = node_tree->create_item(root);
	event_category->set_text(0, TTR("Event"));
	event_category->set_selectable(0, false);
	
	TreeItem *start_node = node_tree->create_item(event_category);
	start_node->set_text(0, TTR("Start"));
	start_node->set_metadata(0, "event.start");
	start_node->set_tooltip_text(0, TTR("Program entry point"));
	
	// I/O nodes
	TreeItem *io_category = node_tree->create_item(root);
	io_category->set_text(0, TTR("Input/Output"));
	io_category->set_selectable(0, false);
	
	TreeItem *print_node = node_tree->create_item(io_category);
	print_node->set_text(0, TTR("Print"));
	print_node->set_metadata(0, "io.print");
	print_node->set_tooltip_text(0, TTR("Print message to console"));
	
	// Data nodes
	TreeItem *data_category = node_tree->create_item(root);
	data_category->set_text(0, TTR("Data"));
	data_category->set_selectable(0, false);
	
	TreeItem *constant_node = node_tree->create_item(data_category);
	constant_node->set_text(0, TTR("Constant"));
	constant_node->set_metadata(0, "data.constant");
	constant_node->set_tooltip_text(0, TTR("Constant value"));
	
	TreeItem *variable_node = node_tree->create_item(data_category);
	variable_node->set_text(0, TTR("Variable"));
	variable_node->set_metadata(0, "data.variable");
	variable_node->set_tooltip_text(0, TTR("Get or set variable"));
	
	// Math nodes
	TreeItem *math_category = node_tree->create_item(root);
	math_category->set_text(0, TTR("Math"));
	math_category->set_selectable(0, false);
	
	TreeItem *add_node = node_tree->create_item(math_category);
	add_node->set_text(0, TTR("Add"));
	add_node->set_metadata(0, "math.add");
	add_node->set_tooltip_text(0, TTR("Add two numbers"));
	
	TreeItem *subtract_node = node_tree->create_item(math_category);
	subtract_node->set_text(0, TTR("Subtract"));
	subtract_node->set_metadata(0, "math.subtract");
	subtract_node->set_tooltip_text(0, TTR("Subtract two numbers"));
	
	TreeItem *multiply_node = node_tree->create_item(math_category);
	multiply_node->set_text(0, TTR("Multiply"));
	multiply_node->set_metadata(0, "math.multiply");
	multiply_node->set_tooltip_text(0, TTR("Multiply two numbers"));
	
	TreeItem *divide_node = node_tree->create_item(math_category);
	divide_node->set_text(0, TTR("Divide"));
	divide_node->set_metadata(0, "math.divide");
	divide_node->set_tooltip_text(0, TTR("Divide two numbers"));
	
	// Control flow nodes
	TreeItem *control_category = node_tree->create_item(root);
	control_category->set_text(0, TTR("Control Flow"));
	control_category->set_selectable(0, false);
	
	TreeItem *if_node = node_tree->create_item(control_category);
	if_node->set_text(0, TTR("If/Else"));
	if_node->set_metadata(0, "control.if_else");
	if_node->set_tooltip_text(0, TTR("Conditional branch"));
	
	TreeItem *loop_node = node_tree->create_item(control_category);
	loop_node->set_text(0, TTR("Loop"));
	loop_node->set_metadata(0, "control.loop");
	loop_node->set_tooltip_text(0, TTR("Repeat execution"));
}

void FlowNodeLibraryDock::_on_node_selected() {
	// Handle node selection
	TreeItem *selected = node_tree->get_selected();
	if (selected && selected->has_meta("0")) {
		String node_type = selected->get_metadata(0);
		// TODO: Show node description or preview
	}
}

void FlowNodeLibraryDock::_on_node_activated() {
	// Handle double-click - could create node at center of viewport
	TreeItem *selected = node_tree->get_selected();
	if (selected && selected->has_meta("0")) {
		String node_type = selected->get_metadata(0);
		// TODO: Signal to create node
		print_line("Node activated: ", node_type);
	}
}

String FlowNodeLibraryDock::get_selected_node_type() const {
	TreeItem *selected = node_tree->get_selected();
	if (selected && selected->has_meta("0")) {
		return selected->get_metadata(0);
	}
	return "";
}
