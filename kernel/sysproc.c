#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  pagetable_t pagetable = myproc()->pagetable;
  uint64 fir_addr, mask_addr;
  int pg_size;
  uint64 mask = 0;
  if (argaddr(0, &fir_addr), fir_addr == -1) {
    return -1;
  } 
  if (argint(1, &pg_size), pg_size == -1) {
    return -1;
  }
  if (argaddr(2, &mask_addr), mask_addr == -1) {
    return -1;
  }
  // 设置可以一次访问的页数为32
  if (pg_size > 32) {
    return -1;
  }

  pte_t* pte = walk(pagetable, fir_addr, 0);

  for (int i = 0; i < pg_size; i++) {
    if ((pte[i] & PTE_A) && (pte[i] & PTE_V)) {
      mask |= (1 << i);
      pte[i] ^= PTE_A; // 确定过了就复位，重新标记回没访问过
    }
  }

  if (copyout(pagetable, mask_addr, (char*) &mask, sizeof(uint))) {
    return -1;
  }
  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
