#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <stdexcept> // Inclua para std::invalid_argument

class Person {
protected:
    std::string name;
    std::string email;
    std::string contact;

public:
    Person();
    Person(std::string n, std::string e, std::string c = "");
    virtual ~Person();

    // Método de validação
    virtual void validate() const;

    virtual std::string getName() const;
    virtual std::string getEmail() const;
    virtual std::string getContact() const;
    virtual std::string toString() const;

    virtual void setName(std::string n);
    virtual void setEmail(std::string e);
    virtual void setContact(std::string c);
};

#endif // PERSON_H