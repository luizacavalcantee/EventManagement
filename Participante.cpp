#include "Participante.h"

//´sobre a classe Participante

Participante::Participante(std::string n, std::string e, std::string c) {
    nome = n;
    email = e;
    contato = c;
}

std::string Participante::getNome() const {
    return nome;
}

std::string Participante::getEmail() const {
    return email;
}

std::string Participante::getContato() const {
    return contato;
}

std::string Participante::toString() const {
    return nome + "," + email + "," + contato;
} 