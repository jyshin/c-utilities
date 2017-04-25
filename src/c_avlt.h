/**************************************************\

Author: Ji-Yong Shin (jiyong.shin@gmail.com)

c_avlt.h

C-based AVLTree implementation.

\**************************************************/

#ifndef __C_AVLT_H__
#define __C_AVLT_H__

#include <stdlib.h>
#include <stdint.h>

#define _LEFT2X (2)
#define _RIGHT2X (-2)
#define _LEFT (1)
#define _RIGHT (-1)
#define _EQUAL (0)

enum rotation_type {
	_RR,
	_RL,
	_LL,
	_LR,
	_RRE,
	_LLE,
};

#define _INIT_VAL UINT32_MAX

typedef struct _avlt_node {
	uint _key;
	void *_value;
	char _balance;
	struct _avlt_node *_parent;
	struct _avlt_node *_left_child;
	struct _avlt_node *_right_child;
} _avlt_node_t;

typedef struct c_avlt {
	_avlt_node_t *_root;
	size_t _size;
} avlt_t;

void avlt_init(avlt_t *_avlt);
void *avlt_search(avlt_t *_avlt, uint _key);
void avlt_insert(avlt_t *_avlt, uint _key, void *_value);
void avlt_delete(avlt_t *_avlt, uint _key);
int avlt_validate_order(_avlt_node_t *_node);
void avlt_print(avlt_t *_avlt);

#endif  // end of #ifndef __C_AVLT_H__
