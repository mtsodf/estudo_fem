#include <stdlib.h>
#include <stdio.h>
#include "fem.h"
#include <math.h>


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

Triangulo::Triangulo(double x0, double y0, double x1, double y1, double x2, double y2){
	x = (double *) malloc(sizeof(double)*3);
	y = (double *) malloc(sizeof(double)*3);

	x[0]=x0; x[1]=x1; x[2]=x2;
	y[0]=y0; y[1]=y1; y[2]=y2;

	qtd_pontos = 3;


}

double Triangulo:: area(){
	double dx1 = x[1] - x[0];
	double dx2 = x[2] - x[0];

	double dy1 = y[1] - y[0];
	double dy2 = y[2] - y[0];

	return 0.5 * abs( dx1*dy2 - dx2*dy1 );
}


double** Triangulo:: get_M(){
	if(M == NULL){
		M = (double **) malloc(sizeof(double *) * qtd_pontos);
		for (int i = 0; i < qtd_pontos; ++i) M[i] = (double *) malloc(sizeof(double) * qtd_pontos);
		
		M[0][0] =    1; M[1][0] =    1; M[2][0] =    0;
		M[0][1] = x[0]; M[1][1] = x[1]; M[2][1] = x[2];
		M[0][2] = y[0]; M[1][2] = y[1]; M[2][2] = y[2];

	} 	

	return M;
}

double** Triangulo:: get_Minv(){
	if(Minv == NULL){
		double ** M = get_M();
		Minv = (double **) malloc(sizeof(double *) * qtd_pontos);
		
		for (int i = 0; i < qtd_pontos; ++i) Minv[i] = (double *) malloc(sizeof(double) * qtd_pontos);



	} 	

	return Minv;
}