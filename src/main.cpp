#include "../lib/socks.h"
#include "../lib/logging.h"

#include <stdlib.h>
#include <unistd.h>

const size_t numChildren = 10;

void server(socks_addr_t addr)
{
	socket_t sock = socks_init();
	socks_bind(sock, addr);
	socks_listen(sock, numChildren);

	for(size_t i = 0; i < 10; i++){
		sock = socks_accept(sock, addr);
		char buff[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
		int written = socks_write(sock, &buff, sizeof(buff));
		if(written == -1){
			logerr("Could not write to socket");
			exit(1); }
		loginfo("Sent message");
		printf("Message sent was: %s\n", buff);
	}

	socks_close(sock);
	loginfo("Closed socket");
}

void client(socks_addr_t addr)
{
	socket_t sock = socks_init();
	socks_connect(sock, addr);

	for(size_t i = 0; i < 10; i++){
		char buff[6];
		if(socks_read(sock, &buff, 6) == -1){
			logerr("Could not read from socket");
			exit(1); }
		loginfo("Received message");
		printf("Message recived was: %s\n", buff);
	}

	socks_close(sock);
}

int main()
{
	pid_t pid = fork();

	if(pid > 0){
		server({ 0, 5000 });
	} else {
		for(size_t i = 0; i < 1; i++){
			pid = fork();
			if(pid == 0){ client({ 0, 5000 }); }
		}
	}
	return 0;
}
