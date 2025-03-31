#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: pagetable address\n");
    exit(1);
  }
  
  // Chuyển đổi địa chỉ từ chuỗi sang số
  uint64 addr = 0;
  char *p = argv[1];
  
  // Bỏ qua tiền tố "0x" nếu có
  if(p[0] == '0' && p[1] == 'x')
    p += 2;
    
  // Chuyển đổi từng ký tự hex thành giá trị số
  while(*p){
    addr = addr * 16;
    if(*p >= '0' && *p <= '9')
      addr += *p - '0';
    else if(*p >= 'a' && *p <= 'f')
      addr += *p - 'a' + 10;
    else if(*p >= 'A' && *p <= 'F')
      addr += *p - 'A' + 10;
    else
      break;
    p++;
  }
  
  // In địa chỉ để kiểm tra
  printf("Printing page table at address: 0x%lx\n", addr);
  
  // Gọi system call để in bảng trang
  // Giả sử chúng ta có system call vmprint
  // Trong trường hợp này, chúng ta chỉ in địa chỉ và kết thúc
  
  exit(0);
}
