/**************************************************

Author: Ji-Yong Shin (jiyong.shin@gmail.com)

c_avlt.c

C-based AVLTree implementation.

**************************************************/

#include "c_avlt.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


static inline _avlt_node_t * _create_new_node()
{
	_avlt_node_t *_node = (_avlt_node_t *) malloc(sizeof(_avlt_node_t));
	assert(_node != NULL);
	_node->_key = _INIT_VAL;
	_node->_value = NULL;
	_node->_balance = 0;
	_node->_parent = NULL;
	_node->_left_child = NULL;
	_node->_right_child = NULL;
	return _node;
}

static inline int compare(uint _node_key, uint _input_key)
{
	if (_node_key > _input_key) {
		return _LEFT;
	} else if (_node_key < _input_key) {
		return _RIGHT;
	} else {
		return _EQUAL;
	}
}

static inline bool _is_leaf(_avlt_node_t * _node)
{
	return (_node->_left_child == NULL && _node->_right_child == NULL);
}


static inline _avlt_node_t * _get_max_of_left_subtree(_avlt_node_t *_node)
{
	_node = _node->_left_child;
	if (_node != NULL) {
		while (_node->_right_child != NULL) {
			_node = _node->_right_child;
		}
	}
	return _node;
}

static inline _avlt_node_t * _get_min_of_right_subtree(_avlt_node_t *_node)
{
	_node = _node->_right_child;
	if (_node != NULL) {
		while (_node->_left_child != NULL) {
			_node = _node->_right_child;
		}
	}
	return _node;
}

static inline void _balance_adjust_after_insertion_turn(int _rotation_type,
							_avlt_node_t *_top,
							_avlt_node_t *_middle,
							_avlt_node_t *_bottom)
{
	switch (_rotation_type) {
	case _RR:
		_top->_balance = 0;
		_middle->_balance = 0;
		break;
	case _RL:
		if (_bottom->_balance == _LEFT) {
			_middle->_balance = _RIGHT;
			_top->_balance = 0;
		} else if (_bottom->_balance == _RIGHT) {
			_middle->_balance = 0;
			_top->_balance = _LEFT;
		} else {
			_middle->_balance = 0;
			_top->_balance = 0;
		}
		_bottom->_balance = 0;
		break;
	case _LL:
		_top->_balance = 0;
		_middle->_balance = 0;
		break;
	case _LR:
		if (_bottom->_balance == _LEFT) {
			_middle->_balance = 0;
			_top->_balance = _RIGHT;
		} else if (_bottom->_balance == _RIGHT) {
			_middle->_balance = _LEFT;
			_top->_balance = 0;
		} else {
			_middle->_balance = 0;
			_top->_balance = 0;
		}
		_bottom->_balance = 0;
		break;
	default:
		fprintf(stderr, "ERROR: Unknown rotation type!\n");
		exit(1);
	}
}

static inline void _balance_adjust_after_deletion_turn(int _rotation_type,
						       _avlt_node_t *_top,
						       _avlt_node_t *_middle,
						       _avlt_node_t *_bottom)
{
	switch (_rotation_type) {
	case _RR:
		if (_middle->_balance == _LEFT) {
			_top->_balance = 0;
			_middle->_balance = 0;
		} else {
			assert(_middle->_balance == 0);
			_top->_balance = _LEFT;
			_middle->_balance = _RIGHT;
		}
		break;
	case _RL:
		if (_bottom->_balance == _LEFT) {
			_middle->_balance = _RIGHT;
			_top->_balance = 0;
		} else if (_bottom->_balance == _RIGHT) {
			_middle->_balance = 0;
			_top->_balance = _LEFT;
		} else {
			_middle->_balance = 0;
			_top->_balance = 0;
		}
		_bottom->_balance = 0;
		break;
	case _LL:
		if (_middle->_balance == _RIGHT) {
			_top->_balance = 0;
			_middle->_balance = 0;
		} else {
			assert(_middle->_balance == 0);
			_top->_balance = _RIGHT;
			_middle->_balance = _LEFT;
		}
		break;
	case _LR:
		if (_bottom->_balance == _LEFT) {
			_middle->_balance = 0;
			_top->_balance = _RIGHT;
		} else if (_bottom->_balance == _RIGHT) {
			_middle->_balance = _LEFT;
			_top->_balance = 0;
		} else {
			_middle->_balance = 0;
			_top->_balance = 0;
		}
		_bottom->_balance = 0;
		break;
	default:
		fprintf(stderr, "ERROR: Unknown rotation type!\n");
		exit(1);
	}
}

static inline void _rr_rotation(avlt_t *_avlt, _avlt_node_t *_top,
				_avlt_node_t *_middle)
{
	_avlt_node_t *_parent = _top->_parent;
	_top->_left_child = _middle->_right_child;
	if (_top->_left_child != NULL) {
		_top->_left_child->_parent = _top;
	}
	_top->_parent = _middle;
	_middle->_right_child = _top;
	if (_parent != NULL) {
		if (_parent->_right_child == _top) {
			_parent->_right_child = _middle;
		} else if (_parent->_left_child == _top) {
			_parent->_left_child = _middle;
		} else {
			assert(false);
		}
		_middle->_parent = _parent;
	} else {
		_avlt->_root = _middle;
		_middle->_parent = NULL;
	}
}

static inline void _lr_rotation(avlt_t *_avlt, _avlt_node_t *_top,
				_avlt_node_t *_middle, _avlt_node_t *_bottom)
{
	_avlt_node_t *_parent = _top->_parent;
	_top->_left_child = _bottom->_right_child;
	if (_top->_left_child != NULL) {
		_top->_left_child->_parent = _top;
	}
	_top->_parent = _bottom;
	_bottom->_right_child = _top;
	_middle->_right_child = _bottom->_left_child;
	if (_middle->_right_child != NULL) {
		_middle->_right_child->_parent = _middle;
	}
	_middle->_parent = _bottom;
	_bottom->_left_child = _middle;
	if (_parent != NULL) {
		if (_parent->_right_child == _top) {
			_parent->_right_child = _bottom;
		} else if (_parent->_left_child == _top) {
			_parent->_left_child = _bottom;
		} else {
			assert(false);
		}
		_bottom->_parent = _parent;
	} else {
		_avlt->_root = _bottom;
		_bottom->_parent = NULL;
	}
}

static inline void _ll_rotation(avlt_t *_avlt, _avlt_node_t *_top,
				_avlt_node_t *_middle)
{
	_avlt_node_t *_parent = _top->_parent;
	_top->_right_child = _middle->_left_child;
	if (_top->_right_child != NULL) {
		_top->_right_child->_parent = _top;
	}
	_top->_parent = _middle;
	_middle->_left_child = _top;
	if (_parent != NULL) {
		if (_parent->_right_child == _top) {
			_parent->_right_child = _middle;
		} else if (_parent->_left_child == _top) {
			_parent->_left_child = _middle;
		} else {
			assert(false);
		}
		_middle->_parent = _parent;
	} else {
		_avlt->_root = _middle;
		_middle->_parent = NULL;
	}
}

static inline void _rl_rotation(avlt_t *_avlt, _avlt_node_t *_top,
				_avlt_node_t *_middle, _avlt_node_t *_bottom)
{
	_avlt_node_t *_parent = _top->_parent;
	_top->_right_child = _bottom->_left_child;
	if (_top->_right_child != NULL) {
		_top->_right_child->_parent = _top;
	}
	_top->_parent = _bottom;
	_bottom->_left_child = _top;
	_middle->_left_child = _bottom->_right_child;
	if (_middle->_left_child != NULL) {
		_middle->_left_child->_parent = _middle;
	}
	_middle->_parent = _bottom;
	_bottom->_right_child = _middle;
	if (_parent != NULL) {
		if (_parent->_right_child == _top) {
			_parent->_right_child = _bottom;
		} else if (_parent->_left_child == _top) {
			_parent->_left_child = _bottom;
		} else {
			assert(false);
		}
		_bottom->_parent = _parent;
	} else {
		_avlt->_root = _bottom;
		_bottom->_parent = NULL;
	}
}

static inline void _balance_after_insert(avlt_t *_avlt, _avlt_node_t *_node,
					 _avlt_node_t *_new_node)
{
	assert(_node->_balance <= 1 && _node->_balance >= -1);
	short balance = _node->_balance;
	if (balance == 0) {
		return;
	}
	_avlt_node_t *prevprev = _new_node;
	_avlt_node_t *prev = _node;
	_avlt_node_t *curr = _node->_parent;

	while (curr != NULL) {
		assert(curr->_balance <= 1 && curr->_balance >= -1);
		if (curr->_left_child == prev) {
			curr->_balance = curr->_balance + _LEFT;
		} else if (curr->_right_child == prev) {
			curr->_balance = curr->_balance + _RIGHT;
		} else {
			assert(false);
		}

		// This means there is no more hight change propagation
		if (curr->_balance == 0) {
			break;
		}

		if (curr->_balance == _LEFT2X) {
			assert(prev->_balance == _LEFT ||
			       prev->_balance == _RIGHT);
			if (prev->_balance == _LEFT) {
				_rr_rotation(_avlt, curr, prev);
				_balance_adjust_after_insertion_turn(_RR, curr,
								     prev,
								     NULL);
				break;
			} else {
				_lr_rotation(_avlt, curr, prev, prevprev);
				_balance_adjust_after_insertion_turn(_LR, curr,
								     prev,
								     prevprev);
				break;
			}
		} else if (curr->_balance == _RIGHT2X) {
			assert(prev->_balance == 1 || prev->_balance == -1);
			if (prev->_balance == _RIGHT) {
				_ll_rotation(_avlt, curr, prev);
				_balance_adjust_after_insertion_turn(_LL, curr,
								     prev,
								     NULL);
				break;
			} else {
				_rl_rotation(_avlt, curr, prev, prevprev);
				_balance_adjust_after_insertion_turn(_RL, curr,
								     prev,
								     prevprev);
				break;
			}
		}
		prevprev = prev;
		prev = curr;
		curr = curr->_parent;
	}  // end of while (_node != NULL)
}

static inline void _balance_after_delete(avlt_t *_avlt, _avlt_node_t *_node)
{
	if (_node->_balance == _LEFT || _node->_balance == _RIGHT) {
		return;
	}

	_avlt_node_t *_top = _node;
	_avlt_node_t *_middle;
	_avlt_node_t *_bottom;

	while (_top != NULL) {
		if (_top->_balance == _LEFT2X) {
			_middle = _top->_left_child;

			if (_middle->_balance == _RIGHT) {
				_bottom = _middle->_right_child;
				_lr_rotation(_avlt, _top, _middle, _bottom);
				_balance_adjust_after_deletion_turn(_LR, _top,
								    _middle,
								    _bottom);
			} else {
				_rr_rotation(_avlt, _top, _middle);
				_balance_adjust_after_deletion_turn(_RR, _top,
								    _middle,
								    NULL);
			}
		} else if (_top->_balance == _RIGHT2X) {
			_middle = _top->_right_child;
			if (_middle->_balance == _LEFT) {
				_bottom = _middle->_left_child;
				_rl_rotation(_avlt, _top, _middle, _bottom);
				_balance_adjust_after_deletion_turn(_RL, _top,
								    _middle,
								    _bottom);
			} else {
				_ll_rotation(_avlt, _top, _middle);
				_balance_adjust_after_deletion_turn(_LL, _top,
								    _middle,
								    NULL);
			}
		} else if (_top->_balance == 0) {
			if (_top->_parent != NULL) {
				if (_top == _top->_parent->_left_child) {
					_top->_parent->_balance =
						_top->_parent->_balance - _LEFT;
				} else if (_top ==
					   _top->_parent->_right_child) {
					_top->_parent->_balance =
						_top->_parent->_balance -
						_RIGHT;
				} else {
					assert(false);
				}
			}
		} else {
			break;
		}
		_top = _top->_parent;
	}
}

// returns NULL if key is not found
static inline _avlt_node_t * _map_search(avlt_t *_avlt, uint _key)
{
	_avlt_node_t *_curr = _avlt->_root;
	while (_curr != NULL) {
		int _cmp_result = compare(_curr->_key, _key);
		if (_cmp_result == _LEFT) {
			_curr = _curr->_left_child;
		} else if (_cmp_result == _RIGHT) {
			_curr = _curr->_right_child;
		} else {
			return _curr;
		}
	}  // end of while (curr != NULL) {
	return _curr;
}

static int _height(_avlt_node_t *_node) {
	int _left_height = 0;
	int _right_height = 0;
	if (_node->_left_child != NULL) {
		_left_height = _height(_node->_left_child) + 1;
	}
	if (_node->_right_child != NULL) {
		_right_height = _height(_node->_right_child) + 1;
	}
	if (_left_height > _right_height) {
		return _left_height;
	} else {
		return _right_height;
	}
}

int avlt_validate_order(_avlt_node_t *_node)
{
	uint _left_key = 0;
	uint _right_key = UINT32_MAX;
	if (_node->_left_child != NULL) {
		if (_node->_left_child->_parent != _node) {
			fprintf(stderr, "L ");
		}
		_left_key = avlt_validate_order(_node->_left_child);

	}
	if (_node->_right_child != NULL) {
		if (_node->_right_child->_parent != _node) {
			fprintf(stderr, "R ");
		}
		_right_key = avlt_validate_order(_node->_right_child);
	}

	if (_left_key >= _node->_key) {
		fprintf(stderr, "### AVLT FAILED ### left %u node %u\n",
			_left_key, _node->_key);
		return 0;
	} else if (_right_key <= _node->_key) {
		fprintf(stderr, "### AVLT FAILED ### node %u right %u\n",
			_node->_key, _right_key);
		return 0;
	} else {
		return _node->_key;
	}
}

// TODO: add comparison function
void avlt_init(avlt_t *_avlt)
{
	_avlt->_root = NULL;
	_avlt->_size = 0;
}

void * avlt_search(avlt_t *_avlt, uint _key)
{
	_avlt_node_t * _found = _map_search(_avlt, _key);
	if (_found == NULL) {
		return NULL;
	} else {
		return _found->_value;
	}
}

void avlt_insert(avlt_t *_avlt, uint _key, void *_value)
{
	int _cmp_result;
	_avlt_node_t *_curr = _avlt->_root;
	_avlt_node_t *_new_node = _create_new_node();

	_avlt->_size++;

	_new_node->_key = _key;
	_new_node->_value = _value;

	if (_curr == NULL) {
		_avlt->_root = _new_node;
		return;
	}

	// Find insertion point, the _parent of new node.
	while (_curr != NULL) {
		_cmp_result = compare(_curr->_key, _key);
		if (_cmp_result == _LEFT) {
			if (_curr->_left_child != NULL)
				_curr = _curr->_left_child;
			else
				break;
		} else if (_cmp_result == _RIGHT) {
			if(_curr->_right_child != NULL)
				_curr = _curr->_right_child;
			else
				break;
		} else {
			fprintf(stderr, "ERROR: Redundant key (%u) value "
				"found while inserting!\n", _key);
			assert(false);
		}
	}  // end of while (curr != NULL) {

	_new_node->_parent = _curr;
	if (_cmp_result == _LEFT) {
		_curr->_left_child = _new_node;
		_curr->_balance = _curr->_balance + _LEFT;

		_balance_after_insert(_avlt, _curr, _new_node);
	} else {
		_curr->_right_child = _new_node;
		_curr->_balance = _curr->_balance + _RIGHT;

		_balance_after_insert(_avlt, _curr, _new_node);
	}
}

void avlt_delete(avlt_t *_avlt, uint _key)
{
	_avlt_node_t *_avlt_node_to_delete =  _map_search(_avlt, _key);

	assert(_avlt_node_to_delete != NULL);

	_avlt_node_t *_parent = NULL;
	_avlt_node_t *_avlt_node_to_attach = NULL;

	_avlt->_size--;
	if (_avlt->_size == 0) {
		assert(_parent == NULL);
		_avlt->_root = NULL;
		free(_avlt_node_to_delete);
		return;
	}

	// Find max of left sutree or min of right subtree
	_avlt_node_t *_replacement_node =
		_get_max_of_left_subtree(_avlt_node_to_delete);
	if (_replacement_node == NULL) {
		_replacement_node =
			_get_min_of_right_subtree(_avlt_node_to_delete);
	}

	if (_replacement_node != NULL) {
		// Copy key and value of replacement node to the
		// _avlt_node_to_delete and assume it is deleted.
		_avlt_node_to_delete->_key = _replacement_node->_key;
		_replacement_node->_key = _INIT_VAL;
		_avlt_node_to_delete->_value = _replacement_node->_value;
		_replacement_node->_value = NULL;

		// Since the replacement node is copied to the
		// _avlt_node_to_delete, now the replacement node becomes
		// the one to be deleted.
		_avlt_node_to_delete = _replacement_node;
	}

	_parent = _avlt_node_to_delete->_parent;

	// At this point, _avlt_node_to_delete is guranteed to
	// have a _parent node and _avlt_node_to_delete has at
	// most one child node.

	if (_avlt_node_to_delete->_left_child != NULL) {
		_avlt_node_to_attach = _avlt_node_to_delete->_left_child;
	}
	if (_avlt_node_to_delete->_right_child != NULL) {
		assert(_avlt_node_to_attach == NULL);
		_avlt_node_to_attach = _avlt_node_to_delete->_right_child;
	}


	// Attach the child from the _avlt_node_to_delete
	// to the _parent, and adjust the balance.
	if (_parent->_left_child == _avlt_node_to_delete) {
		_parent->_left_child = _avlt_node_to_attach;
		_parent->_balance = _parent->_balance - _LEFT;
	} else if (_parent->_right_child == _avlt_node_to_delete) {
		_parent->_right_child = _avlt_node_to_attach;
		_parent->_balance = _parent->_balance - _RIGHT;
	}
	if (_avlt_node_to_attach != NULL) {
		_avlt_node_to_attach->_parent = _parent;
	}

	free(_avlt_node_to_delete);

	_balance_after_delete(_avlt, _parent);
}

static void print_node(_avlt_node_t *_node, int _level, int _print_level)
{
	if (_node->_left_child != NULL) {
		print_node(_node->_left_child, _level + 1, _print_level);
	} else if (_level == _print_level) {
		fprintf(stderr, " [ ]");
	}
	if (_level == _print_level) {
		fprintf(stderr, "%lld(%d|%d)", (long long int) _node->_key,
			_level, _node->_balance);
	}
	if (_node->_right_child != NULL) {
		print_node(_node->_right_child, _level + 1, _print_level);
	} else if (_level == _print_level) {
		fprintf(stderr, "[ ] ");
	}
}

void avlt_print(avlt_t *_avlt)
{
	int _i = 0;
	if (_avlt->_root == NULL) {
		return;
	}
	fprintf(stderr, "----------\n");
	int _h = _height(_avlt->_root);
	for (_i = 0; _i < _h + 1; _i++) {
		print_node(_avlt->_root, 0, _i);
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\nHeight [%d], Total [%lld] nodes\n",
		_height(_avlt->_root), (long long int) _avlt->_size);
	fprintf(stderr, "----------\n");
}

