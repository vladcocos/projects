#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "lib.h"

/* Functie care raporteaza o eroare si inchide programul. */
void error(char *error) {
    perror(error);
    exit(1);
}

/* Functie care returneaza indexul elementului cu socket-ul egal cu
 * cel dat ca parametru.
 */
int get_client_index(Client clients[], int clients_number, int socket) {
    for (int i = 0; i < clients_number; i++) {
        if (clients[i].socket == socket) {
            return i;
        }
    }

    return -1;
}

/* Functie care returneaza un string cu codul de eroare si mesajul
 * corespunzator.
 */
char *output_error(int error_code) {
    char *output = (char*) malloc(sizeof(char) * 40);
    switch (error_code) {
        case -1:
            sprintf(output, "%s", "-1 : Clientul nu este autentificat");
            break;
        case -2:
            sprintf(output, "%s", "-2 : Sesiune deja deschisa");
            break;
        case -3:
            sprintf(output, "%s", "-3 : Pin gresit");
            break;
        case -4:
            sprintf(output, "%s", "-4 : Numar card inexistent");
            break;
        case -5:
            sprintf(output, "%s", "-5 : Card blocat");
            break;
        case -6:
            sprintf(output, "%s", "-6 : Operatie esuata");
            break;
        case -7:
            sprintf(output, "%s", "-7 : Deblocare esuata");
            break;
        case -8:
            sprintf(output, "%s", "-8 : Fonduri insuficiente");
            break;
        case -9:
            sprintf(output, "%s", "-9 : Operatie anulata");
            break;
    }

    return output;
}

/* Functie care autentifica un client. */
char *login(Client clients[], int clients_number, int socket, char *command) {
    int card_number, pin, k;
    char *output = (char*) malloc(sizeof(char) * 40);
    bool correct_card_number = false;
    strtok(command, " ");
    card_number = atoi(strtok(NULL, " "));
    pin = atoi(strtok(NULL, " "));

    /* Verifica daca numarul de card exista. */
    for (int i = 0; i < clients_number; i++) {
        if (clients[i].card_number == card_number) {
            correct_card_number = true;
            k = i;
            break;
        }
    }

    /* Daca numarul de card nu exista, returneaza eroare. */
    if (correct_card_number == false) {
        sprintf(output, "IBANK> %s\n", output_error(-4));
        clients[k].last_socket = socket;
        return output;
    }

    /* Reseteaza contorul de login pentru toti clientii de pe socketi
     * diferiti de socket-ul curent.
     */
    for (int i = 0; i < clients_number; i++) {
        if (clients[i].last_socket != socket) {
            clients[i].login_count = 0;
        }
    }

    /* Returneaza eroare daca client-ul este blocat, logat sau pin-ul este
     * gresit.
     */
    if (clients[k].blocked == true) {
        sprintf(output, "IBANK> %s*%d\n", output_error(-5), card_number);
        clients[k].last_socket = socket;
        return output;
    } else if (clients[k].logged_in == true) {
        sprintf(output, "IBANK> %s\n", output_error(-2));
        clients[k].last_socket = socket;
        return output;
    } else if (clients[k].pin != pin) {
        if (clients[k].last_socket != socket) {
            clients[k].login_count = 0;
        }

        /* Daca e 3-a incercare gresita, blocheaza cardul. */
        if (clients[k].login_count == 2) {
            clients[k].blocked = true;
            sprintf(output, "IBANK> %s*%d\n", output_error(-5), card_number);
            clients[k].last_socket = socket;
            return output;
        }

        clients[k].login_count++;
        sprintf(output, "IBANK> %s\n", output_error(-3));
        clients[k].last_socket = socket;
        return output;
    }

    /* Logare reusita. */
    clients[k].last_socket = socket;
    clients[k].logged_in = true;
    clients[k].login_count = 0;
    clients[k].socket = socket;
    sprintf(output, "IBANK> Welcome %s %s\n", clients[k].last_name, 
        clients[k].first_name);
    return output;
}

/* Functie care dezautentifica un client doar daca acesta este deja logat. */
char *logout(Client clients[], int clients_number, int socket) {
    char *output = (char*) malloc(sizeof(char) * 40);
    int index = get_client_index(clients, clients_number, socket);

    if (clients[index].logged_in == true) {
        sprintf(output, "IBANK> Clientul a fost deconectat\n");
        clients[index].logged_in = false;
        clients[index].socket = -1;
        clients[index].login_count = 0;
    }

    return output;
}

/* Functie care afiseaza soldul unui client. */
char *listsold(Client clients[], int clients_number, int socket) {
    char *output = (char*) malloc(sizeof(char) * 40);
    int index = get_client_index(clients, clients_number, socket);

    sprintf(output, "IBANK> %.2f\n", clients[index].sold);
    return output;
}

/* Functie care verifica toate conditiile necesare pentru transfer si salveaza
 * si salveaza suma si socket-ul clientului care va primi suma.
 */
char *ask_transfer(Client clients[], int clients_number, int socket, 
        double *sum, int *receiver, char *command) {
    int card_number, k, index;
    double transfer_sum;
    index = get_client_index(clients, clients_number, socket);
    char *output = (char*) malloc(sizeof(char) * 40);
    bool correct_card_number = false;
    strtok(command, " ");
    card_number = atoi(strtok(NULL, " "));
    transfer_sum = atof(strtok(NULL, " "));

    /* Verifica daca numarul cardului exista. */
    for (int i = 0; i < clients_number; i++) {
        if (clients[i].card_number == card_number) {
            correct_card_number = true;
            k = i;
            break;
        }
    }

    /* Returneaza eroare daca numarul cardului nu exista sau clientul
     * nu are fonduri suficiente.
     */
    if (correct_card_number == false) {
        sprintf(output, "IBANK> %s\n", output_error(-4));
        return output;
    } else if (clients[index].sold < transfer_sum) {
        sprintf(output, "IBANK> %s\n", output_error(-8));
        return output;
    }

    sprintf(output, "IBANK> Transfer %.2lf catre %s %s? [y/n]\n", transfer_sum,
        clients[k].last_name, clients[k].first_name);
    *receiver = k;
    *sum = transfer_sum;
    return output;
}

/* Functie care efectueaza transferul de la un client la altul. */
char *transfer(Client clients[], int clients_number, int socket, double sum,
        int receiver, char *command) {
    char *output = (char*) malloc(sizeof(char) * 40);
    int index = get_client_index(clients, clients_number, socket);

    clients[index].sold -= sum;
    clients[receiver].sold += sum;

    sprintf(output, "IBANK> Transfer realizat cu succes\n");
    return output;
}

/* Functie care verifica numarul cardului dat pentru deblocare si cere
 * clientului parola pentru deblocare.
 */
char *ask_unlock(Client clients[], int clients_number, char *command) {
    int index;
    char *output = (char*) malloc(sizeof(char) * 40);

    strtok(command, " ");
    int card_number = atoi(strtok(NULL, " \n"));

    /* Verifica daca numarul cardului este corect. */
    bool correct_card_number = false;
    for (int i = 0; i < clients_number; i++) {
        if (clients[i].card_number == card_number) {
            index = i;
            correct_card_number = true;
            break;
        }
    }

    /* Returneaza eroare daca numarul cardului este gresit sau daca
     * cardul este blocat.
     */
    if (correct_card_number == false) {
        sprintf(output, "UNLOCK> %s\n", output_error(-4));
        return output;
    } else if (clients[index].blocked == false) {
        sprintf(output, "UNLOCK> %s\n", output_error(-6));
        return output;
    }

    sprintf(output, "UNLOCK> Trimite parola secreta\n");
    return output;
}

/* Functie care deblocheaza un card daca parola data este corecta. */
char *unlock(Client clients[], int clients_number, char *command) {
    int index;
    char *output = (char*) malloc(sizeof(char) * 40);

    int card_number = atoi(strtok(command, " "));
    char password[8];
    sprintf(password, "%s", strtok(NULL, " \n"));

    /* Verifica daca numarul cardului exista. */
    bool correct_card_number = false;
    for (int i = 0; i < clients_number; i++) {
        if (clients[i].card_number == card_number) {
            index = i;
            correct_card_number = true;
            break;
        }
    }

    /* Returneaza eroare daca numarul cardului este gresit sau parola
     * este gresita.
     */
    if (correct_card_number == false) {
        sprintf(output, "UNLOCK> %s\n", output_error(-4));
        return output;
    } else if (strcmp(clients[index].password, password) != 0) {
        sprintf(output, "UNLOCK> %s\n", output_error(-7));
        return output;
    }

    clients[index].blocked = false;
    clients[index].login_count = 0;

    sprintf(output, "UNLOCK> Card deblocat\n");
    return output;
}

/* Functie care seteaza socket-ul curent si ultimul socket de pe care a fost
 * a fost accesat clientul cu -1, pentru a desemna deconectarea clientului.
 */
void client_quit(Client clients[], int clients_number, int socket) {
    int index = get_client_index(clients, clients_number, socket);
    clients[index].socket = clients[index].last_socket = -1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
       fprintf(stderr, "Utilizare: %s server_port users_data_file\n", argv[0]);
       exit(0);
    }

    /* Declaratii variabile. */
    int clients_number, n, i, transfer_receiver;
    int connected_sockets[2 * MAX_CLIENTS] = {false};
    char input[BUFLEN], *output;
    output = (char*) malloc(sizeof(char) * BUFLEN);
    double transfer_sum;
    bool open_transfer = false, open_unlock = false;
    
    /* Citire fisier input. */
    FILE *input_file = fopen(argv[2], "rt");
    fscanf(input_file, "%d", &clients_number);
    Client clients[clients_number];
    for (i = 0; i < clients_number; i++) {
        fscanf(input_file, "%s%s%d%d%s%lf", clients[i].last_name,
            clients[i].first_name, &(clients[i].card_number), &(clients[i].pin),
            clients[i].password, &(clients[i].sold));
        clients[i].last_name[12] = clients[i].first_name[12] = '\0';
        clients[i].socket = clients[i].last_socket = -1;
        clients[i].logged_in = clients[i].blocked = false;
        clients[i].login_count = 0;
    }
    fclose(input_file);

    /* Definire conexiune TCP. */
    int tcp_sockfd, new_tcp_sockfd, port = atoi(argv[1]);
    unsigned int client_len;
    struct sockaddr_in tcp_serv_addr, tcp_client_addr;

    fd_set read_fds, tmp_fds;
    int fdmax;

    FD_ZERO(&read_fds);
    FD_ZERO(&tmp_fds);

    if ((tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("-10 : Eroare la apel socket\n");
    }

    memset((char *) &tcp_serv_addr, 0, sizeof(tcp_serv_addr));
    tcp_serv_addr.sin_family = AF_INET;
    tcp_serv_addr.sin_addr.s_addr = INADDR_ANY;
    tcp_serv_addr.sin_port = htons(port);

    if (bind(tcp_sockfd, (struct sockaddr*) &tcp_serv_addr, 
            sizeof(struct sockaddr)) < 0) {
        error("-10 : Eroare la apel bind\n");
    }

    /* Definire conexiune UDP. */
    unsigned int udp_sockfd, udp_addr_len = sizeof(struct sockaddr);
    struct sockaddr_in udp_serv_addr, udp_client_addr;
    if ((udp_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        error("-10 : Eroare la apel bind\n");
    }

    udp_serv_addr.sin_family = AF_INET;
    udp_serv_addr.sin_addr.s_addr = INADDR_ANY;
    udp_serv_addr.sin_port = htons(port);

    if (bind(udp_sockfd, (struct sockaddr*) &udp_serv_addr, udp_addr_len) < 0) {
        error("-10 : Eroare la apel bind\n");
    }

    listen(tcp_sockfd, clients_number);

    FD_SET(0, &read_fds);
    FD_SET(udp_sockfd, &read_fds);
    FD_SET(tcp_sockfd, &read_fds);
    fdmax = MAX(tcp_sockfd, udp_sockfd);

	while (true) {
		tmp_fds = read_fds; 

        /* Selectare socket. */
		if (select(fdmax + 1, &tmp_fds, NULL, NULL, NULL) == -1) {
            error("-10 : Eroare la apel select\n");
        }
	
		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &tmp_fds)) {
                if (i == 0) {
                    memset(input, 0, BUFLEN);
                    fgets(input, BUFLEN - 1, stdin);

                    if (strcmp(input, "quit\n") == 0) {
                        sprintf(output, "Serverul s-a inchis\n");
                        for (int j = 0; j <= fdmax; j++) {
                            if (connected_sockets[j] == true) {
                                n = send(j, output, strlen(output), 0);
                                if (n < 0) {
                                    error("-10 : Eroare la apel send\n");
                                }
                                close(j);
                            }
                        }

                        free(output);
                        close(0);
                        close(tcp_sockfd);
                        close(udp_sockfd);
                        return 0;
                    }
                } else if (i == tcp_sockfd) {
                    /* A venit un mesaj pe socket-ul inactiv. Serverul accepta
                        conexiunea. */
					client_len = sizeof(tcp_client_addr);
					if ((new_tcp_sockfd = accept(tcp_sockfd, (struct sockaddr*)
                            &tcp_client_addr, &client_len)) == -1) {
                        error("-10 : Eroare la apel accept\n");
					} else {
						FD_SET(new_tcp_sockfd, &read_fds);
						if (new_tcp_sockfd > fdmax) { 
							fdmax = new_tcp_sockfd;
						}

                        connected_sockets[new_tcp_sockfd] = true;
					}
                } else if (i == udp_sockfd) {
                    /* A venit un mesaj pe socket-ul UDP pentru functia
                     * de unlock.
                     */
                    if ((n = recvfrom(udp_sockfd, input, BUFLEN, 0,
                            (struct sockaddr*) &udp_client_addr, 
                            &udp_addr_len)) < 0) {
                        error("-10 : Eroare la apen recvfrom\n");
                    }

                    /* Daca este primul apel unlock, se apeleaza ask_unlock
                     * pentru a cere parola. Daca nu, se apeleaza unlock pentru
                     * a verifica cardul si parola.
                     */
                    if (strncmp(input, "unlock", 6) == 0) {
                        output = ask_unlock(clients, clients_number, input);
                        open_unlock = true;
                    } else if (open_unlock == true) {
                        output = unlock(clients, clients_number, input);
                        if (strncmp(output, "UNLOCK> Card deblocat", 21) == 0) {
                            open_unlock = false;
                        }
                    }

                    n = sendto(udp_sockfd, output, strlen(output), 0,
                        (struct sockaddr*) &udp_client_addr, udp_addr_len);
                    if (n < 0) {
                        error("-10 : Eroare la apel sendto\n");
                    }
				} else {
                    /* A venit un mesaj de pe un socket TCP (de la un client)
                     * care anunta deconectarea acestuia.
                     */
					if ((n = recv(i, input, sizeof(input), 0)) <= 0) {
						if (n == 0) {
							printf("User-ul de pe socket-ul %d s-a deconectat\n", i);
						} else {
                            error("-10 : Eroare la apel send\n");
						}
						close(i); 
						FD_CLR(i, &read_fds);
                        connected_sockets[i] = false;
					} else {
                        /* A venit un mesaj de pe un socket TCP (de la un
                         * client).
                         */

                        /* Daca nu se afla in mijlocul unui transfer, se poate
                         * apela orice functie. Altfel, se apeleaza functia
                         * transfer daca raspunsul la transfer este pozitiv
                         * pentru a efectua transferul sau eroare in caz contrar.
                         */
                        if (open_transfer == false) {
                            if (strncmp(input, "login", 5) == 0) {
                                output = login(clients, clients_number, i, input);
                            } else if (strncmp(input, "logout", 6) == 0) {
                                output = logout(clients, clients_number, i);
                            } else if (strncmp(input, "listsold", 8) == 0) {
                                output = listsold(clients, clients_number, i);
                            } else if (strncmp(input, "transfer", 8) == 0) {
                                transfer_receiver = -1;
                                output = ask_transfer(clients, clients_number,
                                    i, &transfer_sum, &transfer_receiver, input);
                                if (transfer_receiver != -1) {
                                    open_transfer = true;
                                }
                            } else if (strncmp(input, "quit", 4) == 0) {
                                client_quit(clients, clients_number, i);
                            } else {
                                sprintf(output, "Eroare input\n");
                            }
                        } else {
                            if (input[0] == 'y') {
                                output = transfer(clients, clients_number, i,
                                    transfer_sum, transfer_receiver, input);
                            } else {
                                sprintf(output, "%s", output_error(-9));
                            }
                            open_transfer = false;
                        }

                        /* Trimite mesajul catre client. */
                        n = send(i, output, strlen(output), 0);
                        if (n < 0) {
                            error("-10 : Eroare la apel send\n");
                        }
					}
				}
			}
		}
     }

    /* Eliberare memorie si inchidere socketi. */
    free(output);
    close(i);
    close(tcp_sockfd);
    close(udp_sockfd);

    return 0;
}