#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compare.h"
#include "priority_queue.h"

#define COMMAND_SIZE 20000
#define INSERT_COMMAND "insert"
#define TOP_COMMAND "top\n"
#define POP_COMMAND "pop\n"

int main(int argc, char **argv)
{
	int i;
	priority_queue_t *queue = NULL;
	char command[COMMAND_SIZE];
	FILE *fd = NULL;

	if (create_priority_queue(&queue) == 12) {
		return 12;
	}

	for (i = 0; i < argc; i++) {
		if (argc == 1) {
			fd = stdin;
		} else {
			if (i == 0)
				continue;

			fd = NULL;
			fd = fopen(argv[i], "r");
			if (fd == NULL)
				continue;
		}

		/**
		 * Citeste fiecare linie din fisier sau de la stdin si verifica
		 * comanda asociata.
		 */
		while (fgets(command, COMMAND_SIZE, fd)) {
			if (strncmp(command, INSERT_COMMAND, 6) == 0) {
				int number_of_spaces = 0;
				char *word = command;
				int priority = 0;

				while ((word = strchr(word, ' ')) != NULL) {
					number_of_spaces++;
					word++;
				}

				if (number_of_spaces != 2)
					continue;

				word = strtok(command, " ");
				word = strtok(NULL, " ");
				priority = atoi(strtok(NULL, "\n"));

				if (insert(&queue, word, priority) == 12) {
					free_priority_queue(&queue);

					if (fd != stdin)
						fclose(fd);

					return 12;
				}
			} else if (strncmp(command, TOP_COMMAND, 4) == 0) {
				top(queue);
			} else if (strncmp(command, POP_COMMAND, 4) == 0) {
				pop(&queue);
			}
		}

		if (fd != stdin)
			fclose(fd);
	}

	free_priority_queue(&queue);

	return 0;
}
