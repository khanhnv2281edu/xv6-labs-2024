#include "types.h"
#include "riscv.h"
#include "param.h"
#include "spinlock.h"
#include "defs.h"
#include "sysinfo.h"
#include "proc.h"

// Get current system info
// addr is a user virtual address, pointing to a struct sysinfo.
int
systeminfo(uint64 addr) {
  struct proc *p = myproc();
  struct sysinfo info;

  info.freemem = freemem();
  info.nproc = nproc();
  info.nfile = nfile();

  if(copyout(p->pagetable, addr, (char *)&info, sizeof(info)) < 0)
    return -1;
  return 0;
}