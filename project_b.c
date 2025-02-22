#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h> // Required for wait()

#define BUFFER_SIZE 512

// this code closely follows Listing 44-2 from The Linux Programming Interface by Michael Kerrisk
int main(int argc, char *argv[]) {
    int pipe_fd[2]; 
    pid_t child_pid;

    if (pipe(pipe_fd) == -1) { 
        printf("Pipe creation failed.\n");
        fflush(stdout);
        return EXIT_FAILURE;
    }

    
    switch (fork()) { 
        case -1:
            printf("Fork failed.\n");
            fflush(stdout);
            return EXIT_FAILURE;

        case 0: // child process     
            if (close(pipe_fd[1]) == -1) { // write end
                printf("Close write end failed in child process.\n");
                fflush(stdout);
                return EXIT_FAILURE;
            }

            char buffer[BUFFER_SIZE];
            ssize_t bytes_read;

            printf("Child process is waiting to read from the pipe.\n");
            fflush(stdout);

            for (;;) {        
                ssize_t numRead = read(pipe_fd[0], buffer, BUFFER_SIZE); 
                if (numRead == -1) {
                    printf("Error reading from pipe.\n");
                    fflush(stdout);
                    return EXIT_FAILURE;
                }
                if (numRead == 0)      
                    break;              
                if (write(STDOUT_FILENO, buffer, numRead) != numRead) {
                    printf("Error writing to stdout failed.\n");
                    fflush(stdout);
                    return EXIT_FAILURE;
                }
            }
    
            write(STDOUT_FILENO, "\n", 1); 
            if (close(pipe_fd[0]) == -1) {  
                printf("Close read end failed in child process.\n");
                fflush(stdout);
                return EXIT_FAILURE;
            }
            _exit(EXIT_SUCCESS);

        default: // parent process
            if (close(pipe_fd[0]) == -1) { // read end
                printf("Close read end failed in parent process.\n");
                fflush(stdout);
                return EXIT_FAILURE;
            }

            const char *message = "If this is printed, then the child has done its job.\n";
            printf("Parent process is sending message to child...\n");
            fflush(stdout);

            if (write(pipe_fd[1], message, strlen(message)) != strlen(message)) {
                printf("Error writing to pipe.\n");
                fflush(stdout);
                return EXIT_FAILURE;
            } else {
                printf("Parent process sent message to child.\n");
                fflush(stdout);
            }


          
            if (close(pipe_fd[1]) == -1) { 
                printf("Error closing write end in parent process.\n");
                fflush(stdout);
                return EXIT_FAILURE; 
            }

            printf("Parent process is waiting for child to finish.\n");
            fflush(stdout);
            wait(NULL); 
            printf("Child process has finished.\n");
            fflush(stdout);
            exit(EXIT_SUCCESS); 
    }

    return EXIT_SUCCESS;
}
