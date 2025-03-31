#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("This program will print its page table when executed.\n");
  printf("The page table information is printed by the kernel.\n");
  
  // Không cần làm gì đặc biệt ở đây vì exec() sẽ tự động in bảng trang
  // khi thấy tên chương trình là "printpgtbl"
  
  exit(0);
}
