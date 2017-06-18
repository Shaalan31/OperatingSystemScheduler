#include "semaphore.h"

void down(int sem)
{
  struct sembuf p_op;
  
  p_op.sem_num = 0;
  p_op.sem_op = -1;
  p_op.sem_flg = !IPC_NOWAIT;
  
  if(semop(sem, &p_op, 1) == -1)
  {	
  	perror("Error in p()");
  	exit(-1);
  }
}

void up(int sem)
{
  struct sembuf v_op;
  
  v_op.sem_num = 0;
  v_op.sem_op = 1;
  v_op.sem_flg = !IPC_NOWAIT;
  
  if(semop(sem, &v_op, 1) == -1)
  {	
  	perror("Error in v()");
  	exit(-1);
  }
}

