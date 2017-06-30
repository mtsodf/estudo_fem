
static char help[] = "Solves a tridiagonal linear system with KSP.\n\n";
#include <iostream>
#include <petscksp.h>
#include "fem.h"


using namespace std;

int main(int argc, char **args) {
	Vec x;
	PetscInt N; PetscErrorCode ierr;
	PetscInitialize(&argc,&args,(char*)0,help);
	int rank;
	PetscScalar val;
	int i;


	FemCaso fem = FemCaso();

	fem.carregarNos("nos.txt");
	fem.carregarElementos("elementos.txt");


	//Triangulo& a =  fem.elementos[0];
	printf("k = %lf\n", fem.elementos[0].k_func(0,0));

	ierr = VecCreate(PETSC_COMM_WORLD, &x);CHKERRQ(ierr);
	ierr = VecSetSizes(x, PETSC_DECIDE, N);CHKERRQ(ierr);
	ierr = VecSetType(x, "mpi");CHKERRQ(ierr); 
	ierr = VecSetFromOptions(x);CHKERRQ(ierr);	

	Mat matR;
	N = fem.grausLiberdade();
	ierr = MatCreate(PETSC_COMM_WORLD, &matR);
	ierr = MatSetType(matR, MATMPIAIJ);
	ierr = MatSetSizes(matR, PETSC_DECIDE, PETSC_DECIDE, N, N);
	
	Elemento *elem;
	No *no0, *no1;
	for (int i = 0; i < fem.elementos.size(); ++i)
	{	
		printf("Indices %d\n", i);
		printf("Linha %d\n", __LINE__);
		elem = &fem.elementos[i];
		printf("Linha %d\n", __LINE__);

		for (int no0_ind = 0; no0_ind < elem->qtd_nos; ++no0_ind)
		{	
			printf("Linha %d\n", __LINE__);
			no0 = &fem.nos[elem->nos[no0_ind].global_ind];
			printf("Linha %d\n", __LINE__);
			for (int no1_ind = no0_ind; no1_ind < elem->qtd_nos; ++no1_ind)
			{	
				no1 = &fem.nos[elem->nos[no1_ind].global_ind];
				printf("Linha %d\n", __LINE__);
				ierr =  MatSetValue(matR, no0->global_ind, no1->global_ind, elem->matriz_coeff(no0_ind, no1_ind), INSERT_VALUES);
				printf("Linha %d\n", __LINE__);
			}

		}

	}

/*	for (int i = 1; i < N; ++i)
	{
		for (int j = 0; j < i-1; ++j)
		{	
			val = MatGetValue()
			ierr =  MatSetValue(matR, no0.global_ind, no1.global_ind, elem.matriz_coeff(no0_ind, no1_ind), INSERT_VALUES);
		}
	}*/





/*
	ierr = PetscOptionsGetInt(PETSC_NULL, PETSC_NULL, "-N", &N, PETSC_NULL);CHKERRQ(ierr); 
	ierr = VecCreate(PETSC_COMM_WORLD, &x);CHKERRQ(ierr);
	ierr = VecSetSizes(x, PETSC_DECIDE, N);CHKERRQ(ierr);
	ierr = VecSetType(x, "mpi");CHKERRQ(ierr); 
	ierr = VecSetFromOptions(x);CHKERRQ(ierr);

	ierr = MPI_Comm_rank(PETSC_COMM_WORLD, &rank);CHKERRQ(ierr);


	if (rank == 0) {
		for(i = 0, val = 0.0; i < N; i++, val += 10.0) {
			ierr = VecSetValues(x, 1, &i, &val, INSERT_VALUES);CHKERRQ(ierr);
		} 
	}	

	ierr = VecAssemblyBegin(x);CHKERRQ(ierr); 
	ierr = VecAssemblyEnd(x);CHKERRQ(ierr);

	ierr = VecView(x, PETSC_VIEWER_STDOUT_WORLD);


	ierr = VecDestroy(&x);CHKERRQ(ierr);


	ierr = PetscFinalize();CHKERRQ(ierr);


	  Rectangle rect;
	  rect.set_values (3,4);
	  cout << "area: " << rect.area() << "\n";

	return 0;
*/
}