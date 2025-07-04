#include "Person.h"
#include <regex>     // Para std::regex
#include <algorithm> // Para std::remove_if
#include <string>    // Para std::string
#include <iomanip>   // Para std::setw, std::setfill, se for usar para formatar

Person::Person() : name(""), email(""), contact("") {}

// O construtor agora chama os setters para garantir validação e formatação
Person::Person(std::string n, std::string e, std::string c) {
    setName(n);
    setEmail(e);
    setContact(c); // Chama o setter que valida e formata
}

Person::~Person() {}

// Função auxiliar para remover caracteres não-dígitos
// Mantida como uma função livre ou pode ser um método estático/privado
std::string extractDigits(const std::string& str) {
    std::string digits_only;
    std::copy_if(str.begin(), str.end(), std::back_inserter(digits_only), ::isdigit);
    return digits_only;
}

// Implementação da nova função de formatação
std::string Person::formatPhoneNumber(const std::string& digits_only) const {
    if (digits_only.length() == 10) { // Ex: 8133334444 -> (81) 3333-4444
        return "(" + digits_only.substr(0, 2) + ") " +
               digits_only.substr(2, 4) + "-" +
               digits_only.substr(6, 4);
    } else if (digits_only.length() == 11) { // Ex: 81988887777 -> (81) 98888-7777
        return "(" + digits_only.substr(0, 2) + ") " +
               digits_only.substr(2, 5) + "-" +
               digits_only.substr(7, 4);
    }
    // Se chegou aqui, significa que o comprimento não é 10 nem 11,
    // o que já deveria ter sido pego pela validação em setContact.
    // Retorna a string original ou lança um erro, dependendo da robustez desejada.
    return digits_only; // Fallback, mas não deve ser atingido se a validação for correta.
}


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

    // A validação e formatação do 'contact' agora é feita no setContact.
    // Não precisamos duplicar a lógica aqui, apenas garantir que o campo 'contact'
    // não esteja vazio se for um requisito, mas o setContact já garante o formato.
    // Se o contato for opcional e vazio, ele passará.
}

std::string Person::getName() const {
    return name;
}

std::string Person::getEmail() const {
    return email;
}

std::string Person::getContact() const {
    return contact; // Retorna a string já formatada
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

// Novo setContact que valida e formata
void Person::setContact(std::string c) {
    if (c.empty()) {
        contact = ""; // Permite contato vazio se for opcional
        return;
    }

    std::string cleaned_contact = extractDigits(c); // Remove caracteres não-dígitos

    // Valida o comprimento dos dígitos
    if (cleaned_contact.length() < 10 || cleaned_contact.length() > 11) {
        throw std::invalid_argument("Formato de contato inválido. O telefone com DDD deve ter 10 ou 11 dígitos.");
    }

    // Formata e armazena o número
    contact = formatPhoneNumber(cleaned_contact);
}