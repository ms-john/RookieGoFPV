// Copyright (c), 2022-2024, Mist Studio.

//
// Created by John on 24-7-1.
//

#ifndef WIFIBROADCAST_H
#define WIFIBROADCAST_H


#include <cstring>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int open_udp_socket_for_tx(const std::string &client_addr, int client_port)
{
	struct sockaddr_in saddr;
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		printf("Error opening socket: %s\n", strerror(errno));
	}

	bzero((char *) &saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(client_addr.c_str());
	saddr.sin_port = htons((unsigned short)client_port);

	if (connect(fd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0)
	{
		printf("Connect error: %s\n", strerror(errno));
	}
	return fd;
}

#endif //WIFIBROADCAST_H
