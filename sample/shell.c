#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int get_cmd_args() {
  return 0;
}

int main() {
  // const char* prompt = "SSI: ";
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
	char prompt[1024];
  strcat(prompt, "SSI: ");
  strcat(cwd, " ");
  strcat(prompt, cwd);

  // printf("Turtle failed while fetching current working directory.");
  // return 0;

	int sys_bailout = 0;
	while (!sys_bailout) {

		char* reply = readline(prompt);
		/* Note that readline strips away the final \n */

		if (!strcmp(reply, "quit")) {
			sys_bailout = 1;
		} else {
			printf("\nYou said: %s\n\n", reply);
		}
	
		free(reply);
	}
	printf("Carpe diem.\n");
}
