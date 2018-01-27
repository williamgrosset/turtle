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

bool is_exit_cmd(char* cmd) {
  return (!strcmp(cmd, "quit") || !strcmp(cmd, "exit"));
}

bool is_cd_cmd(char* cmd) {
  return false;
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
    // TODO: Build prompt with updated cwd on every input (?)

    // readline strips away the final \n
    char* reply = readline(prompt);

    if (is_exit_cmd(reply)) {
      sys_bailout = 1;
    } else if (is_cd_cmd(reply)) {
      printf("\ncd cmd: %s\n\n", reply);
    } else {
      printf("\nGeneral said: %s\n\n", reply);

      int pid = fork();

      if (pid == 0) {
        // TODO: Handle cmd error
        // Tokenize strings
        char *tokens[sizeof(reply)];
        char *tok = NULL;
        int i = 0;

        tok = strtok(reply, " ");

        while (tok != NULL) {
          tokens[i++] = tok;
          tok = strtok(NULL, " ");
        }

        // TODO: Don't modify the reply variable
        printf("%s\n", reply);

        execvp(reply, tokens);
      } else {
        waitpid(pid, NULL, 0);
      }
    }

    free(reply);
  }

  printf("Carpe diem.\n");
}
