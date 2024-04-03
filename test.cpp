#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include<iostream>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
int main() {
    fd_set readfds;
    int retval;
    int fd1, fd2,fd3;

    // Open two files
    fd1 = open("gg.html", O_RDONLY);
    if (fd1 == -1) {
        perror("Error opening file1.txt");
        exit(EXIT_FAILURE);
    }
    fd2 = open("default.conf", O_RDONLY);
    if (fd2 == -1) {
        perror("Error opening file2.txt");
        exit(EXIT_FAILURE);
    }

    // Clear the file descriptor set
    FD_ZERO(&readfds);

    // Add file descriptors to the set
    FD_SET(fd1, &readfds);
    FD_SET(fd2, &readfds);
    // std::cout << readfds.fds_bits << std::endl;
    // std::cout <<  FD_ISSET(fd2, &readfds) << std::endl;
    // FD_ISSET(fd1, &readfds);

    // Determine the highest file descriptor
    int maxfd = (fd1 > fd2) ? fd1 : fd2;

    // Set timeout
    struct timeval timeout;
    timeout.tv_sec = 5; // 5 seconds
    timeout.tv_usec = 0;

    // Wait for activity on file descriptors
    retval = select(maxfd + 1, &readfds, NULL, NULL, NULL);
    std::cout << "retval==" << retval << std::endl;
    if (retval == -1) {
        perror("select()");
        exit(EXIT_FAILURE);
    } else if (retval) {
        // At least one file descriptor is ready
        if (FD_ISSET(fd1, &readfds)) {
            printf("Data is available on file1.txt\n");
            // Read data from file1.txt and process it
        }
        if (FD_ISSET(fd2, &readfds)) {
            printf("Data is available on file2.txt\n");
            // Read data from file2.txt and process it
        }
    } else {
        printf("No data within five seconds.\n");
    }

    // Close files
    close(fd1);
    close(fd2);

    return 0;
}
