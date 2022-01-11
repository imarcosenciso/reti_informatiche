#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "client.h"

#define RESPONSE_LEN 9 // HH:MM:SS\0

int main(int argc, char *argv[])
{

    int ret, sd, j, port;

    struct sockaddr_in srv_addr;
    char buffer[BUFFER_SIZE];
    char username[PARAMS_MAX_LENGHT + 1];

    char *cmd = "REQ\0";

    // Check if any port was passed as argument:
    port = argc > 1 ? atoi(argv[1]) : DEFAULT_PORT;

    // Comando da inviare al server

    socket_creation(port, &sd, &srv_addr);

    stablish_connection(&ret, &sd, &srv_addr);

    for (;;)
    {

        // invio della richiesta
        ret = send(sd, cmd, strlen(cmd) + 1, 0);

        if (ret < 0)
        {
            perror("Errore in fase di invio comando: \n");
            exit(1);
        }

        // Attendo risposta
        ret = recv(sd, (void *)buffer, RESPONSE_LEN, 0);

        if (ret < 0)
        {
            perror("Errore in fase di ricezione: \n");
            exit(1);
        }

        // Stampo
        printf("%s\n", buffer);
        sleep(5);

    } //chiudo il "for sempre" (linea 40)
}

void socket_creation(int port, int *sd, struct sockaddr_in *srv_addr)
{
    /* Creazione socket */
    *sd = socket(AF_INET, SOCK_STREAM, 0);

    /* Creazione indirizzo del server */
    memset(srv_addr, 0, sizeof(*srv_addr));
    srv_addr->sin_family = AF_INET;
    srv_addr->sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &srv_addr->sin_addr);
}

void stablish_connection(int *ret, int *sd, struct sockaddr_in *srv_addr)
{
    *ret = connect(*sd, (struct sockaddr *)srv_addr, sizeof(*srv_addr));

    if (*ret < 0)
    {
        perror("Error during connection to server: \n");
        exit(1);
    }

    printf("Succesfully connected to Server!\n");
}

void initial_menu()
{
    char instruction[10], username[PARAMS_MAX_LENGHT + 1], password[PARAMS_MAX_LENGHT + 1];
    int srv_port;
    printf("User has to sign up if they don't have an account. Can log in otherwise.\n"
           "\tTo sign up, type: \"signup username password\"\n"
           "\tTo log in, type \"in srv_port username password\n");

    while (1)
    {
        printf("Command: ");
        scanf("%s", instruction);
        if (strcmp(instruction, "signup "))
        {
            scanf("%s", username);
            scanf("%s", password);
            break;
        }
        else if (strcmp(instruction, "in "))
        {
            scanf("%d", srv_port);
            scanf("%s", username);
            scanf("%s", password);
            break;
        }
        else
        {
            printf("\nUnkown command, please try again.");
        }
    }
}