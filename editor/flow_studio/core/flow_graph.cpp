/**************************************************************************/
/*  flow_graph.cpp                                                        */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/

#include "flow_graph.h"
#include "flow_node.h"
#include "flow_connection.h"

FlowGraph::FlowGraph() {
}

FlowGraph::~FlowGraph() {
	clear();
}

void FlowGraph::_bind_methods() {
	// Node management
	ClassDB::bind_method(D_METHOD("add_node", "node"), &FlowGraph::add_node);
	ClassDB::bind_method(D_METHOD("remove_node", "node_id"), &FlowGraph::remove_node);
	ClassDB::bind_method(D_METHOD("get_node", "node_id"), &FlowGraph::get_node);
	ClassDB::bind_method(D_METHOD("get_all_nodes"), &FlowGraph::get_all_nodes);
	ClassDB::bind_method(D_METHOD("has_node", "node_id"), &FlowGraph::has_node);

	// Connection management
	ClassDB::bind_method(D_METHOD("add_connection", "connection"), &FlowGraph::add_connection);
	ClassDB::bind_method(D_METHOD("remove_connection", "connection"), &FlowGraph::remove_connection);
	ClassDB::bind_method(D_METHOD("get_all_connections"), &FlowGraph::get_all_connections);
	ClassDB::bind_method(D_METHOD("get_connections_from_node", "node_id"), &FlowGraph::get_connections_from_node);
	ClassDB::bind_method(D_METHOD("get_connections_to_node", "node_id"), &FlowGraph::get_connections_to_node);

	// Variable management
	ClassDB::bind_method(D_METHOD("set_variable", "name", "value"), &FlowGraph::set_variable);
	ClassDB::bind_method(D_METHOD("get_variable", "name", "default"), &FlowGraph::get_variable, DEFVAL(Variant()));
	ClassDB::bind_method(D_METHOD("has_variable", "name"), &FlowGraph::has_variable);
	ClassDB::bind_method(D_METHOD("get_all_variables"), &FlowGraph::get_all_variables);

	// Serialization
	ClassDB::bind_method(D_METHOD("to_dict"), &FlowGraph::to_dict);
	ClassDB::bind_method(D_METHOD("from_dict", "dict"), &FlowGraph::from_dict);

	// Utility
	ClassDB::bind_method(D_METHOD("clear"), &FlowGraph::clear);
	ClassDB::bind_method(D_METHOD("get_node_count"), &FlowGraph::get_node_count);
	ClassDB::bind_method(D_METHOD("get_connection_count"), &FlowGraph::get_connection_count);
}

// Node management
int FlowGraph::add_node(const Ref<FlowNode> &p_node) {
	ERR_FAIL_COND_V(p_node.is_null(), -1);
	
	int node_id = next_node_id++;
	p_node->set_id(node_id);
	nodes[node_id] = p_node;
	
	return node_id;
}

void FlowGraph::remove_node(int p_node_id) {
	if (!nodes.has(p_node_id)) {
		return;
	}

	// Remove all connections related to this node
	for (int i = connections.size() - 1; i >= 0; i--) {
		Ref<FlowConnection> conn = connections[i];
		if (conn->get_from_node() == p_node_id || conn->get_to_node() == p_node_id) {
			connections.remove_at(i);
		}
	}

	nodes.erase(p_node_id);
}

Ref<FlowNode> FlowGraph::get_node(int p_node_id) const {
	if (nodes.has(p_node_id)) {
		return nodes[p_node_id];
	}
	return Ref<FlowNode>();
}

Array FlowGraph::get_all_nodes() const {
	Array result;
	for (const KeyValue<int, Ref<FlowNode>> &E : nodes) {
		result.push_back(E.value);
	}
	return result;
}

bool FlowGraph::has_node(int p_node_id) const {
	return nodes.has(p_node_id);
}

// Connection management
void FlowGraph::add_connection(const Ref<FlowConnection> &p_connection) {
	ERR_FAIL_COND(p_connection.is_null());
	connections.push_back(p_connection);
}

void FlowGraph::remove_connection(const Ref<FlowConnection> &p_connection) {
	ERR_FAIL_COND(p_connection.is_null());
	connections.erase(p_connection);
}

Array FlowGraph::get_all_connections() const {
	Array result;
	for (const Ref<FlowConnection> &conn : connections) {
		result.push_back(conn);
	}
	return result;
}

Array FlowGraph::get_connections_from_node(int p_node_id) const {
	Array result;
	for (const Ref<FlowConnection> &conn : connections) {
		if (conn->get_from_node() == p_node_id) {
			result.push_back(conn);
		}
	}
	return result;
}

Array FlowGraph::get_connections_to_node(int p_node_id) const {
	Array result;
	for (const Ref<FlowConnection> &conn : connections) {
		if (conn->get_to_node() == p_node_id) {
			result.push_back(conn);
		}
	}
	return result;
}

// Variable management
void FlowGraph::set_variable(const String &p_name, const Variant &p_value) {
	variables[p_name] = p_value;
}

Variant FlowGraph::get_variable(const String &p_name, const Variant &p_default) const {
	if (variables.has(p_name)) {
		return variables[p_name];
	}
	return p_default;
}

bool FlowGraph::has_variable(const String &p_name) const {
	return variables.has(p_name);
}

Dictionary FlowGraph::get_all_variables() const {
	return variables;
}

// Serialization
Dictionary FlowGraph::to_dict() const {
	Dictionary dict;
	
	// Serialize nodes
	Array nodes_array;
	for (const KeyValue<int, Ref<FlowNode>> &E : nodes) {
		if (E.value.is_valid()) {
			nodes_array.push_back(E.value->to_dict());
		}
	}
	dict["nodes"] = nodes_array;
	
	// Serialize connections
	Array connections_array;
	for (const Ref<FlowConnection> &conn : connections) {
		if (conn.is_valid()) {
			connections_array.push_back(conn->to_dict());
		}
	}
	dict["connections"] = connections_array;
	
	// Serialize variables
	dict["variables"] = variables;
	
	// Metadata
	dict["version"] = "1.0";
	dict["next_node_id"] = next_node_id;
	
	return dict;
}

void FlowGraph::from_dict(const Dictionary &p_dict) {
	clear();
	
	// Load metadata
	if (p_dict.has("next_node_id")) {
		next_node_id = p_dict["next_node_id"];
	}
	
	// Load nodes
	if (p_dict.has("nodes")) {
		Array nodes_array = p_dict["nodes"];
		for (int i = 0; i < nodes_array.size(); i++) {
			Dictionary node_dict = nodes_array[i];
			Ref<FlowNode> node;
			node.instantiate();
			node->from_dict(node_dict);
			
			int node_id = node->get_id();
			nodes[node_id] = node;
			
			// Update next_node_id if needed
			if (node_id >= next_node_id) {
				next_node_id = node_id + 1;
			}
		}
	}
	
	// Load connections
	if (p_dict.has("connections")) {
		Array connections_array = p_dict["connections"];
		for (int i = 0; i < connections_array.size(); i++) {
			Dictionary conn_dict = connections_array[i];
			Ref<FlowConnection> conn;
			conn.instantiate();
			conn->from_dict(conn_dict);
			connections.push_back(conn);
		}
	}
	
	// Load variables
	if (p_dict.has("variables")) {
		variables = p_dict["variables"];
	}
}

// Utility
void FlowGraph::clear() {
	nodes.clear();
	connections.clear();
	variables.clear();
	next_node_id = 1;
}

int FlowGraph::get_node_count() const {
	return nodes.size();
}

int FlowGraph::get_connection_count() const {
	return connections.size();
}
