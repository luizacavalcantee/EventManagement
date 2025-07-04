#include "Event.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

// Constructors
Event::Event() 
    : id(0), name(""), date(""), time(""), location(""), description(""), 
      maxCapacity(100), price(0.0), nextParticipantId(1), category("General"), isActive(true) {}

Event::Event(int id, std::string name, std::string date, std::string time, std::string location, std::string description)
    : id(id), name(name), date(date), time(time), location(location), description(description),
      maxCapacity(100), price(0.0), nextParticipantId(1), category("General"), isActive(true) {}

// Destructor to free dynamically allocated participants
Event::~Event() {
    for (Participant* p : participants) {
        delete p; // Free memory for each participant object
    }
    participants.clear(); // Clear the vector
}

// Getters
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

// Setters
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

// Event specific functionalities
void Event::updateEvent(std::string name, std::string date, std::string time, std::string location, std::string description) {
    setName(name);
    setDate(date);
    setTime(time);
    setLocation(location);
    setDescription(description);
    // Other properties like maxCapacity, price, category, isActive can also be updated here if needed
}

// Save event details and its participants to an output file stream
void Event::saveEvent(std::ofstream &file) {
    // Format: EVENT,<id>,<name>,<date>,<time>,<location>,<description>,<maxCapacity>,<price>,<category>,<isActive>,<nextParticipantId>
    file << "EVENT," << id << "," << name << "," << date << "," 
         << time << "," << location << "," << description << ","
         << maxCapacity << "," << price << "," << category << ","
         << (isActive ? "true" : "false") << "," << nextParticipantId << std::endl;
    
    // Save participants
    for (const auto& participant : participants) {
        // Format: PARTICIPANT,<participant_id>,<name>,<email>,<contact>
        file << "PARTICIPANT," << participant->getId() << "," 
             << participant->getName() << "," << participant->getEmail() << "," 
             << participant->getContact() << std::endl;
    }
}

// Participant management within an Event
void Event::addParticipant(Participant* p) {
    // This overload assumes the ID is already set or will be handled by the caller.
    // However, to ensure unique IDs within this event, we'll assign our internal nextParticipantId
    // unless p->getId() is already non-zero, indicating it's being loaded from persistence.
    if (p->getId() == 0) { // Only assign new ID if it's a new participant (ID is 0)
        p->setId(nextParticipantId++);
    } else { // If ID is provided (e.g., from loading), ensure nextParticipantId is higher
        if (p->getId() >= nextParticipantId) {
            nextParticipantId = p->getId() + 1;
        }
    }
    participants.push_back(p);
}

// Overload to add participant with internal ID generation, primarily for new UI additions
Participant* Event::addParticipant(std::string name, std::string email, std::string contact) {
    Participant* newParticipant = new Participant(nextParticipantId, name, email, contact);
    participants.push_back(newParticipant);
    nextParticipantId++;
    return newParticipant;
}

// Get a specific participant by ID
Participant* Event::getParticipant(int participantId) const {
    for (Participant* p : participants) {
        if (p->getId() == participantId) {
            return p;
        }
    }
    return nullptr; // Participant not found
}

// Get all participants
const std::vector<Participant*>& Event::getAllParticipants() const {
    return participants;
}

// Update participant details by their ID within this event
bool Event::updateParticipant(int participantId, const std::string& newName, const std::string& newEmail, const std::string& newContact) {
    for (Participant* p : participants) {
        if (p->getId() == participantId) {
            p->setName(newName);
            p->setEmail(newEmail);
            p->setContact(newContact);
            return true;
        }
    }
    return false; // Participant not found
}

// Remove participant by their ID
bool Event::removeParticipant(int participantId) {
    for (auto it = participants.begin(); it != participants.end(); ++it) {
        if ((*it)->getId() == participantId) {
            delete *it; // Free memory
            participants.erase(it); // Remove from vector
            return true;
        }
    }
    return false; // Participant not found
}

// Other utility methods
double Event::calculatePrice() const {
    return price; // Returns the set price
}

bool Event::canRegister() const {
    return isActive && (getNumParticipants() < maxCapacity); // Can register if active and not full
}

std::string Event::toString() const {
    std::stringstream ss;
    ss << "ID: " << id << ", Name: " << name << ", Date: " << date 
       << ", Time: " << time << ", Location: " << location 
       << ", Description: " << description
       << ", Category: " << category 
       << ", Status: " << (isActive ? "Active" : "Inactive")
       << ", Participants: " << participants.size() << "/" << maxCapacity
       << ", Price: " << std::fixed << std::setprecision(2) << price; // Add setprecision for price
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