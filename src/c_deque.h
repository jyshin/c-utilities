/**************************************************\

Author: Ji-Yong Shin (jiyong.shin@gmail.com)

c_deque.h

C-based double ended queue implementation.

\**************************************************/

#ifndef __C_DEQUE_H__
#define __C_DEQUE_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>


typedef struct c_deque_node {
	struct c_deque_node *_next;
	struct c_deque_node *_prev;
	void *_data;
} _dq_node_t;

typedef _dq_node_t * dq_itr_t;

typedef struct c_deque {
	_dq_node_t *_front;
	_dq_node_t *_back;
	size_t _size;
} dq_t;


void dq_initialize(dq_t *_dq);
dq_itr_t dq_push_front(dq_t *_dq, void *_data);
dq_itr_t dq_push_back(dq_t *_dq, void *_data);
void *dq_pop_front(dq_t *_dq);
void *dq_pop_back(dq_t *_dq);
void *dq_front(dq_t *_dq);
void *dq_back(dq_t *_dq);
size_t dq_size(dq_t *_dq);
void *dq_at(dq_t *_dq, size_t _index);
void dq_insert_at(dq_t *_dq, size_t _index, void *_data);
void *dq_erase_at(dq_t *_dq, size_t _index);
void dq_clear(dq_t *_dq);
void dq_print(dq_t *_dq, FILE *_fp);

dq_itr_t dq_get_itr(dq_t *_dq);
dq_itr_t dq_itr_move_next(dq_itr_t _dq_itr);
void *dq_erase(dq_t *_dq, dq_itr_t _dq_itr);

#endif // end of __C_DEQUE_H__
