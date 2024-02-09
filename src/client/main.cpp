#include "errors.h"
#include "message.h"

// like 50% of these are unused
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

// argv[1] is dataset path
// argv[2] is the base query, like "r/" or "m/", should be done by a script that calls this
int main (int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Insuficient arguments: need path to dir with socket\n");
		return EXIT_FAILURE;
	}

	if (argc < 3) {
		fprintf(stderr, "Insuficient arguments: need base query\n");
		return EXIT_FAILURE;
	}

	int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        print_error("Error creating socket\n");
        return EXIT_FAILURE;
    }

	struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;

	// this is also VERY bad and WILL crash for big strings, rewrite, the daemon has this as well wtf
	strncpy(server_addr.sun_path, argv[1], sizeof(server_addr.sun_path) - 1);
    strncpy(server_addr.sun_path + strlen(argv[1]), "Themer-socket", sizeof(server_addr.sun_path) - 1);

	socklen_t data_len = strlen(server_addr.sun_path) + sizeof(server_addr.sun_family); // wtf????? should I just use sizeof(addr))???

	if (connect(sockfd, (struct sockaddr *)&server_addr, data_len) < 0) {
        print_error("Error connecting to server\n");
        return EXIT_FAILURE;
    }

	Message msg;
	// this is all very ugly overall, idk, will rewrite
	strncpy(msg.str, argv[2], MESSAGE_STR_SIZE);

	// got lazy. error checking also missing
	std::string type = argv[2];
	size_t pos = type.find('/');
	std::string typestr = type.substr(0, pos + 1);
	if (typestr == "r/") {
		msg.type = READ;
	} else if (typestr == "m/") {
		msg,type = MENU;
		char *info = getenv("ROFI_INFO");
		if (info != nullptr) {
			strncpy(msg.str + strlen(argv[1]), info, MESSAGE_STR_SIZE - strlen(argv[1]));
		}
	} else {
		puts("Error type");
	}

	write(sockfd, &msg, sizeof(Message));

	OutMessage outmsg;
	read(sockfd, &outmsg, sizeof(OutMessage));
	// ugly but has to be done this way, string might have many \0
	write(STDOUT_FILENO, outmsg.str, outmsg.len);


	close(sockfd);
	return 0;
}