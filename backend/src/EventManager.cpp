#include "EventManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include "json/json.hpp"

std::string getCurrentDateFormatted() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* p_tm = std::localtime(&now_c);
    std::ostringstream ss;
    ss << std::put_time(p_tm, "%d/%m/%Y");
    return ss.str();
}

int compareDates(const std::string& date1_str, const std::string& date2_str) {
    int d1 = std::stoi(date1_str.substr(0, 2));
    int m1 = std::stoi(date1_str.substr(3, 2));
    int y1 = std::stoi(date1_str.substr(6, 4));
    int d2 = std::stoi(date2_str.substr(0, 2));
    int m2 = std::stoi(date2_str.substr(3, 2));
    int y2 = std::stoi(date2_str.substr(6, 4));
    if (y1 != y2) return y1 - y2;
    if (m1 != m2) return m1 - m2;
    return d1 - d2;
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
    Event* newEvent = new Event(nextEventId, name, date, time, location, description);
    events.push_back(newEvent);
    nextEventId++;
    saveEventsToFile();
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

const Event& EventManager::getEventById(int id) const {
    const Event* event = findEventById(id);
    if (!event) {
        throw std::runtime_error("Event with ID " + std::to_string(id) + " not found.");
    }
    return *event;
}

Event& EventManager::getEventById(int id) {
    Event* event = findEventById(id);
    if (!event) {
        throw std::runtime_error("Event with ID " + std::to_string(id) + " not found.");
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
        eventToUpdate->updateEvent(name, date, time, location, description);
        saveEventsToFile();
        return true;
    }
    return false;
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
        Participant* newParticipant = event->addParticipant(name, email, contact);
        saveEventsToFile();
        json participantJson;
        participantJson["id"] = newParticipant->getId();
        participantJson["nome"] = newParticipant->getName();
        participantJson["email"] = newParticipant->getEmail();
        participantJson["contato"] = newParticipant->getContact();
        return participantJson;
    }
    throw std::runtime_error("Event with ID " + std::to_string(eventId) + " not found to add participant.");
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
    throw std::runtime_error("Event with ID " + std::to_string(eventId) + " not found to list participants.");
}

bool EventManager::updateParticipantInEvent(int eventId, int participantId,
                                            const std::string& newName,
                                            const std::string& newEmail,
                                            const std::string& newContact) {
    Event* event = findEventById(eventId);
    if (event) {
        bool updated = event->updateParticipant(participantId, newName, newEmail, newContact);
        if (updated) {
            saveEventsToFile();
        }
        return updated;
    }
    return false;
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
        std::cerr << "Error: Could not open eventos.txt for writing." << std::endl;
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
    // CORREÇÃO: Use a função para obter a data atual
    std::string todayDate = getCurrentDateFormatted(); 
    int upcomingEvents = 0;
    int eventsToday = 0;
    for (const auto& event : events) {
        totalParticipants += event->getNumParticipants();
        // Certifique-se de que compareDates pode lidar com DD/MM/AAAA e DD-MM-AAAA
        // Se o backend sempre salvar DD/MM/AAAA, o compareDates está ok.
        // Se salvar DD-MM-AAAA, o compareDates precisará de ajustes ou o frontend 
        // precisará converter antes de enviar.
        int comparison = compareDates(event->getDate(), todayDate);
        if (comparison == 0) {
            eventsToday++;
        } else if (comparison > 0) {
            upcomingEvents++;
        }
    }
    stats["totalParticipantes"] = totalParticipants;
    stats["eventosProximos"] = upcomingEvents;
    stats["eventosHoje"] = eventsToday;
    return stats;
}