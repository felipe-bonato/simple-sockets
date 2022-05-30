#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "logging.h"

typedef int socket_t; // Socket File Descriptor
typedef uint32_t ipaddr_t; // IP Address
typedef uint16_t port_t; // Port

typedef struct socks_addr_t
{
	ipaddr_t ip;
	port_t port;
} socks_addr_t;

struct sockaddr_in __to_sockaddr_in(socks_addr_t sock_addr)
{
	sock_addr.ip = sock_addr.ip == 0 ? INADDR_ANY : sock_addr.ip;
	sock_addr.port = sock_addr.port == 0 ? 8080 : sock_addr.port;

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(sock_addr.ip);
	addr.sin_port = htons(sock_addr.port);
	return addr;
}

/**
 * @brief Initialize a socket
 *
 * @return Socket file descriptor 
 */
socket_t socks_init()
{
	socket_t sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		logerr("Socks: Could not create socket");
		exit(1); }
	logdebug("Socks: Starting...");
	return sock;
}

/**
 * @brief bind a socket to an addr and port 
 * 
 * @param sock Socket file descriptor
 * @param ip_addr Address to bind to; 0 to deafault (ANY)
 * @param port Port to bind to; 0 to default (8080)
 */

void socks_bind(socket_t sock, socks_addr_t sock_addr)
{
	struct sockaddr_in addr = __to_sockaddr_in(sock_addr);

	bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
	logdebug("Socks: Bound");
}

void socks_listen(socket_t sock, size_t allowed_connections)
{
	int listenStatus = listen(sock, allowed_connections);
	if(listenStatus == -1){
		logerr("Socks: Could not listen");
		exit(1); }
	loginfo("Socks: Listening...");
}

socket_t socks_accept(socket_t sock, socks_addr_t sock_addr)
{
	struct sockaddr_in addr = __to_sockaddr_in(sock_addr);
	socklen_t addrlen = sizeof(addr);
	
	sock = accept(sock, (struct sockaddr*)&addr, &addrlen);
	if(sock == -1){
		logerr("Socks: Could not accept connection");
		exit(1); }
	loginfo("Socks: Connection accepted");
	return sock;
}

void socks_connect(socket_t sock, socks_addr_t sock_addr)
{
	struct sockaddr_in addr = __to_sockaddr_in(sock_addr);

	if(connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) == -1){
		logerr("Socks: Could not connect");
		exit(1); }
	loginfo("Socks: Connected");
}

/**
 * @brief Connect to a socket
 * 
 * @param sock Socket file descriptor
 * @param buff Buffer to write to socket
 * @param len Length of buffer
 * 
 * @return Number of bytes written to socket; -1 on error
 */
int64_t socks_write(socket_t sock, const void* buff, size_t len)
{
	return write(sock, buff, len);
}

int64_t socks_read(socket_t sock, void* buff, size_t len)
{
	return read(sock, buff, len);
}

void socks_close(socket_t sock)
{
	close(sock);
}
