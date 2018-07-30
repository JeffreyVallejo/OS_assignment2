#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef READ
#define READ 0
#endif

#ifndef WRITE
#define WRITE 1
#endif

main()

{

	char *path, *inputPath, *outputPath, *argv[20], buf[80], n, *p;

	int m, status, inword, continu, counter, flagIn, flagOut, pipes;

	pid_t pid;

	int right_p[2], left_p[2];


	while(1) {

		inword = 0;
		p = buf;
		m = 0;
		continu= 0;
		counter = 0;
		pipes = 0;
		pid = 0;
		flagIn = 0;
		flagOut = 0;

		//container for arguments
		int argCont[20] = {0};

		printf( "\nshhh> ");

		while ( ( n = getchar() ) != '\n'  || continu ) {

			if ( n ==  ' ' ) { 

			    if ( inword ) {

				inword = 0;

				*p++ = 0;

			    } 

			}

			else if ( n == '\n' ) continu = 0;

			else if ( n == '\\' && !inword ) continu = 1;

			else {

			    if ( !inword ) {

				 inword = 1;

				 argv[m++] = p;

				 *p++ = n;

			     }

			    else 

				 *p++ = n;

			}


		}	

		*p++ = 0;

		argv[m] = 0;

		//exit using quit or exit
		if ( strcmp(argv[0],"quit") == 0 ) exit (0);
		if ( strcmp(argv[0], "exit") == 0 ) exit (0);


		//this while loop handles redirection 
		while (argv[counter] != 0) {
			if (strcmp(argv[counter], "|") == 0) {
				argv[counter] = 0;
				argCont[pipes + 1] = counter + 1;
				++pipes;
			}
			else if (strcmp(argv[counter], ">") == 0) {
				inputPath = strdup(argv[counter + 1]);
				argv[counter] = 0;
				argv[counter + 1] = 0;
				flagOut = 1;
			}
			else if (strcmp(argv[counter], "<") == 0) {
				outputPath = strdup(argv[counter +1]);
				argv[counter] = 0;
				argv[counter + 1] = 0;
				flagIn = 1;
			}
			else {
				argCont[counter] = counter;
			}
			counter++;
		}

		for (int i = 0; i <= pipes; i++) {
			if (i < pipes) {
				pipe(right_p);
			}

			switch (pid = fork()) {
				case -1: 
						perror("fork failed");
						break;
				case 0: 
						if ((i == 0) && (flagIn == 1)) {
							int inFile = open(inputPath, O_RDONLY, 0400);
							if (inFile == -1) {
								printf("input failed\n");
								return(EXIT_FAILURE);
							}
							close(READ);
							dup(inFile);
							close(inFile);
						}
						if ((i == pipes) && (flagOut == 1)) {
							int outFile = open(path, O_WRONLY | O_CREAT, 0755);
							if (outFile < 0) {
								printf("output failed\n");
								return(EXIT_FAILURE);
							}
							dup2(outFile, WRITE);
							close(outFile);
						}
						if (pipes > 0) {
							if (i == 0) {
								close(WRITE);
								dup(left_p[READ]);
								close(left_p[READ]);
								close(left_p[WRITE]);
								close(WRITE);
								dup(right_p[WRITE]);
								close(right_p[READ]);
								close(left_p[WRITE]);
							}
							else {
								close(READ);
								dup(left_p[READ]);
								close(left_p[READ]);
								close(left_p[WRITE]);
							}
						}
						execvp(argv[argCont[i]], &argv[argCont[i]]);
						printf("execvp failed\n");
						break;
				default: 
						if (i > 0) {
							close(left_p[READ]);
							close(left_p[WRITE]);
						}
						left_p[READ] = right_p[READ];
						left_p[WRITE] = right_p[WRITE];

						wait(&status);

						break;			
			}
		}
	}
}

