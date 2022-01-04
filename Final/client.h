typedef int ins_type;

#define INS_SING_UP 1
#define INS_SING_IN 2

typedef struct ins_sing_up
{
    ins_type ins_type; // INS_SING_UP
    char user_dest;
    char user_pass;
};
