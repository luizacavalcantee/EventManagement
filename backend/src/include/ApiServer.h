#ifndef API_SERVER_H
#define API_SERVER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "EventManager.h"

// Classe responsável por gerenciar o servidor de API para eventos
class ApiServer {
private:
    SOCKET serverSocket; // Socket do servidor
    EventManager& eventManager; // Referência ao gerenciador de eventos

    // Obtém o caminho da requisição HTTP
    std::string getRequestPath(const std::string& request);

    // Método legado para parsear campo JSON (não mais usado)
    std::string parseJsonField(const std::string& json, const std::string& field);

    // Decodifica URLs
    std::string urlDecode(const std::string& str);

    // Envia resposta HTTP ao cliente
    void sendResponse(SOCKET clientSocket, const std::string& status,
                      const std::string& contentType, const std::string& content,
                      const std::string& extraHeaders = "");

    // Manipuladores para diferentes rotas/endpoints da API
    void handleGetEvents(SOCKET clientSocket); // Lista eventos
    void handlePostEvent(SOCKET clientSocket, const std::string& requestBody); // Cria evento
    void handleGetDashboardStats(SOCKET clientSocket); // Estatísticas do dashboard
    void handlePostParticipant(SOCKET clientSocket, int eventId, const std::string& requestBody); // Adiciona participante
    void handleGetParticipants(SOCKET clientSocket, int eventId); // Lista participantes
    void handlePutEvent(SOCKET clientSocket, int eventId, const std::string& requestBody); // Atualiza evento
    void handleDeleteEvent(SOCKET clientSocket, int eventId); // Remove evento
    void handlePutParticipant(SOCKET clientSocket, int eventId, int participantId, const std::string& requestBody); // Atualiza participante
    void handleDeleteParticipant(SOCKET clientSocket, int eventId, int participantId); // Remove participante
    void handleGetEventById(SOCKET clientSocket, int eventId); // Busca evento por ID
    void handleGetParticipantById(SOCKET clientSocket, int eventId, int participantId); // Busca participante por ID

public:
    // Construtor recebe referência ao EventManager
    ApiServer(EventManager& manager);

    // Destrutor
    ~ApiServer();

    // Inicia o servidor na porta especificada
    void start(int port);
};

#endif // API_SERVER_H