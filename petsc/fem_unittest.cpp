#include <math.h>
#include "gtest/gtest.h"
#include "fem.h"

TEST(Polimorfismo, TrianguloElement){
  double x0=0.0, y0=0.0;
  double x1=2.0, y1=0.0;
  double x2=0.0, y2=1.0;
  
  Elemento* elem = new Triangulo(x0,y0,x1,y1,x2,y2);
  Triangulo* tri = new Triangulo(x0,y0,x1,y1,x2,y2);
  

  //Teste do polimorfismo
  EXPECT_EQ(elem->matriz_coeff(0,0), tri->matriz_coeff(0,0));
  EXPECT_EQ(elem->matriz_coeff(1,0), tri->matriz_coeff(1,0));
  EXPECT_EQ(elem->matriz_coeff(1,2), tri->matriz_coeff(1,2));



  //Teste da matriz M;
  double ** M = tri->get_M();
  double ** Minv = tri->get_Minv();


  EXPECT_EQ(1.0, M[0][0]);
  EXPECT_EQ(1.0, M[0][1]);
  EXPECT_EQ(1.0, M[0][2]);

  EXPECT_EQ(0.0, M[1][0]);
  EXPECT_EQ(2.0, M[1][1]);
  EXPECT_EQ(0.0, M[1][2]);

  EXPECT_EQ(0.0, M[2][0]);
  EXPECT_EQ(0.0, M[2][1]);
  EXPECT_EQ(1.0, M[2][2]);

  EXPECT_EQ(-0.5, Minv[1][0]);
  EXPECT_EQ(-1.0, Minv[2][0]);


  EXPECT_EQ( 0.5, Minv[1][1]);
  EXPECT_EQ( 0.0, Minv[2][1]);


  EXPECT_EQ( 0.0, Minv[1][2]);
  EXPECT_EQ( 1.0, Minv[2][2]);

  //Teste calculo da area.
  EXPECT_EQ(1.0, tri->area());
  EXPECT_EQ(1.0, elem->area());

  //Teste das funcoes de forma
  EXPECT_EQ(1.0, tri->func_form(0, x0, y0));
  EXPECT_EQ(0.0, tri->func_form(0, x1, y1));
  EXPECT_EQ(0.0, tri->func_form(0, x2, y2));

  EXPECT_EQ(0.0, tri->func_form(1, x0, y0));
  EXPECT_EQ(1.0, tri->func_form(1, x1, y1));
  EXPECT_EQ(0.0, tri->func_form(1, x2, y2));

  EXPECT_EQ(0.0, tri->func_form(2, x0, y0));
  EXPECT_EQ(0.0, tri->func_form(2, x1, y1));
  EXPECT_EQ(1.0, tri->func_form(2, x2, y2));


  //Teste dos coeficientes da matriz de rigidez
  EXPECT_EQ(2.5, elem->matriz_coeff(0,0));
  EXPECT_EQ(-.5, elem->matriz_coeff(0,1));
  EXPECT_EQ(-2., elem->matriz_coeff(0,2));


  //Teste do valor do centro do triangulo
  double xm, ym;
  tri->centro(&xm, &ym);
  EXPECT_EQ(2.0/3, xm);
  EXPECT_EQ(1.0/3, ym);



}

TEST(Matematica, Determinante3Teste) {

  double ** A;
  A = (double **) malloc(sizeof(double *) * 3);
  for (int i = 0; i < 3; ++i) A[i] = (double *) malloc(sizeof(double) * 3);

  for (int i = 0; i < 3; ++i)
  {
    for (int j = 0; j < 3; ++j)
    {
          A[i][j] = 0;
    }
  }

  A[0][0]=A[1][1]=A[2][2]=1;
  EXPECT_EQ(1, det3(A));

  A[2][2] = 0.5; A[1][1] = 2; A[0][0] = 3;  
  EXPECT_EQ(3, det3(A));


  A[1][2] = 1000;
  A[0][1] = -10000;
  EXPECT_EQ(3, det3(A)); 


  A[0][0] = 0;
  EXPECT_EQ(0, det3(A)); 

  A[0][0]=1;A[0][1]=2;A[0][2]=3;
  A[1][0]=3;A[1][1]=6;A[1][2]=9;
  A[2][0]=10;A[2][1]=-50;A[2][2]=45;

  EXPECT_EQ(0, det3(A)); 
}

TEST(Matematica, Inversa3Teste) {

  double ** A, **Ainv;
  A = (double **) malloc(sizeof(double *) * 3);
  Ainv = (double **) malloc(sizeof(double *) * 3);

  for (int i = 0; i < 3; ++i) A[i] = (double *) malloc(sizeof(double) * 3);
  for (int i = 0; i < 3; ++i) Ainv[i] = (double *) malloc(sizeof(double) * 3);

  for (int i = 0; i < 3; ++i)
  {
    for (int j = 0; j < 3; ++j)
    {
          A[i][j] = 0;
    }
  }

  A[0][0]=A[1][1]=A[2][2]=1;
  inversa3(A, Ainv);
  EXPECT_EQ(1, Ainv[0][0]);
  EXPECT_EQ(1, Ainv[1][1]);
  EXPECT_EQ(1, Ainv[2][2]);

  EXPECT_EQ(0, Ainv[0][1]);
  EXPECT_EQ(0, Ainv[0][2]);

  EXPECT_EQ(0, Ainv[1][0]);
  EXPECT_EQ(0, Ainv[1][2]);

  EXPECT_EQ(0, Ainv[2][0]);
  EXPECT_EQ(0, Ainv[2][1]);

  A[1][1] = 2;
  inversa3(A, Ainv);
  EXPECT_EQ(1, Ainv[0][0]);
  EXPECT_EQ(0.5, Ainv[1][1]);
  EXPECT_EQ(1, Ainv[2][2]);

  EXPECT_EQ(0, Ainv[0][1]);
  EXPECT_EQ(0, Ainv[0][2]);

  EXPECT_EQ(0, Ainv[1][0]);
  EXPECT_EQ(0, Ainv[1][2]);

  EXPECT_EQ(0, Ainv[2][0]);
  EXPECT_EQ(0, Ainv[2][1]);

  A[0][0]=1;A[0][1]=2;A[0][2]=3;
  A[1][0]=4;A[1][1]=5;A[1][2]=6;
  A[2][0]=7;A[2][1]=8;A[2][2]=8;
  inversa3(A, Ainv);

  EXPECT_TRUE(fabs(Ainv[0][0] + 2.666667) <  1e-4);
  EXPECT_TRUE(fabs(Ainv[0][1] - 2.666667) <  1e-4);
  EXPECT_TRUE(fabs(Ainv[0][2] + 1.000000) <  1e-4);

  EXPECT_TRUE(fabs(Ainv[1][0] - 3.333333) <  1e-4);
  EXPECT_TRUE(fabs(Ainv[1][1] + 4.333333) <  1e-4);
  EXPECT_TRUE(fabs(Ainv[1][2] - 2.000000) <  1e-4);

  EXPECT_TRUE(fabs(Ainv[2][0] + 1.000000) <  1e-4);
  EXPECT_TRUE(fabs(Ainv[2][1] - 2.000000) <  1e-4);
  EXPECT_TRUE(fabs(Ainv[2][2] + 1.000000) <  1e-4);
}



TEST(ElementoTestes, TrianguloTeste) {

  Triangulo t = Triangulo(0,0,1,0,0,2);

  EXPECT_EQ(1, t.area());

  Triangulo t2 = Triangulo(1,2,2,4,3,2);
  EXPECT_EQ(2, t2.area());

}


