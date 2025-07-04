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

class ApiServer {
private:
    SOCKET serverSocket;
    EventManager& eventManager;

    std::string getRequestPath(const std::string& request);
    std::string parseJsonField(const std::string& json, const std::string& field);
    std::string urlDecode(const std::string& str);
    void sendResponse(SOCKET clientSocket, const std::string& status, 
                      const std::string& contentType, const std::string& content, 
                      const std::string& extraHeaders = "");
    void handleGetEvents(SOCKET clientSocket);
    void handlePostEvent(SOCKET clientSocket, const std::string& requestBody);
    void handleGetDashboardStats(SOCKET clientSocket);
    void handlePostParticipant(SOCKET clientSocket, int eventId, const std::string& requestBody);
    void handleGetParticipants(SOCKET clientSocket, int eventId);
    void handlePutEvent(SOCKET clientSocket, int eventId, const std::string& requestBody);
    void handleDeleteEvent(SOCKET clientSocket, int eventId);
    void handlePutParticipant(SOCKET clientSocket, int eventId, int participantId, const std::string& requestBody);
    void handleDeleteParticipant(SOCKET clientSocket, int eventId, int participantId);
    void handleGetEventById(SOCKET clientSocket, int eventId);

public:
    ApiServer(EventManager& manager);
    ~ApiServer();
    void start(int port);
};

#endif // API_SERVER_H
