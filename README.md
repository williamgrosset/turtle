# turtle
:turtle: A simple shell interpreter.

### Unpacking .tar.gz files
```bash
gunzip -c p1s.tar.gz | tar xopf -
```

### Assignment Requirements
+ Basic Execution (5 marks):
  + `'SSI: <getcwd()> > ' + args`
  + Use `fork()` and `execvp()` to execute arbitrary commands (including options)
+ Changing Directories (5 marks):
  + Create command `cd` (use `getcwd(()` and `chdir()`)
  + All edge cases of `cd`: `cd .., cd ./, cd , cd ~`
  + Get user's home directory via `getenv()`
  + Use system call `chdir()` to change current directory
+ Background Execution (5 marks):
  + Support arbitrary amount of background processes
  + Create command `bg`
    + e.g `bg cat foo.txt`
    + Execute in background and accept more commands from user
  + Create command `bglist` will display list of programs
    + Process ID
    + Program being executed
    + Execution arguments
    + `Total Background jobs: 2`
  + Background jobs must indicate to shell once terminated
  + See `WNOHANG` opton for `waitpid()` system call
+ Test on Linux (ssh)
