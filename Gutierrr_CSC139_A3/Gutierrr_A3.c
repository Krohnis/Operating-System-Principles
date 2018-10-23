//Ricky Gutierrez
//CSC139, A3
//March 30th, 2018

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//To implement first come first serve scheduling
void fcfs(int, int[], int[], int[]);	
//To implement round robin scheduling
void rr(int, int[], int[], int[], int);
//To implement shortest runtime first
void srtf(int, int[], int[], int[]);
//To calculate turn around time
void turn_around(int, int [], int []);
//To calculate the CPU usage
void cpu_usage(int, int[], int);
//For duplicating arrays
void copy_array(int, int[], int[]);

int main(int argc, char *argv[])
{
	//Size of the array
	int size = 0;
	//Time quantum of the scheduling
	int quantum;
	//Information read from the read file
	int pid[20], arrival_time[20], burst_time[20];
	//Variable for the read file
	FILE *file;
	//Sets the file to read
	file = fopen(argv[1], "r");
	
	if ((argc < 3) || (argc > 4))
	{
		printf("Usage: proj2 input_file FCFS|RR|SRJF [quantum]\n");
	}
	else
	{
		if (file)
		{
			while (!feof(file))
			{
				fscanf(file, "%d %d %d", &pid[size], &arrival_time[size], &burst_time[size]);
				size += 1;
			}
		}
	
		//No process in the queue, queue is empty
		if(size == 0)
		{
			goto empty_queue;
		}

		printf("Schdeuling Algorithm: ");
		//First-Come-First-Serve
		if (!strcmp(argv[2], "FCFS"))
		{
			printf("FCFS\n");
			fcfs(size, pid, burst_time, arrival_time);	
		}
		else if (!strcmp(argv[2], "RR"))
		{
			if (argc == 4)
			{
				printf("RR\n");
				quantum = atoi(argv[3]);
				rr(size, pid, burst_time, arrival_time, quantum);
			}
			else
			{
				printf("...\n Invalid Entry\n");
			}
		}
		else if (!strcmp(argv[2], "SRTF"))
		{
			printf("SRTF\n");
			srtf(size, pid, burst_time, arrival_time);
		}
		else
		{
			printf("USAGE: FCFS, RR, SRTF\n");
		}
	}
	/* end of run */
	return 0;

	empty_queue: printf("\nProcess queue is empty. End of run.\n");
}

//SRTF: To implement shortest run-time first
void srtf(int size, int pid[], int burst_time[], int arrival_time[])
{
	int i, j, n, k, time, result;
	int temp_burst[size], waiting[size], response_time[size];
	int turnaround_time[size], wait_time[size];
	float float_result;
	bool done, processed[size];
	//Duplicates burst_time array
	copy_array(size, burst_time, temp_burst);
	
	printf("Total of %d tasks are read. Press 'enter' to start...", size);
	getchar();
	printf("==================================================================\n");
	
	//Initializes all processed to false
	for (i = 0; i < size; ++i)
	{
		processed[i] = false;
	}
	//Initializes the response time
	for (i = 0; i < size; ++i)
	{
		response_time[i] = -1;
	}
	//Initializes the wait time
	for (i = 0; i < size; ++i)
	{
		wait_time[i] = 0;
	}
	
	//Shortest Runtime First
	done = false;
	time = 0;
	while (!done)
	{
		n = 1000000;
		k = -1;
		for (i = 0; i < size; ++i)
		{
			if ((temp_burst[i] != 0) && (arrival_time[i] <= time))
			{
				if (temp_burst[i] < n)
				{
					n = temp_burst[i];
					k = i;
				}
			}
			else if ((temp_burst[i] == 0) && (processed[i] != true))
			{
				printf("<System Time\t%d> Process: %d\t...is finished;\n", time, pid[i]);
				processed[i] = true;
				turnaround_time[i] = time - arrival_time[i];
			}
		}
		if (k != -1)
		{
			printf("<System Time\t%d> Process: %d\t...is running;\n", time, pid[k]);
			temp_burst[k] -= 1;
			if (response_time[k] == -1)
			{
				response_time[k] = time - arrival_time[k];
			}
		}
		for (i = 0; i < size; ++i)
		{
			if ((temp_burst[i] != 0) && (arrival_time[i] <= time))
			{
				if (k != i)
				{
					wait_time[i] += 1;
				}
			}
		}
		done = true;
		for (i = 0; i < size; ++i)
		{
			if ( processed[i] != true)
			{
				done = false;
				time += 1;
				break;
			}
		}
	}
	printf("<System Time\t%d> ...All processes finished;\n", time);
	printf("==================================================================\n");
	
	//CPU Usage
	cpu_usage(size, burst_time, time);
	
	//Average Wait Time
	result = 0;
	for (i = 0; i < size; ++i)
	{
		result += wait_time[i];
	}
	float_result = 1.0 * ((float)result/(float)size);
	printf("Average Wait Time\t: %.2f\n", float_result); 
	
	//Average Response Time
	result = 0;
	for (i = 0; i < size; ++i)
	{
		result += response_time[i];
	}
	float_result = 1.0 * ((float)result/(float)size);
	printf("Average Response Time\t: %.2f\n", float_result); 
	
	//Average Turn Around Time
	result = 0;
	for (i = 0; i < size; ++i)
	{
		result += turnaround_time[i];
	}
	float_result = 1.0 * ((float)result/(float)size);
	printf("Average Turn Around Time: %.2f\n", float_result); 
}

//RR: To implement round robin scheduling
void rr(int size, int pid[], int burst_time[], int arrival_time[], int quantum)
{
	int i, j, n, remain, count, front, rear, time, result;
	int ready_queue[size], temp_burst[size], waiting[size], response_time[size];
	int turnaround[size], waittime[size];
	int flag = 0;
	float float_result;
	bool done, processed[size], waiting_queue[size];
	//Copies the real array into duplicate for modification
	copy_array(size, burst_time, temp_burst);
	
	printf("Total of %d tasks are read. Press 'enter' to start...", size);
	getchar();
	printf("==================================================================\n");
	
	//Determines if the index has been processed
	for (i = 0; i < size; ++i)
	{
		processed[i] = false;
	}
	for (i = 0; i < size; ++i)
	{
		response_time[i] = -1;
	}
	//Initializes the ready queue
	for (i = 0; i < size; ++i)
	{
		ready_queue[i] = -1;
	}
	//Initializes the waiting queue to false
	for (i = 0; i < size; ++i)
	{
		waiting_queue[i] = false;
	}
	//Initializes something
	for (i = 0; i < size; ++i)
	{
		waittime[i] = 0;
	}
	
	//Round Robin
	done = false;
	n = 0;
	time = 0;
	front = 0;
	rear = -1;
	while (!done)
	{
		check_queue: for (i = 0; i < size; ++i)
		{
			if ((arrival_time[i] <= time) && (temp_burst != 0) && (processed[i] != true))
			{
				waiting_queue[i] = false;
				for (j = 0; j < size; ++j)
				{
					if (ready_queue[j] == i)
					{
						waiting_queue[i] = false;
						break;
					}
					else
					{
						waiting_queue[i] = true;
					}
				}
			}
			if (waiting_queue[i] == true)
			{
				rear = (rear + 1) % size;
				ready_queue[rear] = i;
			}
		}
		if (temp_burst[ready_queue[front]] == 0)
		{
			printf("<System Time\t%d> Process: %d\t...is finished;\n", time, pid[ready_queue[front]]);
			processed[ready_queue[front]] = true;
			turnaround[ready_queue[front]] = time - arrival_time[ready_queue[front]];
		}
		if ((n >= quantum) || (processed[ready_queue[front]] == true))
		{
			ready_queue[front] = -1;
			front = (front + 1) % size;
			if (n == quantum)
			{
				n = 0;
				goto check_queue;
			}
			n = 0;
		}
		if (ready_queue[front] != -1)
		{
			n += 1;
			temp_burst[ready_queue[front]] -= 1;
			if (response_time[ready_queue[front]] == -1)
			{
				response_time[ready_queue[front]] = time - arrival_time[ready_queue[front]];
			}
		}
		if (ready_queue[front] != -1)
		{
			printf("<System Time\t%d> Process: %d\t...is running;\n", time, pid[ready_queue[front]]);
		}
		done = true;
		for (i = 0; i < size; ++i)
		{
			if ((temp_burst[i] != 0) && (arrival_time[i] <= time))
			{
				if (ready_queue[front] != i)
				{
					waittime[i] += 1;
				}
			}
		}
		for (i = 0; i < size; ++i)
		{
			if (processed[i] != true)
			{
				done = false;
				time += 1;
				break;
			}
		}
	}
	printf("<System Time\t%d> ...All processes finished;\n", time);
	printf("==================================================================\n");
	
	//Resets processed to false
	for (i = 0; i < size; ++i)
	{
		processed[i] = false;
	}
	
	//CPU Usage
	cpu_usage(size, burst_time, time);
	
	//Resets the temporary array
	copy_array(size, burst_time, temp_burst);
	
	//Average Wait Time
	result = 0;
	for (i = 0; i < size; ++i)
	{
		result += waittime[i];
	}
	float_result = 1.0 * ((float)result/(float)size);
	printf("Average Waiting Time\t: %.2f\n", float_result); 
	
	//Average Response Time
	result = 0;
	for (i = 0; i < size; ++i)
	{
		result += response_time[i];
	}
	float_result = ((float)result/(float)size);
	printf("Average Response Time\t: %.2f\n", float_result);
	
	//Average Turn Around Time
	result = 0;
	for (i = 0; i < size; ++i)
	{
		result += turnaround[i];
	}
	float_result = 1.0 * ((float)result/(float)size);
	printf("Average Turn Around Time: %.2f\n", float_result); 
}

//FCFS : To implement first come first serve scheduling
void fcfs(int size, int pid[], int burst_time[], int arrival_time[])
{
	int i, j, k, n, time, result;
	int temp_burst[size], first_response[size];
	int wait_time[size], turnaround_time[size];
	float float_result;
	bool done, processed[size];
	//Copies the real arrays into duplicates for modification
	copy_array(size, burst_time, temp_burst);
	
	printf("Total of %d tasks are read. Press 'enter' to start...", size);
	getchar();
	printf("==================================================================\n");
	
	//Determines if the index has been processed
	for (i = 0; i < size; ++i)
	{
		processed[i] = false;
	}
	//Initializes all of the first responses to -1
	for (i = 0; i < size; ++i)
	{
		first_response[i] = -1;
	}
	//Initializes you get the idea
	for (i = 0; i < size; ++i)
	{
		wait_time[i] = 0;
	}
	
	//First Come First Serve
	done = false;
	time = 0;
	while (!done)
	{
		n = 1000000;
		k = -1;
		for (i = 0; i < size; ++i)
		{
			if ((temp_burst[i]) != 0 && (arrival_time[i] <= time))
			{
				if ((arrival_time[i] < n))
				{
					n = arrival_time[i];
					k = i;
				}
			}
			else if ((temp_burst[i] == 0) && (processed[i] != true))
			{
				printf("<System Time\t%d> Process: %d\t...is finished;\n", time, pid[i]);
				processed[i] = true;
				turnaround_time[i] = time - arrival_time[i];
			}
		}
		if (k != -1)
		{
			printf("<System Time\t%d> Process: %d\t...is running;\n", time, pid[k]);
			temp_burst[k] -= 1;
			if (first_response[k] == -1)
			{
				first_response[k] = time - arrival_time[k];
			}
		}
		for (i = 0; i < size; ++i)
		{
			if ((temp_burst[i] != 0) && (arrival_time[i] <= time))
			{
				if (k != i)
				{
					wait_time[i] += 1;
				}
			}
		}
		done = true;
		for (i = 0; i < size; ++i)
		{
			if (processed[i] != true)
			{
				done = false;
				time += 1;
				break;
			}
		}
	}
	printf("<System Time\t%d> ...All processes finished;\n", time);
	printf("==================================================================\n");
	
	//Determines if the index has been processed
	for (i = 0; i < size; ++i)
	{
		processed[i] = false;
	}
	
	//CPU Utilization
	cpu_usage(size, burst_time, time);
	
	//Average Waiting Time
	result = 0;
	for (i = 0; i < size; ++i)	
	{
		result += wait_time[i];
	}
	float_result =  ((float)result/(float)size);
	printf("Average Waiting Time\t: %.2f\n", float_result);
	
	//Average Response Time
	result = 0;
	for (i = 0; i < size; ++i)
	{
		result += first_response[i];
	}
	float_result = ((float)result/(float)size);
	printf("Average Response Time\t: %.2f\n", float_result);

	//Turnaround Time
	turn_around(size, burst_time, wait_time);
}

/* turn_around : to calculate turn around time for each process */
void turn_around(int size, int burst_time[], int waiting[])
{
	int i;
	int turn_around_time[size];
	int result = 0;
	float float_result = 0.0;
	
	for(i = 0; i < size; ++i)
	{
		//Turn around time = burst time + waiting time
		turn_around_time[i] = burst_time[i] + waiting[i];
		result += turn_around_time[i];
	}
	float_result = ((float)result/(float)size);
	printf("Average Turn Around Time: %.2f\n", float_result);
}

void cpu_usage(int size, int burst_time[], int time)
{
	float float_result = 0.0;
	int result = 0;
	int i;
	for (i = 0; i < size; ++i)
	{
		result += burst_time[i];
	}
	float_result = 100.0 * ((float)result/(float)time);
	printf("Average CPU Usage\t: %%%.2f\n", float_result);
}

//Copies the content of one array into the other
void copy_array(int size, int A[], int B[])
{
	int i;
	for (i = 0; i < size; ++i)
	{
		B[i] = A[i];
	}
}