/**
 * Write a program that uses UNIX system calls to ''ping-pong'' a byte between two processes over a pair of pipes, one for each direction. 
 * The parent should send a byte to the child; 
 * the child should print "<pid>: received ping", where <pid> is its process ID, write the byte on the pipe to the parent, and exit; 
 * the parent should read the byte from the child, print "<pid>: received pong", and exit. 
 * Your solution should be in the file user/pingpong.c.
*/

#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 1) {
        fprintf(2, "ERROR: pingpong has no paramters!");
        exit(1);
    }
    int p2c[2], c2p[2];
    pipe(p2c);
    pipe(c2p);
    if (fork() == 0) {
        char buf;
        read(p2c[0], &buf, 1);
        close(p2c[0]);
        printf("%d: received ping\n", getpid());
        write(c2p[1], &buf, 1);
        close(c2p[1]);
    } else {
        write(p2c[1], "!", 1);
        close(p2c[1]);
        char buf;
        wait(0);
        read(c2p[0], &buf, 1);
        printf("%d: received pong\n", getpid());
        close(c2p[0]);
    } 
    exit(0);
}