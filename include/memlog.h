#ifndef MEMLOGH
#define MEMLOGH
#include <fstream>
#include <string>
#include <time.h>

// Constantes para estados de registro
static constexpr bool MLATIVO = 1;
static constexpr bool MLINATIVO = 0;

class memlog {
  public:
    int iniciaMemLog(const std::string &nome);
    int ativaMemLog();
    int desativaMemLog();
    int defineFaseMemLog(const int &f);
    int leMemLog(const long int &pos, const long int &tam);
    int escreveMemLog(const long int &pos, const long int &tam);
    int finalizaMemLog();

    // esse membro é público porque assim evitamos uma chamada de função na macro
    // (seria meio bobo usar um getter aqui quando o que queremos é justamente
    // evitar a chamada de funções para aumentar a performance)
    int ativo;

  private:
    const static char ESCRITA = 'E';
    const static char LEITURA = 'L';
    std::ofstream log;
    clockid_t clk_id;
    struct timespec inittime;
    std::string nome;
    long count;
    int fase;

    int geralMemLog(const char &c, const long int &pos, const long int &tam);
};

#define LEMEMLOG(pos, tam)                                                     \
    ((void)((ml->ativo == MLATIVO) ? ml->leMemLog(pos, tam) : 0))
#define ESCREVEMEMLOG(pos, tam)                                                \
    ((void)((ml->ativo == MLATIVO) ? ml->escreveMemLog(pos, tam) : 0))

#endif
