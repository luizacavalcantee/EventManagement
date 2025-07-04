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

// Classe responsável por gerenciar eventos e participantes
class EventManager {
private:
    std::vector<Event*> events; // Lista de ponteiros para eventos
    int nextEventId; // Próximo ID disponível para eventos

    // Busca um evento pelo ID (versão privada)
    Event* findEventById(int id);
    const Event* findEventById(int id) const;

public:
    // Construtor
    EventManager();
    // Destrutor
    ~EventManager();

    // Adiciona um novo evento e retorna um objeto JSON com os dados do evento criado
    json addEvent(const std::string& name, const std::string& date,
                  const std::string& time, const std::string& location,
                  const std::string& description);

    // Retorna uma referência constante para um evento pelo ID
    const Event& getEventById(int id) const;
    // Retorna uma referência para um evento pelo ID
    Event& getEventById(int id);

    // Retorna uma referência constante para o vetor de eventos
    const std::vector<Event*>& getAllEvents() const;

    // Atualiza um evento existente; validação de data/hora no futuro é opcional
    bool updateEvent(int id, const std::string& name, const std::string& date,
                     const std::string& time, const std::string& location,
                     const std::string& description);

    // Remove um evento pelo ID
    bool deleteEvent(int id);

    // Adiciona um participante a um evento e retorna um objeto JSON com os dados do participante
    json addParticipantToEvent(int eventId, const std::string& name,
                               const std::string& email, const std::string& contact);

    // Retorna um objeto JSON com todos os participantes de um evento
    json getParticipantsForEvent(int eventId) const;

    // Retorna um objeto JSON com os dados de um participante específico em um evento
    json getParticipantInEvent(int eventId, int participantId) const;

    // Atualiza os dados de um participante em um evento
    bool updateParticipantInEvent(int eventId, int participantId,
                                  const std::string& newName,
                                  const std::string& newEmail,
                                  const std::string& newContact);

    // Remove um participante de um evento
    bool removeParticipantFromEvent(int eventId, int participantId);

    // Carrega eventos de um arquivo
    void loadEventsFromFile();
    // Salva eventos em um arquivo
    void saveEventsToFile();

    // Retorna estatísticas do dashboard em formato JSON
    json getDashboardStats() const;
};

#endif // EVENT_MANAGER_H