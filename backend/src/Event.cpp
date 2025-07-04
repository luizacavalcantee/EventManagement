#include "Event.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <regex>     // Para validação de data e hora
#include <chrono>    // Para std::chrono
#include <ctime>     // Para std::tm e std::mktime

// Construtor padrão: inicializa os membros com valores padrão
Event::Event()
    : id(0), name(""), date(""), time(""), location(""), description(""),
      maxCapacity(100), price(0.0), nextParticipantId(1), category("General"), isActive(true) {}

// Construtor com parâmetros principais
Event::Event(int id, std::string name, std::string date, std::string time, std::string location, std::string description)
    : id(id), name(name), date(date), time(time), location(location), description(description),
      maxCapacity(100), price(0.0), nextParticipantId(1), category("General"), isActive(true) {}

// Destrutor: libera memória dos participantes
Event::~Event() {
    for (Participant* p : participants) {
        delete p;
    }
    participants.clear();
}

// Método de validação dos dados do evento
void Event::validate(bool forCreation) const {
    // Nome não pode ser vazio e deve ter pelo menos 5 caracteres
    if (name.empty()) {
        throw std::invalid_argument("O nome do evento não pode ser vazio.");
    }
    if (name.length() < 5) {
        throw std::invalid_argument("O nome do evento deve ter pelo menos 5 caracteres.");
    }
    // Local não pode ser vazio
    if (location.empty()) {
        throw std::invalid_argument("O local do evento não pode ser vazio.");
    }
    // Descrição não pode ser vazia
    if (description.empty()) {
        throw std::invalid_argument("A descrição do evento não pode ser vazia.");
    }
    // Capacidade máxima deve ser positiva
    if (maxCapacity <= 0) {
        throw std::invalid_argument("A capacidade máxima deve ser um número positivo.");
    }
    // Preço não pode ser negativo
    if (price < 0) {
        throw std::invalid_argument("O preço não pode ser negativo.");
    }
    // Data deve ser válida
    if (!isValidDate(date)) {
        throw std::invalid_argument("Formato de data inválido ou data inexistente. Use DD/MM/AAAA.");
    }
    // Hora deve ser válida
    if (!isValidTime(time)) {
        throw std::invalid_argument("Formato de hora inválido. Use HH:MM.");
    }
    // Se for criação, data/hora devem estar no futuro
    if (forCreation) {
        if (!isFutureDateTime(date, time)) {
            throw std::invalid_argument("A data e hora de um novo evento devem ser no futuro.");
        }
    }
}

// Valida se a data está no formato correto e é uma data existente
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
    // Ajusta para ano bissexto
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        daysInMonth[2] = 29;
    }
    if (day < 1 || day > daysInMonth[month]) return false;
    return true;
}

// Valida se a hora está no formato correto e é uma hora válida
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

// Verifica se a data/hora informada está no futuro em relação ao sistema
bool Event::isFutureDateTime(const std::string& d, const std::string& t) const {
    std::tm event_tm = {};
    std::istringstream ss(d + " " + t);
    ss >> std::get_time(&event_tm, "%d/%m/%Y %H:%M");
    if (ss.fail()) {
        return false;
    }
    std::time_t now_time_t = std::time(nullptr);
    std::tm now_tm;
    #ifdef _WIN32
        localtime_s(&now_tm, &now_time_t);
    #else
        localtime_r(&now_time_t, &now_tm);
    #endif
    std::time_t event_epoch = std::mktime(&event_tm);
    std::time_t now_epoch = std::mktime(&now_tm);
    if (event_epoch == static_cast<std::time_t>(-1) || now_epoch == static_cast<std::time_t>(-1)) {
        return false;
    }
    // Adiciona buffer de 5 segundos para evitar falsos negativos
    return event_epoch > now_epoch + 5;
}

// Métodos getters
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

// Métodos setters
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

// Atualiza os dados principais do evento
void Event::updateEvent(std::string name, std::string date, std::string time, std::string location, std::string description) {
    setName(name);
    setDate(date);
    setTime(time);
    setLocation(location);
    setDescription(description);
}

// Salva os dados do evento e dos participantes em arquivo
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

// Adiciona participante (objeto já criado)
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

// Cria e adiciona um novo participante
Participant* Event::addParticipant(std::string name, std::string email, std::string contact) {
    Participant* newParticipant = new Participant(nextParticipantId, name, email, contact);
    participants.push_back(newParticipant);
    nextParticipantId++;
    return newParticipant;
}

// Busca participante pelo ID
Participant* Event::getParticipant(int participantId) const {
    for (Participant* p : participants) {
        if (p->getId() == participantId) {
            return p;
        }
    }
    return nullptr;
}

// Retorna todos os participantes
const std::vector<Participant*>& Event::getAllParticipants() const {
    return participants;
}

// Atualiza dados de um participante
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

// Remove participante pelo ID
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

// Calcula o preço do evento (pode ser customizado)
double Event::calculatePrice() const {
    return price;
}

// Verifica se é possível registrar mais participantes
bool Event::canRegister() const {
    return isActive && (getNumParticipants() < maxCapacity);
}

// Retorna uma string com resumo dos dados do evento
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

// Exibe detalhes do evento no console
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
