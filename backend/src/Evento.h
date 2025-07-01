#ifndef EVENTO_H
#define EVENTO_H

#include <string>
#include <vector>
#include "EventoBase.h"
#include "Participante.h"
#include "Organizador.h"

// Classe Evento herda de EventoBase
class Evento : public EventoBase {
private:
    std::vector<Participante*> participantes;
    std::vector<Organizador*> organizadores;
    std::string categoria; // Conferência, Workshop, Palestra, etc.
    bool eventoAtivo;

public:
    // Construtores
    Evento();
    Evento(std::string n, std::string d, std::string h, std::string l, std::string desc);
    Evento(int id, std::string n, std::string d, std::string h, std::string l, std::string desc);
    
    // Destrutor
    ~Evento();
    
    // Sobrescrevendo métodos virtuais da classe base
    virtual std::string getTipoEvento() const override;
    virtual double calcularPreco() const override;
    virtual bool podeInscricao() const override;
    virtual std::string toString() const override;
    virtual void exibirDetalhes() const override;
    
    // Métodos específicos da classe Evento
    void adicionarParticipante(Participante* p);
    void adicionarOrganizador(Organizador* o);
    void removerParticipante(int index);
    void removerOrganizador(int index);
    
    int getNumParticipantes() const;
    int getNumOrganizadores() const;
    Participante* getParticipante(int i) const;
    Organizador* getOrganizador(int i) const;
    
    std::string getCategoria() const;
    bool getEventoAtivo() const;
    void setCategoria(std::string cat);
    void ativarEvento();
    void desativarEvento();
    
    void atualizarEvento(std::string n, std::string d, std::string h, std::string l, std::string desc);
    void salvarEvento(std::ofstream &arquivo);
};

#endif 