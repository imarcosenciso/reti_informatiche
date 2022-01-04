/* First attemp at a server.
*  Should be able to handle multiple users concurrently.
*  v0.1: handle multiple connexions.
*/

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h> // To center text in command line.

#define BUF_LEN 1024
#define MAX_USERS 10
#define REQUEST_LEN 4
#define DEFAULT_PORT 4242

int initial_menu(int port);

int main(int argc, char *argv[])
{
    int ret, newfd, listener, addrlen, i, len, k, port;

    fd_set master;
    fd_set read_fds;
    int fdmax;

    struct sockaddr_in my_addr, cl_addr;
    char buffer[BUF_LEN];

    time_t rawtime;
    struct tm *timeinfo;

    // Check if any port was passed as argument:
    port = argc > 1 ? atoi(argv[1]) : DEFAULT_PORT;

    /* Creazione socket */
    listener = socket(AF_INET, SOCK_STREAM, 0);

    /* Creazione indirizzo di bind */
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(listener, (struct sockaddr *)&my_addr, sizeof(my_addr));

    if (ret < 0)
    {
        perror("Bind non riuscita\n");
        exit(0);
    }

    listen(listener, 10);

    // Reset FDs
    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    // Aggiungo il socket di ascolto (listener), creato dalla socket()
    // all'insieme dei descrittori da monitorare (master)
    FD_SET(listener, &master);

    // Aggiorno il massimo
    fdmax = listener;

    initial_menu(port);

    //main loop
    while (1)
    {
        // Inizializzo il set read_fds, manipolato dalla select()
        read_fds = master;

        // Mi blocco in attesa di descrottori pronti in lettura
        // imposto il timeout a infinito
        // Quando select() si sblocca, in &read_fds ci sono solo
        // i descrittori pronti in lettura!

        ret = select(fdmax + 1, &read_fds, NULL, NULL, NULL);
        if (ret < 0)
        {
            perror("ERRORE SELECT:");
            exit(1);
        }

        // Spazzolo i descrittori
        for (i = 0; i <= fdmax; i++)
        {
            // controllo se i è pronto
            if (FD_ISSET(i, &read_fds))
            {
                // se i è il listener, ho ricevuto una richiesta di connessione
                // (un client ha invocato connect())
                if (i == listener)
                {
                    printf("$ New connection stablished!\n");
                    fflush(stdout);
                    addrlen = sizeof(cl_addr);
                    // faccio accept() e creo il socket connesso 'newfd'
                    newfd = accept(listener,
                                   (struct sockaddr *)&cl_addr, &addrlen);

                    // Aggiungo il descrittore al set dei socket monitorati
                    FD_SET(newfd, &master);

                    // Aggiorno l'ID del massimo descrittore
                    if (newfd > fdmax)
                    {
                        fdmax = newfd;
                    }
                }
                // se non è il listener, 'i'' è un descrittore di socket
                // connesso che ha fatto la richiesta di orario, e va servito
                // ***senza poi chiudere il socket*** perché l'orario
                // potrebbe essere chiesto nuovamente al server
                else
                {
                    // TODO: instruction handler.
                    // Metto la richiesta nel buffer (pacchetto "REQ\0")
                    ret = recv(i, (void *)buffer, REQUEST_LEN, 0); // instruction type!

                    if (ret == 0)
                    {
                        printf("CHIUSURA client rilevata!\n");
                        fflush(stdout);
                        // il client ha chiuso il socket, quindi
                        // chiudo il socket connesso sul server
                        close(i);
                        // rimuovo il descrittore newfd da quelli da monitorare
                        FD_CLR(i, &master);
                    }
                    else if (ret < 0)
                    {
                        perror("ERRORE! \n");
                        // si è verificato un errore
                        close(i);
                        // rimuovo il descrittore newfd da quelli da monitorare
                        FD_CLR(i, &master);
                    }
                    else
                    {
                        printf("REQ client rilevata!\n");
                        fflush(stdout);

                        // Recupero l'ora corrente
                        time(&rawtime);
                        // Converto l'ora

                        timeinfo = localtime(&rawtime);

                        // Creo la risposta mettendola in "buffer"
                        sprintf(buffer, "%d:%d:%d",
                                timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

                        // Invio la risposta (e il terminatore di stringa)
                        len = send(i, (void *)buffer, strlen(buffer) + 1, 0);

                        if (ret < 0)
                        {
                            perror("Errore in fase di comunicazione \n");
                        }
                    }
                }
            }
            //break;
        }
        // ci arrivo solo se monitoro stdin (descrittore 0)
        // -> rompo il while e passo a chiudere il listener
    }
    printf("CHIUDO IL LISTENER!\n");
    fflush(stdout);
    close(listener);
}

int initial_menu(int port)
{
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    char srv[100];

    sprintf(srv, "SERVER STARTED [port: %d]", port);

    int fwidth = (strlen(srv) - w.ws_col) / 2;

    system("clear");

    // TODO: mejorar printing del sistema.
    printf("%*s%s\n", fwidth, "*", srv);

    // TODO: pedir input de usuario.
    return 0;
}