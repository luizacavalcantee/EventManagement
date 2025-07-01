#include "EventoBase.h"
#include <iostream>

// Construtores
EventoBase::EventoBase() : id(0), nome(""), data(""), hora(""), local(""), 
                          descricao(""), capacidadeMaxima(100), preco(0.0) {}

EventoBase::EventoBase(int id, std::string n, std::string d, std::string h, std::string l, std::string desc)
    : id(id), nome(n), data(d), hora(h), local(l), descricao(desc), 
      capacidadeMaxima(100), preco(0.0) {}

// Destrutor virtual
EventoBase::~EventoBase() {}

// Métodos virtuais
std::string EventoBase::toString() const {
    return "ID: " + std::to_string(id) + ", Nome: " + nome + ", Data: " + data + 
           ", Hora: " + hora + ", Local: " + local + ", Preço: R$ " + std::to_string(preco);
}

void EventoBase::exibirDetalhes() const {
    std::cout << "=== Detalhes do Evento ===" << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "Nome: " << nome << std::endl;
    std::cout << "Data: " << data << std::endl;
    std::cout << "Hora: " << hora << std::endl;
    std::cout << "Local: " << local << std::endl;
    std::cout << "Descrição: " << descricao << std::endl;
    std::cout << "Capacidade: " << capacidadeMaxima << std::endl;
    std::cout << "Preço: R$ " << preco << std::endl;
}

// Getters
int EventoBase::getId() const { return id; }
std::string EventoBase::getNome() const { return nome; }
std::string EventoBase::getData() const { return data; }
std::string EventoBase::getHora() const { return hora; }
std::string EventoBase::getLocal() const { return local; }
std::string EventoBase::getDescricao() const { return descricao; }
int EventoBase::getCapacidadeMaxima() const { return capacidadeMaxima; }
double EventoBase::getPreco() const { return preco; }

// Setters
void EventoBase::setId(int id) { this->id = id; }
void EventoBase::setNome(std::string n) { nome = n; }
void EventoBase::setData(std::string d) { data = d; }
void EventoBase::setHora(std::string h) { hora = h; }
void EventoBase::setLocal(std::string l) { local = l; }
void EventoBase::setDescricao(std::string desc) { descricao = desc; }
void EventoBase::setCapacidadeMaxima(int cap) { capacidadeMaxima = cap; }
void EventoBase::setPreco(double p) { preco = p; } 