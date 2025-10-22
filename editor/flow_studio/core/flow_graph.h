/**************************************************************************/
/*  flow_graph.h                                                          */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/

#ifndef FLOW_GRAPH_H
#define FLOW_GRAPH_H

#include "core/object/ref_counted.h"
#include "core/templates/hash_map.h"
#include "core/templates/vector.h"
#include "core/variant/variant.h"

class FlowNode;
class FlowConnection;

class FlowGraph : public RefCounted {
	GDCLASS(FlowGraph, RefCounted);

private:
	HashMap<int, Ref<FlowNode>> nodes;
	Vector<Ref<FlowConnection>> connections;
	Dictionary variables;
	int next_node_id = 1;

protected:
	static void _bind_methods();

public:
	// Node management
	int add_node(const Ref<FlowNode> &p_node);
	void remove_node(int p_node_id);
	Ref<FlowNode> get_node(int p_node_id) const;
	Array get_all_nodes() const;
	bool has_node(int p_node_id) const;

	// Connection management
	void add_connection(const Ref<FlowConnection> &p_connection);
	void remove_connection(const Ref<FlowConnection> &p_connection);
	Array get_all_connections() const;
	Array get_connections_from_node(int p_node_id) const;
	Array get_connections_to_node(int p_node_id) const;

	// Variable management
	void set_variable(const String &p_name, const Variant &p_value);
	Variant get_variable(const String &p_name, const Variant &p_default = Variant()) const;
	bool has_variable(const String &p_name) const;
	Dictionary get_all_variables() const;

	// Serialization
	Dictionary to_dict() const;
	void from_dict(const Dictionary &p_dict);

	// Utility
	void clear();
	int get_node_count() const;
	int get_connection_count() const;

	FlowGraph();
	~FlowGraph();
};

#endif // FLOW_GRAPH_H
