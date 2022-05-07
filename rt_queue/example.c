#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/queue.h>
#include <math.h>

#define QUEUE_SIZE 255
#define period 1e9

RT_TASK tMaster, tSlave;
RT_QUEUE queue;

// function Master
void taskMaster(void *arg)
{
  char message[40];

  rt_task_set_periodic(NULL, TM_NOW, 1e9);

  while (1) {
    rt_queue_read(&queue, message, sizeof(message), TM_INFINITE);

    RT_TASK_INFO curtaskinfo;

    rt_task_inquire(NULL,&curtaskinfo);

    printf("Task name : %s, TICK - %s\n\n", curtaskinfo.name, message);

    rt_task_wait_period(NULL);
  }
 return;
}

// function Slave
void taskSlave(void *arg)
{
  rt_task_set_periodic(NULL, TM_NOW, 1e9);

  char ticks[40];

  int tick = 0;

  while (1) {

    ++tick;

    if (tick%2 == 0)
      sprintf(ticks, "Even number-%d", tick);
    else
      sprintf(ticks, "Odd number-%d", tick);

    rt_queue_write(&queue, ticks, sizeof(ticks), Q_NORMAL);

    RT_TASK_INFO curtaskinfo;

    rt_task_inquire(NULL,&curtaskinfo);

    printf("Task name : %s, SEND TICK\n", curtaskinfo.name);

    rt_task_wait_period(NULL);
  }
 return;
}


int main(int argc, char* argv[])
{
  char  str[2][10] ;

  printf("start task\n");
  sprintf(str[0],"master");
  sprintf(str[1],"slave");

  rt_queue_create(&queue, "Queue", QUEUE_SIZE, 40, Q_FIFO);

  rt_task_create(&tSlave, str[1], 0, 1, 0);
  rt_task_start(&tSlave, &taskSlave, 0);

  rt_task_create(&tMaster, str[0], 0, 1, 0);
  rt_task_start(&tMaster, &taskMaster, 0);

  pause();
}
