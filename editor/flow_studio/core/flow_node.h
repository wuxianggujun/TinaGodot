/**************************************************************************/
/*  flow_node.h                                                           */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/

#ifndef FLOW_NODE_H
#define FLOW_NODE_H

#include "core/object/ref_counted.h"
#include "core/templates/vector.h"
#include "core/variant/variant.h"
#include "flow_port.h"

class FlowNode : public RefCounted {
	GDCLASS(FlowNode, RefCounted);

private:
	int id = -1;
	String type;
	Vector2 position;
	Dictionary properties;
	Vector<Ref<FlowPort>> input_ports;
	Vector<Ref<FlowPort>> output_ports;

protected:
	static void _bind_methods();

public:
	// Basic properties
	void set_id(int p_id);
	int get_id() const;

	void set_type(const String &p_type);
	String get_type() const;

	void set_position(const Vector2 &p_position);
	Vector2 get_position() const;

	// Properties management
	void set_property(const String &p_name, const Variant &p_value);
	Variant get_property(const String &p_name, const Variant &p_default = Variant()) const;
	bool has_property(const String &p_name) const;
	Dictionary get_all_properties() const;
	void set_properties(const Dictionary &p_properties);

	// Port management
	void add_input_port(const Ref<FlowPort> &p_port);
	void add_output_port(const Ref<FlowPort> &p_port);
	void remove_input_port(const String &p_name);
	void remove_output_port(const String &p_name);
	
	Ref<FlowPort> get_input_port(const String &p_name) const;
	Ref<FlowPort> get_output_port(const String &p_name) const;
	Ref<FlowPort> get_port(const String &p_name) const;
	
	Array get_input_ports() const;
	Array get_output_ports() const;
	
	bool has_input_port(const String &p_name) const;
	bool has_output_port(const String &p_name) const;

	// Serialization
	Dictionary to_dict() const;
	void from_dict(const Dictionary &p_dict);

	FlowNode();
	~FlowNode();
};

#endif // FLOW_NODE_H
