#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "commandlinereader.h"

#define VECTOR_SIZE 6
#define ARG_LEN 256
#define __DEBUG__ 0

int main(int argc, char *argv[]){
	char **argVector;
	int i, *childrenPID, children = 0;

	argVector = (char **) malloc(VECTOR_SIZE * sizeof(char*));
	childrenPID = (int *) malloc(0); // para depois fazer realloc

	while(1){
		readLineArguments(argVector, VECTOR_SIZE);

		if(strcmp(argVector[0], "exit") == 0){
			break;
		}else{
			// qualquer comando que nao seja "exit" e considerado como
			// um comando para ser procurado na directoria de trabalho
			// e executado

			int pid = fork();

			if(pid != 0){
				// pai
				if(pid < 0){
					// erro ao criar o processo filho
					perror("Error forking process");
				}
				// neste exercicio o pai nao monitoriza os filhos
				childrenPID = (int *) realloc(childrenPID, (children + 1) * sizeof(int));
				childrenPID[children] = pid;
				children++;
			}else{
				// filho
				// substitui a imagem do executavel actual
				// pelo especificado no comando
				if(execv(argVector[0], argVector)){
					if(__DEBUG__){
						printf("o comando nao existe na directoria actual\n");
					}
				}
				if(execvp(argVector[0], argVector)){
					if(__DEBUG__){
						printf("o comando nao existe em lado nenhum..\n");
					}
					fprintf(stderr, "o comando %s nao existe\n",argVector[0]);
					//perror(erro_perror);
					exit(EXIT_FAILURE);
				}
				printf("Isto nao e suposto ser escrito\n");
			}
			// espirito santo? novo banco?
		}
	}
	// quando sai, verifica se todos os filhos que criou ja terminaram
	// e espera pelos que ainda estao a correr
	printf("Waiting for child processes to finish...\n");
	int status;
	for(i = 0; i < children; i++){
		printf("\t%d processes remaining\n", children - i);
		//waitpid(childrenPID[i], &status, 1);
		wait(&status);
		printf("Process %d terminated with status %d\n", childrenPID[i], status);
	}
	printf("All child processes finished\n");
	free(argVector);

	printf("par-shell terminated\n");

	exit(EXIT_SUCCESS);
}
