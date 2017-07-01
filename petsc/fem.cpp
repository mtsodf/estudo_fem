#include <stdlib.h>
#include <stdio.h>
#include "fem.h"
#include <cmath>
#include <stdexcept>


double default_k(double x, double y){
    return 2.0;
}



//Implementacoes
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


No::No(double x, double y, tipo_no tipo, double contorno){
    this->x=x;
    this->y=y;
    this->tipo = tipo;
    this->contorno = contorno;
}


void FemCaso::carregarNos(char* filename){
    FILE *arq;
    double x0, y0;
    double contorno;
    int tipo;
    arq = fopen(filename, "r");

    int qtd_nos;


    fscanf(arq, "%d", &qtd_nos);

    nos.reserve(qtd_nos);    // make room for 10 elements


    for (int i = 0; i < qtd_nos; ++i)
    {   
        fscanf(arq, "%lf %lf %d %lf", &x0, &y0, &tipo, &contorno);
        No* no = new No(x0, y0, (tipo_no) tipo, contorno);
        no->global_ind = i;
        nos.push_back(no);
    }
    

    fclose(arq);
}


void FemCaso::carregarElementos(char* filename, double (*k_func) (double,double) , double (*lado_direito) (double,double)){
    FILE *arq;
    arq = fopen(filename, "r");
    int no_ind;
    int qtd_elems;


    fscanf(arq, "%d", &qtd_elems);

    elementos.reserve(qtd_elems);    
    for (int i = 0; i < qtd_elems; ++i)
    {   
        Triangulo* elem = new Triangulo();
        elem->k_func = k_func;
        elem->lado_direito = lado_direito;
        for (int j = 0; j < 3; ++j)
        {
            fscanf(arq, "%d", &no_ind);
            elem->nos.push_back(nos[no_ind]);
            //printf("Adicionando no %d ao elemento %d. Local %d\n", no_ind, i, j);
        }
        elementos.push_back(elem);
    }

    fclose(arq);
}


int FemCaso::grausLiberdade(){
    int n = 0;

    for (int i = 0; i < nos.size(); ++i)
    {
        if(nos[i]->eh_livre()) nos[i]->matriz_ind = n++;
    }

    return n;
}

Elemento::~Elemento(){

}


double Elemento::func_form(int pos, double x, double y){
    throw std::invalid_argument( "Nao implementado func_form" );
    return 0.0;
}


double Elemento::dfunc_form(int pos, int var, double x, double y){
    throw std::invalid_argument( "Nao implementado dfunc_form"  );
    return 0.0;
}


double Elemento::matriz_coeff(int vert_ind_0, int vert_ind_1){
    throw std::invalid_argument( "Nao implementado matriz_coeff" );
    return 0.0;
}

double Elemento::fem_lado_direito(int vert_ind){
    throw std::invalid_argument( "Nao implementado fem_lado_direito" );
    return 0.0;
}

void Elemento::g_grad(double x, double y, double *g0, double *g1){
    throw std::invalid_argument( "Nao implementado g_grad" );
}

double Elemento::area(){
    throw std::invalid_argument( "Nao implementado area para elemento"  );
    return 0.0; 
}

void Elemento::printNos(){
    for (int i = 0; i < nos.size(); ++i)
    {
        printf("No(%d) = (%lf, %lf)\n", i, nos[i]->x, nos[i]->y);
    }
}


Triangulo::~Triangulo(){

}

Triangulo::Triangulo(double x0, double y0, double x1, double y1, double x2, double y2){

    nos.reserve(3);

    nos.push_back(new No(x0,y0, livre, 0.0));
    nos.push_back(new No(x1,y1, livre, 0.0));
    nos.push_back(new No(x2,y2, livre, 0.0));

    qtd_nos = 3;

    k_func = default_k;
    M = NULL;
    Minv = NULL;
}


Triangulo::Triangulo(){
    nos.reserve(3);
    qtd_nos = 3;
    M = NULL;
    Minv = NULL;
}


double Triangulo:: area(){
    double dx1 = nos[1]->x - nos[0]->x;
    double dx2 = nos[2]->x - nos[0]->x;

    double dy1 = nos[1]->y - nos[0]->y;
    double dy2 = nos[2]->y - nos[0]->y;

    return 0.5 * fabs( dx1*dy2 - dx2*dy1 );
}


double** Triangulo:: get_M(){
    if(M == NULL){
        M = (double **) malloc(sizeof(double *) * qtd_nos);
        for (int i = 0; i < qtd_nos; ++i) M[i] = (double *) malloc(sizeof(double) * qtd_nos);
        
        M[0][0] =       1.0; M[0][1] =       1.0; M[0][2] =       1.0;
        M[1][0] = nos[0]->x; M[1][1] = nos[1]->x; M[1][2] = nos[2]->x;
        M[2][0] = nos[0]->y; M[2][1] = nos[1]->y; M[2][2] = nos[2]->y;

    }   

    return M;
}


double Triangulo::func_form(int pos, double x, double y){
    get_Minv(); 
    return Minv[0][pos] + Minv[1][pos]*x + Minv[2][pos]*y;
}


double Triangulo::dfunc_form(int pos, int var, double x, double y){
    get_Minv();

    //derivada em relacao a x
    if(var == 0 || var == 1){
        return Minv[var+1][pos];
    }


    throw std::invalid_argument( "dfunc_form so pode ter var=0 ou var=1" );

}

void Triangulo::centro(double* xm, double* ym){
    baric2cart(1.0/3, 1.0/3, 1.0/3, xm, ym);

    return ;
    *xm = (1.0/3) * (nos[0]->x + nos[1]->x + nos[2]->x);
    *ym = (1.0/3) * (nos[0]->y + nos[1]->y + nos[2]->y);

}

//Transforma coordenadas baricentricas em cartesianas
void Triangulo::baric2cart(double coord_baricentricas_0, double coord_baricentricas_1, double coord_baricentricas_2, double* x, double* y){
    *x = coord_baricentricas_0*nos[0]->x + coord_baricentricas_1*nos[1]->x + coord_baricentricas_2*nos[2]->x;
    *y = coord_baricentricas_0*nos[0]->y + coord_baricentricas_1*nos[1]->y + coord_baricentricas_2*nos[2]->y;
}


double Triangulo::matriz_coeff(int vert_ind_0, int vert_ind_1){
    double grad0[2], grad1[2];

    grad0[0] = dfunc_form(vert_ind_0, 0, 0.0, 0.0); 
    grad0[1] = dfunc_form(vert_ind_0, 1, 0.0, 0.0); 
    grad1[0] = dfunc_form(vert_ind_1, 0, 0.0, 0.0); 
    grad1[1] = dfunc_form(vert_ind_1, 1, 0.0, 0.0); 

//  printf("grad_%d = (%lf, %lf)\n", vert_ind_0, grad0[0], grad0[1]);
//  printf("grad_%d = (%lf, %lf)\n", vert_ind_1, grad1[0], grad1[1]);
//  printf("area    = %e\n", area());

    double coeff = grad0[0]*grad1[0] + grad0[1]*grad1[1];

    coeff *= area();

    double xm, ym;

    centro(&xm, &ym);

//  printf("k_func  = %lf\n", k_func(xm, ym));
    coeff *= k_func(xm, ym);

    return coeff;

}


double Triangulo::fem_lado_direito(int vert_ind){
    double xm, ym;
    centro(&xm, &ym);

    //parte relativa a funcao f
    double ld_coeff = area()*lado_direito(xm,ym)*1.0/3;

    //relativa a condicao de contorno de dirichlet
    double g0, g1, dphi0, dphi1;
    
    dphi0 = dfunc_form(vert_ind, 0, xm, ym);
    dphi1 = dfunc_form(vert_ind, 1, xm, ym);

    g_grad(xm, ym, &g0, &g1);
    ld_coeff -= (g0*dphi0 + g1*dphi1) * area() * k_func(xm, ym);

    return ld_coeff;
}

void Triangulo::g_grad(double x, double y, double *g0, double *g1){

    *g0 = 0; *g1 = 0;

    for (int i = 0; i < nos.size(); ++i)
    {
        No* no = nos[i];
        if(no->tipo == dirichlet){
            *g0 += dfunc_form(i, 0, x, y) * no->contorno;
            *g1 += dfunc_form(i, 1, x, y) * no->contorno;
        }

    }
}

double** Triangulo:: get_Minv(){
    if(Minv == NULL){
        get_M();
        Minv = (double **) malloc(sizeof(double *) * qtd_nos);
        
        for (int i = 0; i < qtd_nos; ++i) Minv[i] = (double *) malloc(sizeof(double) * qtd_nos);

        inversa3(M, Minv);
    }   

    return Minv;
}


bool No::eh_livre(){

    return tipo==livre || tipo==neumann;

}