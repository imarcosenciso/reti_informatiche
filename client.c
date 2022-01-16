#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "client.h"

char username_logged_in[PARAMS_MAX_LENGHT + 1];
FILE *fptr;

int main(int argc, char *argv[])
{

    int ret, sd, j, port;

    struct sockaddr_in srv_addr;
    char buffer[BUF_LEN];

    // Check if any port was passed as argument:
    port = argc > 1 ? atoi(argv[1]) : DEFAULT_PORT;

    // Comando da inviare al server
    socket_creation(port, &sd, &srv_addr);

    stablish_connection(&ret, &sd, &srv_addr);

    initial_menu(&sd);
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
        perror("$ Error during connection to server");
        exit(1);
    }

    printf("$ Succesfully connected to Server!\n");
}

void initial_menu(int *sd)
{
    char instruction[PARAMS_MAX_LENGHT + 10], username_temp[PARAMS_MAX_LENGHT + 1], password[PARAMS_MAX_LENGHT + 1];
    int srv_port;
    system("clear");

    printf("User has to sign up if they don't have an account. Can log in otherwise.\n"
           "\tTo sign up, type: \"signup username password\"\n"
           "\tTo log in, type \"in srv_port username password\n");

    while (1)
    {
        printf("Command: ");
        scanf("%s", instruction);
        if (strcmp(instruction, "signup") == 0)
        {
            scanf("%s", username_temp);
            scanf("%s", password);
            sign_up(sd, username_temp, password);
            //TODO ack.
            break;
        }
        else if (strcmp(instruction, "in") == 0)
        {
            scanf("%d", &srv_port);
            scanf("%s", username_temp);
            scanf("%s", password);
            log_in(sd, srv_port, username_temp, password);
            //TODO ack.
            break;
        }
        else
        {
            puts("\nUnkown or wrong command, please try again.");
        }
    }

    system("clear");
    printf("Welcome, %s\n"
           "List of commands:\n"
           "\t* hanging\n"
           "\t* show \"username\"\n"
           "\t* chat \"username\"\n"
           "\t* share \"file_name\"\n"
           "\t* contacts\n"
           "\t* out\n",
           username_logged_in);

    while (1)
    {
        printf("Command: ");
        scanf("%s", instruction);
        if (strcmp(instruction, "out") == 0)
        {
            log_out(sd);
            break;
        }
        else if (strcmp(instruction, "contacts") == 0)
        {
            print_address_book();
            break;
        }
        else
        {
            puts("\nUnkown or wrong command, please try again.");
        }
    }
}

void sign_up(int *sd, char username_temp[PARAMS_MAX_LENGHT + 1], char password[PARAMS_MAX_LENGHT + 1])
{
    int ret;
    ins_sing_up instr;
    uint16_t inst_type;

    instr.ins_type = INS_SING_UP;
    strcpy(instr.username, username_temp);
    strcpy(instr.password, password);

    printf("Signup\n\tusername: %s\n\tpassword: %s\n", instr.username, instr.password);

    // Send instruction + information package to server

    // 1st send instruction type!
    inst_type = htons(instr.ins_type);
    ret = send(*sd, (void *)&inst_type, sizeof(uint16_t), 0);

    // Then send the rest of the struct.
    ret = send(*sd, (void *)&instr, sizeof(instr), 0);

    if (ret < 0)
    {
        perror("$ Error when sending SIGN_IN instruction");
        exit(1);
    }
    else
    {
        printf("$ Sign in successfully made!");
    }
}

void log_in(int *sd, int srv_port, char username_temp[PARAMS_MAX_LENGHT + 1], char password[PARAMS_MAX_LENGHT + 1])
{
    int ret;
    ins_log_in instr;
    uint16_t inst_type;

    instr.ins_type = INS_LOG_IN;
    instr.srv_port = srv_port;
    strcpy(instr.username, username_temp);
    strcpy(instr.password, password);

    // 1st send instruction type:
    inst_type = htons(instr.ins_type);
    ret = send(*sd, (void *)&inst_type, sizeof(uint16_t), 0);

    // Send instruction + information to server
    ret = send(*sd, (void *)&instr, sizeof(instr), 0);

    if (ret < 0)
    {
        perror("$ Error when sending LOG_IN instruction");
        exit(1);
    }
    else
    {
        printf("$ Log in successfully made!");
        strcpy(username_logged_in, username_temp);
        printf("Username: %s\n", username_logged_in);
    }
}

void log_out(int *sd)
{
    int ret, len;
    ins_log_out instr;
    uint16_t inst_type, lmsg;

    instr.ins_type = INS_LOG_OUT;
    strcpy(instr.username, username_logged_in);

    // 1st send instruction type:
    inst_type = htons(instr.ins_type);
    ret = send(*sd, (void *)&inst_type, sizeof(uint16_t), 0);

    // Send instruction + information to server
    ret = send(*sd, (void *)&instr, sizeof(instr), 0);

    if (ret < 0)
    {
        perror("$ Error when sending LOG_OUT instruction");
        exit(1);
    }
    else
    {
        printf("$ Log out successfully made!");
        printf("Bye %s!\n", username_logged_in);
    }
}

void print_address_book()
{
    char buffer[BUF_LEN];
    char name[PARAMS_MAX_LENGHT + 1];
    int port;

    printf("Adress book:\n");

    fptr = fopen(ADRESS_BOOK_PATH, "r");
    if (fptr == NULL)
    {
        printf("Error opening Adress Book");
        exit(1);
    }

    while (fgets(buffer, BUF_LEN, fptr))
    {
        sscanf(buffer, "%s %d", name, &port);
        printf("[Port %d] Username: %s\n", port, name);
    }

    fclose(fptr);
}

void chat(char user_dest[PARAMS_MAX_LENGHT + 1])
{
    // 1st, make sure there are no messages pending from that user.
    // recieve_pending_messages();
    // if there are, not the first time chatting so it's all good.

    // If there are not, check if it's 1st time chatting.
    // Check in contacts file, or better check file with chat logs.
    // If 1st time, contact server.

    // Recieve srv_port from server.

    // Start chat without server...
}