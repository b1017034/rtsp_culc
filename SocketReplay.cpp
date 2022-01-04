//
// Created by root on 2021/07/27.
//
#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "include/RTSP.h"

#define BUFFER_SIZE ( 1024 * 16  )

static int serverSock;
static int readRTSPServerSock;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int createClientSoket(char *host, int port);

void signalHandler( int signal ){
  shutdown(serverSock, 2);
  fprintf(stderr,"EXIT SIGNAL:%d\n",signal);
  exit(EXIT_FAILURE);
}

void *readToWrite(int file[]){
  char buffer[BUFFER_SIZE];

  ssize_t read_size;
  int in_file = file[0];
  int out_file = file[1];
  bool readRTSP = file[2];
  RTSP rtsp(true);

  while(1){
    read_size = read(in_file, buffer, sizeof(buffer));
    if (read_size == 0){
      break; //EOF
    }
    //close
    if(read_size < 0){
      break;
    }

    pthread_mutex_lock( &mutex );
    if(readRTSP) {
      std::vector<unsigned char> vectorVBuffer(buffer, buffer + read_size);
      rtsp.decode(vectorVBuffer, read_size);
//      if (surplus > 0) {
//        printf("surplus!!\n");
//        tmpBuffer.insert(tmpBuffer.end(), vectorVBuffer.begin(), vectorVBuffer.begin() + read_size);
//        surplus = rtsp.decode(tmpBuffer, read_size);
//      } else {
//        surplus = rtsp.decode(vectorVBuffer, read_size);
//      }
//
//      if (surplus > 0) {
//        tmpBuffer.reserve(surplus);
//        tmpBuffer = { buffer + read_size - surplus, buffer + read_size };
//        printf("tempBuffer: %i\n", tmpBuffer.size());
//      }
    }
//    write(1, buffer, (unsigned int) read_size);
//    write(rtsp_client, buffer, (unsigned int) read_size);
    pthread_mutex_unlock( &mutex );

    write(out_file, buffer, (unsigned int) read_size);
  }
  return (file);
}

int createServerSoket(int port){
  int soc;
  char name[MAXHOSTNAMELEN];
  struct sockaddr_in socin;
  struct hostent *servhost;

  bzero(&socin, sizeof(struct sockaddr_in));
  gethostname(name, MAXHOSTNAMELEN);
  servhost=gethostbyname(name);

  if(servhost==NULL){
    fprintf(stderr, "ERROR: gethostbyname\n");
    exit(EXIT_FAILURE);
  }

  socin.sin_family = servhost->h_addrtype;
  socin.sin_port = htons(port);
  if((soc=socket(AF_INET,SOCK_STREAM,0))<0){
    fprintf(stderr, "ERROR: create socket\n");
    exit(EXIT_FAILURE);
  }
  if(bind(soc,(struct sockaddr *)&socin,sizeof(socin))<0){
    close(soc);
    fprintf(stderr, "ERROR: socket bind fail\n");
    exit(EXIT_FAILURE);
  }

  listen(soc,SOMAXCONN);
  return (soc);
}

int createClientSoket(char *host, int port){
  int soc;
  struct hostent *servhost;
  struct sockaddr_in server;

  servhost = gethostbyname(host);
  if ( servhost == NULL ){
    fprintf(stderr, "ERROR:gethostbyname(%s)\n", host);
    exit(EXIT_FAILURE);
  }
  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  bcopy(servhost->h_addr, &server.sin_addr, servhost->h_length);
  server.sin_port = htons(port);
  if ( ( soc = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ){
    fprintf(stderr, "ERROR: create socket\n");
    exit(EXIT_FAILURE);
  }
  if ( connect(soc, (struct sockaddr *)&server, sizeof(server)) == -1 ){
    fprintf(stderr, "ERROR: connect socket\n");
    exit(EXIT_FAILURE);
  }

  return (soc);
}

void *socketRelay(int file[]){
  int connectedSocket = file[0];
  int clientSock = file[1];

  int files_in[3] = {connectedSocket, clientSock, 0};
  pthread_t   thread_id1, thread_id2;
  int         status;

  status = pthread_create(&thread_id1,NULL,(void *(*)(void *))readToWrite, files_in);
  if(status!=0){
    fprintf(stderr,"pthread_create : %s",strerror(status));
    exit(EXIT_FAILURE);
  }

  int files_out[3] = {clientSock, connectedSocket, 2};
  status = pthread_create(&thread_id2,NULL,(void *(*)(void *))readToWrite, files_out);
  if(status!=0){
    fprintf(stderr,"pthread_create : %s",strerror(status));
    exit(EXIT_FAILURE);
  }

  pthread_join(thread_id2,NULL);
  pthread_cancel(thread_id1);

  close(connectedSocket);
  close(clientSock);

  return file;
}

void start(char *host, int serverPort, int clientPort){
  int connectedSocket;
  socklen_t len;
  pthread_t   thread_id1;
  int         status;

  serverSock = createServerSoket(serverPort);

  while(1){
    struct sockaddr_in peer_sin;

    len = sizeof(peer_sin);
    connectedSocket = accept(serverSock, (struct sockaddr *)&peer_sin, &len);
    if ( connectedSocket == -1 ){
      fprintf(stderr, "ERROR:accept socket\n");
      exit(EXIT_FAILURE);
    }

    int clientSock = createClientSoket(host,clientPort);

    int files[2] = {connectedSocket,clientSock};
    status = pthread_create(&thread_id1,NULL,(void *(*)(void *))socketRelay, files);
    if(status != 0){
      fprintf(stderr,"pthread_create : %s",strerror(status));
      exit(EXIT_FAILURE);
    }


  };
}

int main(int argc, char * argv[]) {

  if (argc < 4) {
    printf("Invalid arguments");
    return EXIT_FAILURE;
  }
  char *host = argv[1];
  struct hostent *hn;
  hn = gethostbyname(host);
  if(hn == NULL){
    printf("Invalid hostname");
    return EXIT_FAILURE;
  }

  int i=2;
  for (; i<4; i++) {
    char *port = argv[i];
    while(*port) {
      if(*port >= '0' && *port <= '9') port++;
      else break;
    }
    if (*port) {
      printf("Invalid port number:%s", argv[i]);
      return EXIT_FAILURE;
    }
  }

  signal( SIGINT, signalHandler );
  signal( SIGKILL, signalHandler );
  signal( SIGTERM, signalHandler );

  start(host, atoi(argv[2]), atoi(argv[3]));

  return EXIT_SUCCESS;
}