#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

using namespace std;

char server_name[] = "127.0.0.1";
int port = 8000;
int sockfd, ser_status;
struct sockaddr_in serv_addr;
struct hostent *server;

void error(const char *msg) {

    wcerr << "Error message: " << msg << endl;
    exit(1);
}

void init() {

    server = gethostbyname(server_name);
    if (server == NULL) {
        error("ERROR, no such host");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(port);
}

void send_message(char buffer[256]) {

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
    }
    
    ser_status = write(sockfd,buffer,strlen(buffer));
    if (ser_status < 0) {
         error("ERROR writing to socket");
    }
    
    bzero(buffer,256);
    ser_status = read(sockfd,buffer,255);
    if (ser_status < 0) {
         error("ERROR reading from socket");
    }
    
    cout << "Response: " << buffer << endl;
    close(sockfd);
}

int main()
{
    init();
 
    char buffer[256];

    while (true) {
        cout << "Enter the command: ";  
        bzero(buffer,256);
        fgets(buffer,255,stdin);

        send_message(buffer);
    }

    return 0;
}
