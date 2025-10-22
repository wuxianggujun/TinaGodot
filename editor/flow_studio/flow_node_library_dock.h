/**************************************************************************/
/*  flow_node_library_dock.h                                              */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/

#ifndef FLOW_NODE_LIBRARY_DOCK_H
#define FLOW_NODE_LIBRARY_DOCK_H

#include "scene/gui/box_container.h"
#include "scene/gui/tree.h"

class FlowNodeLibraryDock : public VBoxContainer {
	GDCLASS(FlowNodeLibraryDock, VBoxContainer);

private:
	Tree *node_tree = nullptr;
	
	void _populate_nodes();
	void _on_node_selected();
	void _on_node_activated();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	String get_selected_node_type() const;
	
	FlowNodeLibraryDock();
	~FlowNodeLibraryDock();
};

#endif // FLOW_NODE_LIBRARY_DOCK_H
