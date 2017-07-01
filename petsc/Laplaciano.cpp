
static char help[] = "Solves a tridiagonal linear system with KSP.\n\n";
#include <iostream>
#include <petscksp.h>
#include "fem.h"


using namespace std;


double solucao(double x, double y){
	return x*y*(1-x)*(1-y);
}


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
	printf("k = %lf\n", fem.elementos[0]->k_func(0,0));

	ierr = VecCreate(PETSC_COMM_WORLD, &x);CHKERRQ(ierr);
	ierr = VecSetSizes(x, PETSC_DECIDE, N);CHKERRQ(ierr);
	ierr = VecSetType(x, "mpi");CHKERRQ(ierr); 
	ierr = VecSetFromOptions(x);CHKERRQ(ierr);	

	Mat matR;
	N = fem.grausLiberdade();
	printf("Quantidade de Graus de Liberdade %d\n", N);

	//Calcular matriz de rigidez;
	ierr = MatCreate(PETSC_COMM_WORLD, &matR);
	ierr = MatSetType(matR, MATMPIAIJ);
	ierr = MatSetSizes(matR, PETSC_DECIDE, PETSC_DECIDE, N, N);
	ierr = MatMPIAIJSetPreallocation(matR, 7, NULL, 7, NULL);
	
	Elemento *elem;
	No *no0, *no1;
	double coeff;
	for (int i = 0; i < fem.elementos.size(); ++i)
	{	
		elem = fem.elementos[i];
		if(N<10) printf("Elemento %d\n", i);

		for (int no0_ind_local = 0; no0_ind_local < elem->qtd_nos; ++no0_ind_local)
		{	
			no0 = (elem->nos)[no0_ind_local];
			if(no0->eh_livre()){
				for (int no1_ind_local = 0; no1_ind_local < elem->qtd_nos; ++no1_ind_local)
				{	
					no1 = (elem->nos)[no1_ind_local];
					if(no1->eh_livre()){
						no1 = (elem->nos)[no1_ind_local];
						coeff = elem->matriz_coeff(no0_ind_local, no1_ind_local);
						//printf("\tConexao(%d, %d) = %e\n", no0->global_ind, no1->global_ind, coeff);
						if(N<10) printf("\tConexao(%d, %d) = %e\n", no0->matriz_ind, no1->matriz_ind, coeff);
						ierr =  MatSetValue(matR, no0->matriz_ind, no1->matriz_ind, coeff, ADD_VALUES);
					}
				}
			}
		}
	}

	ierr = MatAssemblyBegin(matR, MAT_FINAL_ASSEMBLY);
	ierr = MatAssemblyEnd(matR, MAT_FINAL_ASSEMBLY);

	if(N<10) ierr = MatView(matR, PETSC_VIEWER_STDOUT_WORLD);

	//Calcular lado direito do sistema.
	Vec ld;

	ierr = VecCreate(PETSC_COMM_WORLD, &ld);
	ierr = VecSetType(ld, VECMPI);
	ierr = VecSetSizes(ld, PETSC_DECIDE, N);
	ierr = VecSetFromOptions(ld);

	for (int i = 0; i < fem.elementos.size(); ++i)
	{	
		elem = fem.elementos[i];
		if(N<10) printf("Elemento %d\n", i);

		for (int no_ind_local = 0; no_ind_local < elem->qtd_nos; ++no_ind_local){
			no0 = (elem->nos)[no_ind_local];
			if(no0->eh_livre()){
				int matriz_ind =  no0->matriz_ind;
				coeff = elem->fem_lado_direito(no_ind_local);
				ierr = VecSetValue(ld, matriz_ind, coeff, ADD_VALUES);
			}
		}
	}	

	ierr = VecAssemblyBegin(ld);
	ierr = VecAssemblyEnd(ld);	

	if(N<10) ierr = VecView(ld, PETSC_VIEWER_STDOUT_WORLD);

	KSP ksp;
	PC pc;
	KSPConvergedReason reason;
	Vec xsol;

	// set ksp and pc
	ierr = KSPCreate(PETSC_COMM_WORLD, &ksp); CHKERRQ(ierr);
	ierr = KSPSetOperators(ksp, matR, matR); CHKERRQ(ierr);
	ierr = KSPSetType(ksp, KSPGMRES); CHKERRQ(ierr);
	ierr = KSPGetPC(ksp, &pc); CHKERRQ(ierr);
	ierr = PCSetType(pc, PCJACOBI); CHKERRQ(ierr);
	ierr = KSPSetTolerances(ksp, 1E-05, PETSC_DEFAULT, PETSC_DEFAULT, 1000);
	ierr = KSPSetFromOptions(ksp); CHKERRQ(ierr);
	ierr = KSPSetUp(ksp); CHKERRQ(ierr);
	
	ierr = VecDuplicate(ld, &xsol); CHKERRQ(ierr);
   	ierr = KSPSolve(ksp, ld, xsol); CHKERRQ(ierr);



   	if(N<10) ierr = VecView(xsol, PETSC_VIEWER_STDOUT_WORLD);

   	PetscScalar solcalc[1], solanalitica;
   	int ix[1];

   	double erro = 0.0;


   	for (int i = 0; i < fem.nos.size(); ++i)
   	{
   		no0 = fem.nos[i];

   		if(no0->eh_livre()){
   			solanalitica = solucao(no0->x, no0->y);
   			ix[0]=no0->matriz_ind;
   			VecGetValues(xsol, 1, ix, solcalc);
   			erro += (solcalc[0]-solanalitica)*(solcalc[0]-solanalitica);
   			if(N<10) printf("%d Calc: %8.4lf Sol: %8.4lf\n", i, solcalc[0], solanalitica);
   		}
   	}

   	erro /= N;

   	printf("Erro: %e\n", erro); 
	
	ierr = VecDestroy(&ld);CHKERRQ(ierr);

	ierr = MatDestroy(&matR);CHKERRQ(ierr);
	ierr = PetscFinalize();CHKERRQ(ierr);


}