#include "Event.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

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

int Event::getId() const {
    return id;
}

std::string Event::getName() const {
    return name;
}

std::string Event::getDate() const {
    return date;
}

std::string Event::getTime() const {
    return time;
}

std::string Event::getLocation() const {
    return location;
}

std::string Event::getDescription() const {
    return description;
}

int Event::getMaxCapacity() const {
    return maxCapacity;
}

double Event::getPrice() const {
    return price;
}

int Event::getNumParticipants() const {
    return participants.size();
}

std::string Event::getCategory() const {
    return category;
}

bool Event::isActiveEvent() const {
    return isActive;
}

void Event::setId(int id) {
    this->id = id;
}

void Event::setName(std::string name) {
    this->name = name;
}

void Event::setDate(std::string date) {
    this->date = date;
}

void Event::setTime(std::string time) {
    this->time = time;
}

void Event::setLocation(std::string location) {
    this->location = location;
}

void Event::setDescription(std::string description) {
    this->description = description;
}

void Event::setMaxCapacity(int capacity) {
    this->maxCapacity = capacity;
}

void Event::setPrice(double p) {
    this->price = p;
}

void Event::setCategory(std::string cat) {
    this->category = cat;
}

void Event::activateEvent() {
    isActive = true;
}

void Event::deactivateEvent() {
    isActive = false;
}

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
