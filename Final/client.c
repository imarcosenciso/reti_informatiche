#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define RESPONSE_LEN 9 // HH:MM:SS\0

#define DEFAULT_PORT 4242

int main(int argc, char *argv[])
{

    int ret, sd, j, port;

    struct sockaddr_in srv_addr;
    char buffer[BUFFER_SIZE];

    char *cmd = "REQ\0";

    // Check if any port was passed as argument:
    port = argc > 1 ? atoi(argv[1]) : DEFAULT_PORT;

    // Comando da inviare al server

    /* Creazione socket */
    sd = socket(AF_INET, SOCK_STREAM, 0);

    /* Creazione indirizzo del server */
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &srv_addr.sin_addr);

    /* connessione */
    ret = connect(sd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));

    if (ret < 0)
    {
        perror("Errore in fase di connessione: \n");
        exit(1);
    }

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