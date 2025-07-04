#include "Participant.h"

Participant::Participant() : Person(), id(0) {}

Participant::Participant(int id, std::string name, std::string email, std::string contact)
    : Person(name, email, contact), id(id) {}

std::string Participant::toString() const {
    return "ID: " + std::to_string(id) + ", " + Person::toString();
}

int Participant::getId() const {
    return id;
}

void Participant::setId(int newId) {
    id = newId;
}