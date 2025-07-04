#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <stdexcept>

// Classe base que representa uma pessoa genérica
class Person {
protected:
    std::string name;    // Nome da pessoa
    std::string email;   // Email da pessoa
    std::string contact; // Contato (telefone) da pessoa

    // Método auxiliar para formatar o número de telefone
    std::string formatPhoneNumber(const std::string& digits_only) const;

public:
    // Construtor padrão
    Person();

    // Construtor com parâmetros: nome, email e contato
    Person(std::string n, std::string e, std::string c);

    // Destrutor virtual
    virtual ~Person();

    // Valida os dados da pessoa (nome, email, contato)
    void validate() const;

    // Métodos de acesso (getters)
    virtual std::string getName() const;
    virtual std::string getEmail() const;
    virtual std::string getContact() const;

    // Retorna uma representação em string do objeto
    virtual std::string toString() const;

    // Métodos de modificação (setters)
    virtual void setName(std::string n);
    virtual void setEmail(std::string e);
    virtual void setContact(std::string c);
};

#endif // PERSON_H
