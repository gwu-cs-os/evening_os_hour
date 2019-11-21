/***
 * Copyright 2017 by Gabriel Parmer.  All rights reserved.
 * This file is licensed under the BSD 2 clause license.
 *
 * Authors: Gabriel Parmer, gparmer@gwu.edu, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <ps_list.h>

#define LIST_LEN 10

struct n {
	int num;
	int padding[15];
	struct ps_list list;
};

void
foo(struct ps_list_head *h, struct n *m)
{ ps_list_head_add_d(h, m); } __attribute__((noinline))

int
main(void)
{
	struct n *nodes[LIST_LEN], *node, *tmp = NULL;
	struct ps_list_head h;
	int i;

	ps_list_head_init(&h);
	for (i = 0 ; i < LIST_LEN ; i++) {
		nodes[i] = malloc(sizeof(struct n));
		assert(nodes[i]);
		ps_list_init_d(nodes[i]);
		nodes[i]->num = i;
	}

	assert(ps_list_head_empty(&h));
	assert(ps_list_singleton_d(nodes[0]));
	assert(ps_list_next_d(nodes[0]) == nodes[0]);
	assert(ps_list_prev_d(nodes[0]) == nodes[0]);
	assert(ps_container(&nodes[0]->list, struct n, list) == nodes[0]);

	for (i = 0 ; i < LIST_LEN ; i++) {
		ps_list_head_add_d(&h, nodes[i]);
		assert(ps_list_prev_d(nodes[i]) == ps_container(&h, struct n, list));
		assert(nodes[i] == ps_list_head_first(&h, struct n, list));
		if (i == 0) {
			assert(ps_list_next_d(nodes[i]) == ps_container(&h, struct n, list));
			assert(ps_list_prev_d(nodes[i]) == ps_container(&h, struct n, list));
			assert(nodes[i] == ps_list_head_first(&h, struct n, list));
			assert(nodes[i] == ps_list_head_last(&h, struct n, list));
		} else {
			assert(ps_list_next_d(nodes[i]) != ps_container(&h, struct n, list));
			assert(nodes[i] != ps_list_head_last(&h, struct n, list));
		}
	}

	i = LIST_LEN-1;
	ps_list_foreach_d(&h, node) {
		assert(node->num == i);
		i--;
	}
	assert(i == -1);

	for (i = 0 ; i < LIST_LEN ; i++) {
		struct n *f = ps_list_head_first(&h, struct n, list);

		assert(tmp != f);
		ps_list_rem_d(f);
		assert(ps_list_singleton_d(f));
		ps_list_head_append_d(&h, f);
		tmp = f;
	}

	i = LIST_LEN - 1;
	ps_list_foreach_d(&h, node) {
		assert(node->num == i);
		i--;
	}
	assert(i == -1);

	ps_list_foreach_del_d(&h, node, tmp) {
		ps_list_rem_d(node);
		assert(ps_list_singleton_d(node));
	}
	assert(ps_list_head_empty(&h));

	printf("Linked List Tests SUCCESS.\n");

	return 0;
}
