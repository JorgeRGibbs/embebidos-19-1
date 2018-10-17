#include <stdio.h>
#include <stdlib.h>
#include "imagen.h"
#include <stdint.h>

#define DIMASK 5 //especifica las dimensiones de la mascara, aqui especificamos una de 3x3

unsigned char *RGBtoGray( unsigned char *imagenRGB, uint32_t width, uint32_t height);
void GraytoRGB( unsigned char *imagenGray, unsigned char *imagenRGB, uint32_t width, uint32_t height);
void brilloImagen (unsigned char *imagenGray, uint32_t width, uint32_t height);
unsigned char *reservarMemoria(uint32_t width, uint32_t height);
void filtroImagen(unsigned char *imagenGray, unsigned char *imagenFiltro, uint32_t width, uint32_t height);
int main()
{
	bmpInfoHeader info;
	unsigned char *imagenRGB, *imagenGray, *imagenFiltro;
	printf("Abriendo la imagen !!! \n");
	imagenRGB = abrirBMP("calle1.bmp", &info);
	displayInfo( &info );

	imagenGray = RGBtoGray( imagenRGB,  info.width, info.height);

	//procesamiento
	//brilloImagen( imagenGray, info.width, info.height);
	imagenFiltro = reservarMemoria (info.width, info.height);
   filtroImagen( imagenGray, imagenFiltro, info.width, info.height);

	//------------------------------------------------------
	GraytoRGB(imagenFiltro, imagenRGB, info.width, info.height) ;

	guardarBMP( "Gaussian.bmp", &info, imagenRGB);//filtro pasabajas
	free(imagenRGB);
	free(imagenGray);
	//free(imagenFiltro);
	return 0;
}

void filtroImagen(unsigned char *imagenGray, unsigned char *imagenFiltro, uint32_t width, uint32_t height)
{
	register int x,y,xm,ym;
	int conv, indice,indicem;
	unsigned char mascara[] =
							{
							 1,1,1,1,1,
							 1,1,1,1,1,
	    					 1,1,1,1,1,
	    					 1,1,1,1,1,
	    					 1,1,1,1,1,
	    					};
	for( y = 0; y <= (height-DIMASK); y++)//Con esta pareja de ciclos se recorre la imagen
		for ( x = 0; x <= (width-DIMASK); x++) //por completo
		{
				conv = 0;
				for(ym=y;ym< y+DIMASK;ym++)//for (ym = 0; ym < DIMASK; ym++ )//con esta pareja podemos recorrer
				{
					for(xm=x;xm< x+DIMASK;xm++)//for (xm = 0; xm < DIMASK; xm++ )//la mascara
					{
						//indice = (width * (y +ym) + (x+xm));
						indice = ym*width+xm;
						indicem = (ym-y)*DIMASK+(xm-x);
						conv += imagenGray[indice]*mascara[indicem];
						//conv += imagenGray[indice] * mascara[indicem++];
					}
				}
					conv /= DIMASK * DIMASK;//la division se hace con la suma de los coeficientes de la mascara, en este caso 9.
					indice = ((y+1)*width + (x+1));
					imagenFiltro[indice]= conv;
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
