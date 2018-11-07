/*****************************************
	The Meta Behavior Tree Library
	Copyright (c) 2017 David Feng
	Distributed under the MIT License.
*****************************************/
#ifndef NODES_HPP
#define NODES_HPP
#include "common_nodes.hpp"
#include <boost/mpl/vector.hpp>
#include <boost/function.hpp>
#include <vector>
namespace mbt
{
	enum node_type
	{
		BASE_NODE = 0,
		PARALLEL_NODE = 1,
		SEQUENCE_NODE = 2,
		SELECTOR_NODE = 3,
		DECORATOR_NODE = 4,
		LEAF_NODE = 5,
	};
	enum node_type_category
	{
		BASE_NODE_CATEGORY = 0,
		COMPOSITE_NODE_CATEGORY = 1,
		ONE_CHILD_NODE_CATEGORY = 2,
		NO_CHILD_NODE_CATEGORY = 3,
	};
	enum status
	{
		SUCCESS = 0,
		FAILURE = 1,
		EXECUTING = 2,
	};
	class base_node;
	class btree
	{
	public:
		inline btree(base_node * root = nullptr);
		inline ~btree();
		inline void _execute();
		inline void _report(status s);
		inline void _notify(base_node * n);
		inline void _reset();
		inline base_node * _get_root_node() const;
		inline base_node * _get_executing_node() const;
		inline status _get_status() const;
	private:
		base_node * _root;
		base_node * _executing;
		status _status;
	};
	class base_node
	{
	public:
		base_node(node_type t = BASE_NODE, node_type_category c = BASE_NODE_CATEGORY, base_node * parent = nullptr, btree * bt = nullptr) : _type(t), _category(c), _parent(parent), _btree(bt)
		{
			_add_child_invoke = std::bind(std::mem_fn(&base_node::_add_child_base_node), this, std::placeholders::_1);
			_execute_invoke = std::bind(std::mem_fn(&base_node::_execute_base_node), this);
			_report_invoke = std::bind(std::mem_fn(&base_node::_report_base_node), this, std::placeholders::_1);
			dtor_bind = std::bind(std::mem_fn(&base_node::_dtor_base_node), this);
		}
		~base_node() { dtor_bind(); }
		void _set_parent(base_node * p) { _parent = p; }
		base_node * _get_parent() const { return _parent; }
		void _set_btree(btree * bt) { _btree = bt; }
		btree * _get_btree() const { return _btree; }
		void _add_child(base_node * c) { _add_child_invoke(c); }
		void _execute() { _execute_invoke(); }
		void _report(status s) { _report_invoke(s); }
		node_type _get_node_type() const { return _type; }
		node_type_category _get_node_type_category() const { return _category; }
	private:
		void _add_child_base_node(base_node * c) { }
		void _execute_base_node() { }
		void _report_base_node(status s) { }
	protected:
		void _dtor_base_node() { }
		const node_type _type;
		const node_type_category _category;
		base_node * _parent;
		btree * _btree;
		::boost::function<void(base_node *)> _add_child_invoke;
		::boost::function0<void> _execute_invoke;
		::boost::function<void(status)> _report_invoke;
		::boost::function0<void> dtor_bind;
	};
	class composite_node : public base_node
	{
	public:
		composite_node(node_type t, base_node * parent = nullptr, btree * bt = nullptr) : base_node(t, COMPOSITE_NODE_CATEGORY, parent, bt)
		{
			dtor_bind = std::bind(std::mem_fn(&composite_node::_dtor_composite_node), this);
		}
		~composite_node() { _dtor_composite_node(); }
	protected:
		void _dtor_composite_node() { _dtor_base_node(); }
	};
	class one_child_node : public base_node
	{
	public:
		one_child_node(node_type t, base_node * parent = nullptr, btree * bt = nullptr) : base_node(t, ONE_CHILD_NODE_CATEGORY, parent, bt)
		{
			dtor_bind = std::bind(std::mem_fn(&one_child_node::_dtor_one_child_node), this);
		}
		~one_child_node() { _dtor_one_child_node(); }
	protected:
		void _dtor_one_child_node() { _dtor_base_node(); }
	};
	class no_child_node : public base_node
	{
	public:
		no_child_node(node_type t, base_node * parent = nullptr, btree * bt = nullptr) : base_node(t, NO_CHILD_NODE_CATEGORY, parent, bt)
		{
			dtor_bind = std::bind(std::mem_fn(&no_child_node::_dtor_no_child_node), this);
		}
		~no_child_node() { _dtor_no_child_node(); }
	protected:
		void _dtor_no_child_node() { _dtor_base_node(); }
	};
	class selector_node : public composite_node
	{
	public:
		typedef ::boost::mpl::vector2<composite_node_tag, selector_node_tag> node_type_tag;
		selector_node(base_node * parent = nullptr, btree * bt = nullptr) : composite_node(SELECTOR_NODE, parent, bt), _children(), _chd_itr()
		{
			_add_child_invoke = std::bind(std::mem_fn(&selector_node::_add_child_selector_node), this, std::placeholders::_1);
			_execute_invoke = std::bind(std::mem_fn(&selector_node::_execute_selector_node), this);
			_report_invoke = std::bind(std::mem_fn(&selector_node::_report_selector_node), this, std::placeholders::_1);
			dtor_bind = std::bind(std::mem_fn(&selector_node::dtor_selector_node), this);
		}
		~selector_node() { dtor_selector_node(); }
	protected:
		void dtor_selector_node()
		{
			for (auto itr = _children.begin(); itr != _children.end(); ++itr)
				if (*itr) { delete (*itr); (*itr) = nullptr; }
			_dtor_composite_node();
		}
	private:
		void _add_child_selector_node(base_node * c) { _children.emplace_back(c); }
		void _execute_selector_node()
		{
			_chd_itr = _children.cbegin();
			if (_chd_itr != _children.cend())
				(*_chd_itr)->_execute();
		}
		void _report_selector_node(status s)
		{
			++_chd_itr;
			switch (s)
			{
			case SUCCESS:
				if (_parent)
					_parent->_report(s);
				else
					_btree->_report(s);
				break;
			case FAILURE:
				{
					if (_chd_itr != _children.cend())
						(*_chd_itr)->_execute();
					else if (_parent)
						_parent->_report(s);
					else
						_btree->_report(s);
				}
				break;
			case EXECUTING:
				break;
			default:
				break;
			}
		}
		std::vector<base_node *> _children;
		std::vector<base_node *>::const_iterator _chd_itr;
	};
	class sequence_node : public composite_node
	{
	public:
		typedef ::boost::mpl::vector2<composite_node_tag, sequence_node_tag> node_type_tag;
		sequence_node(base_node * parent = nullptr, btree * bt = nullptr) : composite_node(SEQUENCE_NODE, parent, bt), _children(), _chd_itr()
		{
			_add_child_invoke = std::bind(std::mem_fn(&sequence_node::_add_child_sequence_node), this, std::placeholders::_1);
			_execute_invoke = std::bind(std::mem_fn(&sequence_node::_execute_sequence_node), this);
			_report_invoke = std::bind(std::mem_fn(&sequence_node::_report_sequence_node), this, std::placeholders::_1);
			dtor_bind = std::bind(std::mem_fn(&sequence_node::dtor_sequence_node), this);
		}
		~sequence_node() { dtor_sequence_node(); }
	protected:
		void dtor_sequence_node()
		{
			for (auto itr = _children.begin(); itr != _children.end(); ++itr)
				if (*itr) { delete (*itr); (*itr) = nullptr; }
			_dtor_composite_node();
		}
	private:
		void _add_child_sequence_node(base_node * c) { _children.emplace_back(c); }
		void _execute_sequence_node()
		{
			_chd_itr = _children.cbegin();
			if (_chd_itr != _children.cend())
				(*_chd_itr)->_execute();
		}
		void _report_sequence_node(status s)
		{
			++_chd_itr;
			switch (s)
			{
			case SUCCESS:
				{
					if (_chd_itr != _children.cend())
						(*_chd_itr)->_execute();
					else if (_parent)
						_parent->_report(s);
					else
						_btree->_report(s);
				}
				break;
			case FAILURE:
				if (_parent)
					_parent->_report(s);
				else
					_btree->_report(s);
			break;
			case EXECUTING:
				break;
			default:
				break;
			}
		}
		std::vector<base_node *> _children;
		std::vector<base_node *>::const_iterator _chd_itr;
	};
	class decorator_node : public one_child_node
	{
	public:
		typedef ::boost::mpl::vector2<one_child_node_tag, decorator_node_tag> node_type_tag;
		decorator_node(base_node * parent = nullptr, btree * bt = nullptr) : one_child_node(DECORATOR_NODE, parent, bt), _child(nullptr)
		{
			_add_child_invoke = std::bind(std::mem_fn(&decorator_node::_add_child_decorator_node), this, std::placeholders::_1);
			_execute_invoke = std::bind(std::mem_fn(&decorator_node::_execute_decorator_node), this);
			_report_invoke = std::bind(std::mem_fn(&decorator_node::_report_decorator_node), this, std::placeholders::_1);
			dtor_bind = std::bind(std::mem_fn(&decorator_node::dtor_decorator_node), this);
		}
		~decorator_node() { dtor_decorator_node(); }
	protected:
		void dtor_decorator_node()
		{
			if (_child) { delete _child; _child = nullptr; }
			_dtor_one_child_node();
		}
	private:
		void _add_child_decorator_node(base_node * c) { _child = c; }
		void _execute_decorator_node()
		{
			if (execute_bind.empty()) return;
			status res = execute_bind();
			switch (res)
			{
			case SUCCESS:
				_child->_execute();
				break;
			case FAILURE:
				_report_invoke(EXECUTING);
				break;
			case EXECUTING:
				_btree->_notify(this);
				break;
			default:
				break;
			}
		}
		void _report_decorator_node(status s)
		{
			if (report_bind.empty()) return;
			status res = report_bind(s);
			switch (res)
			{
			case SUCCESS:
				if (_parent)
					_parent->_report(res);
				else
					_btree->_report(res);
				break;
			case FAILURE:
				if (_parent)
					_parent->_report(res);
				else
					_btree->_report(res);
				break;
			case EXECUTING:
				_execute_invoke();
				break;
			default:
				break;
			}
		}
		base_node * _child;
	protected:
		::boost::function0<status> execute_bind;
		::boost::function<status(status)> report_bind;
	};
	class leaf_node : public no_child_node
	{
	public:
		typedef ::boost::mpl::vector2<no_child_node_tag, leaf_node_tag> node_type_tag;
		leaf_node(base_node * parent = nullptr, btree * bt = nullptr) : no_child_node(LEAF_NODE, parent, bt)
		{
			_execute_invoke = std::bind(std::mem_fn(&leaf_node::_execute_leaf_node), this);
			_report_invoke = std::bind(std::mem_fn(&leaf_node::_report_leaf_node), this, std::placeholders::_1);
			dtor_bind = std::bind(std::mem_fn(&leaf_node::dtor_leaf_node), this);
		}
		~leaf_node() { dtor_leaf_node(); }
	protected:
		void dtor_leaf_node() { _dtor_no_child_node(); }
	private:
		void _execute_leaf_node()
		{
			if (step_bind.empty()) return;
			status res = step_bind();
			switch (res)
			{
			case SUCCESS:
				_report_invoke(res);
				break;
			case FAILURE:
				_report_invoke(res);
				break;
			case EXECUTING:
				_btree->_notify(this);
				break;
			default:
				break;
			}
		}
		void _report_leaf_node(status s)
		{
			if (_parent)
				_parent->_report(s);
			else
				_btree->_report(s);
		}
	protected:
		::boost::function0<status> step_bind;
	};
}
#include "btree.hpp"
#endif
