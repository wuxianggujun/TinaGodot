/**************************************************************************/
/*  flow_connection.h                                                     */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/

#ifndef FLOW_CONNECTION_H
#define FLOW_CONNECTION_H

#include "core/object/ref_counted.h"
#include "core/string/ustring.h"
#include "core/variant/variant.h"

class FlowConnection : public RefCounted {
	GDCLASS(FlowConnection, RefCounted);

private:
	int from_node = -1;
	String from_port;
	int to_node = -1;
	String to_port;

protected:
	static void _bind_methods();

public:
	// Getters and Setters
	void set_from_node(int p_node_id);
	int get_from_node() const;

	void set_from_port(const String &p_port_name);
	String get_from_port() const;

	void set_to_node(int p_node_id);
	int get_to_node() const;

	void set_to_port(const String &p_port_name);
	String get_to_port() const;

	// Utility
	bool is_valid() const;
	bool equals(const Ref<FlowConnection> &p_other) const;

	// Serialization
	Dictionary to_dict() const;
	void from_dict(const Dictionary &p_dict);

	FlowConnection();
	FlowConnection(int p_from_node, const String &p_from_port, int p_to_node, const String &p_to_port);
	~FlowConnection();
};

#endif // FLOW_CONNECTION_H
