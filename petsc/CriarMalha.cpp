#include <stdlib.h>
#include <stdio.h>





int main(int argc, char* argv[]){
	if (argc == 1)
	{
		printf("Coloque a quantidade de nos do grid.\n");
		return 0;
	}

	int n = atoi(argv[1]);

	FILE *nos, *elementos;


	nos = fopen("nos.txt", "w");
	elementos = fopen("elementos.txt", "w");

	double h = 1.0 / (n - 1);
	double x, y;

	//Escrevendo Nos
	for (int j = 0; j < n; ++j)
	{	
		y = j*h;
		for (int i = 0; i < n; ++i)	
		{	
			x = i*h;
			fprintf(nos, "%f %f\n", x, y);
		}

	}

	//Escrevendo Elementos

	int qtd_triangulos = (n - 1) * (n - 1) * 2;

	for (int j = 0; j < n - 1; ++j)
	{
		for (int i = 0; i < n - 1; ++i)
		{	
			int ce = j*n + i;
			fprintf(elementos, "%d %d %d\n", ce, ce+1, ce + n);
			fprintf(elementos, "%d %d %d\n", ce+1, ce + n + 1, ce + n);
		}
	}

	fclose(nos);
	fclose(elementos);

	return 0;
}