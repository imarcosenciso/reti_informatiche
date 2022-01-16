// Some constants
#define BUF_LEN 1024
#define DEFAULT_PORT 4242
#define PARAMS_MAX_LENGHT 30

#define ADRESS_BOOK_FILE ""

// Structs and instruction type definition (same as an enumeration in Java)
typedef enum
{
    INS_SING_UP = 1,
    INS_LOG_IN = 2,
    INS_LOG_OUT = 3
} ins_type;

// Define the structures for the different instructions or messages that are going to be sent.
typedef struct ins_sing_up
{
    ins_type ins_type; // INS_SING_UP
    char username[PARAMS_MAX_LENGHT + 1];
    char password[PARAMS_MAX_LENGHT + 1];
} ins_sing_up;

typedef struct ins_log_in
{
    ins_type ins_type; // INS_LOG_IN
    int srv_port;
    char username[PARAMS_MAX_LENGHT + 1];
    char password[PARAMS_MAX_LENGHT + 1];
} ins_log_in;

typedef struct ins_log_out
{
    ins_type ins_type; // INS_LOG_OUT
    char username[PARAMS_MAX_LENGHT + 1];
} ins_log_out;

// Function definition
void socket_creation(int port, int *sd, struct sockaddr_in *srv_addr);
void stablish_connection(int *ret, int *sd, struct sockaddr_in *srv_addr);

// Functions related to the service
void initial_menu();
void sign_up(int *sd, char username[PARAMS_MAX_LENGHT + 1], char password[PARAMS_MAX_LENGHT + 1]);
void log_in(int *sd, int srv_port, char username[PARAMS_MAX_LENGHT + 1], char password[PARAMS_MAX_LENGHT + 1]);
void log_out(int *sd);