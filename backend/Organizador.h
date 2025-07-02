#ifndef ORGANIZADOR_H
#define ORGANIZADOR_H

#include "Pessoa.h"

// Classe Organizador herda de Pessoa
class Organizador : public Pessoa {
private:
    std::string cargo; // Coordenador, Assistente, etc.
    std::string departamento;
    int nivelAcesso; // 1-5, onde 5 é o mais alto

public:
    // Construtores
    Organizador();
    Organizador(std::string n, std::string e, std::string c);
    Organizador(std::string n, std::string e, std::string c, std::string cargo, std::string dept);
    
    // Sobrescrevendo métodos virtuais da classe base
    virtual std::string toString() const override;
    virtual std::string getTipo() const override;
    
    // Métodos específicos da classe Organizador
    std::string getCargo() const;
    std::string getDepartamento() const;
    int getNivelAcesso() const;
    void setCargo(std::string c);
    void setDepartamento(std::string d);
    void setNivelAcesso(int nivel);
    bool podeEditarEvento() const;
    bool podeExcluirEvento() const;
};

#endif 