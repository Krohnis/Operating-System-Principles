//Ricky Gutierrez
//CSC139, A5
//May 12th, 2018

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//To implement First-In-First-Out
void fcfs(int, int[]);	

//Main
int main(int argc, char *argv[])
{
	//Size of the array
	int i;
	//Information read from the read file
	int request_size, queue[100];
	
	if (argc < 3)
	{
		printf("Usage: proj5 FCFS <Requests>");
	}
	else
	{
		request_size = (argc - 2);
		for (i = 0; i < request_size; ++i)
		{
			queue[i] = atoi(argv[i+2]);
		}
	
		//No process in the queue, queue is empty
		if(request_size <= 0)
		{
			printf("Error: No Inputs\n");
			goto empty_queue;
		}

		//First-Come-First-Serve
		if (!strcmp(argv[1], "fcfs"))
		{
			printf("FCFS\n");
			fcfs(request_size, queue);
			//fifo(request, frame_size, queue);
			//fcfs(size, pid, burst_time, arrival_time);	
		}
	}
	/* end of run */
	empty_queue:
	return 0;
}

void fcfs(int request_size, int queue[])
{
	int i, disk_arm, total_distance;
	
	//First-Come-First-Serve
	disk_arm = 50;
	total_distance = 0;
	
	for (i = 0; i < request_size; ++i)
	{
		if (!(queue[i] < 0 || queue[i] > 100))
		{
			printf("Reading Track %d\n", queue[i]);
			total_distance += abs(disk_arm - queue[i]);
			disk_arm = queue[i];
			queue[i] = -1;
		}
	}
	printf("Total Distance: %d\n", total_distance);
}