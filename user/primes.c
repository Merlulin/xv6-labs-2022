/**
 * Write a concurrent version of prime sieve using pipes.
 * Your goal is to use pipe and fork to set up the pipeline. 
 * The first process feeds the numbers 2 through 35 into the pipeline. 
 * For each prime number, you will arrange to create one process that reads from its left neighbor over a pipe and writes to its right neighbor over another pipe. 
 * Since xv6 has limited number of file descriptors and processes, the first process can stop at 35.
 * 
 * pseudocode：
 * p = get a number from left neighbor
 * print p
 * loop:
 *      n = get a number from left neighbor
 *      if (p does not divide n)
 *          send n to right neighbor
*/
#include "kernel/types.h"
#include "user/user.h"

void solve(int *pleft) {
   int p;
   close(pleft[1]);
   read(pleft[0], &p, sizeof(p)); 
   if (p == -1) {
        exit(0);
   } 
   printf("prime %d\n", p);
   int pright[2];
   pipe(pright);
   if (fork() == 0) {
        solve(pright);
   } else {
        close(pright[0]);
        int prime = p;
        while(read(pleft[0], &p, sizeof(p)) != 0) {
            if (p % prime != 0) {
                write(pright[1], &p, sizeof(p));
            }
        }
        close(pright[1]);
        close(pleft[0]);
        wait(0);
   }
   exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 1) {
        fprintf(2, "ERROR: primes has no paramters!\n");
        exit(1);
    }
    int p1[2];
    pipe(p1);
    if (fork() == 0) {
        solve(p1);
        exit(0);
    } else { 
        close(p1[0]);
        int i;
        for (i = 2; i <= 35; i ++) {
            write(p1[1], &i, sizeof(i));
        }
        i = -1;
        write(p1[1], &i, sizeof(i));
        close(p1[1]);
        wait(0);
    }
    exit(0);
}

