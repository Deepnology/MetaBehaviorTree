#ifndef META_TRAVERSAL_HPP
#define META_TRAVERSAL_HPP
#include "metafunctions.hpp"
#include "common_nodes.hpp"
namespace mbt
{
template<class REST, class FOUNDITR, class ENDITR>
struct getResultvec
{
	typedef typename ::boost::mpl::erase<
		REST
		, FOUNDITR
		, ENDITR
	>::type tmp;
	typedef typename eval_if_compare<
		FOUNDITR
		, ENDITR
		, tmp
		, typename ::boost::mpl::insert<
						tmp
						, typename ::boost::mpl::end<tmp>::type
						, typename ::boost::mpl::deref<FOUNDITR>::type
		>::type
	>::type type;
};

template<class FOUNDITR, class ENDITR>
struct getNextHead
{
	typedef typename eval_if_compare<
		FOUNDITR
		, ENDITR
		, null_node
		, typename ::boost::mpl::deref<FOUNDITR>::type
	>::type type;
};

template<class ROW>
struct getEndItr
{
	typedef typename ::boost::mpl::end<ROW>::type type;
};

template<class ROW>
struct getBeginItr
{
	typedef typename ::boost::mpl::begin<ROW>::type type;
};

template<class ROW>
struct getFront
{
	typedef typename ::boost::mpl::deref<typename getBeginItr<ROW>::type >::type type;
};

template<class ROW>
struct getBack
{
	typedef typename ::boost::mpl::deref<typename getEndItr<ROW>::type >::type type;
};

struct _find_if_composite_functor
{
	template<class NODE>
	struct apply
	{
		typedef typename ::boost::mpl::front<typename NODE::node_type_tag>::type tag;
		typedef ::boost::is_same<
			tag
			, composite_node_tag
		> type;
	};
};

template<class REST>
struct getNextHeadItr
{
	typedef typename ::boost::mpl::find_if<REST
		, _find_if_composite_functor
	>::type type;
};

template<class REST, class BEGINITR, class FOUNDITR, class ENDITR>
struct getStack
{
	typedef typename eval_if_compare<
		FOUNDITR
		, ENDITR
		, ::boost::mpl::vector0<>
		,typename erase_range_and_end<REST, BEGINITR, FOUNDITR>::type
	>::type type;
};

template<class ROW>
struct removeHead
{
	typedef typename ::boost::mpl::begin<ROW>::type begin_itr;
	typedef typename ::boost::mpl::deref<begin_itr>::type front;
	typedef typename ::boost::mpl::remove<ROW, front>::type type;
	//typedef typename ::boost::mpl::pop_front<ROW>::type type;
};

template<class TARGETNODE>
struct _getRow_functor
{
	template<class ACCUVEC, class ROW>
	struct apply
	{
		typedef typename ::boost::mpl::front<ROW>::type head;
		typedef typename eval_if_compare<
			TARGETNODE
			, head
			, typename copy_vector<ACCUVEC, ROW>::type
			, ACCUVEC
		>::type type;
	};
};

template<class COMPOSITE, class TABLE>
struct getRow
{
	typedef typename ::boost::mpl::fold<
		TABLE
		, ::boost::mpl::vector0<>
		, _getRow_functor<COMPOSITE>
	>::type type;
};
}

#endif