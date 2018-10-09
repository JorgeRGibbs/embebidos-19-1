#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define N 8000
#define NUM_HILOS 4

void *funHilo(void*arg);
pthread_mutex_t bloqueo;
int count;
int main(){
	pthread_t tids[NUM_HILOS];
	int nhs[NUM_HILOS] , *res;
	count = 0;
	pthread_mutex_init(&bloqueo,NULL);
	register int nh;

	printf("creando hilos \n");
	for(nh =0; nh<NUM_HILOS; nh++){
		nhs[nh] = nh;
		pthread_create(&tids[nh], NULL,funHilo,(void*)&nhs[nh]);
	}
	for(nh =0; nh<NUM_HILOS; nh++){

		pthread_join(tids[nh], (void **)&res);
		printf("Hilo %d terminado", *res);
	}
	pthread_mutex_destroy(&bloqueo);
	return 0;
}
void *funHilo(void *arg){

	register int i = 0;
	int nh = *(int*)arg;

	count++;
	printf("Iniciando Hilo %d XD con contador %d\n",nh, count);
	while((--i));                               
	//sleep(5);
	printf("terminado Hilo %d XD con contador %d\n",nh, count);
		pthread_mutex_lock(&bloqueo);
	pthread_exit(arg);
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


