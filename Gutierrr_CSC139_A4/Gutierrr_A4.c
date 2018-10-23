//Ricky Gutierrez
//CSC139, A4
//April 28th, 2018

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//To implement First-In-First-Out
void fifo(int, int, int[]);	
//To implement Optimal
void opt(int, int, int[]);
//To implement Least-Recently-Used
void lru(int, int, int[]);

//Main
int main(int argc, char *argv[])
{
	//Size of the array
	int i = 0;
	//Information read from the read file
	int page, frame_size, request, queue[100];
	//Information read from the read file
	//int pid[20], arrival_time[20], burst_time[20];
	//Variable for the read file
	FILE *file;
	//Sets the file to read
	file = fopen(argv[1], "r");
	
	if ((argc < 3) || (argc > 4))
	{
		printf("Usage: proj2 input_file FIFO|OPT|LRU\n");
	}
	else
	{
		if (file)
		{
			fscanf(file, "%d %d %d", &page, &frame_size, &request);
			while (!feof(file))
			{
				fscanf(file, "%d", &queue[i]);
				i += 1;
			}
		}
	
		//No process in the queue, queue is empty
		if(request == 0)
		{
			printf("Error: No Inputs\n");
			goto empty_queue;
		}

		//First-In-First-Out
		if (!strcmp(argv[2], "FIFO"))
		{
			printf("FIFO\n");
			fifo(request, frame_size, queue);
			//fcfs(size, pid, burst_time, arrival_time);	
		}
		//Optimal
		else if (!strcmp(argv[2], "OPT"))
		{
			printf("OPT\n");
			opt(request, frame_size, queue);
		}
		//Least-Recently-Used
		else if (!strcmp(argv[2], "LRU"))
		{
			printf("LRU\n");
			lru(request, frame_size, queue);
		}
		else
		{
			printf("USAGE: FIFO, OPT, LRU\n");
		}
	}
	/* end of run */
	empty_queue:
	return 0;
}

//First-In-First-Out
void fifo(int request, int frame_size, int queue[])
{
	//Variables for the method
	int frame[frame_size];
	int i, j, front, rear, index, page_faults;
	bool in_frame;
	
	//Intializes all Frame indexes to -1 to signify that it is empty
	for (i = 0; i < frame_size; ++i)
	{
		frame[i] = -1;
	}
	
	//Execution
	rear = 0;
	front = 0;
	page_faults = 0;
	for (i = 0; i < request; ++i)
	{
		in_frame = false;
		for (j = 0; j < frame_size; ++j)
		{
			if (frame[j] == queue[i])
			{
				in_frame = true;
				index = j;
				break;
			}
		}
		if (in_frame == false)
		{
			if (frame[rear] != -1)
			{
				printf("Page %d unloaded from Frame %d, ", frame[front], front);
				front = (front + 1) % frame_size;
			}
			printf("Page %d loaded into Frame %d\n", queue[i], rear);
			frame[rear] = queue[i];
			rear = (rear + 1) % frame_size;
			page_faults += 1;
		}
		else
		{
			printf("Page %d already in Frame %d\n", queue[i], index);
		}
	}
	printf("%d Page Faults\n", page_faults);
}

//Optimal
void opt(int request, int frame_size, int queue[])
{
	//Variables
	int frame[frame_size];
	int i, j, k, front, rear, index, page_faults, longest_run, time;
	bool in_frame;
	
	//Intializes all Frame indexes to -100
	for (i = 0; i < frame_size; ++i)
	{
		frame[i] = -1;
	}
	
	//Execution
	rear = 0;
	front = 0;
	page_faults = 0;
	for (i = 0; i < request; ++i)
	{
		in_frame = false;
		longest_run = 0;
		for (j = 0; j < frame_size; ++j)
		{
			if (frame[j] == queue[i])
			{
				in_frame = true;
				index = j;
				break;
			}
		}
		if (in_frame == false)
		{
			if (frame[rear] != -1)
			{
				for (j = 0; j < frame_size; ++j)
				{
					time = 0;
					for (k = i; k < request; ++k)
					{
						time += 1;
						//printf("%d && %d || FRAME %d && QUEUE %d\n", j, k, frame[j], queue[k]);
						if (frame[j] == queue[k] || (k == (request - 1)))
						{
							if (time > longest_run)
							{
								longest_run = time;
								index = j;
							}
							break;
						}
						if (time > longest_run)
						{
							
						}
					}
				}
				printf("Page %d unloaded from Frame %d, ", frame[index], index);
				rear = index;
			}
			printf("Page %d loaded into Frame %d\n", queue[i], rear);
			frame[rear] = queue[i];
			rear = (rear + 1) % frame_size;
			page_faults += 1;
		}
		else
		{
			printf("Page %d already in Frame %d\n", queue[i], index);
		}
	}
	printf("%d Page Faults\n", page_faults);
}

//Least-Recently-Used
void lru(int request, int frame_size, int queue[])
{
	//Variables
	int frame[frame_size], use_time[frame_size], last_used;
	int i, j, front, rear, page_faults, index;
	bool in_frame;
	
	//Intializes all Frames to -1
	for (i = 0; i < frame_size; ++i)
	{
		frame[i] = -1;
	}
	//Initialize Use Time to 0
	for (i = 0; i < frame_size; ++i)
	{
		use_time[i] = 0;
	}
	
	//Execution
	rear = 0;
	page_faults = 0;
	//last_used = 0;
	for (i = 0; i < request; ++i)
	{
		last_used = 0;
		in_frame = false;
		for (j = 0; j < frame_size; ++j)
		{
			if (frame[j] == queue[i])
			{
				in_frame = true;
				index = j;
				break;
			}
		}
		if (in_frame == false)
		{
			if (frame[rear] != -1)
			{
				for (j = 0; j < frame_size; ++j)
				{
					if (use_time[j] > last_used)
					{
						rear = j;
						last_used = use_time[j];
					}
				}
				printf("Page %d unloaded from Frame %d, ", frame[rear], rear);
				use_time[rear] = 0;
			}
			printf("Page %d loaded into Frame %d\n", queue[i], rear);
			frame[rear] = queue[i];
			rear = (rear + 1) % frame_size;
			page_faults += 1;
		}
		else
		{
			printf("Page %d already in Frame %d\n", queue[i], index);
			use_time[index] = 0;
		}
		for (j = 0; j < frame_size; ++j)
		{
			if (frame[j] == -1)
			{
				break;
			}
			use_time[j] += 1;
		}
	}
	printf("%d Page Faults\n", page_faults);
}