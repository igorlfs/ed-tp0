#include "memlog.h"
#include "msgassert.h"

// Descrição: calcula a diferença entre t2 e t1, que é armazenada em resultado
// Entrada: t1, t2, resultado
// Saída: resultado
void clockDifference(struct timespec &t1, struct timespec &t2,
                     struct timespec *resultado) {

    if (t2.tv_nsec < t1.tv_nsec) {
        // ajuste necessário, utilizando um segundo de tv_sec
        resultado->tv_nsec = 1000000000 + t2.tv_nsec - t1.tv_nsec;
        resultado->tv_sec = t2.tv_sec - t1.tv_sec - 1;
    } else {
        // não é necessário ajuste
        resultado->tv_nsec = t2.tv_nsec - t1.tv_nsec;
        resultado->tv_sec = t2.tv_sec - t1.tv_sec;
    }
}

// Descrição: inicializa o registro de acessos
// Entrada: nome
// Saída: resultado
int memlog::iniciaMemLog(const std::string &nome) {

    // escolhe modo do relogio
    this->clk_id = CLOCK_MONOTONIC;

    // abre arquivo de registro e verifica se foi aberto corretamente
    this->nome = nome;
    this->log.open(this->nome);
    erroAssert(this->log.is_open(), "Erro ao abrir arquivo do memlog");

    // captura o tempo inicial do registro
    struct timespec tp;
    int result = clock_gettime(this->clk_id, &tp);
    this->inittime.tv_sec = tp.tv_sec;
    this->inittime.tv_nsec = tp.tv_nsec;

    // inicializa variáveis do TAD
    this->count = 1;
    this->ativo = MLATIVO;
    this->fase = 0;

    // imprime registro inicial e verificado se ocorreu corretamente
    this->log << "I " << this->count << ' ' << tp.tv_sec << '.' << tp.tv_nsec;
    this->log.put('\n');
    erroAssert(!this->log.fail(), "Não foi possível escrever registro");
    return result;
}

// Descrição: ativa o registro de acessos
// Entrada: --
// Saída: MLATIVO
int memlog::ativaMemLog() {
    this->ativo = MLATIVO;
    return MLATIVO;
}

// Descrição: desativa o registro de acessos
// Entrada: --
// Saída: MLINATIVO
int memlog::desativaMemLog() {
    this->ativo = MLINATIVO;
    return MLINATIVO;
}

// Descrição: define a fase de registro de acessos
// Entrada: f
// Saída: valor de f
int memlog::defineFaseMemLog(const int &f) {
    this->fase = f;
    return f;
}

// Descrição: define o id de registro de acessos
// Entrada: id
// Saída: valor do id
int memlog::defineId(const int &id) {
    this->id = id;
    return id;
}

// Descrição: registra acesso de leitura de tam bytes na posição pos
// Entrada: pos,tam
// Saída: resultado da obtencao do relogio
int memlog::leMemLog(const long int &pos, const long int &tam, const int &id) {
    this->defineId(id);
    char op = LEITURA;
    return geralMemLog(op, pos, tam);
}

// Descrição: registra acesso de escrita de tam bytes na posição pos
// Entrada: pos, tam
// Saída: resultado da obtenção do relógio
int memlog::escreveMemLog(const long int &pos, const long int &tam,
                          const int &id) {
    this->defineId(id);
    char op = ESCRITA;
    return geralMemLog(op, pos, tam);
}

// Descrição: registra acesso de escrita de tam bytes na posição pos
// Entrada: c, pos, tam
// Saída: resultado da obtenção do relógio
int memlog::geralMemLog(const char &c, const long int &pos,
                        const long int &tam) {

    // verifica se a opção é válida
    erroAssert(c == ESCRITA || c == LEITURA, "Opção inválida para registro");

    // verifica se registro está ativo
    if (this->ativo == MLINATIVO) return 0;

    // captura tempo atual
    struct timespec tp, tdif;
    int result = clock_gettime(this->clk_id, &tp);

    // calcula a diferença com o tempo inicial, para economia de armazenamento
    clockDifference(this->inittime, tp, &tdif);

    // atualiza contador
    this->count++;

    // imprime registro e verifica se houve algum erro
    this->log << c << ' ' << this->fase << ' ' << this->count << ' ' << this->id
              << ' ' << tdif.tv_sec << '.' << tdif.tv_nsec << ' ' << pos << ' '
              << tam << '\n';
    erroAssert(!this->log.fail(), "Não foi possível escrever registro");

    return result;
}

// Descrição: finaliza o registro de acessos à memória
// Entrada: --
// Saída: resultado da obtemção do relógio
int memlog::finalizaMemLog() {

    // captura o tempo atual
    struct timespec tp;
    int result = clock_gettime(this->clk_id, &tp);

    // atualiza contador
    this->count++;

    // imprime registro final e verifica se houve algum erro
    this->log << "F " << this->count << ' ' << tp.tv_sec << '.' << tp.tv_nsec;
    this->log.put('\n');
    erroAssert(!this->log.fail(), "Não foi possível escrever registro");

    // fecha arquivo e verifica se houve algum erro
    this->log.close();
    erroAssert(!this->log.is_open(), "Erro ao fechar arquivo do memlog");

    // atualiza variável de estado
    this->ativo = MLINATIVO;
    return result;
}
