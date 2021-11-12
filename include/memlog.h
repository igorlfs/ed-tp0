#ifndef MEMLOGH
#define MEMLOGH
#include <fstream>
#include <string>
#include <time.h>

// Constantes para estados de registro
static constexpr bool MLATIVO = 1;
static constexpr bool MLINATIVO = 0;

// Constantes para operações de registro
const static char ESCRITA = 'E';
const static char LEITURA = 'L';

class memlog {
  public:
    int iniciaMemLog(const std::string &nome);
    int ativaMemLog();
    int desativaMemLog();
    int defineFaseMemLog(const int &f);
    int defineId(const int &f);
    int leMemLog(const long int &pos, const long int &tam, const int &id);
    int escreveMemLog(const long int &pos, const long int &tam, const int &id);
    int finalizaMemLog();

    // esse membro é público porque assim evitamos uma chamada de função na macro
    // (seria meio bobo usar um getter aqui quando o que queremos é justamente
    // evitar a chamada de funções para aumentar a performance)
    int ativo;

  private:
    std::ofstream log;
    clockid_t clk_id;
    struct timespec inittime;
    std::string nome;
    long count;
    int fase;
    int id;

    int geralMemLog(const char &c, const long int &pos, const long int &tam);
};

// MACROS para aumentar a perfomance
#define LEMEMLOG(pos, tam, id)                                                 \
    ((void)((ml.ativo == MLATIVO) ? ml.leMemLog(pos, tam, id) : 0))
#define ESCREVEMEMLOG(pos, tam, id)                                            \
    ((void)((ml.ativo == MLATIVO) ? ml.escreveMemLog(pos, tam, id) : 0))

// Variável global de memlog
extern memlog ml;

#endif
