#ifndef MATH
#define MATH

#include "memlog.h"

#define MAXTAM 1000

class matrix {
  public:
    matrix(const int &tx, const int &ty);
    void inicializaMatrizNula();
    void imprimeMatriz(const std::string &file) const;
    matrix transpoeMatriz() const;
    double acessaMatriz() const;
    ~matrix();

    double getElemento(const int &x, const int &y) const;
    void setElemento(const int &x, const int &y, const double &v);

    matrix operator+(const matrix &mat);
    matrix operator*(const matrix &mat);

  private:
    double **m;
    int tamx, tamy;
};

#endif
