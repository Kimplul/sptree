#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stddef.h> /* offsetof */
#include "sp_tree.h"

#define container_of(ptr, type, member)\
	((type *)((char *)(ptr) - offsetof(type, member)))

#define int_container(ptr)\
	container_of(ptr, struct sp_int, sp_n)

struct sp_int {
	int value;
	struct sp_node sp_n;
};

static struct sp_root *tree;
int sp_int_init()
{
	tree = (struct sp_root *)calloc(sizeof(struct sp_root), 1);
	if(!tree)
		return -1;

	return 0;
}

void __sp_int_destroy(struct sp_node *n)
{
	if(sp_left(n))
		__sp_int_destroy(sp_left(n));

	if(sp_right(n))
		__sp_int_destroy(sp_right(n));

	struct sp_int *i = int_container(n);
	free(i);
}

int sp_int_destroy()
{
	if(!tree)
		return -1;

	if(sp_root(tree))
		__sp_int_destroy(sp_root(tree));

	free(tree);
	return 0;
}

struct sp_int *sp_int_insert(int a)
{
	struct sp_node *n = sp_root(tree), *p = NULL;
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
	if(sp_root(tree))
		sp_insert(&sp_root(tree), p, &i->sp_n, d);
	else
		sp_root(tree) = &i->sp_n;

	i->value = a;
	return i;
}

struct sp_int *sp_int_find(int a)
{
	struct sp_node *n = sp_root(tree);
	while(n){
		struct sp_int *t = int_container(n);
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

	sp_remove(&sp_root(tree), &n->sp_n);
	free(n);
	return 0;
}

static void __sp_int_padding(char c, int n)
{
	for(int i = 0; i < n; ++i)
		putchar(c);
}

static void __sp_int_print(struct sp_node *n, int depth)
{
	if(n == NULL){
		__sp_int_padding(' ', 4 * depth);
		puts("~");
		return;
	}

	__sp_int_print(sp_right(n), depth + 1);

	__sp_int_padding(' ', 4 * depth);
	struct sp_int *v = int_container(n);
	printf("%d\n", v->value);

	__sp_int_print(sp_left(n), depth + 1);
}

void sp_int_print()
{
	__sp_int_print(sp_root(tree), 0);
}

int main()
{
	srand(time(0));

	sp_int_init();

	for(int i = 0; i < 1000; ++i)
		sp_int_insert(rand() % 999);

	sp_int_print();

	for(int i = 0; i < 1000; ++i)
		sp_int_remove(rand() % 999);

	sp_int_destroy();

	return 0;
}
