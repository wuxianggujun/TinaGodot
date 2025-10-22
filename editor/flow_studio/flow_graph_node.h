/**************************************************************************/
/*  flow_graph_node.h                                                     */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/

#ifndef FLOW_GRAPH_NODE_H
#define FLOW_GRAPH_NODE_H

#include "editor/flow_studio/core/flow_node.h"
#include "scene/gui/graph_node.h"

class FlowGraphNode : public GraphNode {
	GDCLASS(FlowGraphNode, GraphNode);

private:
	Ref<FlowNode> flow_node;
	
	struct PortInfo {
		String name;
		int index;
	};
	
	Vector<PortInfo> input_port_info;
	Vector<PortInfo> output_port_info;
	
	void _update_node_view();
	Color _get_port_color(FlowPort::PortType p_type) const;

protected:
	static void _bind_methods();

public:
	void set_flow_node(const Ref<FlowNode> &p_node);
	Ref<FlowNode> get_flow_node() const { return flow_node; }
	
	String get_input_port_name(int p_index) const;
	String get_output_port_name(int p_index) const;
	int get_input_port_index(const String &p_name) const;
	int get_output_port_index(const String &p_name) const;
	
	FlowGraphNode();
	~FlowGraphNode();
};

#endif // FLOW_GRAPH_NODE_H
