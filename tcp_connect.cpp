#include "web.h"

SOCKET tcp_connect(const char* host, const char* serv) {
	SOCKET sockfd, connfd = -1;
	struct addrinfo hints, *padr, * padrsave;

	ZeroMemory(&hints, sizeof(addrinfo));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_CANONNAME;

	getaddrinfo(host, serv, &hints, &padr);
	padrsave = padr;
	do
	{
		sockfd = socket(padr->ai_family, padr->ai_socktype, padr->ai_protocol);
		if (sockfd < 0)
			continue;

		connfd = connect(sockfd, padr->ai_addr, padr->ai_addrlen);
		if (connfd == 0)
			break;

		closesocket(sockfd);

	} while ((padr = padr->ai_next) != NULL);

	freeaddrinfo(padrsave);
	return sockfd;
}