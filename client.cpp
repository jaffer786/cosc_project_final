#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sys/time.h>
using namespace std;
using std::string;


#define PORT "3490" // the port client will be connecting to
#define MAXDATASIZE 100 // max number of bytes we can get at once



// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
 if (sa->sa_family == AF_INET) {
 return &(((struct sockaddr_in*)sa)->sin_addr);
}
return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
 int sockfd, numbytes;
 char buf[MAXDATASIZE];
 struct addrinfo hints, *servinfo, *p;
 int rv;
 char s[INET6_ADDRSTRLEN];
 string buf1; 
 string buf3;

 if (argc != 2)
 {
  fprintf(stderr,"usage: client hostname\n");
  exit(1);
 }


 memset(&hints, 0, sizeof hints);
 hints.ai_family = AF_UNSPEC;
 hints.ai_socktype = SOCK_STREAM;
 if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0)
 {
  fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
  return 1;
 }



 // loop through all the results and connect to the first we can
 for(p = servinfo; p != NULL; p = p->ai_next)
 {
  if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1)
  {
   perror("client: socket");
   continue;
  }
  if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
  {
   close(sockfd);
   perror("client: connect");
   continue; 
  }
  break;
 }
 if (p == NULL)
 {
  fprintf(stderr, "client: failed to connect\n");
  return 2;
 }
 inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),s, sizeof s);
 printf("client: connecting to %s\n", s);
 freeaddrinfo(servinfo); // all done with this structure



 struct timeval tm;
 gettimeofday(&tm, NULL);
 long int start_time = tm.tv_sec * 1000 + tm.tv_usec / 1000;
 cout <<"start time ="<< start_time << endl;


char buff [2564097];
if ((numbytes = recv(sockfd, buff, 2564097, 0)) == -1) 
 {
  perror("recv"); exit(0);
 }

ofstream test_file1;
test_file1.open("/home/jaffer/cosc_project/client_doc.txt", ios::out);
test_file1<<buff;



struct timeval tm1;
            gettimeofday(&tm1, NULL);
            long int end_time = tm1.tv_sec * 1000 + tm1.tv_usec / 1000;
            cout<<"end time=" << end_time << endl;





long int total_time = end_time - start_time;
total_time=total_time/1000;

float data_rate=2564097/total_time;
cout<<"data rate ="<<data_rate<<"bytes/sec"<<endl;











close(sockfd);
return 0;
}



