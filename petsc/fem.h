double det3(double **);
void inversa3(double**, double **);

class Rectangle {
    int width, height;
  public:
    void set_values (int,int);
    int area() {return width*height;}
};

class No {
	double x, y;
};

class Elemento{
	public:
		double *x, *y;
		int qtd_pontos;
};

class Triangulo: public Elemento{
	public:
		Triangulo(double, double, double, double, double, double);
		double area();
		double func_form(int, double, double);
	private:
		double **M, **Minv;
		double ** get_M();
		double ** get_Minv();
};



