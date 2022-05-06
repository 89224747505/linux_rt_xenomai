//multitask 5 process

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <alchemy/timer.h>
#include <alchemy/task.h>
#include <math.h>

#define NTASK 5
#define LOOP_PERIOD 1e9

RT_TASK task[NTASK];

// function to be executed by task
void demo(void *arg)
{
  int num = *(int*)arg;

  RT_TASK *curtask;

  RT_TASK_INFO curtaskinfo;

  RTIME tstart, now;

  rt_printf("Hello World\n");

  curtask = rt_task_self();

  rt_task_inquire(curtask, &curtaskinfo);

  rt_task_set_periodic(NULL, TM_NOW, LOOP_PERIOD*num);

  tstart = rt_timer_read();

  int ctr = 0;

  while (1) {
    rt_printf("Task name: %s\n", curtaskinfo.name);

    rt_printf("Loop count: %d, Loop time: %.5f ms\n", ctr, (rt_timer_read()-tstart)/1000000.0);

    ctr++;

    rt_task_wait_period(NULL);
  }

}


int main(int argc, char* argv[])
{
  char  str[5][10];

  mlockall(MCL_CURRENT | MCL_FUTURE);

  rt_printf("start tasks\n");

  int i;

  for (i = 1; i<=5; i++)
     sprintf(str[i],"task%i", i);

  /* Create task
   * Arguments: &task,
   *            name,
   *            stack size (0=default),
   *            priority,
   *            mode (FPU, start suspended, ...)
   */  

  for (i = 1; i<=5; i++)
    rt_task_create(&task[i], str[i], 0, 50, 0);

  /*  Start task
   * Arguments: &task,
   *            task function,
   *            function argument
   */
  for (i = 1; i <=5; i++) 
    rt_task_start(&task[i], &demo, &i);

  pause();

  return 0;
}


