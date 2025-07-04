#include "Event.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <regex>     // Para validação de data e hora
#include <chrono>    // Para std::chrono
#include <ctime>     // Para std::tm e std::mktime

Event::Event()
    : id(0), name(""), date(""), time(""), location(""), description(""),
      maxCapacity(100), price(0.0), nextParticipantId(1), category("General"), isActive(true) {}

Event::Event(int id, std::string name, std::string date, std::string time, std::string location, std::string description)
    : id(id), name(name), date(date), time(time), location(location), description(description),
      maxCapacity(100), price(0.0), nextParticipantId(1), category("General"), isActive(true) {}

Event::~Event() {
    for (Participant* p : participants) {
        delete p;
    }
    participants.clear();
}

// Método de validação com o novo parâmetro `forCreation`
void Event::validate(bool forCreation) const {
    // Validação de Nome: Não vazio, comprimento mínimo
    if (name.empty()) {
        throw std::invalid_argument("O nome do evento não pode ser vazio.");
    }
    if (name.length() < 5) {
        throw std::invalid_argument("O nome do evento deve ter pelo menos 5 caracteres.");
    }
    // Validação de Local: Não vazio
    if (location.empty()) {
        throw std::invalid_argument("O local do evento não pode ser vazio.");
    }
    // Validação de Descrição: Não vazia
    if (description.empty()) {
        throw std::invalid_argument("A descrição do evento não pode ser vazia.");
    }

    // Validação de capacidade e preço (mantidas como antes)
    if (maxCapacity <= 0) {
        throw std::invalid_argument("A capacidade máxima deve ser um número positivo.");
    }
    if (price < 0) {
        throw std::invalid_argument("O preço não pode ser negativo.");
    }

    // Validação de Data: Formato DD/MM/AAAA válido e data existente
    if (!isValidDate(date)) {
        throw std::invalid_argument("Formato de data inválido ou data inexistente. Use DD/MM/AAAA.");
    }
    // Validação de Hora: Formato HH:MM válido
    if (!isValidTime(time)) {
        throw std::invalid_argument("Formato de hora inválido. Use HH:MM.");
    }

    // VALIDAÇÃO CONDICIONAL: Data/Hora no Futuro APENAS para criação de evento
    if (forCreation) {
        if (!isFutureDateTime(date, time)) {
            throw std::invalid_argument("A data e hora de um novo evento devem ser no futuro.");
        }
    }
}

bool Event::isValidDate(const std::string& d) const {
    const std::regex date_regex(R"(^\d{2}/\d{2}/\d{4}$)");
    if (!std::regex_match(d, date_regex)) {
        return false;
    }

    int day = std::stoi(d.substr(0, 2));
    int month = std::stoi(d.substr(3, 2));
    int year = std::stoi(d.substr(6, 4));

    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        daysInMonth[2] = 29; // Ano bissexto
    }

    if (day < 1 || day > daysInMonth[month]) return false;

    return true;
}

bool Event::isValidTime(const std::string& t) const {
    const std::regex time_regex(R"(^\d{2}:\d{2}$)");
    if (!std::regex_match(t, time_regex)) {
        return false;
    }
    int hour = std::stoi(t.substr(0, 2));
    int minute = std::stoi(t.substr(3, 2));
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
        return false;
    }
    return true;
}

bool Event::isFutureDateTime(const std::string& d, const std::string& t) const {
    std::tm event_tm = {};
    std::istringstream ss(d + " " + t);

    ss >> std::get_time(&event_tm, "%d/%m/%Y %H:%M");

    if (ss.fail()) {
        return false; // Falha no parsing
    }

    std::time_t event_time_t = std::mktime(&event_tm);
    std::time_t current_time_t = std::time(nullptr);

    if (event_time_t == static_cast<std::time_t>(-1)) {
        return false; // Erro de conversão de data/hora
    }

    // Adiciona uma pequena margem (ex: 5 segundos) para considerar a precisão do sistema
    return event_time_t > current_time_t + 5;
}

int Event::getId() const { return id; }
std::string Event::getName() const { return name; }
std::string Event::getDate() const { return date; }
std::string Event::getTime() const { return time; }
std::string Event::getLocation() const { return location; }
std::string Event::getDescription() const { return description; }
int Event::getMaxCapacity() const { return maxCapacity; }
double Event::getPrice() const { return price; }
int Event::getNumParticipants() const { return participants.size(); }
std::string Event::getCategory() const { return category; }
bool Event::isActiveEvent() const { return isActive; }

void Event::setId(int id) { this->id = id; }
void Event::setName(std::string name) { this->name = name; }
void Event::setDate(std::string date) { this->date = date; }
void Event::setTime(std::string time) { this->time = time; }
void Event::setLocation(std::string location) { this->location = location; }
void Event::setDescription(std::string description) { this->description = description; }
void Event::setMaxCapacity(int capacity) { this->maxCapacity = capacity; }
void Event::setPrice(double p) { this->price = p; }
void Event::setCategory(std::string cat) { this->category = cat; }
void Event::activateEvent() { isActive = true; }
void Event::deactivateEvent() { isActive = false; }

void Event::updateEvent(std::string name, std::string date, std::string time, std::string location, std::string description) {
    setName(name);
    setDate(date);
    setTime(time);
    setLocation(location);
    setDescription(description);
}

void Event::saveEvent(std::ofstream &file) {
    file << "EVENT," << id << "," << name << "," << date << ","
          << time << "," << location << "," << description << ","
          << maxCapacity << "," << price << "," << category << ","
          << (isActive ? "true" : "false") << "," << nextParticipantId << std::endl;
    for (const auto& participant : participants) {
        file << "PARTICIPANT," << participant->getId() << ","
             << participant->getName() << "," << participant->getEmail() << ","
             << participant->getContact() << std::endl;
    }
}

void Event::addParticipant(Participant* p) {
    if (p->getId() == 0) {
        p->setId(nextParticipantId++);
    } else {
        if (p->getId() >= nextParticipantId) {
            nextParticipantId = p->getId() + 1;
        }
    }
    participants.push_back(p);
}

Participant* Event::addParticipant(std::string name, std::string email, std::string contact) {
    Participant* newParticipant = new Participant(nextParticipantId, name, email, contact);
    participants.push_back(newParticipant);
    nextParticipantId++;
    return newParticipant;
}

Participant* Event::getParticipant(int participantId) const {
    for (Participant* p : participants) {
        if (p->getId() == participantId) {
            return p;
        }
    }
    return nullptr;
}

const std::vector<Participant*>& Event::getAllParticipants() const {
    return participants;
}

bool Event::updateParticipant(int participantId, const std::string& newName, const std::string& newEmail, const std::string& newContact) {
    for (Participant* p : participants) {
        if (p->getId() == participantId) {
            p->setName(newName);
            p->setEmail(newEmail);
            p->setContact(newContact);
            return true;
        }
    }
    return false;
}

bool Event::removeParticipant(int participantId) {
    for (auto it = participants.begin(); it != participants.end(); ++it) {
        if ((*it)->getId() == participantId) {
            delete *it;
            participants.erase(it);
            return true;
        }
    }
    return false;
}

double Event::calculatePrice() const {
    return price;
}

bool Event::canRegister() const {
    return isActive && (getNumParticipants() < maxCapacity);
}

std::string Event::toString() const {
    std::stringstream ss;
    ss << "ID: " << id << ", Name: " << name << ", Date: " << date
       << ", Time: " << time << ", Location: " << location
       << ", Description: " << description
       << ", Category: " << category
       << ", Status: " << (isActive ? "Active" : "Inactive")
       << ", Participants: " << participants.size() << "/" << maxCapacity
       << ", Price: " << std::fixed << std::setprecision(2) << price;
    return ss.str();
}

void Event::displayDetails() const {
    std::cout << "--- Event Details ---" << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Date: " << date << std::endl;
    std::cout << "Time: " << time << std::endl;
    std::cout << "Location: " << location << std::endl;
    std::cout << "Description: " << description << std::endl;
    std::cout << "Category: " << category << std::endl;
    std::cout << "Status: " << (isActive ? "Active" : "Inactive") << std::endl;
    std::cout << "Participants: " << participants.size() << "/" << maxCapacity << std::endl;
    std::cout << "Price: " << std::fixed << std::setprecision(2) << price << std::endl;
}