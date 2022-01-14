#define BUF_LEN 1024
#define DEFAULT_PORT 4242
#define PARAMS_MAX_LENGHT 30
#define MAX_USERS 50
// Structs and instruction type definition (same as an enumeration in Java)
#define INS_SING_UP 1
#define INS_LOG_IN 2

// User registry struct
typedef struct log_entry
{
    char user_dest[PARAMS_MAX_LENGHT + 1];
    int port;
    time_t timestamp_login;
    time_t timestamp_logout;
} log_entry;

typedef int ins_type;

// Define the structures for the different instructions or messages that are going to be sent.
typedef struct ins_sing_up
{
    ins_type ins_type; // INS_SING_UP
    char username[PARAMS_MAX_LENGHT + 1];
    char password[PARAMS_MAX_LENGHT + 1];
} ins_sing_up;

typedef struct ins_log_in
{
    ins_type ins_type; // = INS_SING_IN
    int srv_port;
    char username[PARAMS_MAX_LENGHT + 1];
    char password[PARAMS_MAX_LENGHT + 1];
} ins_log_in;

// Function definition
int initial_menu(int port);
void socket_creation(int port, int *listener, struct sockaddr_in *my_addr);
void instruction_handler(int instr_num, int *sd, int *current_logs, log_entry (*log_tracker)[]);
void add_user(log_entry l, log_entry (*log_tracker)[], int *current_logs);
