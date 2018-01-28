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
  return (!strcmp(cmd, "cd"));
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

    // TODO: Handle cmd error
    // Tokenize strings
    char* tokens[sizeof(reply)];
    char* token = malloc(strlen(reply) + 1);
    char* tok = NULL;
    int i = 0;

    strcpy(token, reply);
    tok = strtok(token, " ");

    while (tok != NULL) {
      tokens[i++] = tok;
      tok = strtok(NULL, " ");
    }

    if (is_exit_cmd(token)) {
      sys_bailout = 1;
    } else if (is_cd_cmd(token)) {
        // Home environment
        if (tokens[1] == NULL || !strcmp(tokens[1], "~")) {
          printf("Go to HOME env...");
        } else {
          printf("Changing directories to %s\n", tokens[1]);
          chdir(tokens[1]);
        }
        // TODO: Support the following:
        // cd ~
        // cd ..
        // cd
        // cd $HOME
        // cd absolute/relative path (cd ../../foo; cd ./foo/bar)
        printf("\ncd cmd: %s\n\n", token);
    } else {
      printf("\nGeneral said: %s\n\n", token);

      int pid = fork();

      if (pid == 0) {
        // TODO: Handle cmd error
        // Tokenize strings
        char* tokens_1[sizeof(reply)];
        char* token_1 = malloc(strlen(reply) + 1);
        char* tok_1 = NULL;
        int i = 0;

        strcpy(token_1, reply);
        tok_1 = strtok(token_1, " ");

        while (tok_1 != NULL) {
          tokens_1[i++] = tok_1;
          tok_1 = strtok(NULL, " ");
        }

        // TODO: Handle execvp error
        execvp(token_1, tokens_1);
      } else {
        waitpid(pid, NULL, 0);
      }
    }

    free(reply);
  }

  printf("Carpe diem.\n");
}
