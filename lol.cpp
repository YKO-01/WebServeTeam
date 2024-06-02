#include <unistd.h> // for pipe, read, write
#include <iostream>

int main() {
    int pipefd[2]; // array to hold the two file descriptors

    // create the pipe
    if (pipe(pipefd) == -1) {
        std::cerr << "Pipe creation failed\n";
        return 1;
    }

    // pipefd[0] is for reading
    // pipefd[1] is for writing

    // For demonstration, let's write and read within the same process.
    const char* message = "Hello, pipe!";
    dup2(pipefd[1], STDOUT_FILENO);
    system("echo Hello, World!");
    write(pipefd[1], message, strlen(message));
    close(pipefd[1]);

    char buffer[128];
    read(pipefd[0], buffer, sizeof(buffer));
    std::cout << "Received message: " << buffer << std::endl;

    return 0;
}
