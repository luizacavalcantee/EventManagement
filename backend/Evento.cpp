#include "Evento.h"
#include <iostream>
#include <fstream>
#include <ostream>

//sobre a classe Evento

Evento::Evento() : EventoBase(), categoria("Geral"), eventoAtivo(true) {}

Evento::Evento(std::string n, std::string d, std::string h, std::string l, std::string desc)
    : EventoBase(0, n, d, h, l, desc), categoria("Geral"), eventoAtivo(true) {}

Evento::Evento(int id, std::string n, std::string d, std::string h, std::string l, std::string desc)
    : EventoBase(id, n, d, h, l, desc), categoria("Geral"), eventoAtivo(true) {}

Evento::~Evento() {
    for (auto p : participantes) {
        delete p;
    }
    participantes.clear();
    
    for (auto o : organizadores) {
        delete o;
    }
    organizadores.clear();
}

void Evento::adicionarParticipante(Participante* p) {
    if (podeInscricao()) {
        participantes.push_back(p);
    }
}

void Evento::adicionarOrganizador(Organizador* o) {
    organizadores.push_back(o);
}

void Evento::removerParticipante(int index) {
    if (index >= 0 && index < participantes.size()) {
        delete participantes[index];
        participantes.erase(participantes.begin() + index);
    }
}

void Evento::removerOrganizador(int index) {
    if (index >= 0 && index < organizadores.size()) {
        delete organizadores[index];
        organizadores.erase(organizadores.begin() + index);
    }
}

int Evento::getNumParticipantes() const {
    return participantes.size();
}

int Evento::getNumOrganizadores() const {
    return organizadores.size();
}

Participante* Evento::getParticipante(int i) const {
    if (i >= 0 && i < participantes.size()) {
        return participantes[i];
    }
    return nullptr;
}

Organizador* Evento::getOrganizador(int i) const {
    if (i >= 0 && i < organizadores.size()) {
        return organizadores[i];
    }
    return nullptr;
}

std::string Evento::getCategoria() const {
    return categoria;
}

bool Evento::getEventoAtivo() const {
    return eventoAtivo;
}

void Evento::setCategoria(std::string cat) {
    categoria = cat;
}

void Evento::ativarEvento() {
    eventoAtivo = true;
}

void Evento::desativarEvento() {
    eventoAtivo = false;
}

void Evento::atualizarEvento(std::string n, std::string d, std::string h, std::string l, std::string desc) {
    setNome(n);
    setData(d);
    setHora(h);
    setLocal(l);
    setDescricao(desc);
}

void Evento::salvarEvento(std::ofstream &arquivo) {
    arquivo << getId() << "," << getNome() << "," << getData() << "," 
           << getHora() << "," << getLocal() << "," << getDescricao() << std::endl;
}

std::string Evento::getTipoEvento() const {
    return "Evento Geral";
}

double Evento::calcularPreco() const {
    return preco;
}

bool Evento::podeInscricao() const {
    return eventoAtivo && participantes.size() < capacidadeMaxima;
}

std::string Evento::toString() const {
    return EventoBase::toString() + ", Categoria: " + categoria + 
           ", Participantes: " + std::to_string(participantes.size()) + 
           ", Status: " + (eventoAtivo ? "Ativo" : "Inativo");
}

void Evento::exibirDetalhes() const {
    EventoBase::exibirDetalhes();
    std::cout << "Categoria: " << categoria << std::endl;
    std::cout << "Status: " << (eventoAtivo ? "Ativo" : "Inativo") << std::endl;
    std::cout << "Participantes: " << participantes.size() << "/" << capacidadeMaxima << std::endl;
    std::cout << "Organizadores: " << organizadores.size() << std::endl;
} 