#ifndef GERENCIADOR_EVENTOS_H
#define GERENCIADOR_EVENTOS_H

#include <string>
#include <vector>
#include "Evento.h"

//declração da classe GerenciadorEventos

class GerenciadorEventos {
private:
    Evento* eventos[10];
    int numEventos;
    int nextId;

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

    void cadastrarEvento(const Evento& evento);
    void atualizarEvento(int id, const Evento& evento);
    void deletarEvento(int id);
    const Evento& getEventoPorId(int id) const;
    Evento& getEventoPorId(int id);
    std::vector<Evento> getEventos() const;
    nlohmann::json gerarRelatorioJson() const;
};

#endif 