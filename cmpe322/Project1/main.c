
/*
 * Check commands with their arguments to search them in the history with dididothat.
 * For instance, if "printfile file.c" is used, then use dididothat "printfile file.c" to check it in history,
 * Not dididothat "printfile". 
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


#define MAX_INPUT_LENGTH	100
#define MAX_WORD_NUMBER		10
#define MAX_WORD_LENGTH		100
#define MAX_LINE_LENGTH		256

#define USE_DEFAULT_EDITOR	0				// Set 1 to use the default text editor, 0 to use gedit

// Global variable
pid_t parent_id;
char editor[40];

// Function prototypes
int split(char *input, char *words[MAX_WORD_NUMBER], char ch);
int in_history(char *command, char **history, int size); 
void call_ls(void);
void call_hostname(void);
void show_ip_address(void);
void printfile(char * filename);
void printfile_redirect(char *src_file, char *new_file);
void open_text_editor(char *filename);


int main(int argc, char *argv[]) {
	parent_id = getpid();					// it is used to check control processes
	strcpy(editor, "gedit");				// set the editor to gedit

#if USE_DEFAULT_EDITOR
	if (getenv("EDITOR") != NULL) {			// if a default editor is set, then use it 
		strcpy(editor, getenv("EDITOR"));
	} else {								// otherwise, use nano as default to avoid an error
		strcpy(editor, "nano");
	}
#endif

	char input[MAX_INPUT_LENGTH];
	char input_copy[MAX_INPUT_LENGTH];
	char *words[MAX_WORD_NUMBER];			// an array for words of the user input
	for (int i = 0; i < MAX_WORD_NUMBER; i++) {
		words[i] = (char *)malloc(MAX_WORD_LENGTH);
	}
	int word_num;

	char **history = NULL;					// history of commands
	int history_size = 0;

	while (1) {
		printf("%s >>> ", getenv("USER"));						// prints the user name
		fgets(input, MAX_INPUT_LENGTH, stdin);					// waits for a command
		if (input[strlen(input)-1] == '\n') {					// replaces line feed at the string by 0
			input[strlen(input)-1] = 0;
		}
		strcpy(input_copy, input);								// stores a copy of the input
		word_num = split(input, words, ' ');					// splits the input to words
		
		if (strcmp(words[0], "exit") == 0) {					// ends the terminal
			break;
		} else if (strcmp(words[0], "listdir") == 0) {			// lists files and directories
			call_ls();
		} else if (strcmp(words[0], "mycomputername") == 0) {	// prints device name
			call_hostname();
		} else if (strcmp(words[0], "whatsmyip") == 0) {		// prints ip address
			show_ip_address();	
		} else if (strcmp(words[0], "printfile") == 0) {
			if (word_num == 4 && strcmp(words[2], ">") == 0) {	// executes "cp src_file dest_file"
				printfile_redirect(words[1], words[3]);
			} else if (word_num == 2) {							// opens the file and prints its content line by line for each enter press
				printfile(words[1]);
			}
		} else if (strcmp(words[0], "dididothat") == 0) {		// searches command in history	
			char *command = words[1]+1;							// get the command and ignore the openning quotation mark
			command[strlen(command)-1] = 0;						// remove the closing quotation mark
			if (in_history(command, history, history_size)) {
				printf("Yes\n");
			} else {
				printf("No\n");
			}
		} else if (strcmp(words[0], "hellotext") == 0) {		// opens a file with the default text editor
			if (word_num == 2) {
				open_text_editor(words[1]);						// if an argument file is given, open it
			} else {
				open_text_editor(NULL);							// If not, open an untitled document
			}
		}

		history = (char **)realloc(history, sizeof(char *)*(history_size+1));	// allocate space for the new command
		history[history_size] = (char *)malloc(MAX_INPUT_LENGTH);
		strcpy(history[history_size], input_copy);								// copy the command into the allocated space
		history_size++;															// increment history size
	}

	for (int i = 0; i < MAX_WORD_NUMBER; i++) {		// free allocated space for words
		free(words[i]);
	}
	for (int i = 0; i < history_size; i++) {		// free allocated space for history
		free(history[i]);
	}
	free(history);									// free history

	return 0;
}

// Splits inputs into words with respect to the given delimeter char and returns the number of words
int split(char *input, char *words[MAX_WORD_NUMBER], char ch) {
	int words_index = 0;
	int input_index = 0;
	int open_quotation = 0;									// strings in quotation are considered as a single word
	while (input[input_index] != 0) {
		if (input[input_index] == ch && !open_quotation) {	// if the pointed char is ch, then replace it with 0 
			input[input_index] = 0;
			strcpy(words[words_index++], input);			// and copy the input upto the 0 that we put
			input += input_index+1;							// input points the next char after 0 that we put
			input_index = 0;								// index = 0
		} else if (input[input_index] == '\"') {			// Check quotation marks
			open_quotation = !open_quotation;
			input_index++;
		} else {
			input_index++;									// if it is not ch, then go to next char
		}
	}
	strcpy(words[words_index++], input);					// copy the last word splitted by ch
	return words_index;										// return the number of words
}

// Checks whether a command is in the history or not
int in_history(char *command, char **history, int size) {
	int max_iteration = 15;								// check max 15 commands
	while (max_iteration > 0 && size > 0) {
		if (strcmp(history[size-1], command) == 0) {	// check commands in the history, starting from that last command and going back
			return 1;
		}
		max_iteration--;
		size--;
	}
	return 0;		// the command is not in the history
}

// Creates a child process which executes /bin/ls command
void call_ls(void) {
	int value = fork();
	if (value < 0) {		// check if fork failed	
		fprintf(stderr, "Fork Failed\n");
		return;
	}

	pid_t pid = getpid();
	if (pid == parent_id) {	// parent process waits the child process to end
		wait(NULL);
	} else {				// child process executes the command
		execlp("/bin/ls", "ls", (char *)NULL);
	}
}

// The child process executes /bin/hostname while the parent process waits it
void call_hostname(void) {
	int value = fork();
	if (value < 0) {
		fprintf(stderr, "Fork Failed\n");
		return;
	}

	pid_t pid = getpid();
	if (pid == parent_id) {
		wait(NULL);
	} else {
		execlp("/bin/hostname", "hostname", (char *) NULL);
	}
}

// Executes "hostnamee -I" which prints ip address
void show_ip_address(void) {
	int value = fork();
	if (value < 0) {
		fprintf(stderr, "Fork Failed\n");
		return;
	}

	pid_t pid = getpid();
	if (pid == parent_id) {
		wait(NULL);
	} else {
		execlp("/bin/hostname", "hostname", "-I", (char *)NULL);
	}
}

// Opens a file and prints a single line for each enter press
void printfile(char * filename) {
	FILE *fp = fopen(filename, "r");
	char line[MAX_LINE_LENGTH];
	while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {		// read until the end of the file
		line[strlen(line)-1] = 0;							// remove line feed
		printf("%s", line);									// print line
		getchar();											// wait for an enter
	}
	fclose(fp);
}

// Copies the content of a source file to a destination file by calling cp command
void printfile_redirect(char *src_file, char *new_file) {
	int value = fork();										// which copies the content of srcfile into destfile
	if (value < 0) {										// If the file does not exist, cp command creates it
		fprintf(stderr, "Fork Failed\n");
		return;
	}

	pid_t pid = getpid();
	if (pid == parent_id) {
		wait(NULL);
	} else {
		execlp("/bin/cp", "cp", src_file, new_file, (char *)NULL);
	}
}

// Opens an editor
void open_text_editor(char *filename) {
	int value = fork();
	if (value < 0) {
		fprintf(stderr, "Fork Failed\n");
		return;
	}

	pid_t pid = getpid();
	if (pid == parent_id) {
		wait(NULL);
	} else {
		if (filename == NULL) {
			execlp(editor, editor, (char*)NULL);
		} else {		
			execlp(editor, editor, filename, (char*)NULL);
		}
	}
}


