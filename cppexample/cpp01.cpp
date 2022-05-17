#include <iostream>
#include <cstdlib>
#include <alchemy/task.h>

using namespace std;

void hallofunc(void *arg) {
    cout << "Hallo, world!!!" << endl;
}


int main()
    {
	//mlockall(MCL_CURRENT|MCL_FUTURE);

	RT_TASK task;

	rt_task_create(&task, "Hallo", 0, 50, 0);
	
	rt_task_start(&task, &hallofunc, NULL);

    }
