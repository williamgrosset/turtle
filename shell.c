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

bool is_cd_cmd(char *cmd) {
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

    // readline strips away the final \n
    char* reply = readline(prompt);

    if (is_exit_cmd(reply)) {
      sys_bailout = 1;
    } else if (is_cd_cmd(reply)) {
      printf("\ncd cmd: %s\n\n", reply);
    } else {
      // TODO: Tokenize string for execvp(char* file, char* argv[])
      // 1) Seperate options from main cmd
      // TODO: Run general cmds
      // 1) Use fork() to call the child process and execvp()
      // 2) Use waitpid() in the parent until process is complete and resources are dealocated
      printf("\nGeneral said: %s\n\n", reply);
    }

    free(reply);
  }

  printf("Carpe diem.\n");
}
