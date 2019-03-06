#ifndef LIB
#define LIB

#define MAX_CLIENTS 10
#define BUFLEN 256
#define bool int
#define true 1
#define false 0
#define MAX(a, b) ((a > b) ? a : b)

typedef struct {
    char first_name[13], last_name[13];
    int card_number, pin;
    char password[9];
    double sold;
    int socket, last_socket, login_count;
    bool logged_in, blocked;
} Client;

typedef struct {
    int card_number;
    bool locked;
} Locked_Card;

char *login(Client clients[], int clients_number, int socked, char *command);
char *logout(Client clients[], int clients_number, int socket);
char *listsold(Client clients[], int clients_number, int socket);
char *ask_transfer(Client clients[], int clients_number, int socket, double *sum, int *receiver, char *command);
char *transfer(Client clients[], int clients_number, int socket, double sum, int receiver, char *command);
char *ask_unlock(Client clients[], int clients_number, char *command);
char *unlock(Client clients[], int clients_number, char *command);
void client_quit(Client clients[], int clients_number, int socket);

#endif
