#include "Evento.h"
#include <iostream>
#include <fstream>
#include <ostream>

//sobre a classe Evento

Evento::Evento(std::string n, std::string d, std::string h, std::string l, std::string desc) {
    id = 0; // Será definido pelo gerenciador
    nome = n;
    data = d;
    hora = h;
    local = l;
    descricao = desc;
    numParticipantes = 0;
}

Evento::Evento(int id, std::string n, std::string d, std::string h, std::string l, std::string desc) {
    this->id = id;
    nome = n;
    data = d;
    hora = h;
    local = l;
    descricao = desc;
    numParticipantes = 0;
}

Evento::~Evento() {
    for (int i = 0; i < numParticipantes; i++) {
        delete participantes[i];
    }
}

void Evento::adicionarParticipante(std::string nome, std::string email, std::string contato) {
    if (numParticipantes < 100) {
        participantes[numParticipantes] = new Participante(nome, email, contato);
        numParticipantes++;
        std::cout << "Participante adicionado com sucesso!" << std::endl;
    } else {
        std::cout << "Evento lotado!" << std::endl;
    }
}

int Evento::getId() const {
    return id;
}

std::string Evento::getNome() const {
    return nome;
}

std::string Evento::getData() const {
    return data;
}

std::string Evento::getHora() const {
    return hora;
}

std::string Evento::getLocal() const {
    return local;
}

std::string Evento::getDescricao() const {
    return descricao;
}

int Evento::getNumParticipantes() const {
    return numParticipantes;
}

Participante* Evento::getParticipante(int i) const {
    return participantes[i];
}

void Evento::setId(int id) {
    this->id = id;
}

void Evento::atualizarEvento(std::string n, std::string d, std::string h, std::string l, std::string desc) {
    nome = n;
    data = d;
    hora = h;
    local = l;
    descricao = desc;
}

void Evento::salvarEvento(std::ofstream &arquivo) {
    arquivo << id << "," << nome.c_str() << "," << data.c_str() << "," << hora.c_str() << "," 
            << local.c_str() << "," << descricao.c_str() << std::endl;
    for (int i = 0; i < numParticipantes; i++) {
        arquivo << "PARTICIPANTE," << participantes[i]->toString().c_str() << std::endl;
    }
} 