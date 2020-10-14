#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "soc.h"
#include <netinet/in.h>
#include <arpa/inet.h>	//inet_addr
int main (int argc, char *argv[]) {
        int err;
        int cSocket;
        struct sockaddr_in sAddr;
        struct sockaddr_in cAddr;
        int cSocLen;
        memset (&cAddr, 0, sizeof (struct sockaddr_in));
        cAddr.sin_family = AF_INET;
        cAddr.sin_port = htons (SERVERPORT);
        cAddr.sin_addr.s_addr = inet_addr(SERVERIP);
        cSocket = socket( AF_INET, SOCK_STREAM, 0); // AF_INET
        if (cSocket == -1) {
                perror ("socClient:  socket creation failed");
                exit (1);
        }
        err = connect(cSocket, (struct sockaddr *)&cAddr, sizeof(struct sockaddr_in));
        if (err == -1) {
                perror ("socClient:  connect failed");
                exit (2);
        }
        //  Send message to server.
        err = send(cSocket, "Connection made!\n", 17, 0);
        printf ("socClient:  number of bytes sent to server:  %d\n", err);
        exit (0);
}
