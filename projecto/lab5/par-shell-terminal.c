#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include "commandlinereader.h"
#include "list.h"

#define EXIT_COMMAND "exit"
#define GLOBAL_EXIT "exit-global"
#define VECTOR_SIZE 6
#define ARG_LEN 256
#define MAXPAR 4
#define __DEBUG__ 0

int _exit_ctrl = 0;

// don't forget to close with close(fd)
int open_pipew(char *pipe_name){
	
	int pipe_fd = open(pipe_name, O_WRONLY);
	
	if(pipe_fd == -1){
			perror("Error opening pipe\n");
			exit(EXIT_FAILURE);
	}
	
	return pipe_fd;
	
}

int main(int argc, char *argv[]){
	
	char **argVector = (char **) malloc(VECTOR_SIZE * sizeof(char*));
	char output[1024], input[1024];

	if(argc < 2){
			printf("Not enough arguments\n"),
			exit(EXIT_FAILURE);
	}
	
	int pipe_fd = open_pipew(argv[1]);
	
	sprintf(output,	"REG %d", getpid());
	write(pipe_fd, output, strlen(output));
	close(pipe_fd);
	
	while(!_exit_ctrl) {
		fgets(input, 1024, stdin);

		if(argVector[0] == NULL){
			continue;
		}

		// caso o utilizador tenha introduzido o comando stats
		if(strcmp(argVector[0], "stats") == 0){
			pipe_fd = open_pipew(argv[1]);
			write(pipe_fd, argVector[0], strlen(argVector[0]));
			close(pipe_fd);
		}else{
			pipe_fd = open_pipew(argv[1]);
			write(pipe_fd, argVector[0], strlen(argVector[0]));
			close(pipe_fd);
		}
		
	}
	
	free(argVector);

}
