#define BUF_LEN 1024
#define DEFAULT_PORT 4242
#define PARAMS_MAX_LENGHT 30
#define MAX_USERS 50

// Structs and instruction type definition (same as an enumeration in Java)
typedef enum
{
    INS_SING_UP = 1,
    INS_LOG_IN = 2,
    INS_LOG_OUT = 3
} ins_type;

// User registry struct
typedef struct log_entry
{
    char user_dest[PARAMS_MAX_LENGHT + 1];
    int port;
    time_t timestamp_login;
    time_t timestamp_logout;
} log_entry;

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

typedef struct ins_log_out
{
    ins_type ins_type; // INS_LOG_OUT
    char username[PARAMS_MAX_LENGHT + 1];
} ins_log_out;

// Function definition
int initial_menu(int port);
void socket_creation(int port, int *listener, struct sockaddr_in *my_addr);
void instruction_handler(int instr_num, int *sd);
void log_user(log_entry l);
void log_out_user(char username[PARAMS_MAX_LENGHT + 1]);
void get_online_users();
