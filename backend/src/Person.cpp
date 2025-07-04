#include "Person.h"

Person::Person() : name(""), email(""), contact("") {}

Person::Person(std::string n, std::string e, std::string c) 
    : name(n), email(e), contact(c) {}

Person::~Person() {}

std::string Person::getName() const {
    return name;
}

std::string Person::getEmail() const {
    return email;
}

std::string Person::getContact() const {
    return contact;
}

std::string Person::toString() const {
    return "Name: " + name + ", Email: " + email + ", Contact: " + contact;
}

void Person::setName(std::string n) {
    name = n;
}

void Person::setEmail(std::string e) {
    email = e;
}

void Person::setContact(std::string c) {
    contact = c;
}
