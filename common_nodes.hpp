/*****************************************
	The Meta Behavior Tree Library
	Copyright (c) 2017 David Feng
	Distributed under the MIT License.
*****************************************/
#ifndef COMMON_NODES_HPP
#define COMMON_NODES_HPP
#include <boost/mpl/vector.hpp>
namespace mbt
{
struct selector_node_tag {};
struct sequence_node_tag {};
struct parallel_node_tag {};
struct decorator_node_tag {};
struct leaf_node_tag {};
struct composite_node_tag {};
struct no_child_node_tag {};
struct one_child_node_tag {};
struct null_node_tag {};
class null_node
{
public:
	typedef ::boost::mpl::vector<null_node_tag, null_node> node_type_tag;
};
}
#endif