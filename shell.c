#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

void build_prompt(char* prompt, char* cwd) {
  strcat(prompt, "SSI: ");
  strcat(cwd, " > ");
  strcat(prompt, cwd);
}

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
  build_prompt(prompt, cwd);

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
        const char* dir = tokens[1];
        // Home environment
        if (dir == NULL || !strcmp(dir, "~")) {
          printf("Go to HOME env...");
          // TODO: Handle error for no HOME env
          chdir(getenv("HOME"));
        } else {
          printf("Changing directories to %s\n", dir);
          // TODO: Error handling for nonexistent directory
          chdir(dir);
          // TODO: Update SSI environment
        }
        build_prompt(strcpy(prompt, ""), getcwd(cwd, sizeof(cwd)));
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
