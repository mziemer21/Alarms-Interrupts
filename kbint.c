/*
 * FILENAME:    kbint.c
 * AUTHORS:     Michael Gilsdorf        Section 2
 *              Matthew Ziemer          Section 1
 * DATE:        October 30, 2013
 * DESCRIPTION: This program takes in a interval and runs for that specific time. While running a interrupt alarm will go off each time a seperate user specified interval
 *  passes. Additionally it takes in a string that will toggle the case when ctrl + c is typed.
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
#include <ctype.h>

/*Used to determine when to end the program */
volatile int numAlarm = 0;
/*Used to determine whether the string should be caps or lower case*/
volatile int caps = 0;

/*signal interrupt handlers*/
void handler();
void stringHandler();

int main (int argc, char *argv[])
{
	/*ints to hold time variables*/
        int almTime, totTime, totSets;
	/*our signal action structs*/
        struct sigaction sigActAlm, sigActIn;
	/*the string to be printed*/
	char *string;
	/*current case of the string*/
	int curCaps = caps;
        /*check for the correct number of arguments*/
        if (argc != 4)
        {
                fprintf(stderr, "usage: alarm interval (in seconds) total run time (in seconds) string\n");
                exit(1);
        }

	/*get interval time*/
        almTime = atoi(argv[1]);
	/*get total run time*/
        totTime = atoi(argv[2]);
	
        /*check that interval time is not greater then total running time
         check that interval time and total running time are posative ints*/
        if ((almTime <= 0) || (totTime <= 0)|| (almTime>totTime))
        {
		/* if incorrect arguments output error message and quit program*/
                fprintf(stderr, "error: Cannot operate with %i second interval, and %i second total run time.\n", almTime, totTime);
                exit(1);
        }

	/*assign method handler to be called when sigActAlm gets a signal*/
        sigActAlm.sa_handler = handler;
	/*tell sigActAlm to listen for a SIGALRM*/
        assert(sigaction(SIGALRM, &sigActAlm, NULL) == 0);
	/*assign method handler to be called when sigActIn gets a signal*/
	sigActIn.sa_handler = stringHandler;
	/*tell sigActIn to listen for a SIGINT*/
        assert(sigaction(SIGINT, &sigActIn, NULL) == 0);
	/*create and start an alarm timer*/
        alarm(almTime);
	/*count the total times we have reset the alarm and used to determine whe to end the program*/
        totSets = 1;

	/*allocate space to store the input string*/
	string = malloc(strlen(argv[3]));
	
	/*store the input string to memory so it can be modified*/
	strncpy(string, argv[3],strlen(argv[3])+1);

	/*while number of times we need to reset is greater than the number of times we have reset, keep running*/
        while((totTime/almTime) > numAlarm){
		/*wait for an interrupt*/
        	pause();	 
		
		/*reset alarm if the alarm was triggered*/
	        if(numAlarm == totSets){
                        alarm(almTime);
                        totSets++;
       		}
		/*if the cap value of the string has changed switch the cap value of the string*/
		if(curCaps != caps){
			curCaps = caps;
			int i;
			/*loop through the input string*/
			for(i=0; i <= strlen(string); i++){
				/*if cap is zero switch to lower case otherwise switch to uppercase*/
				if (caps ==0){
					*(string+(i)) = tolower(*(string+(i)));
				}else{
					*(string+(i)) = toupper(*(string+(i)));
				}
			}
			/*print the string*/
			printf("%s\n", string);
		} 
	}

        return 0;
}

/*called from sigActAlm and prints time and increments numAlarm*/
void handler()
{
	/*get current time*/
        time_t t;
        time(&t);
	/*print current time*/
        printf("current time is %s", ctime(&t));
	/*increment numAlarm, used to track total alarms signaled*/
        numAlarm++;
}

/*called from sigActIn and changes the value of caps to control the case of the input string*/
void stringHandler()
{
	/*increment caps*/
	caps++;
	/*mod caps by 2 to ensure value is only 1 or 0*/
	caps = caps%2;
}

