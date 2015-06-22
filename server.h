#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <regex>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

class Server
{
    const int port = 8000;
    int sockfd, newsockfd;

    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int cli_status;

    bool led_state = false;
    string led_color = "black";
    int  led_rate = -1;
public:
    Server();
    ~Server();

    void loop();
    void processing();
    void response(string mess);

    string set_led_state(string argument);
    string get_led_state();

    string set_led_color(string command);
    string get_led_color();

    string set_led_rate(string command);
    string get_led_rate();
};

#endif // SERVER_H
