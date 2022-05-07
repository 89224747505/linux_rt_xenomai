//With arguments to demo function

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <alchemy/timer.h>
#include <alchemy/task.h>
#include <alchemy/sem.h>
#include <math.h>

static RT_TASK task[5];
static RT_SEM sem;



void demo(void *arg)
{
  RT_TASK *curtask;

  RT_TASK_INFO curtaskinfo;

  curtask = rt_task_self();

  rt_task_inquire(curtask, &curtaskinfo);

  rt_sem_p(&sem, TM_INFINITE);

  rt_printf("%s - priority <<%i>>\n", curtaskinfo.name, curtaskinfo.prio);

}


int main(int argc, char* argv[])
{
  char  str[20];

  int i;

  mlockall(MCL_CURRENT | MCL_FUTURE);

  rt_printf("start tasks\n");

  rt_sem_create(&sem, "MySemaphore", 0, S_FIFO);

  int priority[] = {1,20,40,60,80};

  for (i = 0; i<5; i++)
    {
      sprintf(str, "task-%d", i);

      rt_task_create(&task[i], str, 0, priority[i], 0);

      rt_task_start(&task[i], &demo, &i);
    }

  rt_sem_broadcast(&sem);

  pause();

  return 0;
}
