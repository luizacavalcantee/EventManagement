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
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* p_tm = std::localtime(&now_c); // Use localtime para o fuso horário local (Recife)

    std::ostringstream ss;
    ss << std::put_time(p_tm, "%d/%m/%Y %H:%M");
    return ss.str();
}

// Adapte compareDates para usar o formato completo DD/MM/AAAA HH:MM
// Retorna -1 se dt1 < dt2, 0 se dt1 == dt2, 1 se dt1 > dt2
int compareDates(const std::string& date1_str, const std::string& time1_str,
                 const std::string& date2_str, const std::string& time2_str) {
    std::tm tm1 = {}, tm2 = {};
    std::istringstream ss1(date1_str + " " + time1_str);
    std::istringstream ss2(date2_str + " " + time2_str);

    ss1 >> std::get_time(&tm1, "%d/%m/%Y %H:%M");
    ss2 >> std::get_time(&tm2, "%d/%m/%Y %H:%M");

    if (ss1.fail() || ss2.fail()) {
        // Indica falha no parsing, o que pode ser um problema de formato
        // Para comparação, pode-se lançar um erro ou retornar um valor que indique ambiguidade
        // Por enquanto, retorna 0 (igual) para evitar crash, mas idealmente isso seria tratado como erro de dados.
        return 0;
    }

    std::time_t time1_t = std::mktime(&tm1);
    std::time_t time2_t = std::mktime(&tm2);

    if (time1_t == static_cast<std::time_t>(-1) || time2_t == static_cast<std::time_t>(-1)) {
        // Erro na conversão para time_t (data/hora inválida)
        return 0;
    }

    if (time1_t < time2_t) return -1; // dt1 é anterior a dt2
    if (time1_t > time2_t) return 1;  // dt1 é posterior a dt2
    return 0;                         // dt1 é igual a dt2
}

EventManager::EventManager() : nextEventId(1) {
    loadEventsFromFile();
}

EventManager::~EventManager() {
    for (Event* ev : events) {
        delete ev;
    }
    events.clear();
}

Event* EventManager::findEventById(int id) {
    for (Event* ev : events) {
        if (ev->getId() == id) {
            return ev;
        }
    }
    return nullptr;
}

const Event* EventManager::findEventById(int id) const {
    for (const Event* ev : events) {
        if (ev->getId() == id) {
            return ev;
        }
    }
    return nullptr;
}

json EventManager::addEvent(const std::string& name, const std::string& date,
                            const std::string& time, const std::string& location,
                            const std::string& description) {
    // 1. Crie um evento temporário para validação (sem ID real ainda)
    Event tempEvent(0, name, date, time, location, description);
    tempEvent.setMaxCapacity(100); // Defina valores padrão se a API não os fornecer
    tempEvent.setPrice(0.0);
    tempEvent.setCategory("General");

    // 2. Validação da entidade Event, passando 'true' pois é uma CRIAÇÃO
    try {
        tempEvent.validate(true); // Lançará std::invalid_argument se algo estiver errado
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error("Erro de validação do novo evento: " + std::string(e.what()));
    }

    // 3. Se a validação passou, crie o evento real com o próximo ID
    Event* newEvent = new Event(nextEventId, name, date, time, location, description);
    // Copia os outros atributos validados do tempEvent ou define padrões
    newEvent->setMaxCapacity(tempEvent.getMaxCapacity());
    newEvent->setPrice(tempEvent.getPrice());
    newEvent->setCategory(tempEvent.getCategory());
    newEvent->activateEvent(); // Novos eventos são ativos por padrão

    events.push_back(newEvent);
    nextEventId++;
    saveEventsToFile(); // Salva após adição bem-sucedida

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

json EventManager::getParticipantInEvent(int eventId, int participantId) const {
    const Event* event = findEventById(eventId);
    if (event) {
        const Participant* p = event->getParticipant(participantId);
        if (p) {
            // Se o participante for encontrado, cria e retorna o JSON
            json pJson;
            pJson["id"] = p->getId();
            pJson["nome"] = p->getName();
            pJson["email"] = p->getEmail();
            pJson["contato"] = p->getContact();
            return pJson;
        }
    }
    // Se o evento ou o participante não forem encontrados, lança uma exceção
    throw std::runtime_error("Evento ou Participante não encontrado.");
}

const Event& EventManager::getEventById(int id) const {
    const Event* event = findEventById(id);
    if (!event) {
        throw std::runtime_error("Evento com ID " + std::to_string(id) + " não encontrado.");
    }
    return *event;
}

Event& EventManager::getEventById(int id) {
    Event* event = findEventById(id);
    if (!event) {
        throw std::runtime_error("Evento com ID " + std::to_string(id) + " não encontrado.");
    }
    return *event;
}

const std::vector<Event*>& EventManager::getAllEvents() const {
    return events;
}

bool EventManager::updateEvent(int id, const std::string& name, const std::string& date,
                               const std::string& time, const std::string& location,
                               const std::string& description) {
    Event* eventToUpdate = findEventById(id);
    if (eventToUpdate) {
        // Crie uma cópia temporária do evento original para restaurar em caso de falha
        // ou use setters e chame validate antes de salvar.
        // A abordagem de usar setters e validar é mais simples aqui.
        std::string original_name = eventToUpdate->getName();
        std::string original_date = eventToUpdate->getDate();
        std::string original_time = eventToUpdate->getTime();
        std::string original_location = eventToUpdate->getLocation();
        std::string original_description = eventToUpdate->getDescription();

        eventToUpdate->setName(name);
        eventToUpdate->setDate(date);
        eventToUpdate->setTime(time);
        eventToUpdate->setLocation(location);
        eventToUpdate->setDescription(description);

        // Validação da entidade Event, passando 'false' pois é uma ATUALIZAÇÃO
        try {
            eventToUpdate->validate(false); // Permite datas passadas para atualização
        } catch (const std::invalid_argument& e) {
            // Reverter as alterações para o estado original antes de lançar o erro
            eventToUpdate->setName(original_name);
            eventToUpdate->setDate(original_date);
            eventToUpdate->setTime(original_time);
            eventToUpdate->setLocation(original_location);
            eventToUpdate->setDescription(original_description);
            throw std::runtime_error("Erro de validação ao atualizar evento: " + std::string(e.what()));
        }

        saveEventsToFile(); // Salva após validação bem-sucedida
        return true;
    }
    return false; // Evento não encontrado
}

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

json EventManager::addParticipantToEvent(int eventId, const std::string& name,
                                         const std::string& email, const std::string& contact) {
    Event* event = findEventById(eventId);
    if (event) {
        // 1. Validação de regras de negócio antes de criar o participante
        if (!event->canRegister()) {
            throw std::runtime_error("Não é possível registrar participantes neste evento. Ele pode estar inativo ou lotado.");
        }

        // 2. Crie um participante temporário e valide
        Participant tempParticipant(0, name, email, contact); // ID 0 é um placeholder
        try {
            tempParticipant.validate(); // Validação da entidade Person/Participant
        } catch (const std::invalid_argument& e) {
            throw std::runtime_error("Erro de validação do participante: " + std::string(e.what()));
        }

        // 3. Verifique se o participante já está registrado no evento (ex: pelo email)
        for (const auto& p : event->getAllParticipants()) {
            if (p->getEmail() == email) {
                throw std::runtime_error("Um participante com este email já está registrado neste evento.");
            }
        }

        // 4. Se tudo validado, adicione o participante
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

bool EventManager::updateParticipantInEvent(int eventId, int participantId,
                                            const std::string& newName,
                                            const std::string& newEmail,
                                            const std::string& newContact) {
    Event* event = findEventById(eventId);
    if (event) {
        Participant* participantToUpdate = event->getParticipant(participantId);
        if (participantToUpdate) {
            // Salva os dados originais para possível reversão
            std::string original_name = participantToUpdate->getName();
            std::string original_email = participantToUpdate->getEmail();
            std::string original_contact = participantToUpdate->getContact();

            // Atualiza os dados
            participantToUpdate->setName(newName);
            participantToUpdate->setEmail(newEmail);
            participantToUpdate->setContact(newContact);

            try {
                participantToUpdate->validate(); // Valida o objeto Participant atualizado
            } catch (const std::invalid_argument& e) {
                // Reverte as alterações
                participantToUpdate->setName(original_name);
                participantToUpdate->setEmail(original_email);
                participantToUpdate->setContact(original_contact);
                throw std::runtime_error("Erro de validação ao atualizar participante: " + std::string(e.what()));
            }

            // Verifique unicidade do email para o evento (regra de negócio: email único por evento)
            for (const auto& p : event->getAllParticipants()) {
                if (p->getId() != participantId && p->getEmail() == newEmail) {
                    // Reverte as alterações antes de lançar o erro
                    participantToUpdate->setName(original_name);
                    participantToUpdate->setEmail(original_email);
                    participantToUpdate->setContact(original_contact);
                    throw std::runtime_error("Já existe outro participante com este email neste evento.");
                }
            }

            saveEventsToFile(); // Salvar após a validação e verificação de unicidade bem-sucedidas
            return true;
        }
        return false; // Participante não encontrado no evento
    }
    return false; // Evento não encontrado
}

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

void EventManager::loadEventsFromFile() {
    std::ifstream file("eventos.txt");
    if (!file.is_open()) {
        return;
    }
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

json EventManager::getDashboardStats() const {
    json stats;
    stats["totalEventos"] = events.size();
    int totalParticipants = 0;
    std::string currentDateTimeStr = getCurrentDateTimeFormatted(); // Obtenha data e hora atuais
    std::string todayDateStr = currentDateTimeStr.substr(0, 10); // Apenas a parte da data
    std::string currentTimeStr = currentDateTimeStr.substr(11, 5); // Apenas a parte da hora

    int upcomingEvents = 0;
    int eventsToday = 0;
    for (const auto& event : events) {
        totalParticipants += event->getNumParticipants();
        
        // Compara a data e hora do evento com a data e hora atual
        int comparison = compareDates(event->getDate(), event->getTime(), todayDateStr, currentTimeStr);
        if (comparison > 0) { // Evento é no futuro
            upcomingEvents++;
        } else if (comparison == 0) { // Evento é hoje (ou a mesma hora, para ser mais preciso)
             eventsToday++;
        }
    }
    stats["totalParticipantes"] = totalParticipants;
    stats["eventosProximos"] = upcomingEvents;
    stats["eventosHoje"] = eventsToday;
    return stats;
}