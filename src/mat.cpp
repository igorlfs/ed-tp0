#include "mat.h"
#include "memlog.h"
#include "msgassert.h"
#include <iomanip>

// Variável global para registro
int matrixId = 0;

// Descrição: constrói matriz com dimensões tx por ty
// Entrada: tx, ty
// Saída: matriz
matrix::matrix(const int &tx, const int &ty) {

    // verifica se os valores de tx e ty são válidos
    erroAssert(tx > 0, "Dimensão nula");
    erroAssert(ty > 0, "Dimensão nula");

    // atribui variáveis
    this->tamx = tx;
    this->tamy = ty;
    this->id = matrixId;
    matrixId++;

    // aloca dinamicamente a matriz
    // use nothrow para checar se a alocação ocorreu normalmente
    // assim, se houve algum erro, o ponteiro aponta para nullptr
    this->m = new (std::nothrow) double *[this->tamx];
    erroAssert(this->m, "Falha ao alocar dinamicamente a matriz");
    for (int i = 0; i < this->tamx; ++i) {
        this->m[i] = new (std::nothrow) double[this->tamy];
        erroAssert(this->m[i], "Falha ao alocar dinamicamente a matriz");
    }
}

// Descrição: inicializa mat com valores nulos
// Entrada: --
// Saída: matriz nula
void matrix::inicializaMatrizNula() {

    // inicializa todos os elementos da matriz com 0, por segurança
    for (int i = 0; i < this->tamx; i++) {
        for (int j = 0; j < this->tamy; j++) {
            this->m[i][j] = 0;
            ESCREVEMEMLOG((long int)(&(this->m[i][j])), sizeof(double),
                          this->id);
        }
    }
}

// Descrição: imprime a matriz em um arquivo
// Entrada: nome do arquivo
// Saída: impressão no arquivo
void matrix::imprimeMatriz(const std::string &fileName) const {

    // abre arquivo de output e confere se ocorreu como deveria
    std::ofstream outfile;
    outfile.open(fileName);
    erroAssert(outfile.is_open(), "Erro ao abrir arquivo de output");

    // imprime e verifica se impressão ocorreu normalmente
    outfile << this->tamx << ' ' << this->tamy << '\n';
    for (int i = 0; i < this->tamx; i++) {
        for (int j = 0; j < this->tamy; j++) {
            outfile << std::fixed << std::showpoint;
            outfile << std::setprecision(4);
            outfile << this->m[i][j];
            if (j != this->tamy - 1) outfile << ' ';
            LEMEMLOG((long int)(&(this->m[i][j])), sizeof(double), this->id);
        }
        if (i != this->tamx - 1) outfile.put('\n');
    }
    erroAssert(!outfile.fail(), "Erro ao escrever arquivo de saída");

    // fecha arquivo de output e confere se ocorreu como deveria
    outfile.close();
    erroAssert(!outfile.is_open(), "Erro ao fechar arquivo de output");
}

// Descrição: retorna uma cópia transposta da matriz
// Entrada: --
// Saída: matriz transposta
matrix matrix::transpoeMatriz() const {

    // inicializa a matriz resultante, garantindo a compatibilidade das dimensões
    matrix result(this->tamy, this->tamx);
    result.inicializaMatrizNula();

    // copia a matriz original transpondo
    for (int i = 0; i < this->tamx; i++) {
        for (int j = 0; j < this->tamy; j++) {
            result.m[j][i] = this->m[i][j];
            LEMEMLOG((long int)(&(this->m[i][j])), sizeof(double), this->id);
            ESCREVEMEMLOG((long int)&(result.m[j][i]), sizeof(double),
                          result.id);
        }
    }

    return result;
}

// Descrição: acessa os elementos da matriz para fins de análise
// Entrada: --
// Saída: soma dos elementos da matriz
double matrix::acessaMatriz() const {

    double aux, s = 0.0;

    // acesse os elementos
    for (int i = 0; i < this->tamx; ++i) {
        for (int j = 0; j < this->tamy; ++j) {
            aux = this->m[i][j];
            s += aux;
            LEMEMLOG((long int)(&(this->m[i][j])), sizeof(double), this->id);
        }
    }
    return s;
}

// Descrição: destrói a matriz, tornando-a inacessível
// Entrada: --
// Saída: matriz destruída
matrix::~matrix() {

    // delete as alocações
    for (int i = 0; i < this->tamx; ++i)
        delete[] this->m[i];
    delete[] this->m;

    // torna as dimensões inválidas
    this->tamx = this->tamy = -1;
}

// Descrição: retorna o elemento na posição (x,y)
// Entrada: x,y
// Saída: o elemento na posição (x,y)
double matrix::getElement(const int &x, const int &y) const {

    // verifica se x e y são válidos
    erroAssert((x >= 0) && (x < this->tamx), "Índice inválido");
    erroAssert((y >= 0) && (y < this->tamy), "Índice inválido");

    return this->m[x][y];
}

// Descrição: retorna o endereço do elemento na posição (x,y)
// Entrada: x,y
// Saída: o endereço do elemento na posição (x,y)
double *matrix::getAddress(const int &x, const int &y) const {

    // verifica se x e y são válidos
    erroAssert((x >= 0) && (x < this->tamx), "Índice inválido");
    erroAssert((y >= 0) && (y < this->tamy), "Índice inválido");

    return &this->m[x][y];
}

// Descrição: atribui ao elemento na posição (x,y) o valor v
// Entrada: x,y,v
// Saída: --
void matrix::setElement(const int &x, const int &y, const double &v) {

    // verifica se x e y são válidos
    erroAssert((x >= 0) && (x < this->tamx), "Índice inválido");
    erroAssert((y >= 0) && (y < this->tamy), "Índice inválido");

    this->m[x][y] = v;
    ESCREVEMEMLOG((long int)(&(this->m[x][y])), sizeof(double), this->id);
}

// Descrição: sobreescreve o operador '+' como a soma de matrizes
// Entrada: matriz M
// Saída: matriz resultante
matrix matrix::operator+(const matrix &M) {

    // verifica se as dimensões das matrizes a e b são as mesmas
    erroAssert(this->tamx == M.tamx, "Dimensões incompatíveis");
    erroAssert(this->tamy == M.tamy, "Dimensões incompatíveis");

    // inicializa a matriz resultante, garantindo a compatibilidade das dimensões
    matrix result(this->tamx, this->tamy);
    result.inicializaMatrizNula();

    // faz a soma elemento a elemento
    for (int i = 0; i < this->tamx; i++) {
        for (int j = 0; j < this->tamy; j++) {
            result.m[i][j] = this->m[i][j] + M.m[i][j];
            LEMEMLOG((long int)(&(this->m[i][j])), sizeof(double), this->id);
            LEMEMLOG((long int)(&(M.m[i][j])), sizeof(double), M.id);
            ESCREVEMEMLOG((long int)(&(result.m[i][j])), sizeof(double),
                          result.id);
        }
    }

    return result;
}

// Descrição: sobreescreve o operador '*' como o produto de matrizes
// Entrada: matriz M
// Saída: matriz resultante
matrix matrix::operator*(const matrix &M) {

    // verifica se as dimensões das matrizes a e b são as mesmas
    erroAssert(this->tamy == M.tamx, "Dimensões incompatíveis");

    // inicializa a matriz resultante, garantindo a compatibilidade das dimensões
    matrix result(this->tamx, M.tamy);
    result.inicializaMatrizNula();

    // realiza a multiplicação de matrizes
    for (int i = 0; i < result.tamx; i++) {
        for (int j = 0; j < result.tamy; j++) {
            for (int k = 0; k < this->tamy; k++) {
                result.m[i][j] += this->m[i][k] * M.m[k][j];
                LEMEMLOG((long int)(&(this->m[i][k])), sizeof(double),
                         this->id);
                LEMEMLOG((long int)(&(M.m[k][j])), sizeof(double), M.id);
                ESCREVEMEMLOG((long int)(&(result.m[i][j])), sizeof(double),
                              result.id);
            }
        }
    }

    return result;
}
