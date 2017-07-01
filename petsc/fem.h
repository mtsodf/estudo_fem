#include <vector>

double det3(double **);
void inversa3(double**, double **);


enum tipo_no { livre, dirichlet, neumann };

class Rectangle {
    int width, height;
  public:
    void set_values (int,int);
    int area() {return width*height;}
};

class No {
	public:
		double x, y;
		No(double, double, tipo_no, double);
		int tipo;
		int global_ind; //indice global do no
		int matriz_ind; //indice na matriz do no
		double contorno;
		bool eh_livre();
};

class Elemento{
	public:
		std::vector<No*> nos;
		int qtd_nos;
		virtual double func_form(int, double, double);
		virtual double dfunc_form(int, int, double, double);
		virtual double matriz_coeff(int, int);
		virtual double fem_lado_direito(int);
		void printNos();		
		double (*k_func) (double,double);
		~Elemento();
		//acochambrado
		virtual double area();
		
		
};

class Triangulo: public Elemento{
	public:
		Triangulo(double,double,double,double,double,double);
		Triangulo();
		double area();
		virtual double func_form(int, double, double);
		virtual double dfunc_form(int, int, double, double);
		virtual double matriz_coeff(int, int);
		virtual double fem_lado_direito(int);
		void centro(double *, double *);
		void baric2cart(double, double, double , double* , double*);
		double ** get_Minv();
		double ** get_M();
		~Triangulo();
	private:
		double **M, **Minv;		
		
		
};



class FemCaso{
	public:
		std::vector<No*> nos;
		std::vector<Elemento*> elementos;
		void carregarNos(char*);
		void carregarElementos(char*);
		int grausLiberdade();
};