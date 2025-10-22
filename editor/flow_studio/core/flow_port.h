/**************************************************************************/
/*  flow_port.h                                                           */
/**************************************************************************/
/*                         This file is part of:                          */
/*                          TINAFLOWSTUDIO                                */
/*                       https://tinaflowstudio.org                       */
/**************************************************************************/

#ifndef FLOW_PORT_H
#define FLOW_PORT_H

#include "core/object/ref_counted.h"
#include "core/string/ustring.h"
#include "core/variant/variant.h"

class FlowPort : public RefCounted {
	GDCLASS(FlowPort, RefCounted);

public:
	enum PortType {
		TYPE_EXECUTION, // 执行流
		TYPE_INT,       // 整数
		TYPE_FLOAT,     // 浮点数
		TYPE_STRING,    // 字符串
		TYPE_BOOL,      // 布尔值
		TYPE_VECTOR2,   // 2D向量
		TYPE_COLOR,     // 颜色
		TYPE_ANY        // 任意类型
	};

	enum PortDirection {
		INPUT,  // 输入端口
		OUTPUT  // 输出端口
	};

private:
	String name;
	PortType type = TYPE_ANY;
	PortDirection direction = INPUT;
	Variant default_value;
	bool required = false;
	String description;

protected:
	static void _bind_methods();

public:
	FlowPort();
	~FlowPort();

	// Getters and Setters
	void set_name(const String &p_name) { name = p_name; }
	String get_name() const { return name; }

	void set_type(PortType p_type) { type = p_type; }
	PortType get_type() const { return type; }

	void set_direction(PortDirection p_direction) { direction = p_direction; }
	PortDirection get_direction() const { return direction; }

	void set_default_value(const Variant &p_value) { default_value = p_value; }
	Variant get_default_value() const { return default_value; }

	void set_required(bool p_required) { required = p_required; }
	bool is_required() const { return required; }

	void set_description(const String &p_description) { description = p_description; }
	String get_description() const { return description; }

	// Type compatibility check
	bool is_compatible_with(const Ref<FlowPort> &p_other) const;
	
	// Get type name as string
	static String get_type_name(PortType p_type);
	
	// Get default value for type
	static Variant get_type_default_value(PortType p_type);

	// Serialization
	Dictionary to_dict() const;
	void from_dict(const Dictionary &p_dict);
};

VARIANT_ENUM_CAST(FlowPort::PortType);
VARIANT_ENUM_CAST(FlowPort::PortDirection);

#endif // FLOW_PORT_H
