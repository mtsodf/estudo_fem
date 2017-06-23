#include <stdlib.h>
#include <stdio.h>
#include "fem.h"
#include <math.h>
#include <stdexcept>


double det3(double ** A){
	double det = 0;
	return ((A[0][0]*A[1][1]*A[2][2])+(A[0][1]*A[1][2]*A[2][0])+(A[0][2]*A[1][0]*A[2][1]))-((A[2][0]*A[1][1]*A[0][2])+(A[2][1]*A[1][2]*A[0][0])+(A[2][2]*A[1][0]*A[0][1])); 
}

void inversa3(double **A, double **Ainv){
	double det = det3(A);

	if(det == 0){
		return;
	}

	Ainv[0][0] =   A[1][1]*A[2][2] - A[1][2]*A[2][1];
	Ainv[1][0] = - A[0][1]*A[2][2] + A[2][1]*A[0][2];
	Ainv[2][0] =   A[0][1]*A[1][2] - A[1][1]*A[0][2];


	Ainv[0][1] = - A[1][0]*A[2][2] + A[2][0]*A[1][2];
	Ainv[1][1] =   A[0][0]*A[2][2] - A[2][0]*A[0][2];
	Ainv[2][1] = - A[0][0]*A[1][2] + A[1][0]*A[0][2];

	Ainv[0][2] =   A[1][0]*A[2][1] - A[2][0]*A[1][1];
	Ainv[1][2] = - A[0][0]*A[2][1] + A[2][0]*A[0][1];
	Ainv[2][2] =   A[0][0]*A[1][1] - A[1][0]*A[0][1];

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			Ainv[i][j] /= det;
		}
	}
	
}

No::No(double x, double y){
	this->x=x;
	this->y=y;
}

void FemCaso::carregarNos(char* filename){
	FILE *arq;
	double x0, y0, x1, y1, x2, y2;
	arq = fopen(filename, "r");

	int qtd_nos;

	nos.reserve(qtd_nos);    // make room for 10 elements


	fscanf(arq, "%d", &qtd_nos);
	for (int i = 0; i < qtd_nos; ++i)
	{
		fscanf(arq, "%lf %lf", &x0, &y0);
		nos.push_back(No(x0, y0));
	}
	

	fclose(arq);
}

void FemCaso::carregarElementos(char* filename){
	FILE *arq;
	arq = fopen(filename, "r");
	int no_ind;
	int qtd_elems;

	nos.reserve(qtd_elems);    // make room for 10 elements


	fscanf(arq, "%d", &qtd_elems);
	for (int i = 0; i < qtd_elems; ++i)
	{	
		Triangulo elem = Triangulo();
		for (int i = 0; i < 3; ++i)
		{
			fscanf(arq, "%d", &no_ind);
		}
		
		elementos.push_back(elem);
	}
	

	fclose(arq);
}

Elemento::~Elemento(){

}

double Elemento::func_form(int pos, double x, double y){
	return 0.0;
}

double Elemento::dfunc_form(int pos, int var, double x, double y){
	return 0.0;
	throw std::invalid_argument( "dfunc_form so pode ter var=0 ou var=1" );
}

Triangulo::~Triangulo(){

}

Triangulo::Triangulo(double x0, double y0, double x1, double y1, double x2, double y2){

	nos.reserve(3);

	nos.push_back(No(x0,y0));
	nos.push_back(No(x1,y1));
	nos.push_back(No(x2,y2));

	qtd_nos = 3;

}

double k_func_2(double x, double y){
	return 2;
}

Triangulo::Triangulo(){
	nos.reserve(3);
	qtd_nos = 3;
	k_func = k_func_2;

}



double Triangulo:: area(){
	double dx1 = nos[1].x - nos[0].x;
	double dx2 = nos[2].x - nos[0].x;

	double dy1 = nos[1].y - nos[0].y;
	double dy2 = nos[2].y - nos[0].y;

	return 0.5 * abs( dx1*dy2 - dx2*dy1 );
}


double** Triangulo:: get_M(){
	if(M == NULL){
		M = (double **) malloc(sizeof(double *) * qtd_nos);
		for (int i = 0; i < qtd_nos; ++i) M[i] = (double *) malloc(sizeof(double) * qtd_nos);
		
		M[0][0] =        1; M[1][0] =        1; M[2][0] =    0;
		M[0][1] = nos[0].x; M[1][1] = nos[1].x; M[2][1] = nos[2].x;
		M[0][2] = nos[0].y; M[1][2] = nos[1].y; M[2][2] = nos[2].y;

	} 	

	return M;
}

double Triangulo::func_form(int pos, double x, double y){
	double ** Minv = get_Minv();

	return Minv[0][pos] + Minv[1][pos]*x + Minv[2][pos]*y;
}

double Triangulo::dfunc_form(int pos, int var, double x, double y){
	double ** Minv = get_Minv();
	//derivada em relacao a x
	if(var == 0 || var == 1){
		return M[var+1][pos];
	}


	throw std::invalid_argument( "dfunc_form so pode ter var=0 ou var=1" );

}

double** Triangulo:: get_Minv(){
	if(Minv == NULL){
		double ** M = get_M();
		Minv = (double **) malloc(sizeof(double *) * qtd_nos);
		
		for (int i = 0; i < qtd_nos; ++i) Minv[i] = (double *) malloc(sizeof(double) * qtd_nos);

	} 	

	return Minv;
}