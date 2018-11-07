/*****************************************
	The Meta Behavior Tree Library
	Copyright (c) 2017 David Feng
	Distributed under the MIT License.
*****************************************/
#ifndef BEHAVIOR_TREE_HPP
#define BEHAVIOR_TREE_HPP
#include "nodes.hpp"
#include "tree_builder.hpp"
namespace mbt
{
class behavior_tree_def
{
protected:
	void execute() { }
	void report(status s) { }
	void notify(base_node * n) { }
	void reset() { }
};
template<class Derived>
class behavior_tree : public Derived
{
	typedef typename Derived::root_node root_node;
	typedef typename Derived::config_table user_table;
	typedef tree_builder<root_node, user_table> builder;
	btree * _btree;
public:
behavior_tree<Derived>() : Derived(), _btree(new btree(new root_node()))
{
	_btree->_get_root_node()->_set_btree(_btree);
	stack_type stk;
	builder::GO(_btree->_get_root_node(), &stk, _btree);
}
~behavior_tree<Derived>()
{
	if (_btree) { delete _btree; _btree = nullptr; }
}
void play()
{
	Derived::execute();
	_btree->_execute();
	if (_btree->_get_status() == EXECUTING)
		Derived::notify(_btree->_get_executing_node());
	else
		Derived::report(_btree->_get_status());
}
void reset()
{
	_btree->_reset();
	Derived::reset();
}
};
}
#endif
