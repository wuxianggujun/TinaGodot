/**************************************************************************/
/*  flow_connection.cpp                                                   */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/

#include "flow_connection.h"

FlowConnection::FlowConnection() {
}

FlowConnection::FlowConnection(int p_from_node, const String &p_from_port, int p_to_node, const String &p_to_port) {
	from_node = p_from_node;
	from_port = p_from_port;
	to_node = p_to_node;
	to_port = p_to_port;
}

FlowConnection::~FlowConnection() {
}

void FlowConnection::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_from_node", "node_id"), &FlowConnection::set_from_node);
	ClassDB::bind_method(D_METHOD("get_from_node"), &FlowConnection::get_from_node);

	ClassDB::bind_method(D_METHOD("set_from_port", "port_name"), &FlowConnection::set_from_port);
	ClassDB::bind_method(D_METHOD("get_from_port"), &FlowConnection::get_from_port);

	ClassDB::bind_method(D_METHOD("set_to_node", "node_id"), &FlowConnection::set_to_node);
	ClassDB::bind_method(D_METHOD("get_to_node"), &FlowConnection::get_to_node);

	ClassDB::bind_method(D_METHOD("set_to_port", "port_name"), &FlowConnection::set_to_port);
	ClassDB::bind_method(D_METHOD("get_to_port"), &FlowConnection::get_to_port);

	ClassDB::bind_method(D_METHOD("is_valid"), &FlowConnection::is_valid);
	ClassDB::bind_method(D_METHOD("equals", "other"), &FlowConnection::equals);

	ClassDB::bind_method(D_METHOD("to_dict"), &FlowConnection::to_dict);
	ClassDB::bind_method(D_METHOD("from_dict", "dict"), &FlowConnection::from_dict);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "from_node"), "set_from_node", "get_from_node");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "from_port"), "set_from_port", "get_from_port");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "to_node"), "set_to_node", "get_to_node");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "to_port"), "set_to_port", "get_to_port");
}

// Getters and Setters
void FlowConnection::set_from_node(int p_node_id) {
	from_node = p_node_id;
}

int FlowConnection::get_from_node() const {
	return from_node;
}

void FlowConnection::set_from_port(const String &p_port_name) {
	from_port = p_port_name;
}

String FlowConnection::get_from_port() const {
	return from_port;
}

void FlowConnection::set_to_node(int p_node_id) {
	to_node = p_node_id;
}

int FlowConnection::get_to_node() const {
	return to_node;
}

void FlowConnection::set_to_port(const String &p_port_name) {
	to_port = p_port_name;
}

String FlowConnection::get_to_port() const {
	return to_port;
}

// Utility
bool FlowConnection::is_valid() const {
	return from_node >= 0 && to_node >= 0 && !from_port.is_empty() && !to_port.is_empty();
}

bool FlowConnection::equals(const Ref<FlowConnection> &p_other) const {
	if (p_other.is_null()) {
		return false;
	}
	return from_node == p_other->from_node &&
		   from_port == p_other->from_port &&
		   to_node == p_other->to_node &&
		   to_port == p_other->to_port;
}

// Serialization
Dictionary FlowConnection::to_dict() const {
	Dictionary dict;
	dict["from_node"] = from_node;
	dict["from_port"] = from_port;
	dict["to_node"] = to_node;
	dict["to_port"] = to_port;
	return dict;
}

void FlowConnection::from_dict(const Dictionary &p_dict) {
	if (p_dict.has("from_node")) {
		from_node = p_dict["from_node"];
	}
	if (p_dict.has("from_port")) {
		from_port = p_dict["from_port"];
	}
	if (p_dict.has("to_node")) {
		to_node = p_dict["to_node"];
	}
	if (p_dict.has("to_port")) {
		to_port = p_dict["to_port"];
	}
}
