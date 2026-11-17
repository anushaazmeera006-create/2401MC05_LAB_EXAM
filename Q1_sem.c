#include "types.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"

#define NSEMS 8

struct semaphore {
  int value;
  int used;
};

struct {
  struct spinlock lock;
  struct semaphore sem[NSEMS];
} semtable;

void
seminit(void)
{
  int i;

  initlock(&semtable.lock, "semtable");

  for(i = 0; i < NSEMS; i++){
    semtable.sem[i].value = 0;
    semtable.sem[i].used = 0;
  }
}

int
seminit_id(int id, int value)
{
  if(id < 0 || id >= NSEMS || value < 0)
    return -1;

  acquire(&semtable.lock);
  semtable.sem[id].value = value;
  semtable.sem[id].used = 1;
  release(&semtable.lock);

  return 0;
}

int
semwait(int id)
{
  if(id < 0 || id >= NSEMS)
    return -1;

  acquire(&semtable.lock);

  if(!semtable.sem[id].used){
    release(&semtable.lock);
    return -1;
  }

  while(semtable.sem[id].value == 0){
    sleep(&semtable.sem[id], &semtable.lock);
  }

  semtable.sem[id].value--;

  release(&semtable.lock);
  return 0;
}

int
sempost(int id)
{
  if(id < 0 || id >= NSEMS)
    return -1;

  acquire(&semtable.lock);

  if(!semtable.sem[id].used){
    release(&semtable.lock);
    return -1;
  }

  semtable.sem[id].value++;
  wakeup(&semtable.sem[id]);

  release(&semtable.lock);
  return 0;
}
