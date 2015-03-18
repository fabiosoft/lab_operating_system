// This program is an example of how to run a command such as
// cat nomefile.txt | grep a | wc -c
// using C and Unix.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int pid;
int pipe1[2];
int pipe2[2];


void exec1() {
    // input from stdin (already done)
    // output to pipe1
    dup2(pipe1[1], 1);
    // close fds
    close(pipe1[0]);
    close(pipe1[1]);
    // exec
    execlp("cat", "cat", "nomefile.txt", NULL);
    // exec didn't work, exit
    perror("bad exec cat");
    _exit(1);
}

void exec2() {
    // input from pipe1
    dup2(pipe1[0], 0);
    // output to pipe2
    dup2(pipe2[1], 1);
    // close fds
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);
    // exec
    execlp("grep", "grep", "a", NULL);
    // exec didn't work, exit
    perror("bad exec grep a");
    _exit(1);
}

void exec3() {
    // input from pipe2
    dup2(pipe2[0], 0);
    // output to stdout (already done)
    // close fds
    close(pipe2[0]);
    close(pipe2[1]);
    // exec
    execlp("wc", "wc", "-c", NULL);
    // exec didn't work, exit
    perror("bad exec wc -c");
    _exit(1);
}


int main(int argc, char const *argv[]){
    
    // create pipe1
    if (pipe(pipe1) == -1) {
        perror("bad pipe1");
        exit(1);
    }
    
    // fork (cat nomefile.txt)
    if ((pid = fork()) == -1) {
        perror("bad fork1");
        exit(1);
    } else if (pid == 0) {
        // stdin --> cat --> pipe1
        exec1();
    }
    // parent
    
    // create pipe2
    if (pipe(pipe2) == -1) {
        perror("bad pipe2");
        exit(1);
    }
    
    // fork (grep a)
    if ((pid = fork()) == -1) {
        perror("bad fork2");
        exit(1);
    } else if (pid == 0) {
        // pipe1 --> grep --> pipe2
        exec2();
    }
    // parent
    
    // close unused fds
    close(pipe1[0]);
    close(pipe1[1]);
    
    // fork (grep -c)
    if ((pid = fork()) == -1) {
        perror("bad fork3");
        exit(1);
    } else if (pid == 0) {
        // pipe2 --> grep --> stdout
        exec3();
    }
    // parent
    
    
}