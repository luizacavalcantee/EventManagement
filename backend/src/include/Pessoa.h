#ifndef PESSOA_H
#define PESSOA_H

#include <string>

// Classe base para todas as pessoas no sistema
class Pessoa {
protected:
    std::string nome;
    std::string email;
    std::string contato;

public:
    // Construtores
    Pessoa();
    Pessoa(std::string n, std::string e, std::string c = "");
    virtual ~Pessoa(); // Destrutor virtual para permitir herança

    // Métodos virtuais (podem ser sobrescritos pelas classes filhas)
    virtual std::string getNome() const;
    virtual std::string getEmail() const;
    virtual std::string getContato() const;
    virtual std::string toString() const;
    virtual std::string getTipo() const = 0; // Método puro virtual - classe abstrata

    // Setters
    virtual void setNome(std::string n);
    virtual void setEmail(std::string e);
    virtual void setContato(std::string c);
};

#endif 