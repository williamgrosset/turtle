#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

struct bg_proc {
  pid_t pid;
  char cmd[1024];
  struct bg_proc* next;
};

void build_prompt(char* prompt, char* cwd) {
  strcat(prompt, "SSI: ");
  strcat(cwd, " > ");
  strcat(prompt, cwd);
}

void rm_bg_arg(char** args, int size) {
  int i = 0;
  for (i = 0; i < size; i++) {
    args[i] = args[i + 1];
  }
}

void change_dirs(char* dir) {
  // Home environment
  if (dir == NULL || !strcmp(dir, "~")) {
    // TODO: Handle error for no HOME env
    chdir(getenv("HOME"));
  } else {
    // TODO: Error handling for nonexistent directory
    chdir(dir);
  }
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
  int bg_proc_size = 0;
  struct bg_proc* head = NULL;

  while (!sys_bailout) {
    // readline() strips away the final \n
    char* reply = readline(prompt);

    // Tokenize strings
    char* tokens[sizeof(reply)];
    char* token = NULL;
    int tokens_size = 0;
    int i = 0;

    token = strtok(reply, " ");

    while (token != NULL) {
      tokens[i++] = token;
      token = strtok(NULL, " ");
      tokens_size++;
    }

    if (is_exit_cmd(reply)) {          // exit/quit cmd
      sys_bailout = 1;
    } else if (is_cd_cmd(reply)) {     // cd cmd
      char* dir = tokens[1];
      change_dirs(dir);
      // TODO: Handle getcwd failure
      build_prompt(strcpy(prompt, ""), getcwd(cwd, sizeof(cwd)));
    } else if (is_bg_cmd(reply)) {     // bg cmd
      char** tokens_cpy = tokens;
      rm_bg_arg(tokens_cpy, sizeof(tokens_cpy));
      printf("%lu\n", sizeof(tokens_cpy));

      pid_t pid = fork();

      if (pid == 0) {
        // TODO: Handle execvp error
        printf("\n");
        execvp(tokens_cpy[0], tokens_cpy);
      } else {
        char cmd[1024];
        int k = 0;

        // Note: -1 to avoid un-shifted value from rm_bg_arg()
        for (k = 0; k < tokens_size - 1; k++) {
          strcat(cmd, tokens_cpy[k]);
        }

        if (bg_proc_size == 0) {
          struct bg_proc* proc = NULL;
          proc = malloc(sizeof(struct bg_proc));
          proc->pid = pid;
          strcpy(proc->cmd, cmd);
          head = proc;
        } else {
          struct bg_proc* curr = head;
          while (curr->next != NULL) {
            curr = curr->next;
          }

          curr->next = malloc(sizeof(struct bg_proc));
          curr->next->pid = pid;
          strcpy(curr->next->cmd, cmd);
          curr->next->next = NULL;
        }

        bg_proc_size++;
        memset(cmd, 0, sizeof(cmd));
      }
    } else if (is_bglist_cmd(reply)) { // bglist cmd
      struct bg_proc* curr = head;

      while (curr != NULL) {
        printf("%i: %s\n", curr->pid, curr->cmd);
        curr = curr->next;
      }

      printf("Total background jobs: %i.\n", bg_proc_size);
    } else {                           // general cmd
      pid_t pid = fork();

      if (pid == 0) {
        // TODO: Handle execvp error
        execvp(tokens[0], tokens);
      } else {
        waitpid(pid, NULL, 0);
      }
    }

    // TODO: Check background process termination

    memset(tokens, 0, sizeof(tokens));
    free(reply);
  }

  printf("Carpe diem.\n");
}
