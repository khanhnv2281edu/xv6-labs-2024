#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/riscv.h"

// Khai báo system call vmprint_addr
int vmprint_addr(uint64);

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: page table <address>\n");
    exit(1);
  }
  
  if(strcmp(argv[1], "table") == 0){
    if(argc == 3){
      // Lệnh "page table" được gọi với địa chỉ trang
      // Chuyển đổi địa chỉ từ chuỗi sang số
      uint64 addr = 0;
      char *p = argv[2];
      
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
      
      // Gọi system call để in bảng trang tại địa chỉ cụ thể
      vmprint_addr(addr);
    } else {
      fprintf(2, "Usage: page table <address>\n");
      exit(1);
    }
  } else {
    fprintf(2, "Unknown command: page %s\n", argv[1]);
    exit(1);
  }
  
  exit(0);
}
