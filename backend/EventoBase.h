#ifndef EVENTO_BASE_H
#define EVENTO_BASE_H

#include <string>
#include <vector>
#include "Pessoa.h"

// Classe base abstrata para todos os tipos de eventos
class EventoBase {
private:
    int id;
    std::string nome;
    std::string data;
    std::string hora;
    std::string local;
    std::string descricao;
    int capacidadeMaxima;
    double preco;

public:
    // Construtores
    EventoBase();
    EventoBase(int id, std::string n, std::string d, std::string h, std::string l, std::string desc);
    virtual ~EventoBase(); // Destrutor virtual

    // Métodos virtuais puros (classe abstrata)
    virtual std::string getTipoEvento() const = 0;
    virtual double calcularPreco() const = 0;
    virtual bool podeInscricao() const = 0;

    // Métodos virtuais (podem ser sobrescritos)
    virtual std::string toString() const;
    virtual void exibirDetalhes() const;

    // Getters e Setters
    virtual int getId() const;
    virtual std::string getNome() const;
    virtual std::string getData() const;
    virtual std::string getHora() const;
    virtual std::string getLocal() const;
    virtual std::string getDescricao() const;
    virtual int getCapacidadeMaxima() const;
    virtual double getPreco() const;

    virtual void setId(int id);
    virtual void setNome(std::string n);
    virtual void setData(std::string d);
    virtual void setHora(std::string h);
    virtual void setLocal(std::string l);
    virtual void setDescricao(std::string desc);
    virtual void setCapacidadeMaxima(int cap);
    virtual void setPreco(double p);
};

#endif 