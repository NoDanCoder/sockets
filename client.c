#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

/* creeate socket */

#define IPV4 AF_INET
#define TCP SOCK_STREAM
#define CONSTANTa 0

/* set serv_addr struct */

#define CONSTANTb AF_INET

/* size buffer */

#define BUFSIZE 256

/* Handle error function */

void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int ac, char *av[])
{
	int sockfd, port_num, bytes_sr;
	
	struct sockaddr_in serv_addr;

	struct hostent *server;

	char buffer[BUFSIZE];

	/* check if hostname was given */

	ac--;
	if (ac != 2)
		error("host address and hostname port please");

	/* set port_number of server to connect */

	port_num = atoi(av[2]);

	/* Create note/socket */

	sockfd = socket(IPV4, TCP, CONSTANTa);
	if (sockfd < 0) 
        	error("ERROR opening socket");

	/* try to connect to given address and it will fill "server" struct
	 * this is done to get the IP address
	 */

	server = gethostbyname(av[1]);
	if (server == NULL)
		error("ERROR no such host");

	/* set to zero all values of the struct */

        bzero((char *) &serv_addr, sizeof(serv_addr));

	/* set ser_Addr struct */

	serv_addr.sin_family = CONSTANTb;		/* it's a constant */
	bcopy((char *) server->h_addr,			/* access to h_addr property of "server" and paste it */
		(char *) &serv_addr.sin_addr.s_addr,	/* to serv_addr.sin_addr.s_addr */
		server->h_length);			/* h_addr it's an alias of first element of h_addr_list property of "server" */
	serv_addr.sin_port = htons(port_num);		/*  htons convert "little endian" to "big endian", next we set port number */

	/* It try to connecto to given "serv_Addr" */
	/* Notice that the client needs to know the port number of the
	 * server, but it does not need to know its own port number.
	 * This is typically assigned by the system when connect is called.
	 */

	if ( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 )
		error("ERROR Conecting");

	/*--------------------------------------------------------------------*/

	printf("Conected...\n\n");

	if ( fork() )
	{
		while (1)
		{
			bzero(buffer, BUFSIZE);
			write(1, " s: ", 3);
			fgets(buffer, BUFSIZE, stdin);
			bytes_sr = write(sockfd, buffer, strlen(buffer));
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
			bytes_sr = read(sockfd, buffer, BUFSIZE);
			if (bytes_sr < 0)
				error("ERROR writing to socket");
			printf(" r: %s", buffer);
		}
	}

	return 0;
}
