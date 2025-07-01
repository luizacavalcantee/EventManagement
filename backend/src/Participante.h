#ifndef PARTICIPANTE_H
#define PARTICIPANTE_H

#include "Pessoa.h"

// Classe Participante herda de Pessoa
class Participante : public Pessoa {
private:
    std::string tipoIngresso; // VIP, Normal, Estudante, etc.
    bool presencaConfirmada;

public:
    // Construtores
    Participante();
    Participante(std::string n, std::string e, std::string c = "");
    Participante(std::string n, std::string e, std::string c, std::string tipo);
    
    // Sobrescrevendo métodos virtuais da classe base
    virtual std::string toString() const override;
    virtual std::string getTipo() const override;
    
    // Métodos específicos da classe Participante
    std::string getTipoIngresso() const;
    bool getPresencaConfirmada() const;
    void setTipoIngresso(std::string tipo);
    void confirmarPresenca();
    void cancelarPresenca();
};

#endif 