#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include "json/json.hpp"
#include "Event.h"
#include "Participant.h"

using json = nlohmann::json;

class EventManager {
private:
    std::vector<Event*> events;
    int nextEventId;

    Event* findEventById(int id);
    const Event* findEventById(int id) const;

public:
    EventManager();
    ~EventManager();

    json addEvent(const std::string& name, const std::string& date, 
                  const std::string& time, const std::string& location, 
                  const std::string& description);

    const Event& getEventById(int id) const;
    Event& getEventById(int id);

    const std::vector<Event*>& getAllEvents() const;

    bool updateEvent(int id, const std::string& name, const std::string& date, 
                     const std::string& time, const std::string& location, 
                     const std::string& description);

    bool deleteEvent(int id);

    json addParticipantToEvent(int eventId, const std::string& name, 
                               const std::string& email, const std::string& contact);

    json getParticipantsForEvent(int eventId) const;

    bool updateParticipantInEvent(int eventId, int participantId, 
                                  const std::string& newName, 
                                  const std::string& newEmail, 
                                  const std::string& newContact);

    bool removeParticipantFromEvent(int eventId, int participantId);

    void loadEventsFromFile();
    void saveEventsToFile();

    json getDashboardStats() const;
};

#endif // EVENT_MANAGER_H
