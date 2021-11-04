#ifndef MATH
#define MATH

#include "memlog.h"

// TODO: remover?
#define MAXTAM 10

class matrix {
  public:
    matrix(const int &tx, const int &ty, memlog &ml);
    void inicializaMatrizNula();
    void imprimeMatriz(const std::string &file);
    matrix transpoeMatriz();
    ~matrix();

    // Getters:
    double getElemento(const int &x, const int &y) const;
    double *getAddress(const int &x, const int &y) const;

    void setElemento(const int &x, const int &y, const double &v);

    matrix operator+(const matrix &mat);
    matrix operator*(const matrix &mat);

  private:
    double **m;
    int tamx, tamy;
    memlog *ml;
};

/* void copiaMatriz(matrix *src, matrix *dst); */
#endif
