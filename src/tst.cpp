#include "mat.h"
#include <gtest/gtest.h>

// Variáveis e constantes globais para teste
const int SIZE = 2;
memlog ml;

TEST(matrix, iniciaMatrizNula) {

    // Initicialize matriz
    matrix x(SIZE, SIZE);
    x.inicializaMatrizNula();

    // Assert
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            EXPECT_EQ(0, x.getElemento(i, j));
}

TEST(matrix, somaMatrizes) {

    // Initicialize matrizes e soma
    matrix x(SIZE, SIZE);
    x.inicializaMatrizNula();
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            x.setElemento(i, j, i * (j + 1));
    matrix y(SIZE, SIZE);
    y.inicializaMatrizNula();
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            y.setElemento(i, j, j * (i + 1));
    const matrix w = x + y;

    // Define matriz esperada
    matrix z(SIZE, SIZE);
    z.inicializaMatrizNula();
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            z.setElemento(i, j, i * (j + 1) + j * (i + 1));

    // Assert
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            EXPECT_EQ(w.getElemento(i, j), z.getElemento(i, j));
}

TEST(matrix, multiplicaMatrizes) {

    // Initicialize matrizes e multiplica
    double xElements[SIZE * SIZE] = {1, 2, 3, 4};
    matrix x(SIZE, SIZE);
    x.inicializaMatrizNula();
    for (int i = 0, k = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j, ++k)
            x.setElemento(i, j, xElements[k]);

    int yElements[SIZE * SIZE] = {5, 6, 7, 8};
    matrix y(SIZE, SIZE);
    y.inicializaMatrizNula();
    for (int i = 0, k = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j, ++k)
            y.setElemento(i, j, yElements[k]);

    const matrix w = x * y;

    // Define matriz esperada
    int zElements[SIZE * SIZE] = {19, 22, 43, 50};
    matrix z(SIZE, SIZE);
    z.inicializaMatrizNula();
    for (int i = 0, k = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j, ++k)
            z.setElemento(i, j, zElements[k]);

    // Assert
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            EXPECT_EQ(w.getElemento(i, j), z.getElemento(i, j));
}

TEST(matrix, transpoeMatriz) {

    const double arr[SIZE][SIZE * SIZE] = {{1, 2, 3, 4}, {5, 6, 7, 8}};

    // Initicialize matriz e a transpõe
    matrix x(SIZE, SIZE * SIZE);
    x.inicializaMatrizNula();
    for (int i = 0, k = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE * SIZE; ++j, ++k)
            x.setElemento(i, j, arr[i][j]);
    matrix xTransp = x.transpoeMatriz();

    // Define matriz esperada
    matrix expect(SIZE * SIZE, SIZE);
    expect.inicializaMatrizNula();
    for (int i = 0, k = 0; i < SIZE * SIZE; ++i)
        for (int j = 0; j < SIZE; ++j, ++k)
            expect.setElemento(i, j, arr[j][i]);

    // Assert
    for (int i = 0; i < SIZE * SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            EXPECT_EQ(xTransp.getElemento(i, j), expect.getElemento(i, j));
}

// Descrição: programa principal para execução de testes
// Entrada: argc,argv
// Saida: resultado dos testes
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
