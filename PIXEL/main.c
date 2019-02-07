#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bmp_header.h"
#include <math.h>
//structura care retine datele de intrare citite din fisierul de input
typedef struct
	{
		char numepoza[14];
		unsigned int blue;
		unsigned int green;
		unsigned int red;
		unsigned int p1, p2, p3;
		char pozabonus[15];
	}citire;
//structura care retine fiecare pixel in functie de fiecare componenta a acestuia
typedef struct
	{
		unsigned char blue;
		unsigned char green;
		unsigned char red;
	}pixel;
int main()
{	
	//RowSize este folosit pentru padding
	int i,j,RowSize;
	//fisier reprezinta numele fisierului final care va trebui creat de program
	char fisier[20];
	//variabila de tip "citire" in care sunt stocate datele
	citire date;
	//matrice de tipul "pixel" in care este citita matricea de pixel din fisier
	pixel **v;
	FILE *f,*g,*input;
	input=fopen("input.txt","rt");
	fscanf(input, "%s", date.numepoza);
	fscanf(input, "%d %d %d", &date.blue, &date.green, &date.red);
	f=fopen(date.numepoza, "rb");
	//declararea si citirea variabile "fileh" de tipul "bmp_fileheader"
	// in care sunt stocate informatiile corespunzatoare din fileheader
    	struct bmp_fileheader *fileh;
	fileh = malloc(sizeof(struct bmp_fileheader));
	fread(fileh, sizeof(struct bmp_fileheader), 1, f);
	//declararea si citirea variabilei "filei" de tipu "bmp_infoheader"
	// in care sunt stocate informatiile corespunzatoare din infoheader
	struct bmp_infoheader *filei;
	filei = malloc(sizeof(struct bmp_infoheader));
	fread(filei, sizeof(struct bmp_infoheader), 1 ,f);
	//alocarea matricei de tip pixel
	v=(pixel**)malloc(filei->height*sizeof(pixel*));
	for(i = 0; i < filei->height; i++)
		v[i]=(pixel *)malloc(filei-> width*sizeof(pixel *));
	//formarea de padding
	RowSize=floor((3*8*filei->width+31)/32)*4;
	int padding = RowSize - 3 * filei->width;
	//citirea matricei de pixeli si aplicarea paddingului
	for(i = 0; i < filei->height; i++)
	{
		for(j = 0; j < filei->width; j++)
			fread(&v[i][j], sizeof(pixel),1,f);
	        fseek(f, padding, SEEK_CUR);
	}
	//schimbarea culorilor in functie de valorile date la input
	for(i = 0; i < filei->height; i++)
		for(j = 0; j < filei->width ; j++)
			if(v[i][j].blue != 255 || v[i][j].green != 255 || 
							v[i][j].red != 255)
			{
				v[i][j].blue=date.blue;
				v[i][j].green=date.green;
				v[i][j].red=date.red;
			}
	//formarea noului fisier de output
	strcpy(fisier,date.numepoza);
	for(i = 0; i < strlen(fisier); i++)
	{
		if(fisier[i] == '.')
			fisier[i]=0;
	}
	strcat(fisier,"_task1.bmp");
	//deschiderea noului fisier
	g=fopen(fisier, "wb");
	//verificarea daca s-a deschis corect
	if(!g) 
	{
		printf("eroare\n");
		exit (1);
	}
	//scrierea fileheaderului si infoheaderului
	fwrite(fileh, sizeof(struct bmp_fileheader), 1, g);
	fwrite(filei, sizeof(struct bmp_infoheader), 1, g);
	//scirerea noii matrice de pixeli
	for(i = 0; i < filei->height; i++)
	{
		for(j = 0; j < filei->width ; j++)
			fwrite(&v[i][j], sizeof(pixel), 1, g);
        	int x = 0;
        	fwrite(&x,padding,1,g);
	}
	//eliberarea memoriei si inchiderea fisierelor
	free(filei);
	free(fileh);
	free(v);
	fclose(f);
	fclose(input);
}
