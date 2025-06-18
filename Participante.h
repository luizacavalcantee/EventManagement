#ifndef PARTICIPANTE_H
#define PARTICIPANTE_H

//declaração da classe Participante

#include <string>

class Participante {
private:
    std::string nome;
    std::string email;
    std::string contato;

public:
    Participante(std::string n, std::string e, std::string c = "");
    
    std::string getNome() const;
    std::string getEmail() const;
    std::string getContato() const;
    std::string toString() const;
};

#endif 