#ifndef MATH
#define MATH

#include <string>

class matrix {
  public:
    matrix(const int &tx, const int &ty);
    void inicializaMatrizNula();
    void imprimeMatriz(const std::string &file) const;
    matrix transpoeMatriz() const;
    double acessaMatriz() const;
    ~matrix();

    double getElement(const int &x, const int &y) const;
    double *getAddress(const int &x, const int &y) const;
    void setElement(const int &x, const int &y, const double &v);

    matrix operator+(const matrix &mat);
    matrix operator*(const matrix &mat);

  private:
    double **m;
    int tamx, tamy;
};

#endif
