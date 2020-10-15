#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "soc.h"
#define  BUFL   100

/*      We need the lower bound	then we return the number:
		
	set	upperLimit = 5
		lowerLimit = 2
	
        I figured it would be easier to figure out the lower number of rounds
        and then we can calculate the upper number of rounds
        
        we have minimum wins for best (result) of x
        *We might not need x. The game ends when someone reaches (result)       
	**To get x: x = 2*result-1
        if result is 2 then x = 2*2-1 = 3
        
        we are playing best 2 of 3
 
 We can use these limits to set the max and min amount of servers for the game.
 We need to keep track of wins and whoever 
*/
int randomNumber(int upperLimit, int lowerLimit){
	int result = 0;
	time_t t;
	
	srand((unsigned) time(&t));
	while(result<lowerLimit)
		result = rand()%upperLimit+1;
	
	return result;
}

int main (int argc, char *argv[]) {
        
        int err;
        int sSocket;
        int cSocket;
        struct sockaddr_in sAddr;
        struct sockaddr_in cAddr;
        int cSocLen;
        char Buf[BUFL];
        
        /*Determines the randomly decided number of rounds n and m.
                if minRounds is larger than maxRounds exit(1)*/
        int     maxRounds = 5,  //m
                minRounds = 2,  //n
                determinedMin;
        
        if(maxRounds>minrounds)
               determinedMin = randomNumber(maxRounds,minRounds);
        else{
                printf("Need a higher maxRounds\n");
                exit(1);
        }
        //NOW HAVE MAX SERVERS. (determinedMin * 2 -1
        
        sSocket = socket( AF_INET, SOCK_STREAM, 0); // AF_INET
        if (sSocket == -1) {
                perror ("socServer:  socket creation failed");
                exit (2);
        }
        memset (&sAddr, 0, sizeof (struct sockaddr_in));
        sAddr.sin_family = AF_INET;
        sAddr.sin_port = htons (SERVERPORT);
        sAddr.sin_addr.s_addr = htonl (INADDR_ANY);
        err = bind(sSocket, (struct sockaddr*)&sAddr, sizeof (struct sockaddr_in));
        if (err == -1) {
                perror ("socServer:  bind address to socket failed");
                exit (3);
        }
        
        /*The max number of connections are given by best n out of m. 
        So the max queue length is m. Or 2 * determinedMin - 1*/
        err = listen(sSocket, 2 * determinedMin - 1);
        if (err == -1) {
          perror ("socServer:  listen failed");
          exit (4);
        }
        
        /*This is where we start to loop. There is a minimum of determinedMin iterations to
                2 * determinedMin -1 iterations.
                
                I haven't changed anything else in this loop. I might add
                -fork
                -We need a system to keep track of wins for user and server. Return who won with
                       ssize_t recv(int socket, void *rcvBuffer, size_t bufferLength, int flags)
                       and then increment a counter for who won.
                       -userWins
                       -serverWins
                -need to delete children when they are done.
                */
        int userWins    = 0;
            serverWins  = 0;
        while((userWins < determinedMin) && (serverWins < determinedMin)){
        
                pid_t processID = fork();
                if(processID < 0){
                        perror ("socServer:   fork failed");
                        exit(5);
                }
                /*else if(processID == 0){//if this is the child                //this is from the book Donahoo p.118-119. I need a break
                        close(sSocket); //close the parent
                        //execl
                        exit(0)         //exit the child                        
                }
                
                printf("with child process: %d\n", processID);*/
                
                
                //start example from class
                cSocket = accept(sSocket, (struct sockaddr *)&cAddr, &cSocLen);

                if (cSocket == -1) {
                        perror ("socServer:  accept failed");
                        exit (6);
                }
                //  Read from client socket.
                err = recv(cSocket, Buf, BUFL, 0);
                if (err == -1) {
                        perror ("socServer:  read failed");
                        exit (7);
                }
                printf ("socServer:  msg from client:  %s\n", Buf);
                exit (8);
                //end example from class
                
                serverWins++;//to make sure it doesn't loop!
        }
}
