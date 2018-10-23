//Ricky Gutierrez
//CSC139, A5
//May 12th, 2018

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//To implement First-In-First-Out
void sstf(int, int[]);	

//Main
int main(int argc, char *argv[])
{
	//Size of the array
	int i;
	//Information read from the read file
	int request_size, queue[100];
	
	if (argc < 3)
	{
		printf("Usage: proj5 SSTF <Requests>");
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
		if (!strcmp(argv[1], "sstf"))
		{
			printf("SSTF\n");
			sstf(request_size, queue);
			//fifo(request, frame_size, queue);
			//fcfs(size, pid, burst_time, arrival_time);	
		}
	}
	/* end of run */
	empty_queue:
	return 0;
}

void sstf(int request_size, int queue[])
{
	int i, j, k, disk_arm, total_distance, arm_distance, shortest_distance;
	
	//Shortest-Scan-Time-First
	disk_arm = 50;
	total_distance = 0;
	k = -1;
	
	for (i = 0; i < request_size; ++i)
	{
		shortest_distance = 1000000;
		for (j = 0; j < request_size; ++j)
		{
			if (!(queue[j] < 0 || queue[j] > 100) && (queue[j] != -1))
			{
				arm_distance = abs(disk_arm - queue[j]);
				if (arm_distance < shortest_distance)
				{
					shortest_distance = arm_distance;
					k = j;
				}
			}
		}
		if (!(queue[k] < 0 || queue[k] > 100))
		{
			printf("Reading Track %d\n", queue[k]);
			total_distance += abs(disk_arm - queue[k]);
			disk_arm = queue[k];
			queue[k] = -1;
		}
	}
	printf("Total Distance: %d\n", total_distance);
}