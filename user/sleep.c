/**
 * Implement the UNIX program sleep for xv6; 
 * your sleep should pause for a user-specified number of ticks. 
 * A tick is a notion of time defined by the xv6 kernel, namely the time between two interrupts from the timer chip. 
 * Your solution should be in the file user/sleep.c.
*/

#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(2, "ERROR: sleep time required!");
        exit(1);
    } 
    // 使用标准linux系统调用直接实现
    sleep(atoi(argv[1]));    
    exit(0);
}