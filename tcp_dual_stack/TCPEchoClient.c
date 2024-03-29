#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "../Practical.h"


int main(int argc, char *argv[]) {

    if(argc < 3 || argc > 4) {
        DieWithUserMessage("Parameter(s)", "<Server Address/Name><Echo Word> [<Server Port/Service>]");
    }

    char *server = argv[1];
    char *echoString = argv[2];
    char *service = (argc == 4) ? argv[3] : "echo";

    int sock = SetupTCPClientSocket(server, service);
    if(sock < 0) {
        DieWithUserMessage("SetupTCPClientSocket() failed", "unable to connect");
    }

    size_t echoStringLen = strlen(echoString);
    ssize_t numBytes = send(sock, echoString, echoStringLen, 0);
    if(numBytes < 0) {
        DieWithSystemMessage("send() failed");
    }
    else if(numBytes != echoStringLen) {
        DieWithUserMessage("send()", "sent unexpected number of bytes");
    }

    unsigned int totalBytesRcvd = 0;
    fputs("Received: ", stdout);
    while(totalBytesRcvd < echoStringLen) {
        char buffer[BUFSIZE];
        numBytes = recv(sock, buffer, BUFSIZE - 1, 0);
        if(numBytes < 0) {
            DieWithSystemMessage("recv() failed");
        }
        else if(numBytes == 0) {
            DieWithUserMessage("recv()", "connection closed prematurely");
        }
        totalBytesRcvd += numBytes;
        buffer[numBytes] = '\0';
        fputs(buffer, stdout);
    }
}
