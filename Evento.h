#ifndef EVENTO_H
#define EVENTO_H

#include <string>
#include "Participante.h"

//sobre classe evento

class Evento {
private:
    std::string nome;
    std::string data;
    std::string hora;
    std::string local;
    std::string descricao;
    Participante* participantes[100];
    int numParticipantes;

public:
    Evento(std::string n, std::string d, std::string h, std::string l, std::string desc);
    ~Evento();

    void adicionarParticipante(std::string nome, std::string email, std::string contato = "");
    
    std::string getNome() const;
    std::string getData() const;
    std::string getHora() const;
    std::string getLocal() const;
    std::string getDescricao() const;
    int getNumParticipantes() const;
    Participante* getParticipante(int i) const;

    void atualizarEvento(std::string n, std::string d, std::string h, std::string l, std::string desc);
    void salvarEvento(std::ofstream &arquivo);
};

#endif 