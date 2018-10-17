#include <stdio.h>
#include <stdlib.h>
#include "imagen.h"
#define DIMASK 3

unsigned char * RGBtoGray (unsigned char *imagenRGB,
	uint32_t width, uint32_t height);
void GraytoRGB (unsigned char *imagenGray, unsigned char *imagenRGB,
	uint32_t width, uint32_t height);
void brilloImagen(unsigned char *,unsigned char*, uint32_t width,uint32_t height);
void filtroImagen(unsigned char *,unsigned char*, uint32_t width,uint32_t height);
unsigned char * reservarMemoria(uint32_t width, uint32_t height);
bmpInfoHeader info;

int main()
{
	unsigned char *imagenRGB, *imagenGray, *imagenFiltro;
	imagenRGB = abrirBMP("hoja.bmp", &info);
	displayInfo( &info );
	printf("Abriendo imagen RGB\n");
	imagenGray = RGBtoGray( imagenRGB,info.width,info.height);
	//procesamiento
	brilloImagen(imagenGray,imagenRGB, info.width,info.height);
	filtroImagen(imagenGray,imagenFiltro,info.width,info.height);
	imagenFiltro = reservarMemoria(info.width,info.height);
	GraytoRGB( imagenGray , imagenRGB,info.width,info.height);
	guardarBMP("hojaGray.bmp",&info,imagenRGB);
	free (imagenRGB);
	free (imagenGray);
	free (imagenFiltro);
	return 0;
}

unsigned char * reservarMemoria(uint32_t width,uint32_t height)
{
	unsigned char *mem;
	mem = (unsigned char *)malloc(width*height*sizeof(unsigned char));
		if (!mem)
		{
			perror("Error al asignar memoria");
			exit(EXIT_FAILURE);
		}
		return mem;
}

void filtroImagen(unsigned char *imagenGray,unsigned char *imagenFiltro,uint32_t width,uint32_t height)
{
	register int x,y,xm,ym;
	int conv1, indice,indicem;
	unsigned char mascara[] =
	{1,1,1,
	 1,1,1,
	 1,1,1,	
	};

	//Máscaras GradF y G
	for (y = 0; y < height-DIMASK ; y++)
	{
		for (x = 0; x < width-DIMASK ; x++)
		{
			conv1 = 0;
			for(ym = 0; ym < DIMASK; ym++)
				for(xm = 0; xm < DIMASK; xm++)
				{
					indice = ((y+ym)*width+(x+xm));
					conv1+= imagenGray[indice]*GradF[indicem++];
					//conv+= imagenGray[indice]*mascara[indicem++];
					//magnitud= sqrt()
				}
		}
		//conv = conv /9;
		conv1 = conv1 /4;
		conv2 = conv2 /4;

		indice = ((y+1)*width+(x+1));
		imagenFitro[indice] = magnitud;
	}
}


void brilloImagen (unsigned char * imagenGray,unsigned char* imagenRGB ,uint32_t width, uint32_t height)
{
	register int p;
	unsigned short int pixel;
	for (int i = 0; i < width*height; p++)
	{
		pixel = imagenGray[p]+80;
		imagenGray[p] = (pixel > 255)? 255: (unsigned char)pixel;
	}
}

unsigned char * RGBtoGray (unsigned char *imagenRGB,uint32_t width,uint32_t height)
	{
		register int x,y;
		int indiceRGB, indiceGray;
		unsigned char GrayLevel;
		unsigned char *imagenGray;
		imagenGray = reservarMemoria(info.width,info.height);
		if (imagenGray == NULL)
		{
			perror("Error al asignar memoria");
			exit(EXIT_FAILURE);
		}
		for ( y = 0; y < height ; y++)
		{
			for ( x = 0; x < width; x++)
			{
				indiceGray = (y*width+x); 
				indiceRGB = ((indiceGray << 1) + indiceGray); //indiceGray*3
				//GrayLevel = (imagenRGB[indiceRGB] + imagenRGB[indiceRGB+1] + imagenRGB[indiceRGB+2])/3;
				//GrayLevel = ((0.3*imagenRGB[indiceRGB]) + (0.59*imagenRGB[indiceRGB+1]) + (0.11)*imagenRGB[indiceRGB+2]);
				GrayLevel = ((30*imagenRGB[indiceRGB]) + (59*imagenRGB[indiceRGB+1]) + (11)*imagenRGB[indiceRGB+2])/100;
				imagenGray[indiceGray] = GrayLevel;
			}
		}
		return imagenGray;
	}

void GraytoRGB ( unsigned char *imagenGray, unsigned char *imagenRGB,uint32_t width,uint32_t height)
{
		register int x,y;
		int indiceRGB, indiceGray;

		for ( y = 0; y < height ; y++)
		{
			for ( x = 0; x < width; x++)
			{
				indiceGray = (y*width+x); 
				indiceRGB = indiceGray*3;
				imagenRGB[indiceRGB+0] = imagenGray[indiceGray];
				imagenRGB[indiceRGB+1] = imagenGray[indiceGray];
				imagenRGB[indiceRGB+2] = imagenGray[indiceGray];
			}
		}
}