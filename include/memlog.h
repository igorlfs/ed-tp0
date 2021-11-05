#ifndef MEMLOGH
#define MEMLOGH
#include <fstream>
#include <string>
#include <time.h>
class memlog {
  public:
    int iniciaMemLog(const std::string &nome);
    int ativaMemLog();
    int desativaMemLog();
    int defineFaseMemLog(const int &f);
    int leMemLog(const long int &pos, const long int &tam);
    int escreveMemLog(const long int &pos, const long int &tam);
    int finalizaMemLog();

  private:
    const static char ESCRITA = 'E';
    const static char LEITURA = 'L';
    std::ofstream log;
    clockid_t clk_id;
    struct timespec inittime;
    std::string nome;
    long count;
    int fase;
    int ativo;

    int geralMemLog(const char &c, const long int &pos, const long int &tam);
};
#endif
