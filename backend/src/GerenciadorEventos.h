#ifndef GERENCIADOR_EVENTOS_H
#define GERENCIADOR_EVENTOS_H

#include <string>
#include <vector>
#include <stdexcept> // Para std::runtime_error
#include "Evento.h"
#include "nlohmann/json.hpp" // Incluído para a declaração da função

class GerenciadorEventos {
private:
    // MUDANÇA: Substituímos o array C-style e o contador manual
    // por um único std::vector, que é mais seguro e flexível.
    std::vector<Evento*> eventos;
    int nextId;

public:
    GerenciadorEventos();
    ~GerenciadorEventos();

    // --- Métodos para interação via Console ---
    void cadastrarEvento();
    void inscreverParticipante();
    void gerarRelatorio();
    void atualizarEvento();
    void deletarEvento();
    void salvarEventosEmArquivo();
    void carregarEventosDoArquivo();
    void listarEventos();

    // --- Métodos para uma possível API ou uso interno ---
    void cadastrarEvento(const Evento& evento);
    void atualizarEvento(int id, const Evento& evento);
    void deletarEvento(int id);
    const Evento& getEventoPorId(int id) const;
    Evento& getEventoPorId(int id);

    // MUDANÇA: Retorna uma referência constante ao vetor de ponteiros.
    // É mais eficiente e evita "slicing" (copiar objetos derivados como se fossem base).
    const std::vector<Evento*>& getEventos() const;

    nlohmann::json gerarRelatorioJson() const;
};

#endif // GERENCIADOR_EVENTOS_H