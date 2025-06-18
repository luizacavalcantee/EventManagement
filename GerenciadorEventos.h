#ifndef GERENCIADOR_EVENTOS_H
#define GERENCIADOR_EVENTOS_H

#include <string>
#include "Evento.h"

class GerenciadorEventos {
private:
    Evento* eventos[10];
    int numEventos;

public:
    GerenciadorEventos();
    ~GerenciadorEventos();

    void cadastrarEvento();
    void inscreverParticipante();
    void gerarRelatorio();
    void atualizarEvento();
    void deletarEvento();
    void salvarEventosEmArquivo();
    void carregarEventosDoArquivo();
    void listarEventos();
};

#endif 