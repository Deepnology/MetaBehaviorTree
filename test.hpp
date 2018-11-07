/*****************************************
	The Meta Behavior Tree Library
	Copyright (c) 2017 David Feng
	Distributed under the MIT License.
*****************************************/
#ifndef TEST_HPP
#define TEST_HPP
#include "behavior_tree.hpp"
#include <boost/mpl/vector.hpp>
#include <random>
#include <time.h>
#include <iostream>
namespace test
{
/*
Define a custom player class with the following behavior tree configuration
, in which decorator_node::execute, decorator_node::report and leaf_node::step all return a random status.

   ___________________________________Sel0_________________________________
  /                     |                              |                   \
 Dec0        _________Sel1__________          _______Seq2________         Leaf14
  |         /                |      \        /           |       \
 Dec1     Seq0              Seq1   Leaf7   Dec6         Dec7    Leaf13
  |      /    \            /    \           |            |
Leaf0  Dec2  Dec3        Dec4   Dec5       Seq3         Sel4
	    |     |           |      |        /    \       /    \
	  Leaf1  Sel2        Sel3  Leaf6    Leaf8 Leaf9 Leaf10  Seq4
	        /    \      /    \                             /    \
	      Leaf2 Leaf3 Leaf4 Leaf5                       Leaf11 Leaf12
*/
class player_def : public mbt::behavior_tree_def
{
	static mbt::status Rand() { return (mbt::status)(rand() % 3); }
public:
	// Define sequence nodes
	class Seq0 : public mbt::sequence_node
	{
	public:
		Seq0() { dtor_bind = std::bind(std::mem_fn(&Seq0::dtor), this); std::cout << "Seq0::Ctor" << std::endl; }
		~Seq0() { dtor_sequence_node(); }
		void dtor() { std::cout << "Seq0::Dtor" << std::endl; dtor_sequence_node(); }
	};
	class Seq1 : public mbt::sequence_node
	{
	public:
		Seq1() { dtor_bind = std::bind(std::mem_fn(&Seq1::dtor), this); std::cout << "Seq1::Ctor" << std::endl; }
		~Seq1() { dtor_sequence_node(); }
		void dtor() { std::cout << "Seq1::Dtor" << std::endl; dtor_sequence_node(); }
	};
	class Seq2 : public mbt::sequence_node
	{
	public:
		Seq2() { dtor_bind = std::bind(std::mem_fn(&Seq2::dtor), this); std::cout << "Seq2::Ctor" << std::endl; }
		~Seq2() { dtor_sequence_node(); }
		void dtor() { std::cout << "Seq2::Dtor" << std::endl; dtor_sequence_node(); }
	};
	class Seq3 : public mbt::sequence_node
	{
	public:
		Seq3() { dtor_bind = std::bind(std::mem_fn(&Seq3::dtor), this); std::cout << "Seq3::Ctor" << std::endl; }
		~Seq3() { dtor_sequence_node(); }
		void dtor() { std::cout << "Seq3::Dtor" << std::endl; dtor_sequence_node(); }
	};
	class Seq4 : public mbt::sequence_node
	{
	public:
		Seq4() { dtor_bind = std::bind(std::mem_fn(&Seq4::dtor), this); std::cout << "Seq4::Ctor" << std::endl; }
		~Seq4() { dtor_sequence_node(); }
		void dtor() { std::cout << "Seq4::Dtor" << std::endl; dtor_sequence_node(); }
	};

	// Define selector nodes
	class Sel0 : public mbt::selector_node
	{
	public:
		Sel0() { dtor_bind = std::bind(std::mem_fn(&Sel0::dtor), this); std::cout << "Sel0::Ctor" << std::endl; }
		~Sel0() { dtor_selector_node(); }
		void dtor() { std::cout << "Sel0::Dtor" << std::endl; dtor_selector_node(); }
	};
	class Sel1 : public mbt::selector_node
	{
	public:
		Sel1() { dtor_bind = std::bind(std::mem_fn(&Sel1::dtor), this); std::cout << "Sel1::Ctor" << std::endl; }
		~Sel1() { dtor_selector_node(); }
		void dtor() { std::cout << "Sel1::Dtor" << std::endl; dtor_selector_node(); }
	};
	class Sel2 : public mbt::selector_node
	{
	public:
		Sel2() { dtor_bind = std::bind(std::mem_fn(&Sel2::dtor), this); std::cout << "Sel2::Ctor" << std::endl; }
		~Sel2() { dtor_selector_node(); }
		void dtor() { std::cout << "Sel2::Dtor" << std::endl; dtor_selector_node(); }
	};
	class Sel3 : public mbt::selector_node
	{
	public:
		Sel3() { dtor_bind = std::bind(std::mem_fn(&Sel3::dtor), this); std::cout << "Sel3::Ctor" << std::endl; }
		~Sel3() { dtor_selector_node(); }
		void dtor() { std::cout << "Sel3::Dtor" << std::endl; dtor_selector_node(); }
	};
	class Sel4 : public mbt::selector_node
	{
	public:
		Sel4() { dtor_bind = std::bind(std::mem_fn(&Sel4::dtor), this); std::cout << "Sel4::Ctor" << std::endl; }
		~Sel4() { dtor_selector_node(); }
		void dtor() { std::cout << "Sel4::Dtor" << std::endl; dtor_selector_node(); }
	};

	// Define decorator nodes
	class Dec0 : public mbt::decorator_node
	{
	public:
		Dec0() { dtor_bind = std::bind(std::mem_fn(&Dec0::dtor), this); execute_bind = std::bind(std::mem_fn(&Dec0::execute), this); report_bind = std::bind(std::mem_fn(&Dec0::report), this, std::placeholders::_1); std::cout << "Dec0::Ctor" << std::endl; }
		~Dec0() { dtor_decorator_node(); }
		void dtor() { std::cout << "Dec0::Dtor" << std::endl; dtor_decorator_node(); }
		mbt::status execute() { std::cout << "Dec0::execute()" << std::endl; return Rand(); }
		mbt::status report(mbt::status s) { std::cout << "Dec0::report(" << s << ")" << std::endl; return Rand(); }
	};
	class Dec1 : public mbt::decorator_node
	{
	public:
		Dec1() { dtor_bind = std::bind(std::mem_fn(&Dec1::dtor), this); execute_bind = std::bind(std::mem_fn(&Dec1::execute), this); report_bind = std::bind(std::mem_fn(&Dec1::report), this, std::placeholders::_1); std::cout << "Dec1::Ctor" << std::endl; }
		~Dec1() { dtor_decorator_node(); }
		void dtor() { std::cout << "Dec1::Dtor" << std::endl; dtor_decorator_node(); }
		mbt::status execute() { std::cout << "Dec1::execute()" << std::endl; return Rand(); }
		mbt::status report(mbt::status s) { std::cout << "Dec1::report(" << s << ")" << std::endl; return Rand(); }
	};
	class Dec2 : public mbt::decorator_node
	{
	public:
		Dec2() { dtor_bind = std::bind(std::mem_fn(&Dec2::dtor), this); execute_bind = std::bind(std::mem_fn(&Dec2::execute), this); report_bind = std::bind(std::mem_fn(&Dec2::report), this, std::placeholders::_1); std::cout << "Dec2::Ctor" << std::endl; }
		~Dec2() { dtor_decorator_node(); }
		void dtor() { std::cout << "Dec2::Dtor" << std::endl; dtor_decorator_node(); }
		mbt::status execute() { std::cout << "Dec2::execute()" << std::endl; return Rand(); }
		mbt::status report(mbt::status s) { std::cout << "Dec2::report(" << s << ")" << std::endl; return Rand(); }
	};
	class Dec3 : public mbt::decorator_node
	{
	public:
		Dec3() { dtor_bind = std::bind(std::mem_fn(&Dec3::dtor), this); execute_bind = std::bind(std::mem_fn(&Dec3::execute), this); report_bind = std::bind(std::mem_fn(&Dec3::report), this, std::placeholders::_1); std::cout << "Dec3::Ctor" << std::endl; }
		~Dec3() { dtor_decorator_node(); }
		void dtor() { std::cout << "Dec3::Dtor" << std::endl; dtor_decorator_node(); }
		mbt::status execute() { std::cout << "Dec3::execute()" << std::endl; return Rand(); }
		mbt::status report(mbt::status s) { std::cout << "Dec3::report(" << s << ")" << std::endl; return Rand(); }
	};
	class Dec4 : public mbt::decorator_node
	{
	public:
		Dec4() { dtor_bind = std::bind(std::mem_fn(&Dec4::dtor), this); execute_bind = std::bind(std::mem_fn(&Dec4::execute), this); report_bind = std::bind(std::mem_fn(&Dec4::report), this, std::placeholders::_1); std::cout << "Dec4::Ctor" << std::endl; }
		~Dec4() { dtor_decorator_node(); }
		void dtor() { std::cout << "Dec4::Dtor" << std::endl; dtor_decorator_node(); }
		mbt::status execute() { std::cout << "Dec4::execute()" << std::endl; return Rand(); }
		mbt::status report(mbt::status s) { std::cout << "Dec4::report(" << s << ")" << std::endl; return Rand(); }
	};
	class Dec5 : public mbt::decorator_node
	{
	public:
		Dec5() { dtor_bind = std::bind(std::mem_fn(&Dec5::dtor), this); execute_bind = std::bind(std::mem_fn(&Dec5::execute), this); report_bind = std::bind(std::mem_fn(&Dec5::report), this, std::placeholders::_1); std::cout << "Dec5::Ctor" << std::endl; }
		~Dec5() { dtor_decorator_node(); }
		void dtor() { std::cout << "Dec5::Dtor" << std::endl; dtor_decorator_node(); }
		mbt::status execute() { std::cout << "Dec5::execute()" << std::endl; return Rand(); }
		mbt::status report(mbt::status s) { std::cout << "Dec5::report(" << s << ")" << std::endl; return Rand(); }
	};
	class Dec6 : public mbt::decorator_node
	{
	public:
		Dec6() { dtor_bind = std::bind(std::mem_fn(&Dec6::dtor), this); execute_bind = std::bind(std::mem_fn(&Dec6::execute), this); report_bind = std::bind(std::mem_fn(&Dec6::report), this, std::placeholders::_1); std::cout << "Dec6::Ctor" << std::endl; }
		~Dec6() { dtor_decorator_node(); }
		void dtor() { std::cout << "Dec6::Dtor" << std::endl; dtor_decorator_node(); }
		mbt::status execute() { std::cout << "Dec6::execute()" << std::endl; return Rand(); }
		mbt::status report(mbt::status s) { std::cout << "Dec6::report(" << s << ")" << std::endl; return Rand(); }
	};
	class Dec7 : public mbt::decorator_node
	{
	public:
		Dec7() { dtor_bind = std::bind(std::mem_fn(&Dec7::dtor), this); execute_bind = std::bind(std::mem_fn(&Dec7::execute), this); report_bind = std::bind(std::mem_fn(&Dec7::report), this, std::placeholders::_1); std::cout << "Dec7::Ctor" << std::endl; }
		~Dec7() { dtor_decorator_node(); }
		void dtor() { std::cout << "Dec7::Dtor" << std::endl; dtor_decorator_node(); }
		mbt::status execute() { std::cout << "Dec7::execute()" << std::endl; return Rand(); }
		mbt::status report(mbt::status s) { std::cout << "Dec7::report(" << s << ")" << std::endl; return Rand(); }
	};

	// Define leaf nodes
	class Leaf0 : public mbt::leaf_node
	{
	public:
		Leaf0() { dtor_bind = std::bind(std::mem_fn(&Leaf0::dtor), this); step_bind = std::bind(std::mem_fn(&Leaf0::step), this); std::cout << "Leaf0::Ctor" << std::endl; }
		~Leaf0() { dtor_leaf_node(); }
		void dtor() { std::cout << "Leaf0::Dtor" << std::endl; dtor_leaf_node(); }
		mbt::status step() { std::cout << "Leaf0::step()" << std::endl; return Rand(); }
	};
	class Leaf1 : public mbt::leaf_node
	{
	public:
		Leaf1() { dtor_bind = std::bind(std::mem_fn(&Leaf1::dtor), this); step_bind = std::bind(std::mem_fn(&Leaf1::step), this); std::cout << "Leaf1::Ctor" << std::endl; }
		~Leaf1() { dtor_leaf_node(); }
		void dtor() { std::cout << "Leaf1::Dtor" << std::endl; dtor_leaf_node(); }
		mbt::status step() { std::cout << "Leaf1::step()" << std::endl; return Rand(); }
	};
	class Leaf2 : public mbt::leaf_node
	{
	public:
		Leaf2() { dtor_bind = std::bind(std::mem_fn(&Leaf2::dtor), this); step_bind = std::bind(std::mem_fn(&Leaf2::step), this); std::cout << "Leaf2::Ctor" << std::endl; }
		~Leaf2() { dtor_leaf_node(); }
		void dtor() { std::cout << "Leaf2::Dtor" << std::endl; dtor_leaf_node(); }
		mbt::status step() { std::cout << "Leaf2::step()" << std::endl; return Rand(); }
	};
	class Leaf3 : public mbt::leaf_node
	{
	public:
		Leaf3() { dtor_bind = std::bind(std::mem_fn(&Leaf3::dtor), this); step_bind = std::bind(std::mem_fn(&Leaf3::step), this); std::cout << "Leaf3::Ctor" << std::endl; }
		~Leaf3() { dtor_leaf_node(); }
		void dtor() { std::cout << "Leaf3::Dtor" << std::endl; dtor_leaf_node(); }
		mbt::status step() { std::cout << "Leaf3::step()" << std::endl; return Rand(); }
	};
	class Leaf4 : public mbt::leaf_node
	{
	public:
		Leaf4() { dtor_bind = std::bind(std::mem_fn(&Leaf4::dtor), this); step_bind = std::bind(std::mem_fn(&Leaf4::step), this); std::cout << "Leaf4::Ctor" << std::endl; }
		~Leaf4() { dtor_leaf_node(); }
		void dtor() { std::cout << "Leaf4::Dtor" << std::endl; dtor_leaf_node(); }
		mbt::status step() { std::cout << "Leaf4::step()" << std::endl; return Rand(); }
	};
	class Leaf5 : public mbt::leaf_node
	{
	public:
		Leaf5() { dtor_bind = std::bind(std::mem_fn(&Leaf5::dtor), this); step_bind = std::bind(std::mem_fn(&Leaf5::step), this); std::cout << "Leaf5::Ctor" << std::endl; }
		~Leaf5() { dtor_leaf_node(); }
		void dtor() { std::cout << "Leaf5::Dtor" << std::endl; dtor_leaf_node(); }
		mbt::status step() { std::cout << "Leaf5::step()" << std::endl; return Rand(); }
	};
	class Leaf6 : public mbt::leaf_node
	{
	public:
		Leaf6() { dtor_bind = std::bind(std::mem_fn(&Leaf6::dtor), this); step_bind = std::bind(std::mem_fn(&Leaf6::step), this); std::cout << "Leaf6::Ctor" << std::endl; }
		~Leaf6() { dtor_leaf_node(); }
		void dtor() { std::cout << "Leaf6::Dtor" << std::endl; dtor_leaf_node(); }
		mbt::status step() { std::cout << "Leaf6::step()" << std::endl; return Rand(); }
	};
	class Leaf7 : public mbt::leaf_node
	{
	public:
		Leaf7() { dtor_bind = std::bind(std::mem_fn(&Leaf7::dtor), this); step_bind = std::bind(std::mem_fn(&Leaf7::step), this); std::cout << "Leaf7::Ctor" << std::endl; }
		~Leaf7() { dtor_leaf_node(); }
		void dtor() { std::cout << "Leaf7::Dtor" << std::endl; dtor_leaf_node(); }
		mbt::status step() { std::cout << "Leaf7::step()" << std::endl; return Rand(); }
	};
	class Leaf8 : public mbt::leaf_node
	{
	public:
		Leaf8() { dtor_bind = std::bind(std::mem_fn(&Leaf8::dtor), this); step_bind = std::bind(std::mem_fn(&Leaf8::step), this); std::cout << "Leaf8::Ctor" << std::endl; }
		~Leaf8() { dtor_leaf_node(); }
		void dtor() { std::cout << "Leaf8::Dtor" << std::endl; dtor_leaf_node(); }
		mbt::status step() { std::cout << "Leaf8::step()" << std::endl; return Rand(); }
	};
	class Leaf9 : public mbt::leaf_node
	{
	public:
		Leaf9() { dtor_bind = std::bind(std::mem_fn(&Leaf9::dtor), this); step_bind = std::bind(std::mem_fn(&Leaf9::step), this); std::cout << "Leaf9::Ctor" << std::endl; }
		~Leaf9() { dtor_leaf_node(); }
		void dtor() { std::cout << "Leaf9::Dtor" << std::endl; dtor_leaf_node(); }
		mbt::status step() { std::cout << "Leaf9::step()" << std::endl; return Rand(); }
	};
	class Leaf10 : public mbt::leaf_node
	{
	public:
		Leaf10() { dtor_bind = std::bind(std::mem_fn(&Leaf10::dtor), this); step_bind = std::bind(std::mem_fn(&Leaf10::step), this); std::cout << "Leaf10::Ctor" << std::endl; }
		~Leaf10() { dtor_leaf_node(); }
		void dtor() { std::cout << "Leaf10::Dtor" << std::endl; dtor_leaf_node(); }
		mbt::status step() { std::cout << "Leaf10::step()" << std::endl; return Rand(); }
	};
	class Leaf11 : public mbt::leaf_node
	{
	public:
		Leaf11() { dtor_bind = std::bind(std::mem_fn(&Leaf11::dtor), this); step_bind = std::bind(std::mem_fn(&Leaf11::step), this); std::cout << "Leaf11::Ctor" << std::endl; }
		~Leaf11() { dtor_leaf_node(); }
		void dtor() { std::cout << "Leaf11::Dtor" << std::endl; dtor_leaf_node(); }
		mbt::status step() { std::cout << "Leaf11::step()" << std::endl; return Rand(); }
	};
	class Leaf12 : public mbt::leaf_node
	{
	public:
		Leaf12() { dtor_bind = std::bind(std::mem_fn(&Leaf12::dtor), this); step_bind = std::bind(std::mem_fn(&Leaf12::step), this); std::cout << "Leaf12::Ctor" << std::endl; }
		~Leaf12() { dtor_leaf_node(); }
		void dtor() { std::cout << "Leaf12::Dtor" << std::endl; dtor_leaf_node(); }
		mbt::status step() { std::cout << "Leaf12::step()" << std::endl; return Rand(); }
	};
	class Leaf13 : public mbt::leaf_node
	{
	public:
		Leaf13() { dtor_bind = std::bind(std::mem_fn(&Leaf13::dtor), this); step_bind = std::bind(std::mem_fn(&Leaf13::step), this); std::cout << "Leaf13::Ctor" << std::endl; }
		~Leaf13() { dtor_leaf_node(); }
		void dtor() { std::cout << "Leaf13::Dtor" << std::endl; dtor_leaf_node(); }
		mbt::status step() { std::cout << "Leaf13::step()" << std::endl; return Rand(); }
	};
	class Leaf14 : public mbt::leaf_node
	{
	public:
		Leaf14() { dtor_bind = std::bind(std::mem_fn(&Leaf14::dtor), this); step_bind = std::bind(std::mem_fn(&Leaf14::step), this); std::cout << "Leaf14::Ctor" << std::endl; }
		~Leaf14() { dtor_leaf_node(); }
		void dtor() { std::cout << "Leaf14::Dtor" << std::endl; dtor_leaf_node(); }
		mbt::status step() { std::cout << "Leaf14::step()" << std::endl; return Rand(); }
	};

public:
	// Assign some composite node to root_node
	typedef Sel0 root_node;

	// Define config_table
	struct config_table: boost::mpl::vector<
	//                  composite_node        children_nodes
	//                +----------------+-----------------------------------------+
	boost::mpl::vector< Sel0,			Dec0,Dec1,Leaf0,Sel1,Seq2,Leaf14        >,
	boost::mpl::vector< Seq0,			Dec2,Leaf1,Dec3,Sel2                    >,
	boost::mpl::vector< Sel2,			Leaf2,Leaf3                             >,
	boost::mpl::vector< Sel1,			Seq0,Seq1,Leaf7                         >,
	boost::mpl::vector< Seq1,			Dec4,Sel3,Dec5,Leaf6                    >,
	boost::mpl::vector< Sel3,			Leaf4,Leaf5                             >,
	boost::mpl::vector< Seq2,			Dec6,Seq3,Dec7,Sel4,Leaf13              >,
	boost::mpl::vector< Seq3,			Leaf8,Leaf9                             >,
	boost::mpl::vector< Sel4,			Leaf10,Seq4                             >,
	boost::mpl::vector< Seq4,			Leaf11,Leaf12                           >
	> {};
/*
   ___________________________________Sel0_________________________________
  /                     |                              |                   \
 Dec0        _________Sel1__________          _______Seq2________         Leaf14
  |         /                |      \        /           |       \
 Dec1     Seq0              Seq1   Leaf7   Dec6         Dec7    Leaf13
  |      /    \            /    \           |            |
Leaf0  Dec2  Dec3        Dec4   Dec5       Seq3         Sel4
	    |     |           |      |        /    \       /    \
	  Leaf1  Sel2        Sel3  Leaf6    Leaf8 Leaf9 Leaf10  Seq4
	        /    \      /    \                             /    \
	      Leaf2 Leaf3 Leaf4 Leaf5                       Leaf11 Leaf12
*/

protected:
	// Overwrite member functions
	void execute()
	{
		std::cout << "player::execute()" << std::endl;
	}
	void report(mbt::status s)
	{
		std::cout << "player::report(" << s << ")" << std::endl;
	}
	void notify(mbt::base_node * n)
	{
		std::cout << "player::notify(" << n << ")" << std::endl;
	}
	void reset()
	{
		std::cout << "====== player::reset() ======" << std::endl;
	}
};

// Define the player class
typedef mbt::behavior_tree<player_def> player;

// Driver function for testing
void Run()
{
	srand((size_t)time(0));
	player p;
	for (size_t i = 0; i < 200; ++i)
		p.play();
	p.reset();
	for (size_t i = 0; i < 200; ++i)
		p.play();
}
}
#endif
