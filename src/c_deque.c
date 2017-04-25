/**************************************************\

Author: Ji-Yong Shin (jiyong.shin@gmail.com)

c_deque.c

C-based double ended queue implementation.

\**************************************************/

#include "c_deque.h"

void dq_initialize(dq_t *_dq) {
	memset(_dq, 0, sizeof(dq_t));
}


dq_itr_t dq_push_front(dq_t *_dq, void *_data)
{
	_dq_node_t *_new_node = (_dq_node_t *) malloc(sizeof(_dq_node_t));
	assert(_new_node != NULL);
	memset(_new_node, 0, sizeof(_dq_node_t));
	_new_node->_data = _data;
	if (_dq->_size == 0) {
		assert(_dq->_back == NULL && _dq->_front == NULL);
		_dq->_front = _new_node;
		_dq->_back = _new_node;
	} else {
		assert(_dq->_front->_prev == NULL);
		_dq->_front->_prev = _new_node;
		_new_node->_next = _dq->_front;
		_dq->_front = _new_node;
	}
	_dq->_size++;
	return _new_node;
}


dq_itr_t dq_push_back(dq_t *_dq, void *_data)
{
	_dq_node_t *_new_node = (_dq_node_t *) malloc(sizeof(_dq_node_t));
	assert(_new_node != NULL);
	memset(_new_node, 0, sizeof(_dq_node_t));
	_new_node->_data = _data;
	if (_dq->_size == 0) {
		assert(_dq->_back == NULL && _dq->_front == NULL);
		_dq->_front = _new_node;
		_dq->_back = _new_node;
	} else {
		assert(_dq->_back->_next == NULL);
		_dq->_back->_next = _new_node;
		_new_node->_prev = _dq->_back;
		_dq->_back = _new_node;
	}
	_dq->_size++;

	return _new_node;
}


void *dq_pop_front(dq_t *_dq)
{
	if (_dq->_size == 0) {
		fprintf(stderr, "ERROR: Attempt to remove from an "
			"empty deque!\n");
		exit(EXIT_FAILURE);
	}
	_dq_node_t *_pop_node = _dq->_front;
	void *_return_data = _pop_node->_data;
	_dq->_front = _pop_node->_next;
	if (_dq->_size > 1) {
		_dq->_front->_prev = NULL;
	} else if (_dq->_size == 1) {
		assert(_dq->_front == NULL);
		_dq->_back = NULL;
	}
	_dq->_size--;
	free(_pop_node);
	return _return_data;
}


void *dq_pop_back(dq_t *_dq)
{
	if (_dq->_size == 0) {
		fprintf(stderr, "ERROR: Attempt to remove from an "
			"empty deque!\n");
		exit(EXIT_FAILURE);
	}
	_dq_node_t *_pop_node = _dq->_back;
	void *_return_data = _pop_node->_data;
	_dq->_back = _pop_node->_prev;
	if (_dq->_size > 1) {
		_dq->_back->_next = NULL;
	} else if (_dq->_size == 1) {
		assert(_dq->_back == NULL);
		_dq->_front = NULL;
	}
	_dq->_size--;
	free(_pop_node);
	return _return_data;
}


void dq_print(dq_t *_dq, FILE *_fp)
{
	size_t _i;
	_dq_node_t *_curr_node = _dq->_front;
	for (_i = 0; _i < _dq->_size; _i++) {
		fprintf(_fp, "%lld ", (long long int) _curr_node->_data);
		_curr_node = _curr_node->_next;
	}
	fprintf(_fp, " (size=%zu) \n", dq_size(_dq));
}


void *dq_front(dq_t *_dq)
{
	if (_dq->_size <= 0) {
		fprintf(stderr, "ERROR: Attempt to get front from an "
			"empty deque!\n");
		exit(EXIT_FAILURE);
	}
	return _dq->_front->_data;
}


void *dq_back(dq_t *_dq)
{
	if (_dq->_size <= 0) {
		fprintf(stderr, "ERROR: Attempt to get back from an "
			"empty deque!\n");
		exit(EXIT_FAILURE);
	}
	return _dq->_back->_data;
}


inline
size_t dq_size(dq_t *_dq) {
	return _dq->_size;
}


void *dq_at(dq_t *_dq, size_t _index)
{
	if (_index >= _dq->_size || _index < 0) {
		fprintf(stderr, "ERROR: Attempt to access item at an "
			"out of bound index!n");
		exit(EXIT_FAILURE);
	}
	size_t _i;
	_dq_node_t *_curr_node = _dq->_front;
	for (_i = 0; _i < _index; _i++) {
		_curr_node = _curr_node->_next;
	}
	return _curr_node->_data;
}


void dq_insert_at(dq_t *_dq, size_t _index, void *_data)
{
	assert(_index <= _dq->_size && _index >= 0);
	if (_index == 0) {
		dq_push_front(_dq, _data);
	} else if (_index == _dq->_size) {
		dq_push_back(_dq, _data);
	} else {
		size_t _i;
		_dq_node_t *_curr_node;
		_dq_node_t *_new_node = (_dq_node_t *)
			malloc(sizeof(_dq_node_t));
		assert(_new_node != NULL);
		memset(_new_node, 0, sizeof(_dq_node_t));
		_new_node->_data = _data;
		_dq_node_t *_prev_node;
		_curr_node = _dq->_front;
		for (_i = 0; _i < _index; _i++) {
			_curr_node = _curr_node->_next;
		}
		_prev_node = _curr_node->_prev;
		_curr_node->_prev = _new_node;
		_prev_node->_next = _new_node;
		_new_node->_prev = _prev_node;
		_new_node->_next = _curr_node;
		_dq->_size++;
	}
}


void *dq_erase_at(dq_t *_dq, size_t _index)
{
	assert(_index < _dq->_size && _index >= 0);
	void *_return_data;
	if (_dq->_size == 0) {
		fprintf(stderr, "ERROR: Attempt to remove from an "
			"empty deque!\n");
		exit(EXIT_FAILURE);
	}
	if (_index == 0) {
		_return_data = dq_pop_front(_dq);
	} else if (_index == _dq->_size - 1) {
		_return_data = dq_pop_back(_dq);
	} else {
		size_t _i;
		_dq_node_t *_erase_node = _dq->_front;
		_dq_node_t *_prev_node;
		_dq_node_t *_next_node;
		for (_i = 0; _i < _index; _i++) {
			_erase_node = _erase_node->_next;
		}
		_return_data = _erase_node->_data;
		_prev_node = _erase_node->_prev;
		_next_node = _erase_node->_next;
		_prev_node->_next = _next_node;
		_next_node->_prev = _prev_node;
		free(_erase_node);
		_dq->_size--;
	}
	return _return_data;
}


void dq_clear(dq_t *_dq)
{
	size_t _cnt = 0;
	_dq_node_t *_curr_node = _dq->_front;
	_dq_node_t *_prev_node;
	while (_curr_node != NULL) {
		_prev_node = _curr_node;
		_curr_node = _curr_node->_next;
		free(_prev_node);
		_cnt++;
	}
	assert(_cnt == _dq->_size);
	_dq->_front = NULL;
	_dq->_back = NULL;
	_dq->_size = 0;
}


void *dq_erase(dq_t *_dq, dq_itr_t _dq_itr)
{
	_dq_node_t *_erase_node = _dq_itr;
	void *_return_data;
	if (_erase_node == _dq->_front) {
		_return_data = dq_pop_front(_dq);
	} else if (_erase_node == _dq->_back) {
		_return_data = dq_pop_back(_dq);
	}
	else {
		_return_data = _erase_node->_data;
		_dq_node_t *_prev_node;
		_dq_node_t *_next_node;
		_prev_node = _erase_node->_prev;
		_next_node = _erase_node->_next;
		_prev_node->_next = _next_node;
		_next_node->_prev = _prev_node;
		free(_erase_node);
		_dq->_size--;
	}
	return _return_data;
}


dq_itr_t dq_get_itr(dq_t *_dq)
{
	return _dq->_front;
}


dq_itr_t dq_itr_move_next(dq_itr_t _dq_itr)
{
	return _dq_itr->_next;
}
