#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef struct{
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
}Row;

typedef struct{
    char* buffer;
    size_t buffer_length;
    long long input_length;
} InputBuffer;

//use enum result codes
typedef enum{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
}MetaCommandResult;

typedef enum{
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
}PrepareResult;

typedef enum{
    STATEMENT_INSERT,
    STATEMENT_SELECT
}StatementType;

typedef struct{
    StatementType type;
    Row row_to_insert;
}Statement;

InputBuffer* new_input_buffer(){
    InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
    input_buffer->buffer=NULL;
    input_buffer->buffer_length=0;
    input_buffer->input_length=0;

    return input_buffer;
}

//this function prints a prompt to the user
void print_prompt(){
    printf("db > ");
}

//function to read input from cmd line
void read_input(InputBuffer* input_buffer){
    long long bytes_read = 
        getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

        //if input is empty
        if(bytes_read<0){
            printf("Error reading input\n");
            exit(EXIT_FAILURE);
        }
        //ignore trailing newline
        input_buffer->input_length = bytes_read - 1;
        input_buffer->buffer[bytes_read - 1] = 0;
}

//function to free memory allocated for an instance of InputBuffer and buffer element of structure
void close_input_buffer(InputBuffer* input_buffer){
    free(input_buffer->buffer);
    free(input_buffer);
}

MetaCommandResult do_meta_command(InputBuffer* input_buffer){
    if(strcmp(input_buffer->buffer, ".exit")==0){
        close_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    }else{
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement){
    if(strncmp(input_buffer->buffer, "insert", 6)==0){
        statement->type = STATEMENT_INSERT;
        int args_assigned = sscanf(
            input_buffer->buffer, "insert %d %s %s", &(statement->row_to_insert.id),
            statement->row_to_insert.username, statement->row_to_insert.email);
        
        if(args_assigned<3){
            return PREPARE_SYNTAX_ERROR;
        }
        return PREPARE_SUCCESS;
    }

    if(strcmp(input_buffer->buffer, "select")==0){
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement){
    switch(statement->type){
        case(STATEMENT_INSERT):
            printf("This is where we would do an insert.\n");
            break;
        case(STATEMENT_SELECT):
            printf("This is where we would do a select.\n");
            break;
        
    }
}

/*
Sqlite starts a REPL (read-execute-print loop) when started from cmd line.
The main function will have an infinite loop that prints the prompt, 
gets a line of input, then processes that line of input 
*/
int main(int argc, char* argv[]){
    InputBuffer* input_buffer = new_input_buffer();
    while(true){
        print_prompt();
        read_input(input_buffer);
        
        //parse and execute the command
        //non-sql statements like .exit are called meta-commands
        //they all start with a dot
        //so check and handle them in seperate function
        if(input_buffer->buffer[0]=='.'){
            switch (do_meta_command(input_buffer)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command '%s'\n", input_buffer->buffer);
                    continue;
                
            }
        }
        

        Statement statement;
        switch(prepare_statement(input_buffer, &statement)){
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized keyword at the start of '%s'.\n", input_buffer->buffer);
                continue;
        }
        //this is like the VM
        execute_statement(&statement);
        printf("Executed.\n");
    }
} 