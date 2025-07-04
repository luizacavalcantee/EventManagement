#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <stdexcept> // Para std::invalid_argument

class Person {
protected:
    std::string name;
    std::string email;
    std::string contact; // Agora armazenará o formato (DD) NNNNN-NNNN

    // Nova função auxiliar para formatar o número de telefone
    std::string formatPhoneNumber(const std::string& digits_only) const;

public:
    Person();
    Person(std::string n, std::string e, std::string c);
    virtual ~Person();

    // O método validate() agora só validará nome e email, pois setContact fará a validação do contato
    void validate() const;

    virtual std::string getName() const;
    virtual std::string getEmail() const;
    virtual std::string getContact() const;

    // ESTA É A LINHA CRÍTICA QUE PRECISA TER 'virtual'
    virtual std::string toString() const;

    virtual void setName(std::string n);
    virtual void setEmail(std::string e);
    virtual void setContact(std::string c); // Modificado para validar e formatar
};

#endif // PERSON_H