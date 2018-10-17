#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#define NUM_PROC 4

void proceso_hijo( int np );
void proceso_padre();

int main()
{
	pid_t pid;
	register int np;

	printf("Probando procesos...\n");

	for( np = 0; np < NUM_PROC; np++ )
	{
		pid = fork();
		if( pid == -1 )
		{
			perror("Error al crear el proceso");
			exit(EXIT_FAILURE);
		}
		if( !pid )
		{
			proceso_hijo( np );
		}
	}
	proceso_padre();

	return 0;
}

void proceso_hijo( int np )
{
	printf("Proceso hijo %d con pid %d\n", np, getpid());

	pause();
	printf("Señal recibida en el proceso hijo\n");
	exit(np);
}

void proceso_padre()
{
	register int np;
	int status;
	pid_t pid;

	printf("proceso padre con pid %d\n", getpid());

	for( np = 0; np < NUM_PROC; np++ )
	{
		pid = wait( &status );
		printf("proceso hijo %d terminado con PID %d\n", status>>8, pid);
	}
}
