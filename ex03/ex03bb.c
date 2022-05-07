#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/sem.h>

static RT_TASK t1, t2, t3, t4, t5, t6;

RT_SEM sem;

int global = 0;


void taskOne(void *arg)
{
  while (1)
    {
      rt_sem_p(&sem, TM_INFINITE);
      printf("I am taskOne and global = %d...\n", ++global);
      rt_sem_v(&sem);
      rt_task_sleep(1e9);
    }
}

void taskTwo(void *arg)
{
  while (1)
    {
      rt_sem_p(&sem, TM_INFINITE);
      printf("I am taskTwo and global = %d...\n", --global);
      rt_sem_v(&sem);
      rt_task_sleep(1e9);
    }
}


int main(int argc, char* argv[])
{
  rt_sem_create(&sem, "MySemaphore", 1, S_FIFO);

  rt_task_create(&t1, "task1", 0, 1, 0);
  rt_task_create(&t2, "task2", 0, 1, 0);
  rt_task_create(&t3, "task3", 0, 1, 0);
  rt_task_create(&t4, "task4", 0, 1, 0);
  rt_task_create(&t5, "task5", 0, 1, 0);
  rt_task_create(&t6, "task6", 0, 1, 0);


  rt_task_start(&t1, &taskOne, 0);
  rt_task_sleep(1e7);
  rt_task_start(&t2, &taskTwo, 0);
  rt_task_sleep(2e7);
  rt_task_start(&t3, &taskOne, 0);
  rt_task_sleep(3e7);
  rt_task_start(&t4, &taskTwo, 0);
  rt_task_sleep(4e7);
  rt_task_start(&t5, &taskOne, 0);
  rt_task_sleep(5e7);
  rt_task_start(&t6, &taskTwo, 0);

  pause();

  return 0;
}
