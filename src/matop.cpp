#include "mat.h"
#include "memlog.h"
#include "msgassert.h"
#include <regex>
#include <sstream>
#include <unistd.h>

// Define as operações
enum OP { SOMAR = 1, MULTIPLICAR, TRANSPOR };

// Variáveis globais e inicializações para opções
static int opescolhida = -1;
memlog ml;
std::string logNome, outNome, m1Nome, m2Nome;
bool regmem = 0;

// Descrição: imprime as opções de uso
// Entrada: --
// Saída: impressão das opções de linha de comando
void uso() {

    std::cerr << "matop\n";
    std::cerr << "\t-s \t\t(soma matrizes) \n";
    std::cerr << "\t-m \t\t(multiplica matrizes) \n";
    std::cerr << "\t-t \t\t(transpõe matriz 1)\n";
    std::cerr << "\t-o <arq>\t(matriz resultante)\n";
    std::cerr << "\t-1 <arq>\t(matriz 1)\n";
    std::cerr << "\t-2 <arq>\t(matriz 2)\n";
    std::cerr << "\t-p <arq>\t(registro de desempenho)\n";
    std::cerr << "\t-l \t\t(padrão de acesso e localidade)\n";
    std::cerr << "\t-h \t\timprime essa mensagem\n";
}

// Descrição: lê as opções da linha de comando e inicializa variáveis
// Entrada: argc,argv
// Saída: opescolhida, lognome, outNome, regmem, m1Nome m2Nome
void parseArgs(int argc, char **argv) {

    // variáveis externas do getopt
    extern char *optarg;

    // variável auxiliar
    int c;

    // getopt - letra indica a opção, : junto a letra indica parâmetro
    // no caso de escolher mais de uma operação, vale a última
    while ((c = getopt(argc, argv, "smto:1:2:p:lh")) != EOF) {
        switch (c) {
            case 's':
                avisoAssert(opescolhida == -1,
                            "Mais de uma operação escolhida");
                opescolhida = SOMAR;
                break;
            case 'm':
                avisoAssert(opescolhida == -1,
                            "Mais de uma operação escolhida");
                opescolhida = MULTIPLICAR;
                break;
            case 't':
                avisoAssert(opescolhida == -1,
                            "Mais de uma operação escolhida");
                opescolhida = TRANSPOR;
                break;
            case 'o': outNome = optarg; break;
            case '1': m1Nome = optarg; break;
            case '2': m2Nome = optarg; break;
            case 'p': logNome = optarg; break;
            case 'l': regmem = 1; break;
            case 'h':
            default: uso(); exit(1);
        }
    }
    // verificação da consistência das opções
    erroAssert(opescolhida > 0, "matop - necessário escolher operação");
    erroAssert(
        !logNome.empty(),
        "matop - nome de arquivo de registro de acesso tem que ser definido");
    erroAssert(!outNome.empty(),
               "matop - nome do arquivo de saída tem que ser definido");
    erroAssert(!m1Nome.empty(),
               "matop - nome do arquivo da matriz 1 tem que ser definido");
    if (opescolhida != TRANSPOR) {
        erroAssert(!m2Nome.empty(),
                   "matop - nome do arquivo da matriz 2 tem que ser definido");
    }
}

// Descrição: conte o número de vezes que c aparece em str
// Entrada: str,c
// Saída: número de vezes que c aparece em str
unsigned countCharStr(const std::string &str, const char c) {

    unsigned n = 0;
    for (char i : str)
        if (c == i) n++;
    return n;
}

// Descrição: verifica se outras linhas de um arquivo são consistentes
// Entrada: str
// Saída: retorna verdadeiro se consisntente e falso caso contrário
bool isOtherLineValid(const std::string &str) {

    // Esperado: float separados por um único espaço (termina em float)
    std::regex ex("((-?\\d+\\.?\\d*) )*(-?\\d+\\.?\\d*)");
    return std::regex_match(str, ex);
}

// Descrição: verifica se a 1ª linha de um arquivo é consistente
// Entrada: str
// Saída: retorna verdadeiro se consisntente e falso caso contrário
bool isFirstLineValid(const std::string &str) {

    // Esperado: dois inteiros separados por um único espaço
    std::regex expectedFormat("\\d+\\s\\d+");
    return std::regex_match(str, expectedFormat);
}

// Descrição: verifica se um arquivo contendo uma matriz é válido
// Entrada: string com nome do arquivo
// Saída: matriz construída a partir do arquivo
void isFileValid(std::string &matrixName) {

    // Abre o arquivo
    std::ifstream inFile;
    inFile.open(matrixName);
    erroAssert(inFile.is_open(), "Erro ao abrir arquivo da matriz");

    // Teste a validade da primeira linha
    std::string firstLine;
    std::getline(inFile, firstLine);
    erroAssert(isFirstLineValid(firstLine),
               "A 1ª linha do arquivo da matriz apresenta inconsistência");

    // Variáveis que vão ajudar a verificar o resto
    // Como a primeira linha é válida, podem ser usadas
    unsigned x, y;
    std::stringstream ss(firstLine);
    ss >> x >> y;

    // Teste a validade das outras linhas
    for (int lineCounter = 2;; lineCounter++) {
        // Antes de testar cada linha verifique se o EOF foi atingido
        std::string line;
        std::getline(inFile, line);
        if (inFile.eof()) break;
        erroAssert(isOtherLineValid(line),
                   "Linha " << lineCounter
                            << " do arquivo da matriz é inconsistente\n"
                               "Certeza que ela não termina em um espaço?");
    }
    erroAssert(!inFile.bad(), "Erro na leitura do arquivo da matriz");

    // Como cada linha é válida, e o regex garante que há somente um
    // espaço entre os elementos, em cada linha podemos contar a 
    // quantidade de espaços. Se ela for igual a y - 1, quer dizer
    // que a linha tem a quantidade certa de colunas
    //
    // Similarmente, nós contamos o número de linhas no arquivo para
    // avaliar se temos a quanitidade certa (x + 1)

    // Rebobine o arquivo
    inFile.clear(); // Necessário para remover EOFBIT do arquivo
    inFile.seekg(0);

    std::getline(inFile, firstLine); // Ignore a primeira linha
    for (unsigned lineCounter = 2;; lineCounter++) {
        // Antes de testar cada linha verifique se o EOF foi atingido
        std::string line;
        std::getline(inFile, line);
        if (inFile.eof()) {
            erroAssert(lineCounter == x + 1, "Número de linhas do arquivo da "
                                             "matriz é diferente do esperado");
            break;
        };
        erroAssert(countCharStr(line, ' ') == y - 1,
                   "Linha " << lineCounter
                            << " da matriz não tem o número certo de colunas");
    }
    erroAssert(!inFile.bad(), "Erro na leitura do arquivo da matriz");

    // fecha o arquivo e verifica se ocorreu normalmente
    inFile.close();
    erroAssert(!inFile.is_open(), "Erro ao fechar arquivo da matriz");
}

// Descrição: constrói uma matriz a partir de um nome de um arquivo de entrada
// Entrada: matrixName
// Saída: matriz construída a partir do arquivo
matrix matrixBuilder(std::string &matrixName) {

    // Abre o arquivo
    std::ifstream inFile;
    inFile.open(matrixName);
    erroAssert(inFile.is_open(), "Erro ao abrir arquivo da matriz");

    // Leia as dimensões
    int x, y;
    inFile >> x >> y;

    // Construa a matriz
    matrix mat(x, y);

    // Inicialize a matriz e confere se houve algum erro
    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < y; ++j) {
            double d;
            inFile >> d;
            mat.setElement(i, j, d);
        }
    }
    erroAssert(!inFile.bad(), "Erro na leitura do arquivo da matriz");

    // fecha o arquivo e verifica se ocorreu normalmente
    inFile.close();
    erroAssert(!inFile.is_open(), "Erro ao fechar arquivo da matriz");

    return mat;
}

// Descrição: programa principal para execução de operações de matrizes
// Entrada: argc,argv
// Saída: depende da operação escolhida
int main(int argc, char **argv) {

    // interprete opções de linha de comando
    parseArgs(argc, argv);

    // inicie o registre
    ml.iniciaMemLog(logNome);

    // ative o registro se necessário
    regmem ? ml.ativaMemLog() : ml.desativaMemLog();

    // Verifica a integridade do arquivo
    isFileValid(m1Nome);

    // interprete arquivo contendo matriz 1
    matrix a = matrixBuilder(m1Nome);

    // execução dependente da operação escolhida
    switch (opescolhida) {
        case SOMAR: {
            // Verifica a integridade do arquivo
            isFileValid(m2Nome);
            // interprete arquivo contendo matriz 2
            matrix b = matrixBuilder(m2Nome);
            // cria e imprime uma matriz que é a soma das que foram lidas
            // as matrizes são destrúidas automaticamente pelo destrutor
            ml.defineFaseMemLog(1);
            a.acessaMatriz();
            b.acessaMatriz();
            matrix c = a + b;
            ml.defineFaseMemLog(2);
            c.acessaMatriz();
            if (regmem) c.imprimeMatriz(outNome);
            break;
        }
        case MULTIPLICAR: {
            // interprete arquivo contendo matriz 2
            matrix b = matrixBuilder(m2Nome);
            // cria e imprime uma matriz que é o produto das que foram lidas
            // as matrizes são destrúidas automaticamente pelo destrutor
            ml.defineFaseMemLog(1);
            a.acessaMatriz();
            b.acessaMatriz();
            matrix c = a * b;
            ml.defineFaseMemLog(2);
            c.acessaMatriz();
            if (regmem) c.imprimeMatriz(outNome);
            break;
        }
        case TRANSPOR: {
            // cria e imprime uma matriz sendo a transposta da matriz lida
            ml.defineFaseMemLog(1);
            a.acessaMatriz();
            matrix c = a.transpoeMatriz();
            ml.defineFaseMemLog(2);
            c.acessaMatriz();
            if (regmem) c.imprimeMatriz(outNome);
            break;
        }
        // Não deve chegar aqui
        default: std::cout << "Here be dragons!\n"; exit(1);
    }

    // Confira se o arquivo de saída atende o formato
    isFileValid(outNome);

    // conclui o registro
    return ml.finalizaMemLog();
}
