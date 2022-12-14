// In the project we used, stdio, string, stdlib and math libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_OUTPUT_LINE_NUM		1024
#define MAX_LINE_LENGTH 		256
#define MAX_LINE_NUM			256
#define MAX_EXPR_LENGTH         256
#define MAX_VARIABLE_LENGTH 	30

#define FALSE		0
#define TRUE 		1

#define NO_ERROR	0
#define ERROR		-1

#define MULTP 0
#define ADD 1
#define SUB 2
#define SQRT 3
#define TR 4
#define CHOOSE 5
#define VALUE 6

#define MATRIX 0
#define SCALAR 1


// Matrix definition to keep matrices defined
typedef struct {
	char name[MAX_VARIABLE_LENGTH];
	int row;
	int col;
} Matrix;

// Scalar definition to keep scalars defined
typedef struct {
	char name[MAX_VARIABLE_LENGTH];
	double value;
} Scalar;

// Node structure for the Stack definition
struct Node {
	int arg_count;
	struct Node *next;
};

// Stack definition to evaluate expressions
typedef struct {
	struct Node *top;
	int count;
} Stack;

// Reads the string pointed by sptr until a token is found, 
// updates *sptr such that it points the character after the token we found,
// and returns a char pointer to the string until that token.
// If any token is not found then it returns NULL
char *strseparate(char **sptr, char *tokens) {
	char *string = *sptr;
	for (int i = 0; string[i] != '\0'; i++) {
		for (int j = 0; tokens[j] != '\0'; j++) {
			if (tokens[j] == string[i] || string[i] == '\n') {
				string[i] = '\0';
				*sptr = string+i+1;
				return string;
			}
		}
	}
	return NULL;
}

// Program Functions
void separate(char *source, char *target);
Scalar init_scalar(char *name);
Matrix init_matrix(char *name, int row, int col);
int row_num(char *name);
int col_num(char *name);
int is_initialized_scalar(char *name, int count, Scalar *scalars);
int is_initialized_matrix(char *name, int count, Matrix *matrices);
void clear_string(char *ptr, int count);
void remove_comment(char *line);
void copy_file_to_file(char *source_file, char *target_file);
int valid(char *variable_name);

// Expression Evaluation Functions
int expr(char *infix, char *prefix);
int get_sign_index(char *str, char *tokens, int open_par_num, int open_square_par_num);
int get_sign_index_rev(char *str, char *tokens, int open_par_num, int open_square_par_num);
int term(char *infix, char *prefix);
int factor(char *infix, char *prefix);
void adjust_space(char *dest, char *source);
void strstrip(char *str, char ch);
int convert(char *dest, char *source);
int isnumber(char *cptr);
int compatible(char *prefix);

// Stack Functions
Stack init_stack();
void push(Stack *stack, int count);
int pop(Stack *stack);
int peek(Stack *stack);
int is_empty(Stack *stack);

// Global Variables
Scalar *scalars = NULL;
int scalar_count = 0;
Matrix *matrices = NULL;
int matrix_count = 0;


int main(int argc, char *argv[]) {
	// Check whether a file is given as an argument
	if (argc < 2) {
		return 1;
	}

	// Allocate space for lines
	char **lines = (char **)malloc(MAX_LINE_NUM * sizeof(char *));
	for (int i = 0; i < MAX_LINE_NUM; i++)
		lines[i] = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
	char *buffer = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
	int line_count = 0;

	// Add a new line at the end of the file
	FILE *input_file;
	/*input_file = fopen(argv[1], "a");
	fprintf(input_file, "\n");
	fclose(input_file);
	*/
	// open the input file and store lines in a 2D array
	if ((input_file = fopen(argv[1], "r")) == NULL) {
		exit(EXIT_FAILURE);
	}

	// Read line by line, separate tokens and remove comments
	while (!feof(input_file)) {
		fgets(buffer, MAX_LINE_LENGTH, input_file);
		separate(buffer, lines[line_count]);
		remove_comment(lines[line_count++]);
		clear_string(buffer, MAX_LINE_LENGTH);	
	}
	// Last line of the input file
	clear_string(buffer, MAX_LINE_LENGTH);
	int i = 0;
	char ch;
	while((ch = fgetc(input_file)) != EOF) {
		buffer[i++] = ch;
	}
	separate(buffer, lines[line_count]);
	remove_comment(lines[line_count++]);
	clear_string(buffer, MAX_LINE_LENGTH);
	fclose(input_file);

	// Allocate space for output files
	char **output_lines = (char **)malloc(MAX_OUTPUT_LINE_NUM*sizeof(char *));
	for (int i = 0; i < MAX_LINE_NUM; i++)
		output_lines[i] = (char *)malloc(MAX_LINE_LENGTH*sizeof(char));
	int output_index = 0;

	char *word, *cptr, *temp;
	int doubly_nested = FALSE;
	int for_block_is_open = FALSE;

	for (int line_index = 0; line_index < line_count; line_index++) {
		if (lines[line_index][0] == '\n')
			continue;

		cptr = lines[line_index]; // cptr points (line_index)th line
		word = strseparate(&cptr, " "); // get the first word
		if (word == NULL)
			break;

		if (strcmp(word, "scalar") == 0) {
			char *name = strseparate(&cptr, " "); // get the variable name, you may need to check whether it is valid or not
			if(!valid(name)) {
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
			if (is_initialized_scalar(name, scalar_count, scalars) || is_initialized_matrix(name, matrix_count, matrices)) {
				printf("Error (Line %d)\n", line_index+1);	// if a variable with that name already exists, then error
				return 1;
			}
			scalars = realloc(scalars, (scalar_count+1)*sizeof(Scalar));
			scalars[scalar_count++] = init_scalar(name);
		
			sprintf(output_lines[output_index++], "Variable %s = init_variable(SCALAR,0,0);\n", name);
		}
		else if (strcmp(word, "vector") == 0) {
			char *name = strseparate(&cptr, " "); // get the variable name
			if(!valid(name)) {
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
			if (is_initialized_scalar(name, scalar_count, scalars) || is_initialized_matrix(name, matrix_count, matrices)) {
				printf("Error (Line %d)\n", line_index+1);	// if a variable with that name already exists, then error
				return 1;
			}
			temp = strseparate(&cptr, " "); // we should get "["
			if (strcmp(temp, "[") != 0) {
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
			char *length = strseparate(&cptr, " ");	// Get the length of the vector
			if(!isnumber(length)) { // length should be a numerical value
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
			temp = strseparate(&cptr, " "); // we should get "]"
			if (strcmp(temp, "]") != 0) {
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
			matrices = realloc(matrices, (matrix_count+1)*sizeof(Matrix));
			matrices[matrix_count++] = init_matrix(name, atoi(length), 1);
			
			sprintf(output_lines[output_index++], "Variable %s = init_variable(MATRIX,%d,1);\n", name, atoi(length));
		}
		else if (strcmp(word, "matrix") == 0) {
			char *name = strseparate(&cptr, " "); // get variable name
			if(!valid(name)) {
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
			if (is_initialized_scalar(name, scalar_count, scalars) || is_initialized_matrix(name, matrix_count, matrices)) {
				printf("Error (Line %d)\n", line_index+1);	// if a variable with that name already exists, then error
				return 1;
			}
			temp = strseparate(&cptr, " "); // we should get "["
			if (strcmp(temp, "[") != 0) {
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
			char *row = strseparate(&cptr, " "); // get row of the matrix
			if(!isnumber(row)){
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}	
			temp = strseparate(&cptr, " "); // we should get ","
			if (strcmp(temp, ",") != 0) {
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
			char *col = strseparate(&cptr, " "); // get col of the matrix
			if(!isnumber(col)){
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
			temp = strseparate(&cptr, " "); // we should get "]"
			if (strcmp(temp, "]") != 0) {
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
			matrices = realloc(matrices, (matrix_count+1)*sizeof(Matrix));
			matrices[matrix_count++] = init_matrix(name, atoi(row), atoi(col));

			sprintf(output_lines[output_index++], "Variable %s = init_variable(MATRIX,%d,%d);\n", name, atoi(row), atoi(col));
		}
		else if (strcmp(word, "print") == 0) {
			temp = strseparate(&cptr, " "); // we shoud get "("
			if (strcmp(temp, "(") != 0) {
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
			int index = get_sign_index(cptr, ")", 0, 0); // index of closing paranthesis
			if (index == -1) { // paranthesis is not found
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
			cptr[index-1] = '\0'; // put null before the )
			char prefix[MAX_EXPR_LENGTH] = "\0";
			if (!expr(cptr, prefix)) { // If the expression is not valid then error
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
			char string[MAX_LINE_LENGTH] = "\0";
			char adjusted[MAX_EXPR_LENGTH] = "\0";
			adjust_space(adjusted, prefix);
			if(!convert(string, adjusted)) {
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}	
			cptr += index+1;	
			sprintf(output_lines[output_index++], "print(%s);\n", string);
		}
		else if (strcmp(word, "printsep") == 0) {
			temp = strseparate(&cptr, " "); // we should get "("
			if (strcmp(temp, "(") != 0) {
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
			temp = strseparate(&cptr, " "); // we should get ")"
			if (strcmp(temp, ")") != 0) {
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
			sprintf(output_lines[output_index++], "printf(\"------------\\n\");\n");
		}
		else if (strcmp(word, "for") == 0) { // for ( <id1> in <expr1> : <expr2> : <expr3> )
			temp = strseparate(&cptr, " "); // we should get "("
			if (strcmp(temp, "(") != 0) {
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
			char *var1 = strseparate(&cptr, " "); // get first name
			if (!is_initialized_scalar(var1, scalar_count, scalars) || is_initialized_matrix(var1, matrix_count, matrices)) {
				printf("Error (Line %d)\n", line_index+1);	// if there is no variable with that name, then error
				return 1;
			}
			temp = strseparate(&cptr, " "); // if doubly nested, it is ","
			if (strcmp(temp, ",") == 0)
				doubly_nested = TRUE;	
			if (doubly_nested) { // for ( <id1> , <id2> in <expr1>:<expr2>:<expr3> , <expr4>:<expr5>:<expr6> )
				char *var2 = strseparate(&cptr, " "); 
				if (!is_initialized_scalar(var1, scalar_count, scalars) || is_initialized_matrix(var1, matrix_count, matrices)) {
					printf("Error (Line %d)\n", line_index+1);	// if there is no variable with that name, then error
					return 1;
				}
				temp = strseparate(&cptr, " "); // we shoud get "in"
				if (strcmp(temp, "in") != 0) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				char *lower1 = strseparate(&cptr, ":"); // first lower bound
				char prefix1[MAX_EXPR_LENGTH] = "\0";
				if (!expr(lower1, prefix1)) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				char converted1[MAX_EXPR_LENGTH] = "\0";
				char adjusted1[MAX_EXPR_LENGTH] = "\0";
				adjust_space(adjusted1, prefix1);
				if (!convert(converted1, adjusted1)) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				char *upper1 = strseparate(&cptr, ":"); // first upper bound
				char prefix2[MAX_EXPR_LENGTH] = "\0";
				if (!expr(upper1, prefix2)) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				char converted2[MAX_EXPR_LENGTH] = "\0";
				char adjusted2[MAX_EXPR_LENGTH] = "\0";
				adjust_space(adjusted2, prefix2);
				if (!convert(converted2, adjusted2)) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				char *update1 = strseparate(&cptr, ","); // first update value
				update1[strlen(update1)-1] = '\0';
				char prefix3[MAX_EXPR_LENGTH] = "\0";
				if (!expr(update1, prefix3)) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				char converted3[MAX_EXPR_LENGTH] = "\0";
				char adjusted3[MAX_EXPR_LENGTH] = "\0";
				adjust_space(adjusted3, prefix3);
				if (!convert(converted3, adjusted3)) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				cptr++; // Jump the space character
				char *lower2 = strseparate(&cptr, ":"); // second lower bound
				char prefix4[MAX_EXPR_LENGTH] = "\0";
				if (!expr(lower2, prefix4)) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				char converted4[MAX_EXPR_LENGTH] = "\0";
				char adjusted4[MAX_EXPR_LENGTH] = "\0";
				adjust_space(adjusted4, prefix4);
				if (!convert(converted4, adjusted4)) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				char *upper2 = strseparate(&cptr, ":"); // second upper bound
				char prefix5[MAX_EXPR_LENGTH] = "\0";
				if (!expr(upper2, prefix5)) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				char converted5[MAX_EXPR_LENGTH] = "\0";
				char adjusted5[MAX_EXPR_LENGTH] = "\0";
				adjust_space(adjusted5, prefix5);
				if (!convert(converted5, adjusted5)) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				char *update2 = strseparate(&cptr, ")"); // second update value
				update2[strlen(update2)-1] = '\0';
				char prefix6[MAX_EXPR_LENGTH] = "\0";
				if (!expr(update2, prefix6)) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				char converted6[MAX_EXPR_LENGTH] = "\0";
				char adjusted6[MAX_EXPR_LENGTH] = "\0";
				adjust_space(adjusted6, prefix6);
				if (!convert(converted6, adjusted6)) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				cptr++; // Jump the space character
				temp = strseparate(&cptr, " ");	// this should be "{" 
				if (strcmp(temp, "{") != 0) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				sprintf(output_lines[output_index++], "for (%s = %s; compare(%s, %s); %s = add(%s, %s)) {\n", \
															var1, converted1, var1, converted2, var1, var1, converted3);
				sprintf(output_lines[output_index++], "for (%s = %s; compare(%s, %s); %s = add(%s, %s)) {\n", \
															var2, converted4, var2, converted5, var2, var2, converted6);
			}
			else { // single loop
				if (strcmp(temp, "in") != 0) { // next token after first variable must be "in"
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				char *lower = strseparate(&cptr, ":");	// lower bound of the loop
				char prefix1[MAX_EXPR_LENGTH] = "\0";
				if (!expr(lower, prefix1)) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				char converted1[MAX_EXPR_LENGTH] = "\0";
				char adjusted1[MAX_EXPR_LENGTH] = "\0";
				adjust_space(adjusted1, prefix1);
				if (!convert(converted1, adjusted1)) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				char *upper = strseparate(&cptr, ":");	// upper bound of the loop
				char prefix2[MAX_EXPR_LENGTH] = "\0";
				if (!expr(upper, prefix2)) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				char converted2[MAX_EXPR_LENGTH] = "\0";
				char adjusted2[MAX_EXPR_LENGTH] = "\0";
				adjust_space(adjusted2, prefix2);
				if (!convert(converted2, adjusted2)) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				char *update = strseparate(&cptr, ")");	// update value of the loop
				update[strlen(update)-1] = '\0';
				char prefix3[MAX_EXPR_LENGTH] = "\0";
				if (!expr(update, prefix3)) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				char converted3[MAX_EXPR_LENGTH] = "\0";
				char adjusted3[MAX_EXPR_LENGTH] = "\0";
				adjust_space(adjusted3, prefix3);
				if (!convert(converted3, adjusted3)) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				cptr++; // Jump the space character
				temp = strseparate(&cptr, " ");	// this should be "{" 
				if (strcmp(temp, "{") != 0) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}
				sprintf(output_lines[output_index++], "for (%s = %s; compare(%s, %s); %s = add(%s, %s)) {\n", \
															var1, converted1, var1, converted2, var1, var1, converted3);
			}
		for_block_is_open = TRUE;
		} // end of the "for" block 
		else if (strcmp(word, "}") == 0) {
			if (for_block_is_open) {
				for_block_is_open = FALSE;
				if(doubly_nested)
					sprintf(output_lines[output_index++], "}\n}\n");
				else
					sprintf(output_lines[output_index++], "}\n");
				doubly_nested = FALSE;
			}
			else {
				printf("Error (Line %d)\n", line_index+1); // if there is no open for block, then error
				return 1;
			}
		}
		// if above conditions are not satisfied then the statement must be assign statement
		else if (is_initialized_scalar(word, scalar_count, scalars)) {
			char *token;
			token = strseparate(&cptr, " "); // we should get "="
			if (strcmp(token, "=") != 0) {
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
			char *expression = strseparate(&cptr, "\n");
			strstrip(expression, ' ');
			char prefix[MAX_EXPR_LENGTH] = "\0";
			if (!expr(expression, prefix)) {
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
			char adjusted[MAX_EXPR_LENGTH] = "\0";
			char converted[MAX_EXPR_LENGTH] = "\0";
			adjust_space(adjusted, prefix);
			if (!convert(converted, adjusted)) {
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
			sprintf(output_lines[output_index++], "%s = %s;\n", word, converted);
		}
		else if (is_initialized_matrix(word, matrix_count, matrices)) {
			char *token;
			token = strseparate(&cptr, " "); // we should get "="
			if (strcmp(token, "=") == 0) {
				char *expression = strseparate(&cptr, "\n");
				if (expression[0] == '{') { // A = { <values_separated_by_space> }
					int row = 0, col = 0;
					int row_number = row_num(word), col_number = col_num(word);
					char *token2 = strseparate(&expression, " "); // Get "{"
					token2 = strseparate(&expression, " "); // Get the first value

					while (strcmp(token2, "}") != 0) {
						sprintf(output_lines[output_index++], "%s.value.matrix.values[%d][%d] = %s;\n", word, row, col, token2);
						token2 = strseparate(&expression, " ");
						if (token2 == NULL) {
							printf("Error (Line %d)\n", line_index+1);
							return 1;
						}
						col++;
						if (col == col_num(word)) {
							row++;
							col = 0;
						}
					}
					if (row != row_number ||  col != 0) { // There are more values than needed, overflow
						printf("Error (Line %d)\n", line_index+1);
						return 1;
					}
				}
				else { // A = <expr>
					strstrip(expression, ' ');
					char prefix[MAX_EXPR_LENGTH] = "\0";
					if (!expr(expression, prefix)) {
						printf("Error (Line %d)\n", line_index+1);
						return 1;
					}
					char adjusted[MAX_EXPR_LENGTH] = "\0", converted[MAX_EXPR_LENGTH] = "\0";
					adjust_space(adjusted, prefix);
					if (!convert(converted, adjusted)) {
						printf("Error (Line %d)\n", line_index+1);
						return 1;
					}
					sprintf(output_lines[output_index++], "%s = %s;\n", word, converted);
				}
			}
			else if (strcmp(token, "[") == 0) {
				int index = get_sign_index(cptr, ",]", 0, 0);
				if (index == -1) {
					printf("Error (Line %d)\n", line_index+1);
					return 1;
				}				
				if(cptr[index] == ',') {
					char *expression1 = strseparate(&cptr, ",]");
					expression1[strlen(expression1)-1] = '\0';
					char prefix1[MAX_EXPR_LENGTH] = "\0";
									
					if (!expr(expression1, prefix1)) {
						printf("Error (Line %d)\n", line_index+1);
						return 1;
					}
					char adjusted1[MAX_EXPR_LENGTH] = "\0", converted1[MAX_EXPR_LENGTH] = "\0"; 
					adjust_space(adjusted1, prefix1);
					if (!convert(converted1, adjusted1)) {
						printf("Error (Line %d)\n", line_index+1);
						return 1;
					}
					cptr++;
					char *expression2 = strseparate(&cptr, "]");
					expression2[strlen(expression2)-1] = '\0';
					char prefix2[MAX_EXPR_LENGTH] = "\0";
					if(!expr(expression2, prefix2)) {
						printf("Error (Line %d)\n", line_index+1);
						return 1;
					}
					char adjusted2[MAX_EXPR_LENGTH] = "\0", converted2[MAX_EXPR_LENGTH] = "\0"; 
					adjust_space(adjusted2, prefix2);
					if (!convert(converted2, adjusted2)) {
						printf("Error (Line %d)\n", line_index+1);
						return 1;
					}
					cptr++;
					token = strseparate(&cptr, " ");
					if(strcmp(token, "=") != 0) {
						printf("Error (Line %d)\n", line_index+1);
						return 1;
					}
					char *expression3 = strseparate(&cptr, "\n");
					char prefix3[MAX_EXPR_LENGTH] = "\0";
					if(!expr(expression3, prefix3)) {
						printf("Error (Line %d)\n", line_index+1);
						return 1;
					}
					char adjusted3[MAX_EXPR_LENGTH] = "\0", converted3[MAX_EXPR_LENGTH] = "\0"; 
					adjust_space(adjusted3, prefix3);
					if (!convert(converted3, adjusted3)) {
						printf("Error (Line %d)\n", line_index+1);
					return 1;
					}					
					sprintf(output_lines[output_index++], "assign_index(&%s, %s, %s, %s);\n", word, converted1, converted2, converted3);
				}
				else if(cptr[index] == ']') {
					char *expression1 = strseparate(&cptr, ",]");
					expression1[strlen(expression1)-1] = '\0';
					char prefix1[MAX_EXPR_LENGTH] = "\0";
					
					if (!expr(expression1, prefix1)) {
						printf("Error (Line %d)\n", line_index+1);
						return 1;
					}
					char adjusted1[MAX_EXPR_LENGTH] = "\0", converted1[MAX_EXPR_LENGTH] = "\0"; 
					adjust_space(adjusted1, prefix1);
					if (!convert(converted1, adjusted1)) {
						printf("Error (Line %d)\n", line_index+1);
						return 1;
					}
					cptr++;
					token = strseparate(&cptr, " ");
					if(strcmp(token, "=") != 0) {
						printf("Error (Line %d)\n", line_index+1);
						return 1;
					}
					char *expression2 = strseparate(&cptr, "\n");
					char prefix2[MAX_EXPR_LENGTH] = "\0";
					if(!expr(expression2, prefix2)) {
						printf("Error (Line %d)\n", line_index+1);
						return 1;
					}
					char adjusted2[MAX_EXPR_LENGTH] = "\0", converted2[MAX_EXPR_LENGTH] = "\0"; 
					adjust_space(adjusted2, prefix2);
					if (!convert(converted2, adjusted2)) {
						printf("Error (Line %d)\n", line_index+1);
						return 1;
					}
					sprintf(output_lines[output_index++], "assign_index(&%s, %s, var(1), %s);\n", word, converted1, converted2);
				}
			}
			else {	
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
		}
		else {	
			// if the first word is different than words above, then there is an error
			printf("Error (Line %d)\n", line_index+1);
			return 1;
		}
		
		for (int i = 0; cptr[i] != '\0'; i++) {
			if (cptr[i] != ' ' && cptr[i] != '\n') {
				printf("Error (Line %d)\n", line_index+1);
				return 1;
			}
		}
	} // end of the for loop where lines are read
	
	// If a for block is not closed at the end, then there is an error
	if (for_block_is_open) {
		printf("Error (Line %d)\n", line_count);
		return 1;
	}
	
	// OUTPUT
	copy_file_to_file("content.txt", "file.c");
	FILE *output_file;
	output_file = fopen("file.c", "a");
	for(int i = 0; i < output_index; i++)
		fprintf(output_file, "%s", output_lines[i]);
	fprintf(output_file, "return 0;\n}\n");
	fclose(output_file);
	
	// Free the allocated space
	for (int i = 0; i < MAX_LINE_NUM; i++)
		free(lines[i]);
	free(lines);
	for (int i = 0; i < MAX_OUTPUT_LINE_NUM; i++)
		free(output_lines[i]);
	free(output_lines);
	free(scalars);
	free(matrices);
	free(buffer);
	return 0;
}

// Copies the source string to target string by putting spaces before and after characters "[]{}(),=*+-"
void separate(char *source, char *target) {
	int target_index = 0;
	int space_found = TRUE;
	for (int i = 0; source[i] != '\0'; i++) {
		if (i == 0 && source[i] == '}') {
			target[target_index++] = source[i];
		}
		else if (source[i] == '[' || source[i] == ']' || source[i] == ',' || \
				source[i] == '{' || source[i] == '}' || source[i] == '(' || \
				source[i] == ')' || source[i] == '=' || source[i] == '*' || \
				source[i] == '+' || source[i] == '-') {
			if(!space_found) {
				target[target_index++] = ' ';
			}
			target[target_index++] = source[i];
			target[target_index++] = ' ';
			space_found = TRUE;
		}
		else if (source[i] == ' ') {
			if (!space_found) {
				space_found = TRUE;
				target[target_index++] = ' ';
			}
		}
		else {
            space_found = FALSE;
			target[target_index++] = source[i];
		}		
	}
	target[target_index] = '\0';
}

// Initializes a Scalar and returns it
Scalar init_scalar(char *name) {
	Scalar result;
	strcpy(result.name, name);
	result.value = 0.0;
	return result;
}

// Initializes a Matrix and returns it
Matrix init_matrix(char *name, int row, int col) {
	Matrix result;
	strcpy(result.name, name);
	result.row = row;
	result.col = col;
	return result;
}

// Checks whether a variable name is an initialized scalar or not, return TRUE if it is, otherwise returns FALSE
int is_initialized_scalar(char *name, int count, Scalar *scalars) {
	for (int i = 0; i < count; i++) {
		if (strcmp(name, scalars[i].name) == 0)
			return TRUE;
	}
	return FALSE;
}

// Checks whether a variable name is an initialized matrix or not, return TRUE if it is, otherwise returns FALSE
int is_initialized_matrix(char *name, int count, Matrix *matrices) {
	for (int i = 0; i < count; i++) {
		if (strcmp(name, matrices[i].name) == 0)
			return TRUE;
	}
	return FALSE;
}

// Clears the string
void clear_string(char *ptr, int count) {
	for (int i = 0; i < count; i++)
		ptr[i] = '\0';
}

// Initializes a stack and returns it
Stack init_stack() {
	Stack result;
	result.top = NULL;
	result.count = 0;
	return result;
}

// Adds the integer to the stack
void push(Stack *stack, int count) {
	struct Node *node = (struct Node *)malloc(sizeof(struct Node));
	node->arg_count = count;
	node->next = stack->top;
	stack->top = node;
	stack->count++;
}

// Pops from the stack and returns the integer value, if the stack is empty it returns ERROR
int pop(Stack *stack) {
	if (is_empty(stack))
		return ERROR;
	struct Node *node = stack->top;
	int result = node->arg_count;
	stack->top = node->next;
	stack->count--;
	free(node);
	return result;
}

// Returns the integer at the top of the stack, if the stack is empty it returns ERROR
int peek(Stack *stack) {
	if (is_empty(stack))
		return ERROR;
	return stack->top->arg_count;
}

// Returns TRUE if the stack is empty, otherwise FALSE
int is_empty(Stack *stack) {
	return stack->count == 0;
}

// Removes comments
void remove_comment(char *line) {
   char *p;
   if ((p = strchr(line, '#')) != NULL) {
      *p = '\n';
	  *(p + 1) = '\0'; 
   } 
}

// Returns TRUE if the string is consists of only numerical characters
int isnumber(char *cptr){
	for(int i = 0; cptr[i] != '\0'; i++){
		if(!(cptr[i] > 47 && cptr[i] < 58)) {
			return FALSE;
		}
	}
	return TRUE;
}

// Copies the content of the source file and writes it into the target file
void copy_file_to_file(char *source_file, char *target_file) {
	char c;
	FILE *source, *target;
	source = fopen(source_file, "r");
	target = fopen(target_file, "w");
	while((c = fgetc(source)) != EOF) {
		fputc(c, target);
	}
	fclose(source);
	fclose(target);
}

// 1-) A variable name must consist of alphabetical char, numerical char, and underscore
// 2-) A variable name must contain at least 1 alphabetical char
// 3-) A variable cannot begin with a numerical char
int valid(char *name) {
	if (47 < name[0] && name[0] < 58) {
		return FALSE;
	}
	for (int i = 0; i < name[i] != '\0'; i++) {
		if (!((64 < name[i] && name[i] < 91) || (96 < name[i] && name[i] < 123) || (47 < name[i] && name[i] < 58)||(name[i] == '_'))) {
			return FALSE;
		}
	}
	return TRUE;	
}

// Returns the index of the first occuring token in the string such that number of parantheses in str[:i] is the given number
// If there is no such token found, it returns -1
int get_sign_index(char *str, char *tokens, int open_par_num, int open_square_par_num) {
    int open_parantheses_count = 0;
    int open_square_par_count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        for (int j = 0; tokens[j] != '\0'; j++) {
            if ((str[i] == tokens[j] || str[i] == '\n') && open_parantheses_count == open_par_num && open_square_par_count == open_square_par_num)
                return i;
        }
        open_parantheses_count += (str[i] == '(')?1:(str[i] == ')')?-1:0;
        open_square_par_count += (str[i] == '[')?1:(str[i] == ']')?-1:0;
    }
    return -1;
}

// Returns the index of the first occuring token in the string starting from the end such that 
// number of parantheses in str[i+1:] is the given number
// If there is no such tokenn found, it returns -1
int get_sign_index_rev(char *str, char *tokens, int open_par_num, int open_square_par_num) {
    int open_parantheses_count = 0;
    int open_square_par_count = 0;
    for (int i = strlen(str)-1; i > -1; i--) {
        for (int j = strlen(tokens); j > -1; j--) {
            if (str[i] == tokens[j] && open_parantheses_count == open_par_num && open_square_par_count == open_square_par_num)
                return i;
        }
        open_parantheses_count += (str[i] == '(')?1:(str[i] == ')')?-1:0;
        open_square_par_count += (str[i] == '[')?1:(str[i] == ']')?-1:0;
    }
    return -1;
}

// Evaluates an expression and converts it to prefix notation
// Returns TRUE if the expression is valid. Otherwise, returns FALSE
int expr(char *infix, char *prefix) {
    // Assumes that there is no whitespace at the beginning and the end of the infix string
    // Assumes that there is a single whitespace between every token
    int index = get_sign_index_rev(infix, "+-", 0, 0);
    if (index == -1) {
        char temp[MAX_EXPR_LENGTH] = "\0";
        if (term(infix, temp)) {
            strcat(prefix, temp);
            strcat(prefix, " ");
            return TRUE;
        }
        else
            return FALSE;
    }
    char operator[2] = {infix[index], '\0'};
    infix[index-1] = '\0';
    char temp1[MAX_EXPR_LENGTH] = "\0", temp2[MAX_EXPR_LENGTH] = "\0";
    if (expr(infix, temp1) && term(infix+index+2 /* added 2 to jump the operator and the whitespace after it*/, temp2)) {
        strcat(prefix, operator);
        strcat(prefix, " ");
        strcat(prefix, temp1);
        strcat(prefix, temp2);
        prefix[strlen(prefix)-1] = '\0'; 
        return TRUE;
    }
    return FALSE;
}

// Evaluates a term and converts it to prefix notation
// Returns TRUE if the term is valid. Otherwise, returns FALSE
int term(char *infix, char *prefix) {
    // Assumes that there is no whitespace at the beginning and the end of the infix string
    // Assumes that there is a single whitespace between every token
     int index = get_sign_index_rev(infix, "*", 0, 0);
     if (index == -1) {
        char temp[MAX_EXPR_LENGTH] = "\0";
        if (factor(infix, temp)) {
            strcat(prefix, temp);
            strcat(prefix, " ");
            return TRUE;
        }
        else
            return FALSE;
    }
    infix[index-1] = '\0';
    char temp1[MAX_EXPR_LENGTH] = "\0", temp2[MAX_EXPR_LENGTH] = "\0";
    if (term(infix, temp1) && factor(infix+index+2 /* added 2 to jump the operator and the whitespace after it*/, temp2)) {
        strcat(prefix, "* ");
        strcat(prefix, temp1);
        strcat(prefix, " ");
        strcat(prefix, temp2);
        strcat(prefix, " ");
        return TRUE;
    }
    return FALSE;
}

// Evaluates a factor and converts it to prefix notation
// Returns TRUE if the factor is valid. Otherwise, returns FALSE
int factor(char *infix, char *prefix) {
    // Assumes that there is no whitespace at the beginning and the end of the infix string
    // Assumes that there is a single whitespace between every token
    int len = strlen(infix); 
    if (infix[0] == '(' && infix[len-1] == ')') {
        infix[len-2] = '\0';
        char temp[MAX_EXPR_LENGTH] = "\0";
        if (expr(infix+2 /* added 2 to jump ( and the whitespace after it*/, temp)) {
            strcat(prefix, temp);
            strcat(prefix, " ");
            return TRUE;
        }
        else
            return FALSE;
    }
    else if (strncmp(infix, "sqrt (", 6) == 0 && infix[len-1] == ')') {  // sqrt ( <expr> )
        infix[len-2] = '\0';
        char temp[MAX_EXPR_LENGTH] = "\0";
        if (expr(infix+7 /* added 7 to jump the string "sqrt ( " */, temp)) {
            strcat(prefix, "sqrt ");
            strcat(prefix, temp);
            strcat(prefix, " ");
            return TRUE;
        }
        else 
            return FALSE;
    }
    else if (strncmp(infix, "tr (", 4) == 0 && infix[len-1] == ')') {  // tr ( <expr> )
        infix[len-2] = '\0';
        char temp[MAX_EXPR_LENGTH] = "\0";
        if (expr(infix+5 /* added 7 to jump the string "tr ( " */, temp)) {
            strcat(prefix, "tr ");
            strcat(prefix, temp);
            strcat(prefix, " ");
            return TRUE;
        }
        else
            return FALSE;
    }
    else if (strncmp(infix, "choose (", 8) == 0 && infix[len-1] == ')') {  // choose ( <expr> , <expr> , <expr> , <expr> )
        infix[len-2] = '\0';
        char temp1[MAX_EXPR_LENGTH] = "\0", temp2[MAX_EXPR_LENGTH] = "\0", temp3[MAX_EXPR_LENGTH] = "\0", temp4[MAX_EXPR_LENGTH] = "\0";
        char expr1[MAX_EXPR_LENGTH] = "\0", expr2[MAX_EXPR_LENGTH] = "\0", expr3[MAX_EXPR_LENGTH] = "\0", expr4[MAX_EXPR_LENGTH] = "\0";
        int begin = 9 /* begining of the expr */ , end /* end of the expr */;
        end = get_sign_index(infix+begin, ",", 0, 0) + begin; // index of first ','
        infix[end-1] = '\0';
        strcpy(expr1, infix+begin);
        if (!expr(expr1, temp1))
            return FALSE;

        begin = end + 2;
        end = get_sign_index(infix+begin, ",", 0, 0) + begin; // index of second ','
        infix[end-1] = '\0';
        strcpy(expr2, infix+begin);
        if (!expr(expr2, temp2))
            return FALSE;

        begin = end + 2;
        end = get_sign_index(infix+begin, ",", 0, 0) + begin; // index of third ','
        infix[end-1] = '\0';
        strcpy(expr3, infix+begin);
        if (!expr(expr3, temp3))
            return FALSE;

        begin = end + 2;
        end = get_sign_index(infix+begin, ",", 0, 0) + begin; // get the end of the fourth expr
        infix[end-1] = '\0';
        strcpy(expr4, infix+begin);
        if (!expr(expr4, temp4)) 
            return FALSE;

        strcat(prefix, "choose ");
        strcat(prefix, temp1);
        strcat(prefix, " ");
        strcat(prefix, temp2);
        strcat(prefix, " ");
        strcat(prefix, temp3);
        strcat(prefix, " ");
        strcat(prefix, temp4);
        strcat(prefix, " ");
        return TRUE;
    }

    else if (isnumber(infix)) {
        strcat(prefix,"var(");
		strcat(prefix, infix);
        strcat(prefix, ") ");
        return TRUE;
    }

    // infix has a form of <var_name>
    char *cptr;
    if ((cptr = strseparate(&infix, " ")) == NULL) {
		if (!is_initialized_scalar(infix, scalar_count, scalars) && !is_initialized_matrix(infix, matrix_count, matrices))
			return FALSE;
        strcat(prefix, infix);
        strcat(prefix, " ");
        return TRUE;
    } 

    // infix has a form of M [ <expr> ] or M [ <expr> , <expr> ]
    char name[MAX_VARIABLE_LENGTH] = "\0";
    strcpy(name, cptr);
	
	if (!is_initialized_matrix(name, matrix_count, matrices))
		return FALSE;

    cptr = strseparate(&infix, " ");
    if (strcmp(cptr, "[") != 0)
        return FALSE;

    int index = get_sign_index(infix, ",]", 0, 0);
    if (infix[index] == ',') { //  M [ <expr> , <expr> ]
        infix[index-1] = '\0';
        char expr1[MAX_EXPR_LENGTH] = "\0", temp1[MAX_EXPR_LENGTH] = "\0";
        strcpy(expr1, infix);
        if (!expr(expr1, temp1))
            return FALSE;
            
        infix = infix + index + 2;  // points the second expr (index)
        index = get_sign_index(infix, "]", 0, 0);
        infix[index-1] = '\0';
        char expr2[MAX_EXPR_LENGTH] = "\0", temp2[MAX_EXPR_LENGTH] = "\0";
        strcpy(expr2, infix);
        if (!expr(expr2, temp2))
            return FALSE;

        strcat(prefix, "[] "); // Consider [] as a function such that "[] A 4 2" returns the value of A[4][2]
        strcat(prefix, name);
        strcat(prefix, " ");
        strcat(prefix, temp1);
        strcat(prefix, " ");
        strcat(prefix, temp2);
        strcat(prefix, " ");
        return TRUE;
    }
    else if (infix[index] == ']') { // M [ <expr> ]
        infix[index-1] = '\0';
        char expr1[MAX_EXPR_LENGTH] = "\0", temp1[MAX_EXPR_LENGTH] = "\0";
        strcpy(expr1, infix);
        if (!expr(expr1, temp1))
            return FALSE;

        strcat(prefix, "[] ");
        strcat(prefix, name);
        strcat(prefix, " ");
        strcat(prefix, temp1);
        strcat(prefix, "var(1) ");
        return TRUE;
    }
    return FALSE;
}

// Removes multiple whitspaces
// Post-Condition: Every token in the dest is separated by only one whitespace
void adjust_space(char *dest, char *source) {
    int dest_index = 0;
    for (int i = 0; source[i] != '\0'; i++) {
        if (!(source[i] == ' ' && source[i-1] == ' '))
            dest[dest_index++] = source[i];
    }
    dest[dest_index] = '\0';
}

// Removes a char from the beginning and the end of the string
void strstrip(char *str, char ch) {
	char temp[MAX_EXPR_LENGTH] = "\0";
	int i = 0;
	while (str[i] == ch) {
		i++;
	}
	strcpy(temp, str+i);
	i = strlen(temp)-1;
	while (str[i] == ch) {
		i--;
	}
	temp[i+1] = '\0';
	strcpy(str, temp);
}

// Converts a prefix notation to function notation
// EX: sqrt + + var(5) var(2) m ---> sqrt(add(add(var(5),var(2)),m))
int convert(char *dest, char *prefix) {
	if (!compatible(prefix)) {
		return FALSE;
	}
	char *cptr;
	Stack stack = init_stack();
    while ((cptr = strseparate(&prefix, " ")) != NULL) {
        while (peek(&stack) == 0) { // If a function gets all its arguments then close it
            strcat(dest, ")");
            pop(&stack);
        }
        if (strcmp(cptr, "+") == 0) {
            if (dest[strlen(dest)-1] != '(' && peek(&stack) >= 1) // if not first argument, then put comma before the next argument
                strcat(dest, ",");
            if (!is_empty(&stack))
                push(&stack, pop(&stack)-1); // reduce the number of arguments needed by 1 for the previous function

            strcat(dest, "add(");
			push(&stack, 2);
        }
        else if (strcmp(cptr, "-") == 0) {
            if (dest[strlen(dest)-1] != '(' && peek(&stack) >= 1)
                strcat(dest, ",");
            if (!is_empty(&stack))
                push(&stack, pop(&stack)-1);

            strcat(dest, "sub(");
			push(&stack, 2);
        }
        else if (strcmp(cptr, "*") == 0) {          
			if (dest[strlen(dest)-1] != '(' && peek(&stack) >= 1)
                strcat(dest, ",");
            if (!is_empty(&stack))
                push(&stack, pop(&stack)-1);

            strcat(dest, "multp(");
			push(&stack, 2); 
        }
        else if (strcmp(cptr, "sqrt") == 0) {
            if (dest[strlen(dest)-1] != '(' && peek(&stack) >= 1)
                strcat(dest, ",");
            if (!is_empty(&stack))
                push(&stack, pop(&stack)-1);

            strcat(dest, "square_root(");
			push(&stack, 1);
        }
        else if (strcmp(cptr, "tr") == 0) {
            if (dest[strlen(dest)-1] != '(' && peek(&stack) >= 1)
                strcat(dest, ",");
            if (!is_empty(&stack))
                push(&stack, pop(&stack)-1);

            strcat(dest, "tr(");
			push(&stack, 1);
        }
        else if (strcmp(cptr, "choose") == 0) {
            if (dest[strlen(dest)-1] != '(' && peek(&stack) >= 1)
                strcat(dest, ",");
            if (!is_empty(&stack))
                push(&stack, pop(&stack)-1);

            strcat(dest, "choose(");
			push(&stack, 4);
        }
		else if (strcmp(cptr, "[]") == 0) {
            if (dest[strlen(dest)-1] != '(' && peek(&stack) >= 1)
                strcat(dest, ",");
            if (!is_empty(&stack))
                push(&stack, pop(&stack)-1);

            strcat(dest, "value(");
			push(&stack, 3);
        }
        else {
            if (dest[strlen(dest)-1] != '(' && peek(&stack) >= 1)
                strcat(dest, ",");
            push(&stack, pop(&stack)-1);
            strcat(dest, cptr);
        }
    }
    while (peek(&stack) == 0) {
        strcat(dest, ")");
        pop(&stack);
    }
    strcat(dest, "\0");
	return TRUE;
}

// Returns the row number of the matrix
int row_num(char *name) {
	for (int i = 0; i < matrix_count; i++) {
		if (strcmp(name, matrices[i].name) == 0)
			return matrices[i].row;
	}
	return -1;
}

// Returns the col number of the matrix
int col_num(char *name) {
	for (int i = 0; i < matrix_count; i++) {
		if (strcmp(name, matrices[i].name) == 0)
			return matrices[i].col;
	}
	return -1;
}
int compatible(char *prefix) {
	Stack stack_num = init_stack(); // Number of arguments of the functions
	Stack stack_func = init_stack(); // Functions
	Stack stack_type = init_stack(); // Result type
	Stack stack_row = init_stack(); // Result row
	Stack stack_col = init_stack(); // Result col
	
	char *temp = (char *)calloc(MAX_EXPR_LENGTH, sizeof(char));
	strcpy(temp, prefix);
	char *token = strseparate(&temp, " ");
	if (strcmp(token, "*") != 0 && strcmp(token, "+") != 0 && strcmp(token, "-") != 0 && strcmp(token, "value") != 0 && \
		strcmp(token, "sqrt") != 0 && strcmp(token, "tr") != 0 && strcmp(token, "choose") != 0)
		return TRUE;
	while(token != NULL) {
		if (strcmp(token, "*") == 0) {
			if (!is_empty(&stack_num))
				push(&stack_num, pop(&stack_num)-1);
			push(&stack_func, MULTP);
			push(&stack_num, 2);
		}
		else if (strcmp(token, "+") == 0) {
			if (!is_empty(&stack_num))
				push(&stack_num, pop(&stack_num)-1);
			push(&stack_func, ADD);
			push(&stack_num, 2);
		}
		else if (strcmp(token, "-") == 0) {	
			if (!is_empty(&stack_num))
				push(&stack_num, pop(&stack_num)-1);
			push(&stack_func, SUB);
			push(&stack_num, 2);
		}
		else if (strcmp(token, "sqrt") == 0) {	
			if (!is_empty(&stack_num))
				push(&stack_num, pop(&stack_num)-1);
			push(&stack_func, SQRT);
			push(&stack_num, 1);
		}
		else if (strcmp(token, "tr") == 0) {	
			if (!is_empty(&stack_num))
				push(&stack_num, pop(&stack_num)-1);
			push(&stack_func, TR);
			push(&stack_num, 1);
		}
		else if (strcmp(token, "choose") == 0) {	
			if (!is_empty(&stack_num))
				push(&stack_num, pop(&stack_num)-1);
			push(&stack_func, CHOOSE);
			push(&stack_num, 4);
		}
		else if (strcmp(token, "[]") == 0) {	
			if (!is_empty(&stack_num))
				push(&stack_num, pop(&stack_num)-1);
			push(&stack_func, VALUE);
			push(&stack_num, 3);

		}
		else if (token[strlen(token)-1] == ')' || is_initialized_scalar(token, scalar_count, scalars)) { // var(1) or <scalar_name>			
			push(&stack_type, SCALAR);
			push(&stack_row, 0);
			push(&stack_col, 0);
			push(&stack_num, pop(&stack_num)-1);
		}

		else if (is_initialized_matrix(token, matrix_count, matrices)) { // <var_matrix>
			push(&stack_type, MATRIX);
			push(&stack_row, row_num(token));
			push(&stack_col, col_num(token));
			push(&stack_num, pop(&stack_num)-1);
		}
		while(peek(&stack_num) == 0) {
			int op = pop(&stack_func);
			if(op == ADD || op == SUB) {
				int type1 = pop(&stack_type);
				int type2 = pop(&stack_type);
				if (type1 == SCALAR && type2 == SCALAR) {					
					pop(&stack_col);
					pop(&stack_row);
					push(&stack_type, SCALAR);
				}
				else if (type1 == MATRIX && type2 == MATRIX) {
					int col2 = pop(&stack_col);
					int row2 = pop(&stack_row);

					int col1 = pop(&stack_col);
					int row1 = pop(&stack_row);
					if (col1 != col2 || row1 != row2)
						return FALSE;
					push(&stack_col, col1);
					push(&stack_row, row1);
					push(&stack_type, MATRIX);
				}
				else
					return FALSE;
			}
			else if (op == MULTP) {
				int type2 = pop(&stack_type);
				int type1 = pop(&stack_type);
				if (type1 == SCALAR && type2 == SCALAR) {
					pop(&stack_col);
					pop(&stack_row);
					push(&stack_type, SCALAR);
				}
				else if (type1 == MATRIX && type2 == MATRIX) {
					int col2 = pop(&stack_col);
					int row2 = pop(&stack_row);

					int col1 = pop(&stack_col);
					int row1 = pop(&stack_row);
					if(col1 != row2)
						return FALSE;
					push(&stack_col, col2);
					push(&stack_row, row1);
					push(&stack_type, MATRIX);
				}
				else if (type1 == SCALAR && type2 == MATRIX) {
					int col2 = pop(&stack_col);
					int row2 = pop(&stack_row);
					pop(&stack_col);
					pop(&stack_row);
					push(&stack_col, col2);
					push(&stack_row, row2);
					push(&stack_type, MATRIX);
				}
				else if (type1 == MATRIX && type2 == SCALAR) {
					pop(&stack_col);
					pop(&stack_row);
					int col1 = pop(&stack_col);
					int row1 = pop(&stack_row);
					push(&stack_col, col1);
					push(&stack_row, row1);
					push(&stack_type, MATRIX);
				}
			}
			else if (op == SQRT) {
				int type = peek(&stack_type);
				if (type != SCALAR)
					return FALSE;
			}
			else if (op == TR) {
				int row = pop(&stack_row);
				int col = pop(&stack_col);
				push(&stack_row, col);
				push(&stack_col, row);
			}
			else if (op == CHOOSE) {
				int type4 = pop(&stack_type);
				int type3 = pop(&stack_type);
				int type2 = pop(&stack_type);
				int type1 = pop(&stack_type);
				if (type4 != SCALAR || type3 != SCALAR || type2 != SCALAR || type1 != SCALAR)
					return FALSE;
				for(int i = 0; i < 3; i++) {
					pop(&stack_col);
					pop(&stack_row);
				}
				push(&stack_type, SCALAR);
			}
			else if (op == VALUE) {
				int type3 = pop(&stack_type);
				int type2 = pop(&stack_type);
				int type1 = pop(&stack_type);
				if (type1 != MATRIX || type2 != SCALAR || type3 != SCALAR)
					return FALSE;
				pop(&stack_col);
				pop(&stack_col);
				pop(&stack_col);
				pop(&stack_row);
				pop(&stack_row);
				pop(&stack_row);
				push(&stack_type, SCALAR);
				push(&stack_col, 0);
				push(&stack_row, 0);
			}
			pop(&stack_num);
		}
		token = strseparate(&temp, " ");
	}
	//free(temp); CAUSES CRASH?
	return TRUE;
}
