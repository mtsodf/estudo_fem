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
		int global_ind;
		double contorno;
		bool eh_livre();
};

class Elemento{
	public:
		std::vector<No> nos;
		int qtd_nos;
		virtual double func_form(int, double, double);
		virtual double dfunc_form(int, int, double, double);
		virtual double matriz_coeff(int, int);
		double (*k_func) (double,double);
		~Elemento();
};

class Triangulo: public Elemento{
	public:
		Triangulo(double,double,double,double,double,double);
		Triangulo();
		double area();
		double func_form(int, double, double);
		double dfunc_form(int, int, double, double);
		double matriz_coeff(int, int);
		void centro(double *, double *);
		~Triangulo();
	private:
		double **M, **Minv;
		double ** get_M();
		double ** get_Minv();
};



class FemCaso{
	public:
		std::vector<No> nos;
		std::vector<Elemento> elementos;
		void carregarNos(char*);
		void carregarElementos(char*);
		int grausLiberdade();
};