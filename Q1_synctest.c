#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  int pid;
  sem_init(0, 1);
  sem_init(1, 0);
  sem_init(2, 0);
  pid = fork();

  if(pid == 0){
    int i;
    for(i = 0; i < 5; i++){
      sem_wait(0);
      printf(1, "A");
      sem_post(1);
    }
    exit();
  }
  pid = fork();
  if(pid == 0){
    int i;
    for(i = 0; i < 5; i++){
      sem_wait(1);
      printf(1, "B");
      sem_post(2);
    }
    exit();
  }
  pid = fork();

  if(pid == 0){
    int i;
    for(i = 0; i < 5; i++){
      sem_wait(2);
      printf(1, "C");
      sem_post(0);
    }
    exit();
  }
  wait();
  wait();
  wait();

  printf(1, "\nDone\n");

  exit();
}
