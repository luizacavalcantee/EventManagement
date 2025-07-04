#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <vector>
#include <fstream>
#include "Participant.h"

class Event {
private:
    int id;
    std::string name;
    std::string date;
    std::string time;
    std::string location;
    std::string description;
    int maxCapacity;
    double price;
    std::vector<Participant*> participants;
    int nextParticipantId;
    std::string category;
    bool isActive;

public:
    Event();
    Event(int id, std::string name, std::string date, std::string time, std::string location, std::string description);
    ~Event();

    int getId() const;
    std::string getName() const;
    std::string getDate() const;
    std::string getTime() const;
    std::string getLocation() const;
    std::string getDescription() const;
    int getMaxCapacity() const;
    double getPrice() const;
    int getNumParticipants() const;
    std::string getCategory() const;
    bool isActiveEvent() const;

    void setId(int id);
    void setName(std::string name);
    void setDate(std::string date);
    void setTime(std::string time);
    void setLocation(std::string location);
    void setDescription(std::string description);
    void setMaxCapacity(int capacity);
    void setPrice(double p);
    void setCategory(std::string cat);
    void activateEvent();
    void deactivateEvent();

    void updateEvent(std::string name, std::string date, std::string time, std::string location, std::string description);
    void saveEvent(std::ofstream &file);

    void addParticipant(Participant* p);
    Participant* addParticipant(std::string name, std::string email, std::string contact);

    Participant* getParticipant(int participantId) const;
    const std::vector<Participant*>& getAllParticipants() const;
    bool updateParticipant(int participantId, const std::string& newName, const std::string& newEmail, const std::string& newContact);
    bool removeParticipant(int participantId);

    double calculatePrice() const;
    bool canRegister() const;
    std::string toString() const;
    void displayDetails() const;
};

#endif // EVENT_H
