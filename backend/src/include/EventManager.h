#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <string>
#include <vector>
#include <stdexcept> // For std::runtime_error
#include <memory>    // For std::unique_ptr if we decide to use smart pointers
#include "json/json.hpp" // For JSON handling

#include "Event.h"   // Now includes Event.h
#include "Participant.h" // Also directly include Participant.h for participant objects

// Alias for convenience
using json = nlohmann::json;

class EventManager {
private:
    std::vector<Event*> events; // Stores pointers to Event objects
    int nextEventId; // To assign unique IDs to new events

    // Helper to find an event by ID (internal use)
    Event* findEventById(int id);
    const Event* findEventById(int id) const;

public:
    // Constructor and Destructor
    EventManager();
    ~EventManager(); // Destructor to free dynamically allocated Event objects

    // --- Core Event Management Methods (for API/Internal use) ---
    
    // Creates and adds a new event, returns its JSON representation
    json addEvent(const std::string& name, const std::string& date, 
                  const std::string& time, const std::string& location, 
                  const std::string& description);

    // Retrieves an event by its ID
    const Event& getEventById(int id) const;
    Event& getEventById(int id); // Non-const version for modification

    // Retrieves all events
    const std::vector<Event*>& getAllEvents() const;

    // Updates an existing event, returns true on success
    bool updateEvent(int id, const std::string& name, const std::string& date, 
                     const std::string& time, const std::string& location, 
                     const std::string& description);

    // Deletes an event by its ID, returns true on success
    bool deleteEvent(int id);

    // --- Participant Management Methods ---

    // Adds a participant to a specific event, returns participant JSON
    json addParticipantToEvent(int eventId, const std::string& name, 
                               const std::string& email, const std::string& contact);

    // Retrieves all participants for a specific event as JSON array
    json getParticipantsForEvent(int eventId) const;

    // Updates a participant in a specific event, returns true on success
    bool updateParticipantInEvent(int eventId, int participantId, 
                                  const std::string& newName, 
                                  const std::string& newEmail, 
                                  const std::string& newContact);

    // Removes a participant from a specific event, returns true on success
    bool removeParticipantFromEvent(int eventId, int participantId);

    // --- Data Persistence Methods ---
    void loadEventsFromFile();
    void saveEventsToFile();

    // --- Reporting/Statistics Methods ---
    json getDashboardStats() const; // Renamed from gerarRelatorioJson
};

#endif // EVENT_MANAGER_H