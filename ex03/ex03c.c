//With arguments to demo function

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <alchemy/timer.h>
#include <alchemy/task.h>
#include <alchemy/sem.h>
#include <math.h>

#define NTASK 5

static RT_TASK task[NTASK];

static RT_SEM sem;

int global = 0;

void demo(void *arg)
{
  int num = *(int*)arg;

  RT_TASK *curtask;

  RT_TASK_INFO curtaskinfo;

  curtask = rt_task_self();

  rt_task_inquire(curtask, &curtaskinfo);

  while (1)
    {
      (num % 2 == 0) ? --global : ++global;

      rt_sem_p(&sem, TM_INFINITE);

      rt_printf("%s number << %i >> global << %i >> \n", curtaskinfo.name, num, global);

      rt_sem_v(&sem);

      rt_task_sleep(3e9);
    }
}


int main(int argc, char* argv[])
{
  char  str[5][10];

  mlockall(MCL_CURRENT | MCL_FUTURE);

  rt_printf("start tasks\n");

  rt_sem_create(&sem, "MySemaphore", 0, S_PRIO);

  int i;

  for (i = 1; i<=5; i++)
     sprintf(str[i],"<< %i >>", i);

  for (i = 1; i<=5; i++)
    rt_task_create(&task[i], str[i], 0, 19*i, 0);

  for (i = 1; i<=5; i++)
    rt_task_start(&task[i], &demo, &i);

  pause();

  return 0;
}
