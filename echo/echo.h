#ifndef _ECHO_H_
#define _ECHO_H_
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#define USE_DEBUG

// RFC 862
class EchoServer
{
public:
    EchoServer() {};
    ~EchoServer() {};
    
    void start();
	
	static void onConnection( struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* addr, int socklen, void * userdata);
	static void onWriteMessage(struct bufferevent * bevent, void* data);
	static void onReadMessage(struct bufferevent * bevent, void* data);
	static void onClose(struct bufferevent * bevent, short events,  void* data);
	
};
#endif
