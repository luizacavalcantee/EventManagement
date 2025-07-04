#include "Person.h"
#include <regex>
#include <algorithm>
#include <string>
#include <iomanip>

// Construtor padrão: inicializa os atributos como strings vazias
Person::Person() : name(""), email(""), contact("") {}

// Construtor com parâmetros: utiliza os setters para validar e definir os atributos
Person::Person(std::string n, std::string e, std::string c) {
    setName(n);
    setEmail(e);
    setContact(c);
}

// Destrutor padrão
Person::~Person() {}

// Função auxiliar para extrair apenas os dígitos de uma string
std::string extractDigits(const std::string& str) {
    std::string digits_only;
    // Copia apenas os caracteres que são dígitos para a nova string
    std::copy_if(str.begin(), str.end(), std::back_inserter(digits_only), ::isdigit);
    return digits_only;
}

// Formata o número de telefone conforme o padrão brasileiro (com ou sem nono dígito)
std::string Person::formatPhoneNumber(const std::string& digits_only) const {
    if (digits_only.length() == 10) {
        // Formato: (XX) XXXX-XXXX
        return "(" + digits_only.substr(0, 2) + ") " +
               digits_only.substr(2, 4) + "-" +
               digits_only.substr(6, 4);
    } else if (digits_only.length() == 11) {
        // Formato: (XX) XXXXX-XXXX
        return "(" + digits_only.substr(0, 2) + ") " +
               digits_only.substr(2, 5) + "-" +
               digits_only.substr(7, 4);
    }
    // Retorna apenas os dígitos se não estiver no formato esperado
    return digits_only;
}

// Valida os atributos da pessoa
void Person::validate() const {
    if (name.empty()) {
        throw std::invalid_argument("O nome não pode ser vazio.");
    }
    if (name.length() < 3) {
        throw std::invalid_argument("O nome deve ter pelo menos 3 caracteres.");
    }

    // Expressão regular para validar o formato do email
    const std::regex email_regex(R"(^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$)");
    if (!std::regex_match(email, email_regex)) {
        throw std::invalid_argument("Formato de email inválido.");
    }
}

// Retorna o nome
std::string Person::getName() const {
    return name;
}

// Retorna o email
std::string Person::getEmail() const {
    return email;
}

// Retorna o contato (telefone formatado)
std::string Person::getContact() const {
    return contact;
}

// Retorna uma representação em string do objeto
std::string Person::toString() const {
    return "Name: " + name + ", Email: " + email + ", Contact: " + contact;
}

// Define o nome
void Person::setName(std::string n) {
    name = n;
}

// Define o email
void Person::setEmail(std::string e) {
    email = e;
}

// Define o contato (telefone), validando e formatando
void Person::setContact(std::string c) {
    if (c.empty()) {
        contact = "";
        return;
    }

    // Remove caracteres não numéricos
    std::string cleaned_contact = extractDigits(c);

    // Verifica se o número tem 10 ou 11 dígitos
    if (cleaned_contact.length() < 10 || cleaned_contact.length() > 11) {
        throw std::invalid_argument("Formato de contato inválido. O telefone com DDD deve ter 10 ou 11 dígitos.");
    }

    // Formata e armazena o contato
    contact = formatPhoneNumber(cleaned_contact);
}