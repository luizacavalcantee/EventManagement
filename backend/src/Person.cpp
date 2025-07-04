#include "Person.h"
#include <regex>     // Para std::regex

Person::Person() : name(""), email(""), contact("") {}

Person::Person(std::string n, std::string e, std::string c)
    : name(n), email(e), contact(c) {}

Person::~Person() {}

void Person::validate() const {
    // Validação de Nome: Não vazio, comprimento mínimo
    if (name.empty()) {
        throw std::invalid_argument("O nome não pode ser vazio.");
    }
    if (name.length() < 3) {
        throw std::invalid_argument("O nome deve ter pelo menos 3 caracteres.");
    }

    // Validação de Email: Formato válido usando regex
    const std::regex email_regex(R"(^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$)");
    if (!std::regex_match(email, email_regex)) {
        throw std::invalid_argument("Formato de email inválido.");
    }

    // Validação de Contato (Telefone): Não vazio (se fornecido), comprimento mínimo, e apenas dígitos
    if (!contact.empty()) {
        if (contact.length() < 8) { // Exemplo: mínimo de 8 dígitos para telefones no Brasil
            throw std::invalid_argument("O contato deve ter pelo menos 8 dígitos.");
        }
        // Opcional: Regex para permitir apenas dígitos. Se quiser permitir () ou -, ajuste o regex.
        const std::regex digits_only_regex(R"(^\d+$)");
        if (!std::regex_match(contact, digits_only_regex)) {
            throw std::invalid_argument("O contato deve conter apenas dígitos.");
        }
    }
}

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