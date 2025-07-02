#include "Pessoa.h"

// Construtores
Pessoa::Pessoa() : nome(""), email(""), contato("") {}

Pessoa::Pessoa(std::string n, std::string e, std::string c) 
    : nome(n), email(e), contato(c) {}

// Destrutor virtual
Pessoa::~Pessoa() {}

// Métodos virtuais
std::string Pessoa::getNome() const {
    return nome;
}

std::string Pessoa::getEmail() const {
    return email;
}

std::string Pessoa::getContato() const {
    return contato;
}

std::string Pessoa::toString() const {
    return "Nome: " + nome + ", Email: " + email + ", Contato: " + contato;
}

// Setters
void Pessoa::setNome(std::string n) {
    nome = n;
}

void Pessoa::setEmail(std::string e) {
    email = e;
}

void Pessoa::setContato(std::string c) {
    contato = c;
} 