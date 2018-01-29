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
    char* home_dir = getenv("HOME");
    if (home_dir == NULL) {
      perror("No home directory available.\n");
    } else {
      chdir(home_dir);
    }
  } else {
    if (chdir(dir) == -1) {
      printf("Directory not found.\n");
    }
  }
}

bool is_exit_cmd(char* cmd) {
  return strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0;
}

bool is_cd_cmd(char* cmd) {
  return strcmp(cmd, "cd") == 0;
}

bool is_bg_cmd(char* cmd) {
  return strcmp(cmd, "bg") == 0;
}

bool is_bglist_cmd(char* cmd) {
  return strcmp(cmd, "bglist") == 0;
}

int main() {
  char cwd[1024];
  char prompt[1024];
  getcwd(cwd, sizeof(cwd));
  build_prompt(prompt, cwd);

  struct bg_proc* head = NULL;
  int bg_proc_size = 0;
  int sys_bailout = 0;

  while (!sys_bailout) {
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
      build_prompt(strcpy(prompt, ""), getcwd(cwd, sizeof(cwd)));
    } else if (is_bg_cmd(reply)) {     // bg cmd
      char** tokens_subset = tokens;
      rm_bg_arg(tokens_subset, sizeof(tokens_subset));

      pid_t pid = fork();

      if (pid == -1) {
        printf("Fork failed.\n");
      } else if (pid == 0) {
        printf("\n");
        execvp(tokens_subset[0], tokens_subset);
        printf("Command failed.\n");
        _Exit(3);
      } else {
        char cmd[1024];
        int k = 0;

        // -1 to avoid un-shifted value from rm_bg_arg()
        for (k = 0; k < tokens_size - 1; k++) {
          if (k != 0) strcat(cmd, " ");
          strcat(cmd, tokens_subset[k]);
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

      if (pid == -1) {
        printf("Fork failed.\n");
      } else if (pid == 0) {
        execvp(tokens[0], tokens);
        printf("Command failed.\n");
        _Exit(3);
      } else {
        waitpid(pid, NULL, 0);
      }
    }

    if (bg_proc_size > 0) {
      pid_t ter = waitpid(0, NULL, WNOHANG);

      if (ter > 0) {
        if (head->pid == ter) {
          printf("%i: %s has terminated.\n", head->pid, head->cmd);
          head = head->next;
        } else {
          struct bg_proc* curr = head;
          struct bg_proc* prev = head;

          if (curr != NULL && curr->pid == ter) {
            printf("%i: %s has terminated.\n", curr->pid, curr->cmd);
            head = curr->next;
            free(curr);
          } else {
            while (curr != NULL && curr->pid != ter) {
              prev = curr;
              curr = curr->next;
            }
            printf("%i: %s has terminated.\n", curr->pid, curr->cmd);
            prev->next = curr->next;
            free(curr);
          }
        }
        bg_proc_size--;
      }
    }

    memset(tokens, 0, sizeof(tokens));
    free(reply);
  }

  printf("Farewell.\n");
}
