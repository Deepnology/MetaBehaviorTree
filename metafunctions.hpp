#ifndef METAFUNCTIONS_HPP
#define METAFUNCTIONS_HPP
#include <boost/mpl/set.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/prior.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/replace.hpp>
#include <boost/mpl/remove.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/reverse.hpp>
#include <boost/mpl/reverse_fold.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/utility/enable_if.hpp>
namespace mbt
{
template<class OUTPUT, class INPUT>
struct copy_vector
{
	typedef typename ::boost::mpl::fold<
		INPUT
		, OUTPUT
		, ::boost::mpl::push_back<
			::boost::mpl::placeholders::_1
			, ::boost::mpl::placeholders::_2
		>
	>::type type;
};

template<class COMP1, class COMP2, class TRUE, class FALSE>
struct eval_if_compare
{
	typedef typename ::boost::mpl::eval_if<
		::boost::is_same<
			COMP1
			, COMP2
		>
		, ::boost::mpl::identity<TRUE>
		, ::boost::mpl::identity<FALSE>
	>::type type;
};

struct _count_element_single
{
	template<class ELEMENT>
	struct apply
	{
		typedef typename ::boost::integral_constant<bool,1>::type type;
	};
};

template<class SEQ>
struct count_element
{
	typedef typename ::boost::mpl::count_if<
		SEQ
		, _count_element_single
	>::type type;
};

template<class SEQ, class BEGIN_ITR, class LAST_ITR>
struct erase_range_and_end
{
	typedef typename ::boost::mpl::erase<
		SEQ
		, BEGIN_ITR
		, LAST_ITR
	>::type tmp;
	typedef typename ::boost::mpl::deref<LAST_ITR>::type last;
	typedef typename ::boost::mpl::remove<
		tmp
		, last
	>::type type;
};

template<class INPUT_STACK, class ACCU_STACK>
struct push_stack//concatenate INPUT_STACK to the front of ACCU_STACK
{
	typedef typename ::boost::mpl::reverse_fold<
		INPUT_STACK
		, ACCU_STACK
		, ::boost::mpl::push_front<
			::boost::mpl::placeholders::_1
			, ::boost::mpl::placeholders::_2
		>
	>::type type;
};

template<class ITR, class STACK>
struct pop_stack//remove elements from begin<STACK> to ITR
{
	typedef typename ::boost::mpl::begin<STACK>::type begin;
	typedef typename erase_range_and_end<
		STACK
		, begin
		, ITR
	>::type type;
};
}
#endif