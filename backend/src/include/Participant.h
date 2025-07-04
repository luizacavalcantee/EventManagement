#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <string>
#include "Person.h"

class Participant : public Person {
private:
    int id;

public:
    Participant();
    Participant(int id, std::string name, std::string email, std::string contact = "");
    virtual std::string toString() const override;
    int getId() const;
    void setId(int newId);
    // Herda validate() de Person, não precisa de override a menos que adicione regras específicas de participante
};

#endif // PARTICIPANT_H