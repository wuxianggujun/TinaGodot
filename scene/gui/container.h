/**************************************************************************/
/*  container.h                                                           */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include "scene/gui/control.h"

class Container : public Control {
	GDCLASS(Container, Control);

	bool pending_sort = false;
	void _sort_children();
	void _child_minsize_changed();

protected:
    // 中文导读：
    // Container 是用于对子控件进行自动布局的基类（如 BoxContainer、GridContainer）。
    // 核心点：
    // - 当子控件增删/移动/最小尺寸变化时，触发排序（queue_sort -> _sort_children）。
    // - 子控件通过 size flags/expand/minimum size 影响分配策略。
    // - fit_child_in_rect 将子控件约束在计算出的矩形内。
    // - 仅可见（或在树中可见）的子控件参与布局，可通过可见性模式调整。
    enum class SortableVisibilityMode {
        VISIBLE,
        VISIBLE_IN_TREE,
        IGNORE,
    };

	void queue_sort();
	Control *as_sortable_control(Node *p_node, SortableVisibilityMode p_visibility_mode = SortableVisibilityMode::VISIBLE_IN_TREE) const;

	virtual void add_child_notify(Node *p_child) override;
	virtual void move_child_notify(Node *p_child) override;
	virtual void remove_child_notify(Node *p_child) override;

	GDVIRTUAL0RC(Vector<int>, _get_allowed_size_flags_horizontal)
	GDVIRTUAL0RC(Vector<int>, _get_allowed_size_flags_vertical)

	void _notification(int p_what);
	static void _bind_methods();

public:
	enum {
		NOTIFICATION_PRE_SORT_CHILDREN = 50,
		NOTIFICATION_SORT_CHILDREN = 51,
	};

    void fit_child_in_rect(Control *p_child, const Rect2 &p_rect);
    // 中文：将子控件放入给定矩形，一般由派生容器在
    // _notification(NOTIFICATION_SORT_CHILDREN) 中按策略计算后调用。

	virtual Vector<int> get_allowed_size_flags_horizontal() const;
	virtual Vector<int> get_allowed_size_flags_vertical() const;

	PackedStringArray get_configuration_warnings() const override;

	Container();
};
