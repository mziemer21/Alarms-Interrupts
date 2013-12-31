/*
 * FILENAME:	alarm.c
 * AUTHORS:	Michael Gilsdorf	Section 2
 * 		Matthew Ziemer		Section 1
 * DATE:	October 30, 2013
 * DESCRIPTION:	This program takes in a interval and runs for that specific time. While running a interrupt alarm will go off each time a seperate user specified interval
 *  passes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>

/* used to determine when to end the program*/
volatile int numAlarm = 0;

void handler();

int main (int argc, char *argv[])
{
	/* ints to hold time variables*/
        int almTime, totTime, totSets;
	/* our signal action struct*/
	struct sigaction sigAct;

	/*check for the correct number of arguments*/
	if (argc != 3)
	{
		/* if incorrect output error message and quit program*/
		fprintf(stderr, "usage: alarm interval (in seconds) total run time (in seconds)\n");
		exit(1);
	}
        /* get interval time*/
        almTime = atoi(argv[1]);
	/* get total running time*/
	totTime = atoi(argv[2]);
		
	/* check that interval time is not greater then total running time
	 check that interval time and total running time are posative ints*/
	if ((almTime <= 0) || (totTime <= 0) || (almTime > totTime))
	{
		/* if incorrect arguments output error message and quit program*/
		fprintf(stderr, "error: Cannot operate with %i second interval, and %i second total run time.\n", almTime, totTime);
	        exit(1);
	}
	
	/* assign method handler to be called when sigAct gets a signal*/
	sigAct.sa_handler = handler;

	/* tell sigAct to listen for a SIGALRM*/
	assert(sigaction(SIGALRM, &sigAct, NULL) == 0);

	/* create and start a alarm timer*/
	alarm(almTime);

	/* count the total times we have reset the alarm
	 used to determine when to end the program*/
	totSets = 1;
	
	/* while number of times we need to reset is greater than number of times we have reset, keep running*/
	while((totTime/almTime) > numAlarm){

		/*reset alarm if it was triggered*/
		if(numAlarm == totSets){
			alarm(almTime);
			totSets++;
		}
	}
	
	return 0;
}

/* called from sigAct
 prints time and increments numAlarm*/
void handler()
{
	/* gets current time*/
        time_t t;
	time(&t);

	/* prints current time*/
 	printf("current time is %s", ctime(&t));

	/* increments numAlarm */
	/* used for tracking total running time*/
	numAlarm++;
}
