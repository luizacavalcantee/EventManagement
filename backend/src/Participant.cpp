#include "Participante.h"

//´sobre a classe Participante

// Construtores
Participante::Participante() : Pessoa(), tipoIngresso("Normal"), presencaConfirmada(false) {}

Participante::Participante(std::string n, std::string e, std::string c) 
    : Pessoa(n, e, c), tipoIngresso("Normal"), presencaConfirmada(false) {}

Participante::Participante(std::string n, std::string e, std::string c, std::string tipo)
    : Pessoa(n, e, c), tipoIngresso(tipo), presencaConfirmada(false) {}

// Sobrescrevendo métodos virtuais da classe base
std::string Participante::toString() const {
    return Pessoa::toString() + ", Tipo: " + tipoIngresso + 
           ", Presença: " + (presencaConfirmada ? "Confirmada" : "Pendente");
}

std::string Participante::getTipo() const {
    return "Participante";
}

// Métodos específicos da classe Participante
std::string Participante::getTipoIngresso() const {
    return tipoIngresso;
}

bool Participante::getPresencaConfirmada() const {
    return presencaConfirmada;
}

void Participante::setTipoIngresso(std::string tipo) {
    tipoIngresso = tipo;
}

void Participante::confirmarPresenca() {
    presencaConfirmada = true;
}

void Participante::cancelarPresenca() {
    presencaConfirmada = false;
} 