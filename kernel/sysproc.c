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
  if(n < 0)
    n = 0;
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
sys_pgpte(void)
{
  uint64 va;
  struct proc *p;  

  p = myproc();
  argaddr(0, &va);
  pte_t *pte = pgpte(p->pagetable, va);
  if(pte != 0) {
      return (uint64) *pte;
  }
  return 0;
}
#endif

#ifdef LAB_PGTBL
int
sys_kpgtbl(void)
{
  struct proc *p;  

  p = myproc();
  vmprint(p->pagetable);
  return 0;
}
#endif

#ifdef LAB_PGTBL
uint64
sys_pgaccess(void)
{
  // Lab pgtbl: your code here.
  uint64 va;        // starting virtual address
  int num_pages;    // number of pages to check
  uint64 user_addr; // user address to store bitmap

  // Parse arguments
  argaddr(0, &va);
  argint(1, &num_pages);
  argaddr(2, &user_addr);

  // Set a reasonable limit on the number of pages to check
  if(num_pages <= 0 || num_pages > 32)
    return -1;

  struct proc *p = myproc();
  
  // Create a temporary buffer to store the bitmap
  char bitmap = 0;
  
  // Check each page for access
  for(int i = 0; i < num_pages; i++) {
    uint64 page_va = va + i * PGSIZE;
    
    // Check if the page is within process memory
    if(page_va >= p->sz)
      continue;
      
    // Find the PTE for this page
    pte_t *pte = walk(p->pagetable, page_va, 0);
    if(pte == 0 || (*pte & PTE_V) == 0)
      continue;
      
    // Check if the page has been accessed
    if(*pte & PTE_A) {
      // Set the corresponding bit in the bitmap
      bitmap |= (1 << i);
      
      // Clear the access bit
      *pte &= ~PTE_A;
    }
  }
  
  // Copy the bitmap to user space
  if(copyout(p->pagetable, user_addr, (char*)&bitmap, sizeof(bitmap)) < 0)
    return -1;
    
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
