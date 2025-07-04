#include "Person.h"
#include <regex>
#include <algorithm>
#include <string>
#include <iomanip>

Person::Person() : name(""), email(""), contact("") {}

Person::Person(std::string n, std::string e, std::string c) {
    setName(n);
    setEmail(e);
    setContact(c);
}

Person::~Person() {}

std::string extractDigits(const std::string& str) {
    std::string digits_only;
    std::copy_if(str.begin(), str.end(), std::back_inserter(digits_only), ::isdigit);
    return digits_only;
}

std::string Person::formatPhoneNumber(const std::string& digits_only) const {
    if (digits_only.length() == 10) {
        return "(" + digits_only.substr(0, 2) + ") " +
               digits_only.substr(2, 4) + "-" +
               digits_only.substr(6, 4);
    } else if (digits_only.length() == 11) {
        return "(" + digits_only.substr(0, 2) + ") " +
               digits_only.substr(2, 5) + "-" +
               digits_only.substr(7, 4);
    }
    return digits_only;
}

void Person::validate() const {
    if (name.empty()) {
        throw std::invalid_argument("O nome não pode ser vazio.");
    }
    if (name.length() < 3) {
        throw std::invalid_argument("O nome deve ter pelo menos 3 caracteres.");
    }

    const std::regex email_regex(R"(^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$)");
    if (!std::regex_match(email, email_regex)) {
        throw std::invalid_argument("Formato de email inválido.");
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
    if (c.empty()) {
        contact = "";
        return;
    }

    std::string cleaned_contact = extractDigits(c);

    if (cleaned_contact.length() < 10 || cleaned_contact.length() > 11) {
        throw std::invalid_argument("Formato de contato inválido. O telefone com DDD deve ter 10 ou 11 dígitos.");
    }

    contact = formatPhoneNumber(cleaned_contact);
}
