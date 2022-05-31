#include "../lib/socks.h"
#include "../lib/logging.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_PLAYERS 10

void server(socks_addr_t addr)
{
	socket_t sock = socks_init();
	socks_bind(sock, addr);
	socks_listen(sock, NUM_PLAYERS);

	// Get all players PIDs
	pid_t players[NUM_PLAYERS];
	for(size_t i = 0; i < NUM_PLAYERS; i++){
		socket_t conn = socks_accept(sock, addr);
		int written = socks_read(
			conn,
			&players[i],
			sizeof(sizeof(players[i])));
		if(written == -1){
			logerr("Could not read players PID");
			exit(1); }
		loginfo("PID recieved: %d", players[i]);
		socks_close(conn);
	}

	// Select winner
	pid_t winner = players[rand() % NUM_PLAYERS];

	// Send winner to all players
	for(size_t i = 0; i < NUM_PLAYERS; i++){
		int written = socks_write(sock, &winner, sizeof(winner));
		if(written == -1){
			logerr("Could not write winner to player");
			exit(1); }
		loginfo("Winner sent");
	}

	socks_close(sock);
	loginfo("Closed socket");
}

void client(socks_addr_t addr)
{
	socket_t sock = socks_init();
	socks_connect(sock, addr);

	// Sending current pid
	pid_t thisPid = getpid();
	if(socks_write(sock, &thisPid, sizeof(thisPid)) == -1){
		logerr("Could write this PID to socket");
		exit(1); }
	loginfo("PID sent");
	socks_close(sock);

	// Waiting for winner
	sock = socks_init();
	socks_connect(sock, addr);
	pid_t winner;
	if(socks_read(sock, &winner, sizeof(winner)) == -1){
		logerr("Could not read winner from socket");
		exit(1); }
	printf("Winner is: %d\n", winner);

	socks_close(sock);

	// If winner, print pid
	if(winner == thisPid)
		printf("%d: fui sorteado", thisPid);
}

int main()
{
	pid_t pid = fork();
	socks_addr_t addr = { 0, 5000 };

	if(pid > 0){
		printf("Server PID: %d\n", getpid());
		server(addr);
		wait(NULL);
	} else {
		for(size_t i = 0; i < 10; i++){
			pid = fork();
			if(pid == 0){
				printf("Client[%ld] PID: %d\n", i, getpid());
				client(addr);
			}
		}
	}

	return 0;
}
