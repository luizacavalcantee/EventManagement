#include "EventManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <ctime>     // Para std::tm, std::mktime, std::localtime, std::time
#include "json/json.hpp"

// Função auxiliar para obter a data e hora atuais formatadas (DD/MM/AAAA HH:MM)
std::string getCurrentDateTimeFormatted() {
    // Obtém o tempo atual do sistema
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* p_tm = std::localtime(&now_c); // Converte para o horário local

    // Formata a data e hora como string
    std::ostringstream ss;
    ss << std::put_time(p_tm, "%d/%m/%Y %H:%M");
    return ss.str();
}

// Função para comparar duas datas e horas no formato DD/MM/AAAA HH:MM
// Retorna -1 se dt1 < dt2, 0 se dt1 == dt2, 1 se dt1 > dt2
int compareDates(const std::string& date1_str, const std::string& time1_str,
                 const std::string& date2_str, const std::string& time2_str) {
    std::tm tm1 = {}, tm2 = {};
    std::istringstream ss1(date1_str + " " + time1_str);
    std::istringstream ss2(date2_str + " " + time2_str);

    // Faz o parsing das datas e horas
    ss1 >> std::get_time(&tm1, "%d/%m/%Y %H:%M");
    ss2 >> std::get_time(&tm2, "%d/%m/%Y %H:%M");

    if (ss1.fail() || ss2.fail()) {
        // Falha no parsing, retorna 0 (igual) para evitar crash
        return 0;
    }

    // Converte para time_t para comparação
    std::time_t time1_t = std::mktime(&tm1);
    std::time_t time2_t = std::mktime(&tm2);

    if (time1_t == static_cast<std::time_t>(-1) || time2_t == static_cast<std::time_t>(-1)) {
        // Erro na conversão para time_t
        return 0;
    }

    // Compara os valores
    if (time1_t < time2_t) return -1;
    if (time1_t > time2_t) return 1;
    return 0;
}

// Construtor: inicializa o próximo ID e carrega eventos do arquivo
EventManager::EventManager() : nextEventId(1) {
    loadEventsFromFile();
}

// Destrutor: libera memória dos eventos
EventManager::~EventManager() {
    for (Event* ev : events) {
        delete ev;
    }
    events.clear();
}

// Busca evento pelo ID (não const)
Event* EventManager::findEventById(int id) {
    for (Event* ev : events) {
        if (ev->getId() == id) {
            return ev;
        }
    }
    return nullptr;
}

// Busca evento pelo ID (const)
const Event* EventManager::findEventById(int id) const {
    for (const Event* ev : events) {
        if (ev->getId() == id) {
            return ev;
        }
    }
    return nullptr;
}

// Adiciona um novo evento ao sistema
json EventManager::addEvent(const std::string& name, const std::string& date,
                            const std::string& time, const std::string& location,
                            const std::string& description) {
    // Cria um evento temporário para validação
    Event tempEvent(0, name, date, time, location, description);
    tempEvent.setMaxCapacity(100); // Valor padrão
    tempEvent.setPrice(0.0);
    tempEvent.setCategory("General");

    // Valida o evento (lança exceção se inválido)
    try {
        tempEvent.validate(true);
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error("Erro de validação do novo evento: " + std::string(e.what()));
    }

    // Cria o evento real com ID sequencial
    Event* newEvent = new Event(nextEventId, name, date, time, location, description);
    newEvent->setMaxCapacity(tempEvent.getMaxCapacity());
    newEvent->setPrice(tempEvent.getPrice());
    newEvent->setCategory(tempEvent.getCategory());
    newEvent->activateEvent(); // Evento ativo por padrão

    events.push_back(newEvent);
    nextEventId++;
    saveEventsToFile(); // Salva após adicionar

    // Monta JSON de retorno
    json eventJson;
    eventJson["id"] = newEvent->getId();
    eventJson["nome"] = newEvent->getName();
    eventJson["data"] = newEvent->getDate();
    eventJson["hora"] = newEvent->getTime();
    eventJson["local"] = newEvent->getLocation();
    eventJson["descricao"] = newEvent->getDescription();
    eventJson["numParticipantes"] = newEvent->getNumParticipants();
    return eventJson;
}

// Busca um participante em um evento pelo ID
json EventManager::getParticipantInEvent(int eventId, int participantId) const {
    const Event* event = findEventById(eventId);
    if (event) {
        const Participant* p = event->getParticipant(participantId);
        if (p) {
            // Participante encontrado, retorna JSON
            json pJson;
            pJson["id"] = p->getId();
            pJson["nome"] = p->getName();
            pJson["email"] = p->getEmail();
            pJson["contato"] = p->getContact();
            return pJson;
        }
    }
    // Evento ou participante não encontrado
    throw std::runtime_error("Evento ou Participante não encontrado.");
}

// Retorna referência constante para evento pelo ID (lança exceção se não encontrado)
const Event& EventManager::getEventById(int id) const {
    const Event* event = findEventById(id);
    if (!event) {
        throw std::runtime_error("Evento com ID " + std::to_string(id) + " não encontrado.");
    }
    return *event;
}

// Retorna referência para evento pelo ID (lança exceção se não encontrado)
Event& EventManager::getEventById(int id) {
    Event* event = findEventById(id);
    if (!event) {
        throw std::runtime_error("Evento com ID " + std::to_string(id) + " não encontrado.");
    }
    return *event;
}

// Retorna todos os eventos
const std::vector<Event*>& EventManager::getAllEvents() const {
    return events;
}

// Atualiza os dados de um evento existente
bool EventManager::updateEvent(int id, const std::string& name, const std::string& date,
                               const std::string& time, const std::string& location,
                               const std::string& description) {
    Event* eventToUpdate = findEventById(id);
    if (eventToUpdate) {
        // Salva dados originais para possível reversão
        std::string original_name = eventToUpdate->getName();
        std::string original_date = eventToUpdate->getDate();
        std::string original_time = eventToUpdate->getTime();
        std::string original_location = eventToUpdate->getLocation();
        std::string original_description = eventToUpdate->getDescription();

        // Atualiza os dados
        eventToUpdate->setName(name);
        eventToUpdate->setDate(date);
        eventToUpdate->setTime(time);
        eventToUpdate->setLocation(location);
        eventToUpdate->setDescription(description);

        // Valida o evento atualizado
        try {
            eventToUpdate->validate(false); // Permite datas passadas
        } catch (const std::invalid_argument& e) {
            // Reverte alterações em caso de erro
            eventToUpdate->setName(original_name);
            eventToUpdate->setDate(original_date);
            eventToUpdate->setTime(original_time);
            eventToUpdate->setLocation(original_location);
            eventToUpdate->setDescription(original_description);
            throw std::runtime_error("Erro de validação ao atualizar evento: " + std::string(e.what()));
        }

        saveEventsToFile(); // Salva após sucesso
        return true;
    }
    return false; // Evento não encontrado
}

// Remove um evento pelo ID
bool EventManager::deleteEvent(int id) {
    auto it = std::remove_if(events.begin(), events.end(),
                             [id](Event* ev) { return ev->getId() == id; });
    if (it != events.end()) {
        delete *it;
        events.erase(it, events.end());
        saveEventsToFile();
        return true;
    }
    return false;
}

// Adiciona um participante a um evento
json EventManager::addParticipantToEvent(int eventId, const std::string& name,
                                         const std::string& email, const std::string& contact) {
    Event* event = findEventById(eventId);
    if (event) {
        // Verifica se é possível registrar
        if (!event->canRegister()) {
            throw std::runtime_error("Não é possível registrar participantes neste evento. Ele pode estar inativo ou lotado.");
        }

        // Cria participante temporário e valida
        Participant tempParticipant(0, name, email, contact);
        try {
            tempParticipant.validate();
        } catch (const std::invalid_argument& e) {
            throw std::runtime_error("Erro de validação do participante: " + std::string(e.what()));
        }

        // Verifica se já existe participante com o mesmo email
        for (const auto& p : event->getAllParticipants()) {
            if (p->getEmail() == email) {
                throw std::runtime_error("Um participante com este email já está registrado neste evento.");
            }
        }

        // Adiciona participante ao evento
        Participant* newParticipant = event->addParticipant(name, email, contact);
        saveEventsToFile();
        json participantJson;
        participantJson["id"] = newParticipant->getId();
        participantJson["nome"] = newParticipant->getName();
        participantJson["email"] = newParticipant->getEmail();
        participantJson["contato"] = newParticipant->getContact();
        return participantJson;
    }
    throw std::runtime_error("Evento com ID " + std::to_string(eventId) + " não encontrado para adicionar participante.");
}

// Retorna todos os participantes de um evento
json EventManager::getParticipantsForEvent(int eventId) const {
    const Event* event = findEventById(eventId);
    if (event) {
        json participantsArray = json::array();
        for (const auto& p : event->getAllParticipants()) {
            json pJson;
            pJson["id"] = p->getId();
            pJson["nome"] = p->getName();
            pJson["email"] = p->getEmail();
            pJson["contato"] = p->getContact();
            participantsArray.push_back(pJson);
        }
        return participantsArray;
    }
    throw std::runtime_error("Evento com ID " + std::to_string(eventId) + " não encontrado para listar participantes.");
}

// Atualiza os dados de um participante em um evento
bool EventManager::updateParticipantInEvent(int eventId, int participantId,
                                            const std::string& newName,
                                            const std::string& newEmail,
                                            const std::string& newContact) {
    Event* event = findEventById(eventId);
    if (event) {
        Participant* participantToUpdate = event->getParticipant(participantId);
        if (participantToUpdate) {
            // Salva dados originais para reversão
            std::string original_name = participantToUpdate->getName();
            std::string original_email = participantToUpdate->getEmail();
            std::string original_contact = participantToUpdate->getContact();

            // Atualiza dados
            participantToUpdate->setName(newName);
            participantToUpdate->setEmail(newEmail);
            participantToUpdate->setContact(newContact);

            // Valida participante atualizado
            try {
                participantToUpdate->validate();
            } catch (const std::invalid_argument& e) {
                // Reverte alterações em caso de erro
                participantToUpdate->setName(original_name);
                participantToUpdate->setEmail(original_email);
                participantToUpdate->setContact(original_contact);
                throw std::runtime_error("Erro de validação ao atualizar participante: " + std::string(e.what()));
            }

            // Verifica unicidade do email no evento
            for (const auto& p : event->getAllParticipants()) {
                if (p->getId() != participantId && p->getEmail() == newEmail) {
                    // Reverte alterações antes de lançar erro
                    participantToUpdate->setName(original_name);
                    participantToUpdate->setEmail(original_email);
                    participantToUpdate->setContact(original_contact);
                    throw std::runtime_error("Já existe outro participante com este email neste evento.");
                }
            }

            saveEventsToFile();
            return true;
        }
        return false; // Participante não encontrado
    }
    return false; // Evento não encontrado
}

// Remove um participante de um evento
bool EventManager::removeParticipantFromEvent(int eventId, int participantId) {
    Event* event = findEventById(eventId);
    if (event) {
        bool removed = event->removeParticipant(participantId);
        if (removed) {
            saveEventsToFile();
        }
        return removed;
    }
    return false;
}

// Salva todos os eventos e participantes no arquivo "eventos.txt"
void EventManager::saveEventsToFile() {
    std::ofstream file("eventos.txt");
    if (!file.is_open()) {
        std::cerr << "Erro: Não foi possível abrir eventos.txt para escrita." << std::endl;
        return;
    }
    for (const auto& event : events) {
        event->saveEvent(file);
    }
    file.close();
}

// Carrega eventos e participantes do arquivo "eventos.txt"
void EventManager::loadEventsFromFile() {
    std::ifstream file("eventos.txt");
    if (!file.is_open()) {
        return;
    }
    // Limpa eventos existentes
    for (Event* ev : events) {
        delete ev;
    }
    events.clear();
    nextEventId = 1;
    std::string line;
    Event* currentEvent = nullptr;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type;
        std::getline(ss, type, ',');
        if (type == "EVENT") {
            // Lê dados do evento
            int id;
            std::string name, date, time, location, description;
            int maxCapacity;
            double price;
            std::string category_str, isActive_str;
            int nextParticipantId_from_file;
            std::string id_str;
            std::getline(ss, id_str, ','); id = std::stoi(id_str);
            std::getline(ss, name, ',');
            std::getline(ss, date, ',');
            std::getline(ss, time, ',');
            std::getline(ss, location, ',');
            std::getline(ss, description, ',');
            std::string maxCap_str, price_str;
            std::getline(ss, maxCap_str, ','); maxCapacity = std::stoi(maxCap_str);
            std::getline(ss, price_str, ','); price = std::stod(price_str);
            std::getline(ss, category_str, ',');
            std::getline(ss, isActive_str, ','); bool isActive_val = (isActive_str == "true");
            std::string nextPartId_str;
            std::getline(ss, nextPartId_str, ','); nextParticipantId_from_file = std::stoi(nextPartId_str);
            currentEvent = new Event(id, name, date, time, location, description);
            currentEvent->setMaxCapacity(maxCapacity);
            currentEvent->setPrice(price);
            currentEvent->setCategory(category_str);
            if (!isActive_val) currentEvent->deactivateEvent();
            currentEvent->setId(id);
            events.push_back(currentEvent);
            if (id >= nextEventId) {
                nextEventId = id + 1;
            }
        } else if (type == "PARTICIPANT") {
            // Lê dados do participante e adiciona ao evento atual
            if (currentEvent) {
                int p_id;
                std::string p_name, p_email, p_contact;
                std::string p_id_str;
                std::getline(ss, p_id_str, ','); p_id = std::stoi(p_id_str);
                std::getline(ss, p_name, ',');
                std::getline(ss, p_email, ',');
                std::getline(ss, p_contact, ',');
                Participant* loadedParticipant = new Participant(p_id, p_name, p_email, p_contact);
                currentEvent->addParticipant(loadedParticipant);
            }
        }
    }
    file.close();
}

// Retorna estatísticas para o dashboard (total de eventos, participantes, eventos futuros e de hoje)
json EventManager::getDashboardStats() const {
    json stats;
    stats["totalEventos"] = events.size();
    int totalParticipants = 0;
    std::string currentDateTimeStr = getCurrentDateTimeFormatted(); // Data e hora atuais
    std::string todayDateStr = currentDateTimeStr.substr(0, 10); // Data atual
    std::string currentTimeStr = currentDateTimeStr.substr(11, 5); // Hora atual

    int upcomingEvents = 0;
    int eventsToday = 0;
    for (const auto& event : events) {
        totalParticipants += event->getNumParticipants();
        
        // Compara data/hora do evento com a atual
        int comparison = compareDates(event->getDate(), event->getTime(), todayDateStr, currentTimeStr);
        if (comparison > 0) { // Evento no futuro
            upcomingEvents++;
        } else if (comparison == 0) { // Evento hoje
             eventsToday++;
        }
    }
    stats["totalParticipantes"] = totalParticipants;
    stats["eventosProximos"] = upcomingEvents;
    stats["eventosHoje"] = eventsToday;
    return stats;
}