H#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <pipe_networking.h>

int client_handshake(int *to_client) {

  //Step 3: Create 'private' FIFO
  char private_pipe[64];
  sprintf(private_pipe, "%d", getpid());
  mkfifo(private_pipe, 0644);
  printf("[client] private FIFO %s\n", private_pipe);

  
  //Step 4: Connects to WKP, sends name of 'private' fifo
  int *to_client = open("wkp", O_WRONLY); //writing
  printf("[client] connected to WKP");

  write(*to_client, private_pipe, strlen(private_pipe));
  printf("[client] sent 'private' fifo\n");


  //Step 5: Waits for message from server
  int fd = open(private_pipe, O_RDONLY); //reading
  printf("[client] waiting for server response...\n");

  
  //Step 8: Client receive server message, remove 'private' fifo
  char new_message[MESSAGE_BUFFER_SIZE];

  read(fd, new_message, MESSAGE_BUFFER_SIZE);

  printf("[client] message received: %s\n", new_message);

  remove(fd);

  
  //Step 9:
  write(*to_client, "received\n", 10);
  printf("[client] message sent to server\nConnection established\n");
  
  
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

  //Step 1: Server creates WKP
  mkfifo("wkp", 0644);
  printf("[server] WKP created\n");

  
  //Step 2: Connects and waits for connection
  *from_client  = open("wkp", O_RDONLY);

  
  //Step 6: Server receives message from WKP, removes WKP
  char priv_fifo_name[MESSAGE_BUFFER_SIZE];
  read(*from_client, priv_fifo_name, O_RDONLY);
  printf("[server] client message received\n");

  remove("wkp");

  
  //Step 7: Server connects to 'private' fifo, sends acknowledgement
  int fd = open(priv_fifo_name, O_RDONLY);
  write(fd, "received\n", 10);
  printf("[server] message sent\n");

  //Step 9:
  char new_message[MESSAGE_BUFFER_SIZE];
  read(fd, new_message, MESSAGE_BUFFER_SIZE];
  printf("[server] message received");
  
  return fd;
}

