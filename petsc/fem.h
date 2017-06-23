#include <vector>

double det3(double **);
void inversa3(double**, double **);

class Rectangle {
    int width, height;
  public:
    void set_values (int,int);
    int area() {return width*height;}
};

class No {
	public:
		double x, y;
		No(double, double);
};

class Elemento{
	public:
		std::vector<No> nos;
		int qtd_nos;
		char tipo_contorno;
		virtual double func_form(int, double, double);
		virtual double dfunc_form(int, int, double, double);
		double (*k_func) (double,double);
		~Elemento();
};

class Triangulo: public Elemento{
	public:
		Triangulo(double, double, double, double, double, double);
		Triangulo();
		double area();
		double func_form(int, double, double);
		double dfunc_form(int, int, double, double);
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
};