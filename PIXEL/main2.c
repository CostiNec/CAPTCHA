#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bmp_header.h"
#include <math.h>
typedef struct
{
	char numepoza[14];
	unsigned int blue;
	unsigned int green;
	unsigned int red;
	unsigned int p1, p2, p3;
	char pozabonus[15];
} citire;
typedef struct
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
} pixel;

//verifica daca pixelul este diferit de alb 
int check(pixel **v,int i,int j)
{
	if(v[i][j].red!=255 || v[i][j].blue!=255 || v[i][j].green!=255)
		return 1;
	return 0;
}
//verifica daca cifra pe care o gaseste este unu
int checkone(char mat[][100],int i,int j)
{
	int i1;
	for(i1=i;i1<i+5;i1++)
		if(mat[i1][j]!='*')
			return 0;
	for(i1=i;i1<i+5;i1++)
		if(mat[i1][j-1]=='*')
			return 0;
	for(i1=i;i1<i+5;i1++)
		if(mat[i1][j+1]=='*')
			return 0;
	return 1;

}
//identifica cifra pe care o gaseste
int identify(char mat[][100],int i,int j)
{
	if(mat[i][j+2]=='*' && mat[i+2][j+2]!='*' && mat[i+1][j+4]=='*' && 
		mat[i+3][j+4]=='*' && mat[i+4][j+2]=='*' && mat[i+1][j]=='*' &&
								mat[i+3][j]=='*')
		return 0;
	if(mat[i][j+2]=='*' && mat[i+2][j+2]=='*' && mat[i+1][j+4]=='*' &&
		mat[i+3][j+4]!='*' && mat[i+4][j+2]=='*' && mat[i+1][j]!='*' &&
								mat[i+3][j]=='*')
		return 2;
	if(mat[i][j+2]=='*' && mat[i+2][j+2]=='*' && mat[i+1][j+4]=='*' &&
		mat[i+3][j+4]=='*' && mat[i+4][j+2]=='*' && mat[i+1][j]!='*' &&
								mat[i+3][j]!='*')
		return 3;
	if(mat[i][j+2]!='*' && mat[i+2][j+2]=='*' && mat[i+1][j+4]=='*' && 
		mat[i+3][j+4]=='*' && mat[i+4][j+2]!='*' && mat[i+1][j]=='*' &&
								mat[i+3][j]!='*')
		return 4;
	if(mat[i][j+2]=='*' && mat[i+2][j+2]=='*' && mat[i+1][j+4]!='*' && 
		mat[i+3][j+4]=='*' && mat[i+4][j+2]=='*' && mat[i+1][j]=='*' &&
								mat[i+3][j]!='*')
		return 5;
	if(mat[i][j+2]=='*' && mat[i+2][j+2]=='*' && mat[i+1][j+4]!='*' &&
		mat[i+3][j+4]=='*' && mat[i+4][j+2]=='*' && mat[i+1][j]=='*' &&
								mat[i+3][j]=='*')
		return 6;
	if(mat[i][j+2]=='*' && mat[i+2][j+2]!='*' && mat[i+1][j+4]=='*' &&
		mat[i+3][j+4]=='*' && mat[i+4][j+2]!='*' && mat[i+1][j]!='*' &&
								mat[i+3][j]!='*')
		return 7;
	if(mat[i][j+2]=='*' && mat[i+2][j+2]=='*' && mat[i+1][j+4]=='*' &&
		mat[i+3][j+4]=='*' && mat[i+4][j+2]=='*' && mat[i+1][j]=='*' &&
								mat[i+3][j]=='*')
		return 8;
	if(mat[i][j+2]=='*' && mat[i+2][j+2]=='*' && mat[i+1][j+4]=='*' &&
		mat[i+3][j+4]=='*' && mat[i+4][j+2]=='*' && mat[i+1][j]=='*' &&
								mat[i+3][j]!='*')
		return 9;
	return 1;
}
//functia modify hasureaza aria de 5x5 unde este situata cifra gasita
void modify(char mat[][100],int i, int j)
{
	int i1,j1;
	if(mat[i][j]=='*')
		for(i1=i; i1<i+5; i1++)
			for(j1=j; j1<j+5; j1++)
				mat[i1][j1]='x';
}
int main()
{
	//RowSize este folosit pentru padding
	int i,j,RowSize,n=0,m=0;
	//mat este o matrice de tip caracter in care sunt convertiti pixelii
	// in caractere "-"=alb, "*"= alta culoare, iar "x"= hasura 
	char fisier[20],mat[100][100];
	citire date;
	pixel **v;
	FILE *f,*g,*input;
	//deschiderea fisierului de input
	input=fopen("input.txt","rt");
	//citirea datelor din fisierul de input
	fscanf(input, "%s", date.numepoza);
	fscanf(input, "%d %d %d", &date.blue, &date.green, &date.red);
	//deschiderea fisierelui bmp corespunzator
	f=fopen(date.numepoza, "rb");
	//crearea si deschiderea fisierului de output
	strcpy(fisier,date.numepoza);
	for(i = 0; i < strlen(fisier); i++)
	{
		if(fisier[i] == '.')
		fisier[i]=0;
	}
	strcat(fisier,"_task2.txt");
	g=fopen(fisier, "wt");
	//verificarea ca fisierul este bine deschis
	if(!g)
	{
		printf("eroare\n");
		exit (1);
	}
	//citirea datelor din fileheader
	struct bmp_fileheader *fileh;
	fileh = malloc(sizeof(struct bmp_fileheader));
	fread(fileh, sizeof(struct bmp_fileheader), 1, f);
	//citirea datelor din infoheader
	struct bmp_infoheader *filei;
	filei = malloc(sizeof(struct bmp_infoheader));
	fread(filei, sizeof(struct bmp_infoheader), 1,f);
	//alocarea si citirea matricei de pixeli plus alocarea paddingului
	v=(pixel**)malloc(filei->height*sizeof(pixel*));
	for(i = 0; i < filei->height; i++)
		v[i]=(pixel *)malloc(filei-> width*sizeof(pixel *));
	RowSize=floor((3*8*filei->width+31)/32)*4;
	int padding = RowSize - 3 * filei->width;
	for(i = 0; i < filei->height; i++)
	{
		for(j = 0; j < filei->width; j++)
			fread(&v[i][j], sizeof(pixel),1,f);
		fseek(f, padding, SEEK_CUR);
	}
	//m reprezinta numarul de coloane
	//n reprezinta numarul de linii
	m=filei->width;
	//n va creste dupa fiecare linie din matricea de pixeli citita 
	for(i = filei->height -1; i >= 0 ; i--)
	{
		for(j = 0; j < filei->width ; j++)
			if(check(v,i,j)==1)
				mat[n][j]='*';
			else 
				mat[n][j]='-';
		n++;
	}
	//in aceste for-uri matricea este parcursa si la fiecare 1 gasit se va
	// adauga o steluta in plus la o distanta de 3 spatii
	for(j=0; j<m; j++)
		for(i=0; i<=n; i++)
			if(checkone(mat,i,j)==1 && mat[i][j]=='*')
				mat[i][j-4]='*';
	//matricea este parcursa si in momentul in care se gaseste o steluta
	// se va identifica cifra corespunzatoare (la 1 este un caz special insa
	// acesta este tratat mai sus si apoi verificat in acesta parcurgere)
	for(j=0; j<m; j++)
		for(i=0; i<=n; i++)
		{
			if(mat[i][j]=='*')
			{
				fprintf(g,"%d",identify(mat,i,j));
				modify(mat,i,j);
			}
		}
	//eliberarea memoriei si inchiderea fisierelor
	free(filei);
	free(fileh);
	free(v);
	fclose(f);
	fclose(input);
	fclose(g);
}
