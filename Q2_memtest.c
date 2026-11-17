#include "types.h"
#include "stat.h"
#include "user.h"

int global_var = 12345;

int
main(void)
{
  int before, after, pid;

  printf(1, "Free pages at startup: %d\n", freepages());

  sbrk(10 * 4096);

  printf(1, "Free pages after sbrk: %d\n", freepages());

  printf(1, "Global virtual address: 0x%x\n",
         (uint)&global_var);

  printf(1, "Global physical address: 0x%x\n",
         va2pa((int)&global_var));

  printf(1, "Invalid address translation: %d\n",
         va2pa(0x7fffffff));

  before = freepages();

  printf(1, "Free pages before fork: %d\n", before);

  pid = fork();

  if(pid < 0){
    printf(1, "Fork failed\n");
    exit();
  }

  if(pid == 0){
    printf(1, "Child virtual address: 0x%x\n",
           (uint)&global_var);

    printf(1, "Child physical address: 0x%x\n",
           va2pa((int)&global_var));

    sleep(20);
    exit();
  }

  printf(1, "Parent virtual address: 0x%x\n",
         (uint)&global_var);

  printf(1, "Parent physical address: 0x%x\n",
         va2pa((int)&global_var));

  after = freepages();

  printf(1, "Free pages after fork: %d\n", after);

  wait();

  printf(1, "Free pages after child wait: %d\n",
         freepages());

  printf(1, "Free pages before fork: %d\n", before);

  exit();
}
