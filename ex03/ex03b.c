#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/sem.h>

#define ITER 5

static RT_TASK t1;
static RT_TASK t2;

RT_SEM sem1, sem2;

int global = 0;


void taskOne(void *arg)
{
  while (1)
    {
      rt_sem_p(&sem1, TM_INFINITE);
      printf("I am taskOne and global = %d...\n", ++global);
    }
}

void taskTwo(void *arg)
{
  while (1)
    {
      rt_sem_p(&sem2, TM_INFINITE);
      printf("I am taskTwo and global = %d...\n", --global);
    }
}


int main(int argc, char* argv[])
{
  rt_sem_create(&sem1, "MySemaphore1", 0, S_FIFO);
  rt_sem_create(&sem2, "MySemaphore2", 0, S_FIFO);

  rt_task_create(&t1, "task1", 0, 1, 0);
  rt_task_create(&t2, "task2", 0, 2, 0);

  rt_task_start(&t1, &taskOne, 0);
  rt_task_start(&t2, &taskTwo, 0);

  while(1)
    {
	rt_sem_v(&sem1);
	rt_sem_v(&sem2);
	rt_timer_spin(2e9);
    }

  pause();

  return 0;
}
