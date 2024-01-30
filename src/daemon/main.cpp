#include "data.h"

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

#define PORT 8080

// message is "<mode>/<remaining string>"
// q: query
// m: menu
// because it is string it is assumed to be null terminated
// void messageHandler(char *buffer, OUT_STRING *res) {
// 	switch (buffer[0])
// 	{
// 	case 'q': // query
// 		queryHandler(data, buffer + 2, res);
// 		break;
// 	case 'm': // menu
// 		menuHandler(data, buffer + 2, res);
// 		break;

// 	case 'k': // kill
// 		sigterm_handler(SIGTERM);
// 		break;
	
// 	case 'w': // save to file (idk it probably uses $HOME to figure out the path)
// 		saveTableToFile(data, "table", dir);
// 		res->len = 5;
// 		strcpy(res->str, "Saved");
// 		// send(client_fd, "Saved", 5, 0);
// 		break;

// 	case 'd': // print debug of entire data, will not work in daemon mode as stdout is closed 
// 		// will improve in the future to dump into a file
// 		dumpTable(data, 0);
// 		close(client_fd);
// 		break;
	
// 	default:
// 		break;
// 		// exit(EXIT_FAILURE);

// 	}
// }


// argv[1] == directory with data
int main (int argc, char **argv) {
	// parse data
	if (argc < 2) {
		fprintf(stderr, "Insuficient arguments: need dataset path\n");
		exit(EXIT_FAILURE);
	}

	//////////////////////////////////////////////// creating socket

	struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
	int server_fd;
  
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
	}

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
	inet_pton(AF_INET, "127.14.14.14", &address.sin_addr);
    //address.sin_addr.s_addr =  INADDR_ANY;
    address.sin_port = htons(PORT);
  
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

	//////////////////////////////////////////////// parsing theme file

	Data data(argv[1]); // will parse the data
	data.print();

	//////////////////////////////////////////////// creating daemon

	// pid_t pid, sid;

    // // Fork the process
    // pid = fork();

    // if (pid < 0) {
	// // error
    //     exit(EXIT_FAILURE);
    // }

    // if (pid > 0) {
    //     // Parent process
    //     exit(EXIT_SUCCESS);
    // }

    // // Set file mode creation mask to 0
    // umask(0);

    // // Create a new session
    // sid = setsid();
    // if (sid < 0) {
    //     exit(EXIT_FAILURE);
    // }

    // // Change the working directory to root
    // if (chdir("/") < 0) {
    //     exit(EXIT_FAILURE);
    // }

    // // Close standard file descriptors
    // close(STDIN_FILENO);
    // close(STDOUT_FILENO);
    // close(STDERR_FILENO);

	//////////////////////////////////////////////// listen for requests on the socket

	// int valread;//, valsend;
	// char buffer[STR_RESULT_SIZE];
	// message = malloc(sizeof(OUT_STRING));
    // while (1) {
	// 	if ((client_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
	// 		perror("accept");
    //     	exit(EXIT_FAILURE);
	// 	}

	// 	while ((valread = read(client_fd, buffer, STR_RESULT_SIZE - 1)) > 0) {
	// 		// reset since it is reused
	// 		message->len = 0;

	// 		buffer[valread] = '\0';
	// 		// printf("Received %s, calling handler\n", buffer);
	// 		messageHandler(buffer, message);
	// 		// printf("Final message is:\n");
	// 		// fflush(stdout);
	// 		// write(STDOUT_FILENO, message->str, message->len);
	// 		send(client_fd, message->str, message->len, 0);
	// 		// sigterm_handler(SIGTERM);
	// 	}
	// 	// here????
	// 	close(client_fd);
    // }

	close(server_fd);
    return 0;
}
