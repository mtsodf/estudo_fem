include ${PETSC_DIR}/lib/petsc/conf/variables
include ${PETSC_DIR}/lib/petsc/conf/rules

ex1: ex1.o  chkopts
	-${CLINKER} -o ex1 ex1.o  ${PETSC_KSP_LIB}
	${RM} ex1.o

CriandoVetor: CriandoVetor.o  chkopts
	-${CLINKER} -o CriandoVetor CriandoVetor.o  ${PETSC_KSP_LIB}
	${RM} CriandoVetor.o




include ${PETSC_DIR}/lib/petsc/conf/test
