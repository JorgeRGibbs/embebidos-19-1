#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include "hamming.h"
#include "defs.h"
#include "procesos.h"

extern float *w;

void procesos(float seno[]){
	pid_t pid;
	register int np,i;
	w = malloc(sizeof(int)* MUESTRAS);
	//y = malloc(sizeof(float)* MUESTRAS);
/*	for (i = 0; i < N ; i++)
	{
		w[i] = (0.54 - (0.46*cosf(((2*M_PI*i)/(MUESTRAS-1)))));
	}*/
	int pipefd[NUM_PROC][2], pipe_status;
	for ( np = 0; np < NUM_PROC; np++)
	{
		pipe_status = pipe (&pipefd[np][0]);
		if (pipe_status == -1)
		{
			perror("error al crear el pipe");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("No se creo el proceso\n");
			exit(EXIT_FAILURE);
		}
		if (!pid)
		{
			procesoHijo( np , &pipefd[np][0], seno);
		}
	}
	procesoPadre(pipefd);
}



void procesoPadre( int pipefd[NUM_PROC][2] )
{
	int np, npc, tambloque;
	pid_t pid_hijo;
	tambloque = N/NUM_PROC;
	printf("Proceso padre ejecutado con pid %d\n", getpid());

	for( np = 0; np < NUM_PROC; np++ )		
	{
		pid_hijo = wait( &npc );
		npc = npc >> 8;
		close( pipefd[npc][1] );
		int inibloque = tambloque*npc;
		read( pipefd[npc][0], w+inibloque, sizeof(int)*tambloque);
		printf("proceso hijo %d, terminado con el pid %d\n", npc,pid_hijo);
	}
	close( pipefd[np][0] );
}

void procesoHijo (int np, int pipefd[],float seno[])
{
	int tamBloque = N / NUM_PROC;
	int iniBloque = np * tamBloque;
	int finBloque = iniBloque + tamBloque;
	printf("Proceso hijo %d ejecutado con pid %d\n", np, getpid() );
	close(pipefd[0]);
	register int i;
	for (i = iniBloque; i < finBloque; i++)
	{
		//w[i] = (0.54 - (0.46*cos(((2*M_PI*i)/(MUESTRAS-1)))));
		w[i] = (0.54 - 0.46*cos(((2*M_PI*i)/(MUESTRAS-1))));
		//y[i] = seno[i] * w[i];
	}
	//Hamming(seno,y,finBloque);
	//Reparieren sie der ordnung und sortieren stück
	write( pipefd[1] , w+iniBloque,sizeof(int)*tamBloque);
	close (pipefd[1]);
	exit(np);
}