#ifndef __PRIORITY_QUEUE_H_
#define __PRIORITY_QUEUE_H_
#include <stdio.h>

/**
 * Structura care retine un element din coada. Contine cuvantul,
 * prioritatea si pointer catre urmatorul element.
 */
typedef struct node_t {
	char *word;
	int priority;
	struct node_t *next;
} node_t;

/**
 * Structura care retine coada de prioritati. Contine dimensiunea
 * si pointer catre primul element, cel cu prioritatea cea mai mare.
 */
typedef struct priority_queue_t {
	unsigned int size;
	node_t *first;
} priority_queue_t;

int create_node(node_t **new_node, char *word, int priority);
int create_priority_queue(priority_queue_t **queue);
int insert(priority_queue_t **queue, char *word, int priority);
void top(priority_queue_t *queue);
void pop(priority_queue_t **queue);
void free_priority_queue(priority_queue_t **queue);

#endif
