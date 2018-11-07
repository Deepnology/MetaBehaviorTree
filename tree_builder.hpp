/*****************************************
	The Meta Behavior Tree Library
	Copyright (c) 2017 David Feng
	Distributed under the MIT License.
*****************************************/
#ifndef TREE_BUILDER_HPP
#define TREE_BUILDER_HPP
#include "metafunctions.hpp"
#include "meta_traversal.hpp"
#include "nodes.hpp"
#include <deque>
namespace mbt
{
typedef std::deque<base_node *> stack_type;

template<class COMPOSITE, class TABLE, class STACK = ::boost::mpl::vector0<> >
struct tree_builder//look for row starting with COMPOSITE in TABLE (in this case COMPOSITE is not a null_node)
{
	//get row
	typedef typename getRow<COMPOSITE, TABLE>::type row;
	typedef typename removeHead<row>::type rest;
	typedef typename getBeginItr<rest>::type begin_itr;
	typedef typename getEndItr<rest>::type end_itr;
	typedef typename getNextHeadItr<rest>::type found_itr;//get the 1st composite_node: found_itr or end<STACK>
	typedef typename getNextHead<found_itr, end_itr>::type next_head;//next_head or null_node
	typedef typename getResultvec<rest, found_itr, end_itr>::type resultvec;//doesn't include head; include next_head
	typedef typename getStack<rest, begin_itr, found_itr, end_itr>::type tmp_stack;
	typedef typename push_stack<tmp_stack, STACK>::type stack;
	static size_t const tmp_stack_length = ::boost::mpl::size<tmp_stack>::type::value;//since the tree_builder template will be diff for diff rows, this const value doesn't share with each other (each template instance has an independent static const value)
	static void GO(base_node * p, stack_type * s, btree * bt)//row level recursion
	{
		//now consume all nodes in resultvec, start inner recur
		base_node * last_node = 
			instantiate<resultvec>::NEW(p, p, bt);//last_node must be a composite_node: either the same as p (no composite_node in resultvec) or diff from p (has a composite_node in the end of resultvec)
		
		//get size<tmp_stack>, then s->push_front(p) for size<tmp_stack> times
		for (static size_t i = 0; i < tmp_stack_length; ++i)
			s->push_front(p);

		//all nodes in resultvec have been constructed, move on to stack or next row depending on if next_head is a null_node or not
		tree_builder<next_head, TABLE, stack>::GO(last_node, s, bt);//next row with next_head (actually next_head's instance is last_node)
	}
};

template<class TABLE, class STACK>
struct tree_builder<null_node, TABLE, STACK>//in this case next_head is a null_node (end of search TABLE), look for stack
{
	typedef typename getBeginItr<STACK>::type begin_itr;
	typedef typename getEndItr<STACK>::type end_itr;
	typedef typename getNextHeadItr<STACK>::type found_itr;//get the 1st composite_node: found_itr or end<STACK>
	typedef typename getNextHead<found_itr, end_itr>::type next_head;//next_head or null_node
	typedef typename getResultvec<STACK, found_itr, end_itr>::type resultvec;//get the vector from begin<STACK> to the 1st composite node (found_itr included)
	typedef typename pop_stack<found_itr, STACK>::type stack;//remove the nodes from begin<STACK> to found_itr (found_itr is removed)
	static void GO(base_node * p, stack_type * s, btree * bt)//row level recursion
	{
		//now consume all nodes in resultvec, start inner recur
		base_node * last_node = 
			instantiate_from_stack<resultvec>::NEW(s, s->front(), s->front(), bt);//last_node could be either: a composite_node (next_head, continue recursive search TABLE) or null ptr (end)

		//now all nodes in resutvec have been constructed, look for next row starting with next_head in TABLE
		tree_builder<next_head, TABLE, stack>::GO(last_node, s, bt);
	}
};

template<class TABLE>
struct tree_builder<null_node, TABLE, ::boost::mpl::vector0<> >//in this case next_head is a null_node and stack is vector0<>, then end of tree building
{
	static void GO(base_node * p, stack_type * s, btree * bt)
	{
		//recursion stop here
	}
};

template<class CHILDVEC>
struct instantiate
{
	typedef typename getFront<CHILDVEC>::type child;
	typedef typename removeHead<CHILDVEC>::type remain_children;
	static base_node * NEW(base_node * p1, base_node * p2, btree * bt)//in the beginning, p1 = p2
	{
		//now instantiate new child node
		base_node * new_node = new child();
		new_node->_set_parent(p2);
		new_node->_set_btree(bt);
		p2->_add_child(new_node);
		if (new_node->_get_node_type_category() == ONE_CHILD_NODE_CATEGORY)//new_node is a decorator_node, which must not be the last node in vector
		{
			return instantiate<remain_children>::NEW(p1, new_node, bt);
		}
		else if (new_node->_get_node_type_category() == COMPOSITE_NODE_CATEGORY)//new_node is a composite_node, which must be the last node in vector
		{
			return instantiate<remain_children>::NEW(p1, new_node, bt);
		}
		else//new_node must be a leaf node
		{
			return instantiate<remain_children>::NEW(p1, p1, bt);
		}
	}
};

template<>
struct instantiate<::boost::mpl::vector0<> >//in the case CHILDVEC is empty
{
	static base_node * NEW(base_node * p1, base_node * p2, btree * bt)
	{
		//end of instantiating nodes in CHILDVEC
		return p2;//return original head parent if the last node is a leaf, otherwise return the last composite node if the last node is a composite
	}
};

template<class CHILDVEC>
struct instantiate_from_stack
{
	typedef typename getFront<CHILDVEC>::type child;
	typedef typename removeHead<CHILDVEC>::type remain_children;
	static base_node * NEW(stack_type * s, base_node * p1, base_node * p2, btree * bt)//return the last composite_node in CHILDVEC; if no composite_node in the end, then end stack
	{
		s->pop_front();
		//now instantiate new child node
		base_node * new_node = new child();
		new_node->_set_parent(p2);
		new_node->_set_btree(bt);
		p2->_add_child(new_node);
		if (new_node->_get_node_type_category() == ONE_CHILD_NODE_CATEGORY)//new_node is a decorator_node, which must not be the last node in vector
		{
			return instantiate_from_stack<remain_children>::NEW(s, s->front(), new_node, bt);
		}
		else if (new_node->_get_node_type_category() == COMPOSITE_NODE_CATEGORY)//new_node is a composite_node, which must be the last node in vector
		{
			return instantiate_from_stack<remain_children>::NEW(s, p1, new_node, bt);//s must be empty, remain_children must be vector0<> in this case
		}
		else//new_node must be a leaf node
		{
			if (!s->empty())
				return instantiate_from_stack<remain_children>::NEW(s, s->front(), s->front(), bt);
			else
				return instantiate_from_stack<remain_children>::NEW(s, 0, 0, bt);
		}
	}
};

template<>
struct instantiate_from_stack<::boost::mpl::vector0<> >
{
	static base_node * NEW(stack_type * s, base_node * p1, base_node * p2, btree * bt)//return the last composite_node in CHILDVEC; if no composite_node in the end, then end stack
	{
		return p2;
	}
};
}
#endif