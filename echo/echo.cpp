#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include "echo.h"
#include <iostream>
#include <string.h>
#include <errno.h>

void EchoServer::start()
{
    struct event_base *base;
    struct evconnlistener * listener;
    struct sockaddr_in addr;
    
    base = event_base_new();
    
    if(base == NULL)
    {
        std::cout<<"Could not initialize libevent"<<std::endl;
        return;
    }
    
    memset(&addr, 0 ,sizeof(sockaddr_in));
    addr.sin_port = htons(8080);
    addr.sin_family = AF_INET;
    listener = evconnlistener_new_bind(base, EchoServer::onConnection, base, 
        LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1, (sockaddr*) &addr, sizeof(addr));
    
    event_base_dispatch(base);  
    
    evconnlistener_free(listener);
    event_base_free(base);
}


void EchoServer::onConnection( struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* addr, int socklen, void * userdata)
{
    struct event_base * base = (struct event_base* ) userdata;
    struct bufferevent * bevent;
    
    sockaddr_in * sin =(sockaddr_in*) addr;
    std::cout<<"new connection from : "<<sin->sin_port<<std::endl;
    bevent = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bevent) 
	{
        std::cout<< "Error constructing bufferevent!";
        event_base_loopbreak(base);
        return;
    }
    
	bufferevent_setcb(bevent, EchoServer::onReadMessage, EchoServer::onWriteMessage, EchoServer::onClose, NULL );
	
	bufferevent_enable(bevent, EV_WRITE | EV_READ);
	//char MESSAGE[] = "Hello, World!\n";
	//bufferevent_write(bevent, MESSAGE, sizeof(MESSAGE));
}

void EchoServer::onWriteMessage(struct bufferevent * bevent, void* data)
{
	struct evbuffer *output = bufferevent_get_output(bevent);
	if (evbuffer_get_length(output) == 0) {
		printf("flushed answer\n");
		//bufferevent_free(bevent);
	}
}

void EchoServer::onReadMessage(struct bufferevent * bevent, void* data)
{
    //struct evbuffer * input = bufferevent_get_input(bevent);
	char buf[1024] = {0};
	
	bufferevent_read(bevent, buf, sizeof(buf));
	
	std::cout<<"received data: "<<buf<<std::endl;
}

void EchoServer::onClose(struct bufferevent * bevent, short events, void* data)
{
    if(events & BEV_EVENT_EOF)
	{
	    std::cout<<"Connection closed.\n";
	}
	else if( events & BEV_EVENT_ERROR)
	{
	    std::cout<<"Got an error on the connection: "<<errno<<std::endl;
	}
	bufferevent_free(bevent);
}

