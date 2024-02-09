#include "errors.h"
#include "data.h"
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

#define MAX_CONN 1

// argv[1] == directory with data
// argv[2] == directory to place the socket
// since this is a daemon and paths are lost, please enter all paths as hardcoded
int main (int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Insuficient arguments: need dataset path\n");
		return EXIT_FAILURE;
	}
	if (argc < 3) {
		fprintf(stderr, "Insuficient arguments: need path to dir to create socket\n");
		return EXIT_FAILURE;
	}

	//////////////////////////////////////////////// creating unix socket

	// Create a Unix domain socket
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        print_error("Error creating socket\n");
        return EXIT_FAILURE;
    }

    // Define the address structure for the socket
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;

	strncpy(addr.sun_path, argv[2], sizeof(addr.sun_path) - 1);
    strncpy(addr.sun_path + strlen(argv[2]), "Themer-socket", sizeof(addr.sun_path) - 1);

    // Bind the socket to the address
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        print_error("Error binding socket\n");
        close(sockfd);
        return EXIT_FAILURE;
    }

    // Listen for incoming connections
    if (listen(sockfd, MAX_CONN) < 0) {
        print_error("Error listening on socket\n");
        close(sockfd);
        return EXIT_FAILURE;
    }

	//////////////////////////////////////////////// parsing theme file

	Data data(argv[1]); // will parse the data
	// data.print();

	// std::string input = "rofi/*";
	// puts(data.read(input).c_str());

	// std::string input = "1/dunst";
	// std::string res = data.menu(input);

	// // this is kind of cursed but the string has some \0 and they would get cutoff otherwise
	// if (write(STDOUT_FILENO, res.c_str(), res.size()) == -1) {
	// 	print_error("Error writing data");
	// }

	//////////////////////////////////////////////// creating daemon

	pid_t pid, sid;

    // Fork the process
    pid = fork();

    if (pid < 0) {
	// error
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Parent process
        exit(EXIT_SUCCESS);
    }

    // Set file mode creation mask to 0
    umask(0);

    // Create a new session
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    // Change the working directory to root
    if (chdir("/") < 0) {
        exit(EXIT_FAILURE);
    }

    // // Close standard file descriptors
    // close(STDIN_FILENO);
    // close(STDOUT_FILENO);
    // close(STDERR_FILENO);

	//////////////////////////////////////////////// listen for requests on the socket

	Message msg;
	while (true) {
		int clientSock = accept(sockfd, nullptr, nullptr);
		if (clientSock < 0) {
		    print_error("Error accepting connection\n");
		    close(sockfd);
		    return EXIT_FAILURE;
		}

		read(clientSock, &msg, sizeof(Message));
		// handle message..........................................................
		// answer...........................................................
		// write(clientSock, "Message received", 16);

		close(sockfd);
	}




	close(sockfd);
	// TODO: remove the socket file as well
    return 0;
}
