#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>

#define N 32
#define NUM_HILOS 4

int *reservaMemoria();
void imprimirArreglo(int * datos);
void llenarArreglo(int *datos);
int *menorArreglo (int *datos);
int *ordenaArreglo(int *datos);
int *mayorArreglo (int *datos);
int *promedioArreglo(int *datos);
pthread_mutex_t bloqueo;

int main()
{
	pthread_t tids[NUM_HILOS];
	int *datos;
	int hs[NUM_HILOS], *res;
	datos = reservaMemoria();
	llenarArreglo(datos);
	printf("Probando hilos...\n");

	register int nh;
	for (int nh = 0; nh < NUM_HILOS; nh++)
	{
		hs[nh] = nh;
		if(hs[0] == 0){
			pthread_create(&tids[nh],NULL,mayorArreglo, (void*) &hs[nh]);
		}
		if (hs[1] == 1)
		{
			pthread_create(&tids[nh],NULL,menorArreglo, (void*) &hs[nh]);
		}
		if (hs[2] == 2)
		{
			pthread_create(&tids[nh],NULL,promedioArreglo, (void*) &hs[nh]);
		}
		if (hs[3] == 3)
		{
			pthread_create(&tids[nh],NULL,ordenaArreglo, (void*) &hs[nh]);
		}
	}
	for(nh =0; nh<NUM_HILOS; nh++)
	{
		if(nh==0){
			pthread_join(tids[nh], (void **)&res);
			printf("El mayor es: %d\n",*res);
		}
		if(nh==1){
			pthread_join(tids[nh], (void **)&res);
			printf("El menor es: %d\n",*res);
		}
		if(nh==2){
			pthread_join(tids[nh], (void **)&res);
			printf("El promedio es: %d\n",*res);
		}
		if(nh==3){
			pthread_join(tids[nh], NULL);
			printf("datos ordenados :");
			imprimirArreglo(res);
		}
	}
	pthread_mutex_destroy(&bloqueo);

	//pthread_join(pthread_t thread, void ** retval);
	return 0;
}

int * reservaMemoria()
{
	int *mem;
	mem = (int *) malloc(N * sizeof(int ));
	printf("Memoria reservada en %p \n", &mem);
	if (!mem)
	{
		perror("Error al asignar memoria");
		exit(EXIT_FAILURE);
	}
	return mem;
}

int * ordenaArreglo(int *datos)
{	
	register int i;
	int d,t;
	for (i = 1 ; i <= N - 1; i++) {
    	d = i;
    	while ( d > 0 && datos[d-1] > datos[d]) {
    		t          = datos[d];
      		datos[d]   = datos[d-1];
      		datos[d-1] = t;
 			d--;
    	}
  	}
 	/*printf("Arreglo ordenado\n");
  	for (i = 0; i <= N - 1; i++) {
    	printf("Dato %d : %d\n", i,datos[i]);
  }*/
  return *datos;
 
}

void imprimirArreglo(int * datos)
{
	//int mayor,menor;
	register int i;
	datos = reservaMemoria();
	//llenarArreglo(datos);
	/*for (int i = 0; i < N ; i++)
	{
		printf("dato %d : %d\n",i,datos[i] );
	}*/
	//imprimirArreglo(datos);
	//mayor = mayorArreglo(datos);
	//menor = menorArreglo(datos);

	//printf("El numero mayor es: %d\n",mayor);
	//printf("El numero mayor es: %d\n",menor);

	free(datos);
	printf("Memoria liberada\n");
}


void llenarArreglo(int *datos)
{
	srand(time(NULL));
	register int i;
	for (i = 0; i < N; i++)
	{
		datos[i] = rand() % 255;
	}
}


int * menorArreglo (int *arg)
{
	register int i;
	static int menor;
	int *datos = (int*)arg;
	menor  = datos[0];
	for ( i = 0; i < N ; i++)
	{
		if (datos [i] < menor)
		{
			menor = datos[i];
		}
	}
	pthread_mutex_lock(&bloqueo);
	pthread_exit((void*)&menor);
	//printf("menor: %d\n", menor);
}

int * mayorArreglo (int *arg)
{
	register int i;
	static int mayor;
	int *datos = (int*)arg;
	mayor  = datos[0];
	for (i = 0; i < N; i++)
	{
		if (datos [i] > mayor)
		{
			mayor = datos[i];
		}
	}
	pthread_mutex_lock(&bloqueo);
	pthread_exit((void*)&mayor);
	//printf("mayor: %d\n",mayor);
}

int * promedioArreglo(int *datos)
{
	register int i;
	int prom,aux,sum;
	for (i = 0; i < N ; i++)
	{
		aux = datos[i];
//		printf("aux: %d\n",datos[i] );
		sum= sum + aux;
		fflush(stdout);
	}

	prom = sum/N;
	pthread_mutex_lock(&bloqueo);
	pthread_exit(&prom);
}

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
	pthread_mutex_lock(&bloqueo);
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
	pthread_mutex_lock(&bloqueo);
	pthread_exit(&menor);
}
int * reservarMemoria(){
	int *mem;
	mem = (int*)malloc(sizeof(int)*N);
	if(!mem){

		perror("error al hacer la memoria");
		exit(EXIT_FAILURE);
	}
	return mem;
}
void imprimir(int * datos){
	register int i;
	for(i = 0; i<N;i++){
		if(!(i%16)){
			printf("\n");
		}
		printf("%d",datos[i]);
		printf(",");
	}
	printf("\n");
}
void llenarArreglo(int * datos){
	register int i;
	srand(getpid());
	for(i=0;i<N;i++){
		datos[i] = rand()%255;
	}
}