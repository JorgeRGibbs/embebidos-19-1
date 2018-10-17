//PRODUCTO PUNTO CON PROCESOS Y POR BLOQUES
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#define N 32
#define NUM_PROC 4

void * funHilo(void *arg);
int * reservaMemoria();
void llenarArreglo(int * datos);
void imprimirArreglo(int *datos);
void procesoPadre(int pipefd[NUM_PROC][2]);
void procesoHijo(int np, int pipefd[]);

int *A,*B,*AB, PP;

int main(){
	pid_t pid;
	register int np;
	int pipefd[NUM_PROC][2], pipe_status;

	A = reservaMemoria();
	llenarArreglo ( A );
	imprimirArreglo( A );

	B = reservaMemoria();
	llenarArreglo( B );
	imprimirArreglo( B );

	AB = reservaMemoria();	

	printf("probando procesos...\n");
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
			procesoHijo( np , &pipefd[np][0]);
		}
	}
	procesoPadre(pipefd);
	imprimirArreglo( AB );

	printf("El producto punto es: %d \n",PP);
	free ( A );
	free ( B );
	free ( AB );

	return 0;
}


int * reservaMemoria(){
	int *mem;
	mem = (int*)malloc(sizeof(int)*N);
	if(!mem){

		perror("error al hacer la memoria");
		exit(EXIT_FAILURE);
	}
	return mem;
}

void llenarArreglo(int * datos){
	register int i;
	//srand(getpid());
	for(i=0;i<N;i++){
		datos[i] = rand()%255;
	}
}

void imprimirArreglo(int * datos){
	register int i;
	for(i = 0; i<N;i++){
		if(!(i%16)){
			printf("\n");
		}
		printf("%5d",datos[i]);
		printf(",");
	}
	printf("\n");
}

void procesoPadre(int pipefd[NUM_PROC][2])
{
	int np,npc,tamBloque = N / NUM_PROC,suma_parcial;

	pid_t pid_hijo;

	printf("Proceso padre ejecutado con pid %d\n", getpid() );


	for (np = 0; np < NUM_PROC; np++)
	{

		pid_hijo = wait(&npc);
		npc = npc >> 8;
		close( pipefd[npc][1]);
		int iniBloque = npc * tamBloque;
		int finBloque = iniBloque + tamBloque;
		read (pipefd[npc][0], AB+iniBloque , sizeof(int)* tamBloque);
		printf("proceso hijo %d terminado con pid %d\n", npc, pid_hijo );
		close(pipefd[npc][0]);
	}
	for (int i = 0; i < N ; i++)
	{
		PP += AB[i];
	}
}

void procesoHijo (int np, int pipefd[])
{
	int tamBloque = N / NUM_PROC;
	int iniBloque = np * tamBloque;
	int finBloque = iniBloque + tamBloque;

	printf("Proceso hijo %d ejecutado con pid %d\n", np, getpid() );
	close(pipefd[0]);
	register int i;
	for (i = iniBloque; i < finBloque; i++)
	{
		AB[i] = A[i] * B[i];
	}
	write( pipefd[1] , AB+iniBloque,sizeof(int)*tamBloque);
	close (pipefd[1]);
	exit(np);
}

