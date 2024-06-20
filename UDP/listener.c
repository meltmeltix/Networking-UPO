#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

char buffer[256] = "";

int main(int argc, char *argv[]) {
    int simpleSocket = 0;
    int simplePort = 0;
    int returnStatus = 0;
    struct sockaddr_in simpleServer;
    char *pin;

    if (2 != argc) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    simpleSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if (simpleSocket == -1) {
        fprintf(stderr, "Could not create a socket!\n");
        exit(1);

    } else {
        fprintf(stderr, "Socket created!\n");
    }

    /* retrieve the port number for listening */
    simplePort = atoi(argv[1]);

    /* setup the address structure */
    /* use INADDR_ANY to bind to all local addresses  */
    memset(&simpleServer, '\0', sizeof(simpleServer));
    simpleServer.sin_family = AF_INET;
    simpleServer.sin_addr.s_addr = htonl(INADDR_ANY);
    simpleServer.sin_port = htons(simplePort);

    /*  bind to the address and port with our socket  */
    returnStatus = bind(
        simpleSocket, 
        (struct sockaddr *)&simpleServer,
        sizeof(simpleServer)
    );

    if (returnStatus == 0) {
        fprintf(stderr, "Bind completed!\n");
    } else {
        fprintf(stderr, "Could not bind to address!\n");
        close(simpleSocket);
        exit(1);
    }

    while (1) {
        struct sockaddr_in clientName = {0};
        char whoareyou[50];
        int simpleChildSocket = 0;
        int clientNameLength = sizeof(clientName);

        /* wait here */

        /* handle the new connection request  */
        memset(buffer, 0, sizeof(buffer));

        returnStatus =
            recvfrom(simpleSocket, buffer, sizeof(buffer) - 1, 0,
                     (struct sockaddr *)&clientName, &clientNameLength);
        if (returnStatus > 0) {
            /* If the string is terminated by \n, remove it */
            pin = strrchr(buffer, '\n');
            if (pin != NULL) *pin = '\0';

            /* Print the IP address we are receiving the message from */
            inet_ntop(
                AF_INET, 
                &(clientName.sin_addr.s_addr), 
                whoareyou,
                sizeof(whoareyou)
            );
            printf("%s from %s\n", buffer, whoareyou);
        } else {
            fprintf(
                stderr, 
                "Return Status = %d Error %d (%s)\n", 
                returnStatus,
                errno, 
                strerror(errno)
            );
        }
    }

    close(simpleSocket);
    return 0;
}
