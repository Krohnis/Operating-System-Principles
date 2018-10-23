//Ricky Gutierrez
//CSC139, A5
//May 12th, 2018

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//To implement First-In-First-Out
void look(int, int[]);	

//Main
int main(int argc, char *argv[])
{
	//Size of the array
	int i;
	//Information read from the read file
	int request_size, queue[100];
	
	if (argc < 3)
	{
		printf("Usage: proj5 LOOK <Requests>");
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
		if (!strcmp(argv[1], "look"))
		{
			printf("LOOK\n");
			look(request_size, queue);
			//fifo(request, frame_size, queue);
			//fcfs(size, pid, burst_time, arrival_time);	
		}
	}
	/* end of run */
	empty_queue:
	return 0;
}

void look(int request_size, int queue[])
{
	int lower_array[100], upper_array[100];
	int i, j, k, disk_arm, total_distance, arm_distance, shortest_distance, lower, upper;
	
	//Assign to lower and upper array
	disk_arm = 50;
	lower = 0;
	upper = 0;
	
	for (i = 0; i < request_size; ++i)
	{
		if (queue[i] <= disk_arm)
		{
			lower_array[lower] = queue[i];
			lower += 1;
		}
		else if (queue[i] > disk_arm)
		{
			upper_array[upper] = queue[i];
			upper += 1;
		}
	}
	
	//LOOK
	total_distance = 0;
	k = -1;
	
	for (i = 0; i < lower; ++i)
	{
		shortest_distance = 1000000;
		for (j = 0; j < lower; ++j)
		{
			if (!(lower_array[j] < 0 || lower_array[j] > 100) && (lower_array[j] != -1))
			{
				arm_distance = abs(disk_arm - lower_array[j]);
				if (arm_distance < shortest_distance)
				{
					shortest_distance = arm_distance;
					k = j;
				}
			}
		}
		if (!(lower_array[k] < 0 || lower_array[k] > 100))
		{
			printf("Reading Track %d\n", lower_array[k]);
			total_distance += abs(disk_arm - lower_array[k]);
			disk_arm = lower_array[k];
			lower_array[k] = -1;
		}
	}
	
	for (i = 0; i < upper; ++i)
	{
		shortest_distance = 1000000;
		for (j = 0; j < upper; ++j)
		{
			if (!(upper_array[j] < 0 || upper_array[j] > 100) && (upper_array[j] != -1))
			{
				arm_distance = abs(disk_arm - upper_array[j]);
				if (arm_distance < shortest_distance)
				{
					shortest_distance = arm_distance;
					k = j;
				}
			}
		}
		if (!(upper_array[k] < 0 || upper_array[k] > 100))
		{
			printf("Reading Track %d\n", upper_array[k]);
			total_distance += abs(disk_arm - upper_array[k]);
			disk_arm = upper_array[k];
			upper_array[k] = -1;
		}
	}
	
	printf("Total Distance: %d\n", total_distance);
	
	//LOOK
	/*
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
	*/
}