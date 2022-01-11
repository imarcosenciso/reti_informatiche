// Some constants
#define BUFFER_SIZE 1024
#define DEFAULT_PORT 4242
#define PARAMS_MAX_LENGHT 30

// Structs and instruction type definition (same as an enumeration in Java)
#define INS_SING_UP 1
#define INS_SING_IN 2

typedef int ins_type;

// Define the structures for the different instructions or messages that are going to be sent.
typedef struct ins_sing_up
{
    ins_type ins_type; // INS_SING_UP
    char user_dest[PARAMS_MAX_LENGHT + 1];
    char user_pass[PARAMS_MAX_LENGHT + 1];
} ins_sing_up;

typedef struct ins_sing_in
{
    ins_type ins_type; // = INS_SING_IN
    char user_dest[PARAMS_MAX_LENGHT + 1];
    char user_pass[PARAMS_MAX_LENGHT + 1];
} ins_sing_in;

// Function definition
void socket_creation(int port, int *sd, struct sockaddr_in *srv_addr);
void stablish_connection(int *ret, int *sd, struct sockaddr_in *srv_addr);

// Functions related to the service
char *signup();
