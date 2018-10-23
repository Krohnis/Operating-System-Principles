//Ricky Gutierrez
//CSC 139
//March 16th, 2018

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"
#define RAND_DIVISOR 100000000
#define TRUE 1

//Mutex Lock
pthread_mutex_t mutex;
//Semaphores
sem_t full, empty;
//Buffer
buffer_item buffer[BUFFER_SIZE];
//Buffer Counter
int counter;
//Producer Thread
void *producer(void *param);
//Consumer Thread
void *consumer(void *param);

//Add an item to the buffer
int insert_item(buffer_item item) 
{
   //When the buffer is not full add the item and increment the counter
   if(counter < BUFFER_SIZE) 
   {
      buffer[counter] = item;
      counter++;
      return 0;
   }
   else 
   { 
	//Error the buffer is full
      return -1;
   }
}

//Remove an item from the buffer
int remove_item(buffer_item *item) 
{
   //When the buffer is not empty remove the item and decrement the counter
   if(counter > 0) 
   {
      *item = buffer[(counter-1)];
      counter--;
      return 0;
   }
   else 
   {
	   //Error the buffer is empty
      return -1;
   }
}

//Producer Thread
void *producer(void *param) 
{
   buffer_item item;
   printf("Buffer Item = %d\n", item);
   while(TRUE) 
   {
	  //Sleep for a random peroid of time
      int rNum = rand() / RAND_DIVISOR;
      sleep(rNum);
      //Generate a random number
      item = rand();
      //Acquire the empty lock
      sem_wait(&empty);
      //Acquire the mutex lock
      pthread_mutex_lock(&mutex);
	  //If insert_item returns -1, buffer is full
      if(insert_item(item)) 
	  {
         fprintf(stderr, " Producer report error condition\n");
      }
	  //If insert_item returns 0, buffer can store item
      else 
	  {
         printf("producer produced %d\n", item);
      }
      //Release the Mutex Lock
      pthread_mutex_unlock(&mutex);
      //Signal Full
      sem_post(&full);
   }
}

//Consumer Thread
void *consumer(void *param) 
{
   buffer_item item;
   while(TRUE) 
   {
      //Sleep for a random period of time
      int rNum = rand() / RAND_DIVISOR;
      sleep(rNum);
      //Acquire the full lock
      sem_wait(&full);
      //Acquire the mutex lock
      pthread_mutex_lock(&mutex);
	  //If remove_item returns -1, buffer is empty
      if(remove_item(&item)) 
	  {
         fprintf(stderr, "Consumer report error condition\n");
      }
	  //If remove_item returns 0, buffer item may be consumed
      else 
	  {
         printf("consumer consumed %d\n", item);
      }
      //Release the Mutex Lock
      pthread_mutex_unlock(&mutex);
      //Signal Empty
      sem_post(&empty);
   }
}

//Main Loop
int main(int argc, char *argv[]) 
{
	//Thread ID
	pthread_t tid;
	//Set of thread attributes
	pthread_attr_t attr;
   //Loop Counter
   int i;
   //Verify the correct number of arguments were passed in
   if(argc != 4) 
   {
      fprintf(stderr, "USAGE:./main.out <INT> <INT> <INT>\n");
   }
   //Time in seconds for the main to sleep
   int sleepTime = atoi(argv[1]);
   //Number of producer threads
   int numProd = atoi(argv[2]);
   //Number of consumer threads
   int numCons = atoi(argv[3]);
   
   //Create the Mutex Lock
   pthread_mutex_init(&mutex, NULL);
   //Create the full semaphore and initialize to 0
   sem_init(&full, 0, 0);
   //Create the empty semaphore and initialize to BUFFER_SIZE
   sem_init(&empty, 0, BUFFER_SIZE);
   //Get the default attributes
   pthread_attr_init(&attr);
   //Initial Buffer
   counter = 0;

   //Create the producer threads
   for(i = 0; i < numProd; i++) 
   {
      //Create the thread
      pthread_create(&tid, &attr, producer, NULL);
    }

   //Create the consumer threads
   for(i = 0; i < numCons; i++) 
   {
      //Create the thread
      pthread_create(&tid, &attr, consumer, NULL);
   }

   //Sleep for specificed amount of time in miliseconds
   sleep(sleepTime);
   //Exit the program
   printf("Exit the program\n");
   exit(0);
}