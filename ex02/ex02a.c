#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <alchemy/timer.h>
#include <alchemy/task.h>
#include <math.h>

#define LOOP_PERIOD 1e8

RT_TASK loop_task;

// function to be executed by task
void demo(void *arg)
{
  RT_TASK *curtask;

  RT_TASK_INFO curtaskinfo;

  RTIME tstart, now;

  curtask = rt_task_self();

  rt_task_inquire(curtask,&curtaskinfo);

  printf("Task name : %s ", curtaskinfo.name);

  rt_task_set_periodic(NULL, TM_NOW, LOOP_PERIOD);

  tstart = rt_timer_read();

  int ctr = 0;

  while (1) {
    rt_printf("Loop count: %d, Loop time: %.5f ms\n", ctr, (rt_timer_read()-tstart)/1000000.0);

    ctr++;

    rt_task_wait_period(NULL);
  }
}


int main(int argc, char* argv[])
{
  char  str[20] ;

  mlockall(MCL_CURRENT | MCL_FUTURE);

  printf("start cylic tasks\n");
  sprintf(str,"cyclic_task");
  
  /* Create task
   * Arguments: &task,
   *            name,
   *            stack size (0=default),
   *            priority,
   *            mode (FPU, start suspended, ...)
   */  
  rt_task_create(&loop_task, str, 0, 99, 0);

  /*  Start task
   * Arguments: &task,
   *            task function,
   *            function argument
   */
  rt_task_start(&loop_task, &demo, 0);

  pause();

  return 0;
}
