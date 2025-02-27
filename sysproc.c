#include "date.h"
#include "defs.h"
#include "memlayout.h"
#include "mmu.h"
#include "param.h"
#include "proc.h"
#include "stat.h"
#include "types.h"
#include "x86.h"

int sys_fork(void) { return fork(); }

int sys_exit(void) {
  exit();
  return 0; // not reached
}

int sys_wait(void) { return wait(); }

int sys_kill(void) {
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int sys_getpid(void) { return myproc()->pid; }

int sys_sbrk(void) {
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int sys_sleep(void) {
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
int sys_uptime(void) {
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

void sys_shutdown(void) {
  outw(0xB004, 0x0 | 0x2000);
  outw(0x604, 0x0 | 0x2000);
}

int sys_incr(void) {
  int num;
  argint(0, &num); // retrieving first integer argument

  // cprintf("%d - Inside system call!",num);

  return num + 1;
}

int sys_add(void) {
  struct mystat *ct;
  argptr(0, (void *)&ct, sizeof(*ct));
  int s = 0;
  int i;
  for (i = 0; i < ct->sz; i++) {
    s += ct->nums[i];
    // cprintf("%d " , ct->nums[i]);
  }
  return s;
}

int sys_mult(void) {
  struct number *n;
  argptr(0, (void *)&n, sizeof(*n));
  return (n->num1 * n->num2);
}

int sys_div(void) {
  struct number *n;
  argptr(0, (void *)&n, sizeof(*n));
  return (1.0 * n->num1 / n->num2);
}

int sys_mod(void) {
  struct number *n;
  argptr(0, (void *)&n, sizeof(*n));
  return (n->num1 % n->num2);
}