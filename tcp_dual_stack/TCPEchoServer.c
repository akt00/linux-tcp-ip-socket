#include <stdio.h>
#include <unistd.h>
#include "../Practical.h"


int main(int argc, char **argv) {

    if(argc != 2) {
        DieWithUserMessage("Parameter(s)", "<Port/Service>");
    }

    char *service = argv[1];

    int servSock = SetupTCPServerSocket(service);
    if(servSock < 0) {
        DieWithUserMessage("SetupTCPServerSocket() failed", service);
    }

    for(;;) {

        int clntSock = AcceptTCPConnection(servSock);

        HandleTCPClient(clntSock);
        close(clntSock);
    }
    
    return 0;
}
