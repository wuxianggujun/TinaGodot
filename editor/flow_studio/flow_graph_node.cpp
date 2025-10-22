/**************************************************************************/
/*  flow_graph_node.cpp                                                   */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/

#include "flow_graph_node.h"
#include "editor/flow_studio/core/flow_port.h"
#include "scene/gui/label.h"

FlowGraphNode::FlowGraphNode() {
	set_draggable(true);
	set_selectable(true);
	set_resizable(false);
}

FlowGraphNode::~FlowGraphNode() {
}

void FlowGraphNode::_bind_methods() {
}

void FlowGraphNode::set_flow_node(const Ref<FlowNode> &p_node) {
	flow_node = p_node;
	_update_node_view();
}

void FlowGraphNode::_update_node_view() {
	if (flow_node.is_null()) {
		return;
	}
	
	// Clear existing children
	for (int i = get_child_count() - 1; i >= 0; i--) {
		Node *child = get_child(i);
		remove_child(child);
		child->queue_free();
	}
	
	input_port_info.clear();
	output_port_info.clear();
	
	// Set title based on node type
	String type = flow_node->get_type();
	if (type == "event.start") {
		set_title("Start");
	} else if (type == "io.print") {
		set_title("Print");
	} else if (type == "data.constant") {
		set_title("Constant");
	} else if (type == "data.variable") {
		set_title("Variable");
	} else if (type == "math.add") {
		set_title("Add");
	} else if (type == "control.if_else") {
		set_title("If/Else");
	} else {
		set_title(type);
	}
	
	// Add input ports
	Array inputs = flow_node->get_input_ports();
	for (int i = 0; i < inputs.size(); i++) {
		Ref<FlowPort> port = inputs[i];
		if (port.is_null()) {
			continue;
		}
		
		Label *label = memnew(Label);
		label->set_text(port->get_name());
		add_child(label);
		
		int slot_index = get_child_count() - 1;
		Color port_color = _get_port_color(port->get_type());
		
		set_slot(slot_index, true, 0, port_color, false, 0, Color());
		
		PortInfo info;
		info.name = port->get_name();
		info.index = slot_index;
		input_port_info.push_back(info);
	}
	
	// Add output ports
	Array outputs = flow_node->get_output_ports();
	for (int i = 0; i < outputs.size(); i++) {
		Ref<FlowPort> port = outputs[i];
		if (port.is_null()) {
			continue;
		}
		
		Label *label = memnew(Label);
		label->set_text(port->get_name());
		label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_RIGHT);
		add_child(label);
		
		int slot_index = get_child_count() - 1;
		Color port_color = _get_port_color(port->get_type());
		
		set_slot(slot_index, false, 0, Color(), true, 0, port_color);
		
		PortInfo info;
		info.name = port->get_name();
		info.index = slot_index;
		output_port_info.push_back(info);
	}
}

Color FlowGraphNode::_get_port_color(FlowPort::PortType p_type) const {
	switch (p_type) {
		case FlowPort::TYPE_EXECUTION:
			return Color(1.0, 1.0, 1.0); // White
		case FlowPort::TYPE_INT:
			return Color(0.4, 0.6, 1.0); // Blue
		case FlowPort::TYPE_FLOAT:
			return Color(0.4, 1.0, 0.6); // Green
		case FlowPort::TYPE_STRING:
			return Color(1.0, 0.9, 0.4); // Yellow
		case FlowPort::TYPE_BOOL:
			return Color(1.0, 0.4, 0.4); // Red
		case FlowPort::TYPE_VECTOR2:
			return Color(0.8, 0.4, 1.0); // Purple
		case FlowPort::TYPE_COLOR:
			return Color(1.0, 0.6, 0.4); // Orange
		case FlowPort::TYPE_ANY:
			return Color(0.7, 0.7, 0.7); // Gray
		default:
			return Color(0.5, 0.5, 0.5);
	}
}

String FlowGraphNode::get_input_port_name(int p_index) const {
	for (const PortInfo &info : input_port_info) {
		if (info.index == p_index) {
			return info.name;
		}
	}
	return "";
}

String FlowGraphNode::get_output_port_name(int p_index) const {
	for (const PortInfo &info : output_port_info) {
		if (info.index == p_index) {
			return info.name;
		}
	}
	return "";
}

int FlowGraphNode::get_input_port_index(const String &p_name) const {
	for (const PortInfo &info : input_port_info) {
		if (info.name == p_name) {
			return info.index;
		}
	}
	return -1;
}

int FlowGraphNode::get_output_port_index(const String &p_name) const {
	for (const PortInfo &info : output_port_info) {
		if (info.name == p_name) {
			return info.index;
		}
	}
	return -1;
}
