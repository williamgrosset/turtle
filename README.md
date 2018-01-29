# turtle
:turtle: A simple shell interpreter.

![alt text](https://github.com/williamgrosset/turtle/blob/master/example.gif "Shell")

## Overview
This projct was an assignment for the [Operating Systems](https://github.com/williamgrosset/turtle/blob/master/csc360_a1.pdf) class at the University of Victoria. The simple shell supports the basic functionality of a Linux terminal shell (see below) and uses the following kernel system calls: `fork()`, `execvp()`, and `waitpid()`.

### Commands
The simple shell supports basic execution, changing directories, and background processes. To create a background process, run `bg <cmd> <opts>`. To view running background processes, run `bglist`. Neither `bg`, `bglist`, or `cd` commands are case-sensitive.

### Usage
#### Compile
```
make setup
```

#### Run shell
```
make run
```

#### Clean build
```
make clean
```
