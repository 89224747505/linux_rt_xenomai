//multitask 5 process

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <alchemy/timer.h>
#include <alchemy/task.h>
#include <math.h>

#define NTASK 5

RT_TASK task[NTASK];

// function to be executed by task
void demo(void *arg)
{
  RT_TASK *curtask;

  RT_TASK_INFO curtaskinfo;

  rt_printf("Hello World\n");

  curtask = rt_task_self();

  rt_task_inquire(curtask, &curtaskinfo);

  rt_printf("Task name: %s\n", curtaskinfo.name);

}


int main(int argc, char* argv[])
{
  char  str[5][10];

  mlockall(MCL_CURRENT | MCL_FUTURE);

  rt_printf("start tasks\n");

  int i;

  for (i = 0; i<=5; i++)
     sprintf(str[i],"task%i", i);

  /* Create task
   * Arguments: &task,
   *            name,
   *            stack size (0=default),
   *            priority,
   *            mode (FPU, start suspended, ...)
   */  

  for (i = 0; i<=5; i++)
    rt_task_create(&task[i], str[i], 0, 50 + i, 0);

  /*  Start task
   * Arguments: &task,
   *            task function,
   *            function argument
   */
  for (i = 0; i <=5; i++) 
    rt_task_start(&task[i], &demo, 0);

  return 0;
}
