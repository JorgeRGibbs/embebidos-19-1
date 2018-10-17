#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "imagen.h"
#include <stdint.h>
#include <pthread.h>
#include <stdint.h>

#define DIMASK 3 //especifica las dimensiones de la mascara, aqui especificamos una de 3x3
#define NUM_HILOS 4

unsigned char *RGBtoGray( unsigned char *imagenRGB, uint32_t width, uint32_t height);
void GraytoRGB( unsigned char *imagenGray, unsigned char *imagenRGB, uint32_t width, uint32_t height);
void brilloImagen (unsigned char *imagenGray, uint32_t width, uint32_t height);
unsigned char *reservarMemoria(uint32_t width, uint32_t height);
void filtroImagen(unsigned char *imagenGray, unsigned char *imagenFiltro, uint32_t width, uint32_t height);
void * funhilo(void *);
bmpInfoHeader info;
unsigned char *imagenRGB, *imagenGray, *imagenFiltro;
//int altura = *(int*)info.height;

int main()
{
	imagenRGB = abrirBMP("calle1.bmp", &info);
	displayInfo( &info );

	imagenGray = RGBtoGray( imagenRGB,  info.width, info.height);

	//procesamiento
	//brilloImagen( imagenGray, info.width, info.height);
	imagenFiltro = reservarMemoria (info.width, info.height);
    filtroImagen( imagenGray, imagenFiltro, info.width, info.height);

	//------------------------------------------------------
	GraytoRGB(imagenFiltro, imagenRGB, info.width, info.height) ;
	guardarBMP( "SobelconHilos.bmp", &info, imagenRGB);//filtro pasabajas
	free(imagenRGB);
	free(imagenGray);
	//free(imagenFiltro);
	return 0;
}
void * funHilo(void *arg){
	//int altura  = (int*)info.height;
	int nh = *(int*)arg;
	int tamBloque = 0;
	tamBloque = info.height / NUM_HILOS;
	int iniBloque = nh * tamBloque;
	int finBloque = iniBloque + tamBloque;
	register int i=0;
	register int x,y,xm,ym;
	pthread_mutex_t bloqueo;
	int conv,conv1,conv2;
		char GradF[] =
				{1,0,-1,
				 2,0,-2,
				 1,0,-1
				};
	char GradC[] =
				{-1,-2,-1,
				  0, 0,-0,
				  1, 2, 1
				};
	register int indice,indicem;
	//int *A;
	//while((--i));
	for( y = iniBloque; y <= finBloque; y++)//
		for ( x = 0; x <= (info.width-DIMASK); x++) //
		{
				conv = 0;
				int magnitud=0;
				for(ym=y;ym< y+DIMASK;ym++)//for (ym = 0; ym < DIMASK; ym++ )
				{	
					for(xm=x;xm< x+DIMASK;xm++)//for (xm = 0; xm < DIMASK; xm++ )//mascara
					{
						//indice = (width * (y +ym) + (x+xm));
						indice = ym*info.width+xm;
						indicem = (ym-y)*DIMASK+(xm-x);
						conv1 += imagenGray[indice]*GradF[indicem];
						conv2 += imagenGray[indice]*GradC[indicem];
					}
				}
					conv1 /= 4;//la division se hace con la suma de los coeficientes de la mascara, en este caso 9.
					conv2 /= 4;
					int add = pow(conv1,2)+pow(conv2,2);
					magnitud = sqrt(add);

					indice = ((y+1)*info.width + (x+1));
					imagenFiltro[indice]= magnitud;
		}

	/*///////////
	for (i = nh ; i < N ; i+= NUM_HILOS)
	{
		AB[i]=A[i]*B[i];
		suma += AB[i];
	}*/
	//pthread_mutex_lock(&bloqueo);
	//prom += suma;
	//printf("suma: %d\n",prom);
	if (nh == 3)
	{
		printf("complete\n");
	}
	//pthread_mutex_unlock(&bloqueo);
	
	pthread_exit(imagenFiltro);
}

void filtroImagen(unsigned char *imagenGray, unsigned char *imagenFiltro, uint32_t width, uint32_t height)
{
	register int nh = 0;
	int nhs[NUM_HILOS];
	int conv,conv1,conv2, indice,indicem;
	pthread_t tids[NUM_HILOS];
	/*unsigned char mascara[] =
							{1,1,1,
							 1,1,1,
	    					 1,1,1};*/


	for(nh =0; nh<NUM_HILOS; nh++){
		nhs[nh] = nh;
		pthread_create(&tids[nh], NULL,funHilo,(void*)&nhs[nh]);
		//printf("Hilos creados\n");
	}

	for(nh =0; nh<NUM_HILOS; nh++){

		pthread_join(tids[nh], (void **)imagenFiltro);
		//printf("Hilo %d terminado\n", nh);
	}
}




unsigned char *reservarMemoria(uint32_t width, uint32_t height) {
	unsigned char *imagen;
	imagen = (unsigned char*) malloc(width*height*sizeof(unsigned char));
	if(imagen == NULL)
	{
		perror("Error al asignar memoria \n");
		exit(EXIT_FAILURE);
	}
	return imagen;
}



unsigned char *RGBtoGray( unsigned char *imagenRGB, uint32_t width, uint32_t height)
{
	register int y,x ;
	int indiceRGB, indiceGray;
	unsigned char grayLevel;
	unsigned char *imagenGray;
	imagenGray = reservarMemoria(width,height);
	if(imagenGray == NULL)
	{
		perror("Error al asignar memoria \n");
		exit(EXIT_FAILURE);
	}

	for ( y = 0; y < height ; y++ )
		for ( x = 0; x < width; x++ )
		{
			indiceGray = (width * y + x);
			//indiceRGB = indiceGray * 3 ;//multiplicar por 3 con corrimientos
			indiceRGB = ((indiceGray << 1) + indiceGray);
			grayLevel = (30*imagenRGB[indiceRGB]+59*imagenRGB[indiceRGB+1]+11*imagenRGB[indiceRGB+2])/100;
			imagenGray[indiceGray] = grayLevel;
		}
	return imagenGray;
}

void brilloImagen (unsigned char *imagenGray, uint32_t width, uint32_t height)
{
	register int p;
	unsigned short int pixel;
	for (p = 0 ; p < width*height ; p++ )
	{
		pixel = imagenGray[p] + 80;
		imagenGray[p] = (pixel>255)? 255 : (unsigned char)pixel;//si el valor de pixel es mayor a 255, devuelve 255 sino se castea el valor de pixel a unsigned char y se guarda en imagenGray.
	}
}

void GraytoRGB( unsigned char *imagenGray, unsigned char *imagenRGB, uint32_t width, uint32_t height){
	register int y,x ;
	int indiceRGB, indiceGray;
	//imagenGray = (unsigned char*) malloc(width*height*sizeof(unsigned char));
        for ( y = 0; y < height ; y++ )
                for ( x = 0; x < width; x++ )
                {
			indiceGray = (y*width+x);
			indiceRGB = indiceGray * 3;

			imagenRGB[indiceRGB+0] = imagenGray [indiceGray];
			imagenRGB[indiceRGB+1] = imagenGray [indiceGray];
			imagenRGB[indiceRGB+2] = imagenGray [indiceGray];

		}
}
