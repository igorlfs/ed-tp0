#include "mat.h"
#include "memlog.h"
#include "msgassert.h"

// Descrição: constrói matriz com dimensões tx por ty
// Entrada: tx, ty, memlog
// Saída: matriz
matrix::matrix(const int &tx, const int &ty, memlog &ml) {

    // verifica se os valores de tx e ty são válidos
    erroAssert(tx > 0, "Dimensão nula");
    erroAssert(ty > 0, "Dimentão nula");
    erroAssert(tx <= MAXTAM, "Dimensão maior que permitido");
    erroAssert(ty <= MAXTAM, "Dimensão maior que permitido");

    // atribui variáveis
    this->tamx = tx;
    this->tamy = ty;
    this->ml = &ml;

    // aloca dinamicamente a matriz
    this->m = new double *[this->tamx];
    this->ml->escreveMemLog((long int)(&(this->m)), sizeof(double));
    for (int i = 0; i < this->tamx; ++i) {
        this->m[i] = new double[this->tamy];
        this->ml->escreveMemLog((long int)(&(this->m[i])), sizeof(double));
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
            this->ml->escreveMemLog((long int)(&(this->m[i][j])),
                                    sizeof(double));
        }
    }
}

// Descrição: imprime a matriz em um arquivo
// Entrada: nome do arquivo
// Saída: impressão no arquivo
void matrix::imprimeMatriz(const std::string &fileName) {

    // segurança, mas erro não deve acontecer jamais
    erroAssert(this->tamx <= MAXTAM, "Dimensão maior que permitido");
    erroAssert(this->tamy <= MAXTAM, "Dimensão maior que permitido");

    // abre arquivo de output
    std::ofstream outfile;
    outfile.open(fileName);
    erroAssert(!outfile.fail(), "Erro ao abrir arquivo de output");

    // imprime
    for (int i = 0; i < this->tamx; i++) {
        for (int j = 0; j < this->tamy; j++) {
            outfile << this->m[i][j];
            if (j != this->tamy - 1) outfile << ' ';
            this->ml->leMemLog((long int)(&(this->m[i][j])), sizeof(double));
        }
        outfile.put('\n');
    }

    // fecha arquivo de output
    outfile.close();
}

// Descrição: retorna a matriz transposta da chamada
// Entrada: --
// Saída: matriz transposta
matrix matrix::transpoeMatriz() {

    // inicializa a matriz resultante, garantindo a compatibilidade das dimensões
    matrix result(this->tamy, this->tamx, *this->ml);
    result.inicializaMatrizNula();

    // copia a matriz original transpondo
    for (int i = 0; i < this->tamx; i++) {
        for (int j = 0; j < this->tamy; j++) {
            result.setElemento(j, i, this->m[i][j]);
            this->ml->leMemLog((long int)(&(this->m[i][j])), sizeof(double));
            this->ml->escreveMemLog((long int)(&(result.m[j][i])),
                                    sizeof(double));
        }
    }

    return result;
}

// Descrição: acessa os elementos da matriz para fins de análise
// Entrada: --
// Saída: soma dos elementos da matriz
double matrix::acessaMatriz() const {

    double aux, s = 0.0;

    for (int i = 0; i < this->tamx; ++i) {
        for (int j = 0; j < this->tamy; ++j) {
            aux = this->m[i][j];
            s += aux;
            this->ml->leMemLog((long int)(&(this->m[i][j])), sizeof(double));
        }
    }
    return s;
}

// Descrição: destrói a matriz, tornando-a inacessível
// Entrada: --
// Saída: matriz destruída
matrix::~matrix() {

    // apenas um aviso se a matriz for destruída mais de uma vez
    avisoAssert(((this->tamx > 0) && (this->tamy > 0)),
                "Matriz já foi destruída");

    // delete as alocações
    for (int i = 0; i < this->tamx; ++i) {
        delete[] this->m[i];
    }
    delete[] this->m;

    // torna as dimensões inválidas
    this->tamx = this->tamy = -1;
}

// Descrição: retorna o elemento na posição (x,y)
// Entrada: x,y
// Saída: o elemento na posição (x,y)
double matrix::getElemento(const int &x, const int &y) const {

    // verifica se x e y são válidos
    erroAssert((x > 0) || (x < this->tamx), "Índice inválido");
    erroAssert((y > 0) || (y < this->tamy), "Índice inválido");

    return this->m[x][y];
}

// Descrição: retorna o endereço do elemento na posição (x,y)
// Entrada: x,y
// Saída: o endereço do elemento na posição (x,y)
double *matrix::getAddress(const int &x, const int &y) const {

    // verifica se x e y são válidos
    erroAssert((x > 0) || (x < this->tamx), "Índice inválido");
    erroAssert((y > 0) || (y < this->tamy), "Índice inválido");

    return &this->m[x][y];
}

// Descrição: atribui ao elemento na posição (x,y) o valor v
// Entrada: x,y,v
// Saída: --
void matrix::setElemento(const int &x, const int &y, const double &v) {

    // verifica se x e y são válidos
    erroAssert((x > 0) || (x <= this->tamx), "Índice inválido");
    erroAssert((y > 0) || (y <= this->tamy), "Índice inválido");

    this->m[x][y] = v;
    this->ml->escreveMemLog((long int)&(this->m[x][y]), sizeof(double));
}

// Descrição: sobreescreve o operador '+' como a soma de matrizes
// Entrada: matriz M
// Saida: matriz resultante
matrix matrix::operator+(const matrix &M) {

    // verifica se as dimensões das matrizes a e b são as mesmas
    erroAssert(this->tamx == M.tamx, "Dimensões incompatíveis");
    erroAssert(this->tamy == M.tamy, "Dimensões incompatíveis");

    // inicializa a matriz resultante, garantindo a compatibilidade das dimensões
    matrix result(this->tamx, this->tamy, *this->ml);
    result.inicializaMatrizNula();

    // faz a soma elemento a elemento
    for (int i = 0; i < this->tamx; i++) {
        for (int j = 0; j < this->tamy; j++) {
            result.m[i][j] = this->m[i][j] + M.m[i][j];
            this->ml->leMemLog((long int)(&(this->m[i][j])), sizeof(double));
            this->ml->leMemLog((long int)(&(M.m[i][j])), sizeof(double));
            this->ml->escreveMemLog((long int)(&(result.m[i][j])),
                                    sizeof(double));
        }
    }

    return result;
}

// Descrição: sobreescreve o operador '*' como o produto de matrizes
// Entrada: matriz M
// Saida: matriz resultante
matrix matrix::operator*(const matrix &M) {
    // verifica se as dimensões das matrizes a e b são as mesmas
    erroAssert(this->tamy == M.tamx, "Dimensões incompatíveis");

    // inicializa a matriz resultante, garantindo a compatibilidade das dimensões
    matrix result(this->tamx, M.tamy, *this->ml);
    result.inicializaMatrizNula();

    // realiza a multiplicação de matrizes
    for (int i = 0; i < result.tamx; i++) {
        for (int j = 0; j < result.tamy; j++) {
            for (int k = 0; k < this->tamy; k++) {
                result.m[i][j] += this->m[i][k] * M.m[k][j];
                this->ml->leMemLog((long int)(&(this->m[i][k])),
                                   sizeof(double));
                this->ml->leMemLog((long int)(&(M.m[k][j])), sizeof(double));
                this->ml->escreveMemLog((long int)(&(result.m[i][j])),
                                        sizeof(double));
            }
        }
    }

    return result;
}
