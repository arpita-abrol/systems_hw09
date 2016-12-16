#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int client_handshake(int *to_client) {

  int wkp = open("pipe_1", O_WRONLY);

  mkfifo("private",0644);
  int fd = open("private", O_WRONLY);

  write(wkp, "[client] connected\n", 20);
 
  return fd;
}

/*

Basic Handshake Procedure

    1) Server creates  FIFO (Well Known Pipe)
    2) Server waits for a connection
    3) Client creates a "private" FIFO
    4) Client connects to server and sends the private FIFO name
    5) Client waits for a message from the server
    6) Server receives client's message and removes the WKP
    7) Server connects to client FIFO, sending an initial acknowledgement
    8) Client receives server's message, removes its "private" FIFO
    9) Client sends a message, verifying the connection

 */

//server goes first
int server_handshake(int *from_client) {
  
  mkfifo("pipe_1", 0644);
  int fd = open("pipe_1", O_RDONLY);
  

  return fd;
}

int main() {

  return 0;
}
