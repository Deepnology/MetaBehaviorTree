#ifndef BTREE_HPP
#define BTREE_HPP
namespace mbt
{
btree::btree(base_node * root) : _root(root), _executing(nullptr), _status(SUCCESS)
{

}
btree::~btree()
{
	if (_root) { delete _root; _root = nullptr; }
}
void btree::_execute()
{
	if (_status != EXECUTING)
	{
		_status = EXECUTING;
		_root->_execute();
	}
	else
		_executing->_execute();
}
void btree::_report(status s)
{
	_status = s;
}
void btree::_notify(base_node * n)
{
	_status = EXECUTING;
	_executing = n;
}
void btree::_reset()
{
	_status = SUCCESS;
	_executing = nullptr;
}
base_node * btree::_get_root_node() const { return _root; }
base_node * btree::_get_executing_node() const { return _executing; }
status btree::_get_status() const { return _status; }
}
#endif
