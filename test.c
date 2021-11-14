#include <stdlib.h>
#include <stddef.h> /* offsetof */
#include "sp_tree.h"

#define container_of(ptr, type, member)\
	((type *)((char *)(ptr) - offsetof(type, member)))

struct sp_int {
	int value;
	struct sp_node sp_n;
};

static struct sp_int *tree;
struct sp_int *sp_int_insert(int a)
{
	struct sp_node *n = &tree->sp_n, *p = NULL;
	enum sp_dir d;
	while(n){
		struct sp_int *t = container_of(n, struct sp_int, sp_n);
		if(a == t->value)
			return t;

		p = n;

		if(a < t->value){
			n = sp_left(n);
			d = LEFT;
		}

		else if(a > t->value){
			n = sp_right(n);
			d = RIGHT;
		}
	}

	struct sp_int *i = (struct sp_int *)calloc(sizeof(struct sp_int), 1);
	sp_insert(&tree->sp_n, p, &i->sp_n, d);
}

struct sp_int *sp_int_find(int a)
{
	struct sp_node *n = &tree->sp_n;
	while(n){
		struct sp_int *t = container_of(n, struct sp_int, sp_n);
		if(a == t->value)
			return t;

		if(a < t->value)
			n = sp_left(n);
		else if (a > t->value)
			n = sp_right(n);
	}

	return 0;
}

int sp_int_remove(int a)
{
	struct sp_int *n = sp_int_find(a);
	if(!n)
		return -1;

	sp_remove(&tree->sp_n, &n->sp_n);
	free(n);
	return 0;
}

int main()
{
}
