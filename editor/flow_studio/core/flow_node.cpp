/**************************************************************************/
/*  flow_node.cpp                                                         */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/

#include "flow_node.h"

FlowNode::FlowNode() {
}

FlowNode::~FlowNode() {
}

void FlowNode::_bind_methods() {
	// Basic properties
	ClassDB::bind_method(D_METHOD("set_id", "id"), &FlowNode::set_id);
	ClassDB::bind_method(D_METHOD("get_id"), &FlowNode::get_id);

	ClassDB::bind_method(D_METHOD("set_type", "type"), &FlowNode::set_type);
	ClassDB::bind_method(D_METHOD("get_type"), &FlowNode::get_type);

	ClassDB::bind_method(D_METHOD("set_position", "position"), &FlowNode::set_position);
	ClassDB::bind_method(D_METHOD("get_position"), &FlowNode::get_position);

	// Properties management
	ClassDB::bind_method(D_METHOD("set_property", "name", "value"), &FlowNode::set_property);
	ClassDB::bind_method(D_METHOD("get_property", "name", "default"), &FlowNode::get_property, DEFVAL(Variant()));
	ClassDB::bind_method(D_METHOD("has_property", "name"), &FlowNode::has_property);
	ClassDB::bind_method(D_METHOD("get_all_properties"), &FlowNode::get_all_properties);
	ClassDB::bind_method(D_METHOD("set_properties", "properties"), &FlowNode::set_properties);

	// Port management
	ClassDB::bind_method(D_METHOD("add_input_port", "port"), &FlowNode::add_input_port);
	ClassDB::bind_method(D_METHOD("add_output_port", "port"), &FlowNode::add_output_port);
	ClassDB::bind_method(D_METHOD("remove_input_port", "name"), &FlowNode::remove_input_port);
	ClassDB::bind_method(D_METHOD("remove_output_port", "name"), &FlowNode::remove_output_port);

	ClassDB::bind_method(D_METHOD("get_input_port", "name"), &FlowNode::get_input_port);
	ClassDB::bind_method(D_METHOD("get_output_port", "name"), &FlowNode::get_output_port);
	ClassDB::bind_method(D_METHOD("get_port", "name"), &FlowNode::get_port);

	ClassDB::bind_method(D_METHOD("get_input_ports"), &FlowNode::get_input_ports);
	ClassDB::bind_method(D_METHOD("get_output_ports"), &FlowNode::get_output_ports);

	ClassDB::bind_method(D_METHOD("has_input_port", "name"), &FlowNode::has_input_port);
	ClassDB::bind_method(D_METHOD("has_output_port", "name"), &FlowNode::has_output_port);

	// Serialization
	ClassDB::bind_method(D_METHOD("to_dict"), &FlowNode::to_dict);
	ClassDB::bind_method(D_METHOD("from_dict", "dict"), &FlowNode::from_dict);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "id"), "set_id", "get_id");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "type"), "set_type", "get_type");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "position"), "set_position", "get_position");
}

// Basic properties
void FlowNode::set_id(int p_id) {
	id = p_id;
}

int FlowNode::get_id() const {
	return id;
}

void FlowNode::set_type(const String &p_type) {
	type = p_type;
}

String FlowNode::get_type() const {
	return type;
}

void FlowNode::set_position(const Vector2 &p_position) {
	position = p_position;
}

Vector2 FlowNode::get_position() const {
	return position;
}

// Properties management
void FlowNode::set_property(const String &p_name, const Variant &p_value) {
	properties[p_name] = p_value;
}

Variant FlowNode::get_property(const String &p_name, const Variant &p_default) const {
	if (properties.has(p_name)) {
		return properties[p_name];
	}
	return p_default;
}

bool FlowNode::has_property(const String &p_name) const {
	return properties.has(p_name);
}

Dictionary FlowNode::get_all_properties() const {
	return properties;
}

void FlowNode::set_properties(const Dictionary &p_properties) {
	properties = p_properties;
}

// Port management
void FlowNode::add_input_port(const Ref<FlowPort> &p_port) {
	ERR_FAIL_COND(p_port.is_null());
	p_port->set_direction(FlowPort::INPUT);
	input_ports.push_back(p_port);
}

void FlowNode::add_output_port(const Ref<FlowPort> &p_port) {
	ERR_FAIL_COND(p_port.is_null());
	p_port->set_direction(FlowPort::OUTPUT);
	output_ports.push_back(p_port);
}

void FlowNode::remove_input_port(const String &p_name) {
	for (int i = 0; i < input_ports.size(); i++) {
		if (input_ports[i]->get_name() == p_name) {
			input_ports.remove_at(i);
			return;
		}
	}
}

void FlowNode::remove_output_port(const String &p_name) {
	for (int i = 0; i < output_ports.size(); i++) {
		if (output_ports[i]->get_name() == p_name) {
			output_ports.remove_at(i);
			return;
		}
	}
}

Ref<FlowPort> FlowNode::get_input_port(const String &p_name) const {
	for (const Ref<FlowPort> &port : input_ports) {
		if (port->get_name() == p_name) {
			return port;
		}
	}
	return Ref<FlowPort>();
}

Ref<FlowPort> FlowNode::get_output_port(const String &p_name) const {
	for (const Ref<FlowPort> &port : output_ports) {
		if (port->get_name() == p_name) {
			return port;
		}
	}
	return Ref<FlowPort>();
}

Ref<FlowPort> FlowNode::get_port(const String &p_name) const {
	Ref<FlowPort> port = get_input_port(p_name);
	if (port.is_valid()) {
		return port;
	}
	return get_output_port(p_name);
}

Array FlowNode::get_input_ports() const {
	Array result;
	for (const Ref<FlowPort> &port : input_ports) {
		result.push_back(port);
	}
	return result;
}

Array FlowNode::get_output_ports() const {
	Array result;
	for (const Ref<FlowPort> &port : output_ports) {
		result.push_back(port);
	}
	return result;
}

bool FlowNode::has_input_port(const String &p_name) const {
	return get_input_port(p_name).is_valid();
}

bool FlowNode::has_output_port(const String &p_name) const {
	return get_output_port(p_name).is_valid();
}

// Serialization
Dictionary FlowNode::to_dict() const {
	Dictionary dict;
	
	dict["id"] = id;
	dict["type"] = type;
	dict["position"] = position;
	dict["properties"] = properties;
	
	// Serialize input ports
	Array input_array;
	for (const Ref<FlowPort> &port : input_ports) {
		if (port.is_valid()) {
			input_array.push_back(port->to_dict());
		}
	}
	dict["input_ports"] = input_array;
	
	// Serialize output ports
	Array output_array;
	for (const Ref<FlowPort> &port : output_ports) {
		if (port.is_valid()) {
			output_array.push_back(port->to_dict());
		}
	}
	dict["output_ports"] = output_array;
	
	return dict;
}

void FlowNode::from_dict(const Dictionary &p_dict) {
	if (p_dict.has("id")) {
		id = p_dict["id"];
	}
	if (p_dict.has("type")) {
		type = p_dict["type"];
	}
	if (p_dict.has("position")) {
		position = p_dict["position"];
	}
	if (p_dict.has("properties")) {
		properties = p_dict["properties"];
	}
	
	// Load input ports
	input_ports.clear();
	if (p_dict.has("input_ports")) {
		Array input_array = p_dict["input_ports"];
		for (int i = 0; i < input_array.size(); i++) {
			Dictionary port_dict = input_array[i];
			Ref<FlowPort> port;
			port.instantiate();
			port->from_dict(port_dict);
			input_ports.push_back(port);
		}
	}
	
	// Load output ports
	output_ports.clear();
	if (p_dict.has("output_ports")) {
		Array output_array = p_dict["output_ports"];
		for (int i = 0; i < output_array.size(); i++) {
			Dictionary port_dict = output_array[i];
			Ref<FlowPort> port;
			port.instantiate();
			port->from_dict(port_dict);
			output_ports.push_back(port);
		}
	}
}
