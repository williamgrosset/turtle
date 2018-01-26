#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int get_cmd_args() {
  return 0;
}

bool is_exit_cmd(char *cmd) {
  return (!strcmp(cmd, "quit") || !strcmp(cmd, "exit"));
}

bool is_general_cmd(char *cmd) {
  return true;
}

int main() {
  // const char* prompt = "SSI: ";
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
	char prompt[1024];
  strcat(prompt, "SSI: ");
  strcat(cwd, " > ");
  strcat(prompt, cwd);

  // TODO: Handle getcwd failure
  // printf("Turtle failed while fetching current working directory.");
  // return 0;

	int sys_bailout = 0;
	while (!sys_bailout) {

		// readline strips away the final \n
		char* reply = readline(prompt);

		if (is_exit_cmd(reply)) {
			sys_bailout = 1;
		} else if (is_general_cmd(reply)) {
			printf("\nGeneral cmd: %s\n\n", reply);
    } else {
			printf("\nYou said: %s\n\n", reply);
		}
	
		free(reply);
	}
	printf("Carpe diem.\n");
}
