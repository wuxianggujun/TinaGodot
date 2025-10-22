/**************************************************************************/
/*  flow_port.cpp                                                         */
/**************************************************************************/

#include "flow_port.h"

FlowPort::FlowPort() {
}

FlowPort::~FlowPort() {
}

void FlowPort::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_name", "name"), &FlowPort::set_name);
	ClassDB::bind_method(D_METHOD("get_name"), &FlowPort::get_name);
	
	ClassDB::bind_method(D_METHOD("set_type", "type"), &FlowPort::set_type);
	ClassDB::bind_method(D_METHOD("get_type"), &FlowPort::get_type);
	
	ClassDB::bind_method(D_METHOD("set_direction", "direction"), &FlowPort::set_direction);
	ClassDB::bind_method(D_METHOD("get_direction"), &FlowPort::get_direction);
	
	ClassDB::bind_method(D_METHOD("set_default_value", "value"), &FlowPort::set_default_value);
	ClassDB::bind_method(D_METHOD("get_default_value"), &FlowPort::get_default_value);
	
	ClassDB::bind_method(D_METHOD("is_compatible_with", "other"), &FlowPort::is_compatible_with);
	ClassDB::bind_method(D_METHOD("to_dict"), &FlowPort::to_dict);
	ClassDB::bind_method(D_METHOD("from_dict", "dict"), &FlowPort::from_dict);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "set_type", "get_type");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "direction"), "set_direction", "get_direction");
	ADD_PROPERTY(PropertyInfo(Variant::NIL, "default_value"), "set_default_value", "get_default_value");

	BIND_ENUM_CONSTANT(TYPE_EXECUTION);
	BIND_ENUM_CONSTANT(TYPE_INT);
	BIND_ENUM_CONSTANT(TYPE_FLOAT);
	BIND_ENUM_CONSTANT(TYPE_STRING);
	BIND_ENUM_CONSTANT(TYPE_BOOL);
	BIND_ENUM_CONSTANT(TYPE_VECTOR2);
	BIND_ENUM_CONSTANT(TYPE_COLOR);
	BIND_ENUM_CONSTANT(TYPE_ANY);

	BIND_ENUM_CONSTANT(INPUT);
	BIND_ENUM_CONSTANT(OUTPUT);
}

bool FlowPort::is_compatible_with(const Ref<FlowPort> &p_other) const {
	if (p_other.is_null()) {
		return false;
	}

	// Execution flow is only compatible with execution flow
	if (type == TYPE_EXECUTION || p_other->type == TYPE_EXECUTION) {
		return type == p_other->type;
	}

	// Any type is compatible with everything
	if (type == TYPE_ANY || p_other->type == TYPE_ANY) {
		return true;
	}

	// Same type is always compatible
	if (type == p_other->type) {
		return true;
	}

	// Implicit conversions
	// Int -> Float
	if (type == TYPE_INT && p_other->type == TYPE_FLOAT) {
		return true;
	}

	// Any type -> String (for printing)
	if (p_other->type == TYPE_STRING) {
		return true;
	}

	// Bool -> Int
	if (type == TYPE_BOOL && p_other->type == TYPE_INT) {
		return true;
	}

	return false;
}

String FlowPort::get_type_name(PortType p_type) {
	switch (p_type) {
		case TYPE_EXECUTION: return "Execution";
		case TYPE_INT: return "Int";
		case TYPE_FLOAT: return "Float";
		case TYPE_STRING: return "String";
		case TYPE_BOOL: return "Bool";
		case TYPE_VECTOR2: return "Vector2";
		case TYPE_COLOR: return "Color";
		case TYPE_ANY: return "Any";
		default: return "Unknown";
	}
}

Variant FlowPort::get_type_default_value(PortType p_type) {
	switch (p_type) {
		case TYPE_EXECUTION: return Variant();
		case TYPE_INT: return 0;
		case TYPE_FLOAT: return 0.0;
		case TYPE_STRING: return "";
		case TYPE_BOOL: return false;
		case TYPE_VECTOR2: return Vector2();
		case TYPE_COLOR: return Color(1, 1, 1, 1);
		case TYPE_ANY: return Variant();
		default: return Variant();
	}
}

Dictionary FlowPort::to_dict() const {
	Dictionary dict;
	dict["name"] = name;
	dict["type"] = (int)type;
	dict["direction"] = (int)direction;
	dict["default_value"] = default_value;
	dict["required"] = required;
	dict["description"] = description;
	return dict;
}

void FlowPort::from_dict(const Dictionary &p_dict) {
	if (p_dict.has("name")) {
		name = p_dict["name"];
	}
	if (p_dict.has("type")) {
		type = (PortType)(int)p_dict["type"];
	}
	if (p_dict.has("direction")) {
		direction = (PortDirection)(int)p_dict["direction"];
	}
	if (p_dict.has("default_value")) {
		default_value = p_dict["default_value"];
	}
	if (p_dict.has("required")) {
		required = p_dict["required"];
	}
	if (p_dict.has("description")) {
		description = p_dict["description"];
	}
}
