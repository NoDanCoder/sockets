#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h> 

/* creeate socket */

#define IPV4 AF_INET
#define TCP SOCK_STREAM
#define CONSTANTa 0

/* set serv_addr struct */

#define CONSTANTb AF_INET
#define SERVER_ADDR INADDR_ANY

/* size buffer */

#define BUFSIZE 256

/* Handle error function */

void error(char *msg)
{
	perror(msg);
	exit(1);
}

/* Main - function */

int main(int ac, char *av[])
{
	int sockfd, newsockfd, port_num, cli_len, bytes_sr;

	char buffer[BUFSIZE];

	struct sockaddr_in serv_addr, cli_addr;

	/* check if port number was insert */

	ac--;
	if (ac != 1)
		error("no port provided");

	/* Create note/socket */

	sockfd = socket(IPV4, TCP, CONSTANTa);
	if (sockfd < 0)
		error("ERROR opening socket");

	/* set to zero all values of the struct */

	bzero((char *) &serv_addr, sizeof(serv_addr));

	/* port number where the server start to listen for connections */

	port_num = atoi(av[1]);

	/* set serv_addr struct */

	serv_addr.sin_family = CONSTANTb;		/* it's a constant */
	serv_addr.sin_port = htons(port_num);		/* htons convert "little endian" to "big endian", next we set port number */
	serv_addr.sin_addr.s_addr = SERVER_ADDR;		/* set ip address where the current server is running */

	/* set conection parameters (serv_addr) to current process (sockfd) */

	if ( bind( sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr) ) < 0 )
		error("ERROR on binding");

	/* allow to "sockfd" to recive conections and set max num of conection nodes */

	listen(sockfd, 5);
	printf("Mi ip address: %s\n", inet_ntoa(serv_addr.sin_addr));

	/* stop process upto recive a successfull conection, comunicaction will be
	 * handlen on the new socket "newsockfd", this does not kill "sockfd" process
	 * and fill client info in "cli_addr"
	 */

	cli_len = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);
	if (newsockfd < 0)
		error("ERROR on accept");

	/*------------------------------------------------------------------------------------*/
	
	if ( fork() )
	{
		while (1)
		{
			bzero(buffer, BUFSIZE);
			write(stdout, " s: ", 3);
			fgets(buffer, BUFSIZE, stdin);
			bytes_sr = write(newsockfd, buffer, strlen(buffer));
			if (bytes_sr < 0)
				error("ERROR writing to socket");
		}
	}
	else
	{
		setsid();
		while (1)
		{
			bzero(buffer, BUFSIZE);
			bytes_sr = read(newsockfd, buffer, BUFSIZE);
                        if (bytes_sr < 0)
                                error("ERROR writing to socket");
			printf(" r: %s", buffer);
		}
	}

	/* Finish the program */

	return 0;
}
