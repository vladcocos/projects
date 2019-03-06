#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compare.h"
#include "priority_queue.h"

/**
 * Functie care creeaza un nod. Se aloca memorie si se salveaza in nod
 * cuvantul si prioritatea. Intoarce 12 daca nu se aloca memorie.
 */
int create_node(node_t **new_node, char *word, int priority)
{
	*new_node = (node_t *) malloc(sizeof(node_t));
	if (*new_node == NULL)
		return 12;

	(*new_node)->word = NULL;
	(*new_node)->word = (char *) malloc(sizeof(char) * (strlen(word) + 1));
	if ((*new_node)->word == NULL) {
		free(*new_node);
		return 12;
	}

	strcpy((*new_node)->word, word);
	(*new_node)->priority = priority;
	(*new_node)->next = NULL;

	return 0;
}

/**
 * Functie care creeaza coada de prioritati. Se aloca memorie si se
 * seteaza size ca 0 si primul element cu null.
 */
int create_priority_queue(priority_queue_t **queue)
{
	*queue = (priority_queue_t *) malloc(sizeof(priority_queue_t));
	if (*queue == NULL)
		return 12;

	(*queue)->size = 0;
	(*queue)->first = NULL;

	return 0;
}

/**
 * Functie care adauga in coada un element nou. Se creeaza nodul cu
 * prioritatea si cuvantul date si se adauga in coada in functie de
 * prioritatea acestuia.
 */
int insert(priority_queue_t **queue, char *word, int priority)
{
	node_t *new_node = NULL;

	if (create_node(&new_node, word, priority) == 12)
		return 12;

	if ((*queue)->size == 0) {
		(*queue)->first = new_node;
	} else {
		node_t *current_node = (*queue)->first;
		node_t *prev_node = (*queue)->first;

		while (compare(current_node->priority, priority) >= 0) {
			prev_node = current_node;
			current_node = current_node->next;

			if (current_node == NULL)
				break;
		}

		if (current_node == prev_node) {
			new_node->next = current_node;
			(*queue)->first = new_node;
		} else if (current_node == NULL) {
			prev_node->next = new_node;
		} else {
			prev_node->next = new_node;
			new_node->next = current_node;
		}
	}

	(*queue)->size++;
	return 0;
}

/**
 * Functie care afiseaza elementul cu prioritatea cea mai mare
 * (din varful cozii).
 */
void top(priority_queue_t *queue)
{
	if (queue->size == 0)
		printf("\n");
	else
		printf("%s\n", queue->first->word);
}

/**
 * Functie care elimina din coada elementul cu prioritatea cea mai mare.
 */
void pop(priority_queue_t **queue)
{
	if ((*queue)->size != 0) {
		node_t *current_node = (*queue)->first;
		(*queue)->first = (*queue)->first->next;
		current_node->next = NULL;
		free(current_node->word);
		free(current_node);
		(*queue)->size--;
	}
}

/**
 * Functie care sterge toate elementele din coada si elibereaza memoria
 * alocata pentru coada.
 */
void free_priority_queue(priority_queue_t **queue)
{
	if ((*queue)->size != 0) {
		node_t *current_node = (*queue)->first;

		while (current_node != NULL) {
			(*queue)->first = (*queue)->first->next;
			free(current_node->word);
			free(current_node);
			current_node = (*queue)->first;
		}
	}

	free(*queue);
}
