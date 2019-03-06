#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "lib.h"

/* Functie care raporteaza o eroare si inchide programul. */
void error(char *error) {
    perror(error);
    exit(1);
}

/* Functie care returneaza un char pointer ce contine ultimele n caractere
 * dintr-un string. Folosit la obtinerea numarului cardului blocat din
 * string-ul "Card blocat <numar card>".
 */
char *last_n_characters(char *input, int n) {
    char *str = (char*) malloc(sizeof(char) * (n + 3));
    for (int i = 0; i < n + 2; i++) {
        str[i] = input[strlen(input) - n + i];
    }

    str[n] = '\0';
    return str;
}

/* Functie care returneaza numarul cardului din input-ul de login. */
int get_card_number(char *input) {
    char *aux = strdup(input);
    strtok(aux, " ");
    return atoi(strtok(NULL, " "));
}

/* Functie care blocheaza un card din vectorul de carduri accesate deja. */
void lock_card(Locked_Card locked[], int *locked_cards, int card_number) {
    for (int i = 0; i < *locked_cards; i++) {
        if (locked[i].card_number == card_number) {
            return;
        }
    }

    locked[*locked_cards].card_number = card_number;
    locked[*locked_cards].locked = true;
    (*locked_cards)++;
}

/* Functie care deblocheaza un card din vectorul de carduri accesate deja. */
void unlock_card(Locked_Card locked[], int locked_cards, int card_number) {
    for (int i = 0; i < locked_cards; i++) {
        if (locked[i].card_number == card_number) {
            locked[i].locked = false;
        }
    }
}

/* Functie care returneaza true sau false daca numarul de card dat ca parametru
 * este blocat sau nu.
 */
bool check_locked_card(Locked_Card locked[], int locked_cards, int card_number) {
    for (int i = 0; i < locked_cards; i++) {
        if (card_number == locked[i].card_number) {
            return true;
        }
    }

    return false;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
       fprintf(stderr,"Utilizare: %s server_address server_port\n", argv[0]);
       exit(0);
    }

    /* Declaratii variabile. */
    int i, n, last_card_number = 0, locked_cards = 0;
    bool logged_in = false, open_unlock = false;
    Locked_Card locked[MAX_CLIENTS];

    fd_set read_fds, tmp_fds;
    int fdmax;

    FD_ZERO(&read_fds);
    FD_ZERO(&tmp_fds);

    /* Definire conexiune TCP. */
    int tcp_sockfd;
    struct sockaddr_in tcp_serv_addr;

    tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_sockfd < 0) {
        error("-10 : Eroare la apel socket\n\n");
    }

    tcp_serv_addr.sin_family = AF_INET;
    tcp_serv_addr.sin_port = htons(atoi(argv[2]));
    inet_aton(argv[1], &tcp_serv_addr.sin_addr);
    
    if (connect(tcp_sockfd,(struct sockaddr*) &tcp_serv_addr,
        sizeof(tcp_serv_addr)) < 0) {
        error("-10 : Eroare la apel connect\n\n");
    }
    
    /* Definire conexiune UDP. */
    int udp_sockfd;
    unsigned int udp_addr_len;
    struct sockaddr_in udp_serv_addr;
    struct hostent *host = (struct hostent*) gethostbyname((char*) argv[1]);

    if ((udp_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        error("-10 : Eroare la apel socket\n");
    }

    udp_serv_addr.sin_family = AF_INET;
    udp_serv_addr.sin_port = htons(atoi(argv[2]));
    udp_serv_addr.sin_addr = *((struct in_addr*) host->h_addr);
    bzero(&(udp_serv_addr.sin_zero), 8);
    udp_addr_len = sizeof(struct sockaddr);

    /* Deschidere fisier log. */
    FILE *log;
    char *input, *output, *file_name;
    input = (char*) malloc(sizeof(char) * BUFLEN);
    output = (char*) malloc(sizeof(char) * BUFLEN);
    file_name = (char*) malloc(sizeof(char) * BUFLEN);
    sprintf(file_name, "client-%d.log", getpid());
    log = fopen(file_name, "wt");
    free(file_name);
    
    /* Setare socketi stdin, TCP si UDP. */
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
                    /* Citeste comanda de la tastatura (socket-ul stdin). */
                    memset(input, 0, BUFLEN);
                    memset(output, 0, BUFLEN);
                    fgets(input, BUFLEN - 1, stdin);
                    fprintf(log, "%s", input);

                    if (strncmp(input, "quit", 4) == 0) {
                        /* Daca comanda este quit, elibereaza memoria,
                         * inchidere fisierul, socketii deschisi si clientul.
                         */
                        fclose(log);
                        close(0);
                        close(tcp_sockfd);
                        close(udp_sockfd);
                        free(input);
                        free(output);
                        return 0;
                    } else if (strncmp(input, "login", 5) == 0) {
                        /* Daca comanda este login, salveaza ultimul card
                         * accesast. Daca este deja logat, afiseaza eroare.
                         */
                        last_card_number = get_card_number(input);
                        if (logged_in == true) {
                            printf("IBANK> -2 : Sesiune deja deschisa\n\n");
                            fprintf(log, "IBANK> -2 : Sesiune deja deschisa\n\n");
                            continue;
                        }
                    } else if ((strncmp(input, "login", 5) != 0 &&
                                logged_in == false && 
                                strncmp(input, "unlock", 6) != 0 &&
                                open_unlock != 1)
                            || (strncmp(input, "unlock", 6) == 0 &&
                                check_locked_card(locked, locked_cards, 
                                last_card_number) == false)) {
                        /* Daca s-a dat orice comanda in afara de login,
                         * sesiunea este inchisa si procesul de unlock s-a
                         * incheiat din partea clientului, sau a fost data
                         * comanda unlock pentru un card care nu este blocat,
                         * se returneaza "operatie esuata".
                         */
                        printf("IBANK> -6 : Operatie esuata\n\n");
                        fprintf(log, "IBANK> -6 : Operatie esuata\n\n");
                        continue;
                    } else if (strncmp(input, "unlock", 6) == 0) {
                        /* Daca se da comanda unlock, se trimite prin UDP
                         * numarul cardului de deblocat. Open_unlock este 1.
                         */
                        sprintf(input, "%s %d\n", "unlock", last_card_number);
                        open_unlock = 1;
                    } else if (open_unlock == 1) {
                        /* Daca open_unlock este 1, procesul de deblocare
                         * se afla la trimiterea parolei. Open_unlock devine
                         * egal cu 2, adica asteptarea verificarii cardului
                         * si parolei.
                         */
                        char aux[BUFLEN];
                        sprintf(aux, "%s", input);
                        sprintf(input, "%d %s\n", last_card_number, aux);
                        open_unlock = 2;
                    } else if (strncmp(input, "logout", 6) == 0) {
                        if (logged_in == true) {
                            logged_in = false;
                        }
                    }

                    /* Trimite mesajul prin TCP sau UDP, in functie de
                     * open_unlock. Daca este 0, se trimite prin TCP, altfel
                     * pe UDP.
                     */
                    if (open_unlock == 0) {
                        n = send(tcp_sockfd, input, strlen(input), 0);
                        if (n < 0) {
                            error("-10 : Eroare la apel send\n\n");
                        }
                    } else {
                        n = sendto(udp_sockfd, input, strlen(input), 0,
                            (struct sockaddr*) &udp_serv_addr, udp_addr_len);
                        if (n < 0) {
                            error("-10 : Eroare la apel sendto\n\n");
                        }
                    }

                    continue;
                } else if (i == tcp_sockfd) {
                    /* S-a primit mesaj pe socket-ul TCP. */
                    n = recv(tcp_sockfd, output, BUFLEN, 0);
                    if (n < 0) {
                        error("-10 : Eroare la apel recv\n\n");
                    }

                    /* Daca login-ul a fost cu succes, se salveaza sesiunea
                     * curenta deschisa.
                     */
                    if (strncmp(output, "IBANK> Welcome", 14) == 0) {
                        logged_in = true;
                    } else if (strncmp(output, "Serverul s-a inchis", 19) == 0) {
                        /* Daca s-a primit quit de la server, se elibereaza
                         * memoria, se inchid socket-ii si clientul.
                         */
                        printf("%s\n", output);
                        fprintf(log, "%s\n", output);
                        fclose(log);
                        close(0);
                        close(tcp_sockfd);
                        close(udp_sockfd);
                        free(input);
                        free(output);
                        return 0;
                    } else if (strncmp(output, "IBANK> -5", 9) == 0) {
                        /* Daca s-a primit mesajul "Card blocat", se blocheaza
                         * in vectorul de carduri blocate cardul primit in
                         * mesaj, daca nu este blocat deja.
                         */
                        char *p = last_n_characters(output, 7);
                        lock_card(locked, &locked_cards, atoi(p));
                        free(p);
                    }
                } else if (i == udp_sockfd) {
                    /* S-a primit mesaj pe socket-ul UDP. */
                    n = recvfrom(udp_sockfd, output, BUFLEN, 0,
                        (struct sockaddr*) &udp_serv_addr, &udp_addr_len);
                    if (n < 0) {
                        error("-10 : Eroare la apel recvfrom\n\n");
                    }

                    /* Daca deblocarea a avut succes, deblocheaza cardul din
                     * vectorul de carduri blocate si inchide procesul de
                     * deblocare.
                     */
                    if (strncmp(output, "UNLOCK> Card deblocat", 21) == 0) {
                        open_unlock = 0;
                        unlock_card(locked, locked_cards, last_card_number);
                    } else if (open_unlock == 2) {
                        /* Daca deblocarea nu a avut succes la ultimul pas
                         * (verificarea parolei), se inchide procesul de
                         * deblocare. Va fi redeschis la introducerea unei noi
                         * comenzei "unlock".
                         */
                        open_unlock = 0;
                    }
                }

                /* Obtine mesajul de output (util doar in cazul trimiterii
                 * mesajului "Card blocat <numar card>"), afiseaza si scrie
                 * in fisier.
                 */
                strtok(output, "*");
                if (strncmp(output, "IBANK> -5", 9) == 0) {
                    printf("%s\n\n", output);
                    fprintf(log, "%s\n\n", output);
                } else {
                    printf("%s\n", output);
                    fprintf(log, "%s\n", output);
                }
            }
        }
    }

    /* Inchidere fisier, socketi si eliberare memorie. */
    fclose(log);
    close(0);
    close(tcp_sockfd);
    close(udp_sockfd);
    free(input);
    free(output);
    return 0;
}