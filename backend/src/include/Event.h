#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept> 
#include "Participant.h"

// Classe que representa um Evento
class Event {
private:
    int id; // Identificador único do evento
    std::string name; // Nome do evento
    std::string date; // Data do evento (formato string)
    std::string time; // Horário do evento (formato string)
    std::string location; // Localização do evento
    std::string description; // Descrição do evento
    int maxCapacity; // Capacidade máxima de participantes
    double price; // Preço do evento
    std::vector<Participant*> participants; // Lista de ponteiros para participantes
    int nextParticipantId; // Próximo ID disponível para participante
    std::string category; // Categoria do evento
    bool isActive; // Indica se o evento está ativo

public:
    // Construtor padrão
    Event();

    // Construtor com parâmetros principais
    Event(int id, std::string name, std::string date, std::string time, std::string location, std::string description);

    // Destrutor
    ~Event();

    // Método de validação do evento (parâmetro indica se é para criação)
    void validate(bool forCreation = true) const;

    // Métodos auxiliares de validação de data/hora
    bool isValidDate(const std::string& d) const;
    bool isValidTime(const std::string& t) const;
    bool isFutureDateTime(const std::string& d, const std::string& t) const;

    // Métodos getters
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

    // Métodos setters
    void setId(int id);
    void setName(std::string name);
    void setDate(std::string date);
    void setTime(std::string time);
    void setLocation(std::string location);
    void setDescription(std::string description);
    void setMaxCapacity(int capacity);
    void setPrice(double p);
    void setCategory(std::string cat);
    void activateEvent(); // Ativa o evento
    void deactivateEvent(); // Desativa o evento

    // Atualiza os dados principais do evento
    void updateEvent(std::string name, std::string date, std::string time, std::string location, std::string description);

    // Salva os dados do evento em arquivo
    void saveEvent(std::ofstream &file);

    // Adiciona participante (por ponteiro)
    void addParticipant(Participant* p);

    // Cria e adiciona participante a partir dos dados
    Participant* addParticipant(std::string name, std::string email, std::string contact);

    // Recupera participante pelo ID
    Participant* getParticipant(int participantId) const;

    // Retorna todos os participantes
    const std::vector<Participant*>& getAllParticipants() const;

    // Atualiza dados de um participante
    bool updateParticipant(int participantId, const std::string& newName, const std::string& newEmail, const std::string& newContact);

    // Remove participante pelo ID
    bool removeParticipant(int participantId);

    // Calcula o preço do evento (pode incluir lógica extra)
    double calculatePrice() const;

    // Verifica se ainda é possível registrar participantes
    bool canRegister() const;

    // Retorna uma string com os dados do evento
    std::string toString() const;

    // Exibe detalhes do evento
    void displayDetails() const;
};

#endif // EVENT_H