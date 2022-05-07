#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/sem.h>

#define NTASKS 3
#define HIGH 52
#define MID 51
#define LOW 50
#define EXECTIME 1e8
#define SPINTIME 1e7

RT_TASK task[NTASKS];
RT_SEM mysync;

void demo(void *arg)
    {
	RTIME starttime, runtime;
	RT_TASK_INFO taskinfo;
	int num = *(int*)arg;
	printf("Task: %d\n", num);
	rt_sem_p(&mysync, TM_INFINITE);
	runtime = 0;
	while (runtime < EXECTIME) {
	  if (num==2 & runtime/1000000==50)
	    {
		printf("*********************************\n");
		rt_task_set_priority(&task[1], MID+10);
		rt_task_set_priority(&task[0], LOW+10);
	    }

	  rt_timer_spin(SPINTIME);
	  runtime = runtime + SPINTIME;
	  printf("Running Task: %d at ms: %d\n", num, runtime/1000000);
	}
	printf("End Task: %d\n", num);
    }

void startup()
    {
	int i;
	char str[20];
	rt_sem_create(&mysync, "MySemaphore", 0, S_FIFO);

	for (i=0; i < NTASKS; i++) {
	  printf("start task: %d\n", i);
	  sprintf(str, "task%d", i);
	  rt_task_create(&task[i], str, 0, 50, 0);
	  rt_task_start(&task[i], &demo, &i);
	}

	rt_task_set_priority(&task[0], LOW);
	rt_task_set_priority(&task[1], MID);
	rt_task_set_priority(&task[2], HIGH);
	printf("wake up all tasks\n");
	rt_sem_broadcast(&mysync);
    }

int main(int argc, char* argv[])
{
    startup();

    printf("\nCtrl-C to exit\n\n");

    pause();
}
