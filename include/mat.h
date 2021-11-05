#ifndef MATH
#define MATH

#include "memlog.h"

#define MAXTAM 1000

class matrix {
  public:
    matrix(const int &tx, const int &ty, memlog &ml);
    void inicializaMatrizNula();
    void imprimeMatriz(const std::string &file) const;
    matrix transpoeMatriz() const;
    double acessaMatriz() const;
    ~matrix();

    void setElemento(const int &x, const int &y, const double &v);

    matrix operator+(const matrix &mat);
    matrix operator*(const matrix &mat);

  private:
    double **m;
    int tamx, tamy;
    memlog *ml;
};

#endif
