#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

struct bg_pro {
  pid_t pid;
  char command[1024];
  struct bg_pro* next;
};

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

bool is_bg_cmd(char* cmd) {
  return (!strcmp(cmd, "bg"));
}

bool is_bglist_cmd(char* cmd) {
  return (!strcmp(cmd, "bglist"));
}

int main() {
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  char prompt[1024];
  build_prompt(prompt, cwd);

  // TODO: Handle getcwd failure
  // printf("Turtle failed while fetching current working directory.");
  // return 0;

  int sys_bailout = 0;
  while (!sys_bailout) {
    // readline() strips away the final \n
    char* reply = readline(prompt);

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

    if (is_exit_cmd(token)) {          // exit/quit cmd
      sys_bailout = 1;
    } else if (is_cd_cmd(token)) {     // cd cmd
        const char* dir = tokens[1];
        // Home environment
        if (dir == NULL || !strcmp(dir, "~")) {
          // TODO: Handle error for no HOME env
          chdir(getenv("HOME"));
        } else {
          // TODO: Error handling for nonexistent directory
          chdir(dir);
        }
        build_prompt(strcpy(prompt, ""), getcwd(cwd, sizeof(cwd)));
    } else if (is_bg_cmd(token)) {     // bg cmd
      printf("bg cmd");
    } else if (is_bglist_cmd(token)) { // bglist cmd
      printf("bglist cmd");
    } else {                           // general cmd
      printf("\nGeneral said: %s\n\n", token);

      int pid = fork();

      if (pid == 0) {
        // TODO: Handle execvp error
        execvp(tokens[0], tokens);
      } else {
        waitpid(pid, NULL, 0);
      }
      strcpy(token, "");
      memset(tokens, 0, sizeof(tokens));
    }

    free(reply);
  }

  printf("Carpe diem.\n");
}
