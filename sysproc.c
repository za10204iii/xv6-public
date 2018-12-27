#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk_la(int nbyte)
{
  int old_addr;
  int new_addr;
  int n;
  uint sz;
  // input offset amount, then change to int value
  n = nbyte;
  if (argint(0, &n) < 0)
    return -1;

  cprintf("offset: %p\n", n);

  old_addr = myproc()->sz;
  cprintf("proc old address: %p\n", old_addr);
  // check the sign of offset, if negative, free memory, otherwise move the physical memory address
  if (n < 0) {
    new_addr = myproc()->sz - n;
    if ((sz = deallocuvm(myproc()->pgdir, old_addr, new_addr)) == 0) {
      cprintf("Deallocating failed...\n");
      return -1;
    } else {
      myproc()->sz -= n;
      cprintf("proc new address: %p\n", myproc()->sz);
    }
  } else {
    new_addr = myproc()->sz + n;
    if ((sz = allocuvm(myproc()->pgdir, old_addr, new_addr)) == 0) {
      cprintf("Allocating failed...\n");
      return -1;
    } else {
      myproc()->sz += n;
      cprintf("proc new address: %p\n", myproc()->sz);
    }
  }

  return new_addr;
}

// ===== Version 2.0 (add allocation/deallocation func)=====
int
sys_sbrk(void)
{
  int addr, new_addr;
  int n;
  uint sz;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  cprintf("proc old size: %p\n", addr);                               // testing code

  /***
    checking the sign of offset, free the memory when offset is negative; 
    otherwise, move the physical memory address
   ***/

  if (n < 0) {
    new_addr = myproc()->sz - n;
    if ((sz = deallocuvm(myproc()->pgdir, addr, new_addr)) == 0) {    // deallocate memory
      cprintf("Deallocating failed...\n");                            // testing code
      return -1;
    } else {
      myproc()->sz -= n;                                              // change the size of process, but not allocate physical memory
      cprintf("proc new size: %p\n", myproc()->sz);                   // testing code
    }
  } else {
    new_addr = myproc()->sz + n;
    if ((sz = allocuvm(myproc()->pgdir, addr, new_addr)) == 0) {      // allocate memory
      cprintf("Allocating failed...\n");                              // testing code
      return -1;
    } else {
      myproc()->sz += n;                                              // change the size of process, but not allocate physical memory
      cprintf("proc new size: %p\n", myproc()->sz);                   // testing code
    }
  }
  new_addr = myproc()->sz;

  return addr;
}

// // ===== Version 1.0 (add lazy page allocation func) =====
// int
// sys_sbrk(void)
// {
//   int addr, new_addr;
//   int n;
  
//   if (argint(0, &n) < 0)
//     return -1;
//   addr = myproc()->sz;
//   cprintf("proc old size: %p\n", addr);             // testing code

//   myproc()->sz += n;                                // change the size of process, but not allocate physical memory

//   new_addr = myproc()->sz;
//   cprintf("proc new size: %p\n", new_addr);         // testing code

//   return addr;
// }

// // ===== Origin version =====
// int
// sys_sbrk(void)
// {
//   int addr, new_addr;
//   int n;

//   if (argint(0, &n) < 0)
//     return -1;
//   addr = myproc()->sz;
//   cprintf("proc old size: %p\n", addr);             // testing code

//   if (growproc(n) < 0)
//     return -1;

//   new_addr = myproc()->sz;
//   cprintf("proc new size: %p\n", new_addr);         // testing code

//   return addr;
// }

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (myproc()->killed) {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_cps(void)
{
  return cps();
}

//sys_calloc
int
sys_calloc(void)
{
  return calloc();
}


int
sys_realloc(void)
{
  cprintf("[success] sysproc.c sys_realloc(void) test!\n");
  return realloc();
}
