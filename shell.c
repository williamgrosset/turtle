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

void tokenize_str(char* str) {
  for (char* p = strtok(str," "); p != NULL; p = strtok(NULL, " ")) {
    puts(p);
  }
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
      // TODO: Tokenize string for execvp(char* file, char* argv[]) (e.g execvp(args[0], args))
      printf("\nGeneral said: %s\n\n", reply);

      tokenize_str(reply);

      int pid = fork();
      if (pid == 0) {
        char* argv[3];
        argv[0] = "ls";
        argv[1] = "-la";
        argv[2] = NULL;

        // TODO: Handle cmd error
        execvp("ls", argv);
      } else {
        waitpid(pid, NULL, 0);
      }
    }

    // Should we free(rebuild(prompt))?
    free(reply);
  }

  printf("Carpe diem.\n");
}
