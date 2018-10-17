#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

#define N 64
#define NUM_PROC 4
//Paralelismo por bloques y procesos
/*Tarea: Llenar arreglos con datos */
/*Usar funcion seno[] -> A*/
/*B -> Hamming[] Investigar ecuación de Hamming*/
/*Usar N = 256 muestras*/

/*Usar hilos y procesos*/
void * funHilo(void *arg);
int * reservaMemoria();
void llenarArreglo(int * datos);
void imprimirArreglo(int *datos);
void procesoPadre(int pipefd[NUM_PROC][2]);
void procesoHijo(int np, int pipefd[]);


int *A,*B,*C;
//pthread_mutex_t bloqueo;
int contador;

int main(){
	pid_t pid;
	register int np;
	int pipefd[NUM_PROC][2], pipe_status;

	A = reservaMemoria();
	llenarArreglo ( A );
	imprimirArreglo( A );

	B = reservaMemoria();
	llenarArreglo ( B );
	imprimirArreglo( B );

	C = reservaMemoria();
	printf("probando procesos...\n");
	//imprimirArreglo( C );
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
	imprimirArreglo(C);
	free ( A );
	free ( B );
	free ( C );
/*
	datos = memoria();
	llenarArreglo(datos);
	imprimir(datos);
*/

/*	procesoPadre( pipefd );
	imprimirArreglo( C );
	free(A);
	free(B);
	free(C);
*/
	return 0;
}
//parellismo alternado
//2,6,14, .... 
//1,5,9 ....

/*void * funHilo(void *arg){
	register int i=0;
	int nh = *(int*)arg;

	for (i = nh ; i < N ; i+= NUM_PROC)
	{
		C[i] = A[i] * B[i];
	}

	pthread_exit(arg);
}*/

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
		if(!(i%8)){
			printf("\n");
		}
		printf("%5d",datos[i]);
		printf(",");
	}
	printf("\n");
}

void procesoPadre(int pipefd[NUM_PROC][2])
{
	int np,npc,tamBloque = N / NUM_PROC;
	pid_t pid_hijo;

	printf("Proceso padre ejecutado con pid %d\n", getpid() );


	for (np = 0; np < NUM_PROC; np++)
	{

		pid_hijo = wait(&npc);
		npc = npc >> 8;
		close( pipefd[npc][1]);
		int iniBloque = npc * tamBloque;
		read (pipefd[npc][0], C+iniBloque , sizeof(int)* tamBloque);
		printf("proceso hijo %d terminado con pid %d\n", npc, pid_hijo );
	}
	close(pipefd[npc][0]);
}

void procesoHijo (int np, int pipefd[])
{
	int tamBloque = N / NUM_PROC;
	int iniBloque = np * tamBloque;
	int finBloque = iniBloque + tamBloque;

	printf("Proceso hijo %d ejecutado con pid %d\n", np, getpid() );
	close(pipefd[0]);
	register int i;
	for (i = 0; i < finBloque; i++)
	{
		C[i] = A[i]*B[i];
	}
	write( pipefd[1] , C+iniBloque,sizeof(int)*tamBloque);
	close (pipefd[1]);
	exit(np);
}
/*
void * Mayor(void * arg){
	register int i;
	static int mayor;
	int *datos =(int *)arg;
	mayor = datos[0];
	for(i =1; i<N;i++){
		if(datos[i]>mayor){
			mayor = datos[i];
		}
	}
	pthread_exit(&mayor);
}
void * Menor(void * arg){
	register int i;
	static int menor;
	int *datos =(int *)arg;
	menor = datos[0];
	for(i =1; i<N;i++){
		if(datos[i]<menor){
			menor = datos[i];
		}
	}
	pthread_exit(&menor);
}

}*/
