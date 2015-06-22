#include "server.h"

void error(const char *msg)
{
    wcerr << "Error message: " << msg << endl;
    exit(1);
}

Server::Server()
{   
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(sockfd,
             (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0) {
          error("ERROR on binding");
    }
    listen(sockfd,5);
    cout << "Start Server" << endl << flush;
    this->loop();
}

Server::~Server() {
    close(this->sockfd);
}

void Server::loop() {

    socklen_t clilen;
    clilen = sizeof(cli_addr);

    while (true) {
        newsockfd = accept(sockfd,
                     (struct sockaddr *) &cli_addr,
                     &clilen);
        if (newsockfd < 0) {
                  error("ERROR on accept");
        }

        processing();

        cout << "Led Diod - state: " << this->led_state
             << "; color: " << this->led_color
             << "; rate: " << this->led_rate << endl;
    }
}

void Server::response(string mess) {
    cli_status = write( newsockfd,mess.c_str(),mess.size() );
    if (cli_status < 0) {
        error("ERROR writing to socket");
    }
    close(newsockfd);
}

void Server::processing() {

    bzero(buffer,256);
    cli_status = read(newsockfd,buffer,255);
    if (cli_status < 0) {
        error("ERROR reading from socket");
    }

    if ( regex_match(buffer,  regex("(set-led-state)(.*)") ) ) {
        char *argument =  buffer + strlen("set-led-state ");
        response( this->set_led_state(argument) );
        return;
    }

    if ( regex_match(buffer, regex("(get-led-state)(.*)") ) ) {
        response(this->get_led_state());
        return;
    }

    if ( regex_match(buffer,  regex("(set-led-color)(.*)") ) ) {
        char *argument =  buffer + strlen("set-led-color ");
        response(this->set_led_color(argument));
        return;
    }

    if ( regex_match(buffer,  regex("(get-led-color)(.*)") ) ) {
        response(this->get_led_color());
        return;
    }

    if ( regex_match(buffer,  regex("(set-led-rate)(.*)") ) ) {
        char *argument =  buffer + strlen("set-led-rate ");
        response(this->set_led_rate(argument));
        return;
    }

    if ( regex_match(buffer, regex("(get-led-rate)(.*)") ) ) {
        response(this->get_led_rate());
        return;
    }

    response("unknown message");
}

string Server::set_led_state(string argument) {

    if ( regex_match(argument, regex("(on)(.*)") ) ) {
        this->led_state = true;
        return "OK";
    }

    if ( regex_match(argument, regex("(off)(.*)") ) ) {
        this->led_state = false;
        return "OK";
    }

    return "FAILED";
}

string Server::get_led_state() {

    if ( this->led_state ) {
        return "OK on";
    }
    else {
        return "OK off";
    }

    return "FAILED";
}

string Server::set_led_color(string argument) {

    if ( regex_match(argument, regex("(red)(.*)") ) ) {
        this->led_color = "red";
        return "OK";
    }

    if ( regex_match(argument, regex("(green)(.*)") ) ) {
        this->led_color = "green";
        return "OK";
    }

    if ( regex_match(argument, regex("(blue)(.*)") ) ) {
        this->led_color = "blue";
        return "OK";
    }

    return "FAILED";
}

string Server::get_led_color() {

    if ( this->led_color != "black" ) {
        return ("OK " + this->led_color);
    }

    return "FAILED";
}

string Server::set_led_rate(string argument) {

    if ( regex_match(argument, regex("(0)(.*)") ) ||
         regex_match(argument, regex("(1)(.*)") ) ||
         regex_match(argument, regex("(2)(.*)") ) ||
         regex_match(argument, regex("(3)(.*)") ) ||
         regex_match(argument, regex("(4)(.*)") ) ||
         regex_match(argument, regex("(5)(.*)") ) ) {
        this->led_rate = atoi( &argument.c_str()[0] );
        cout << led_rate << endl;
        return "OK";
    }

    return "FAILED";
}

string Server::get_led_rate() {

    if ( this->led_rate != -1 ) {
        string begin("OK ");
        string end( to_string(this->led_rate) );
        return (begin+end);
    }

    return "FAILED";
}
