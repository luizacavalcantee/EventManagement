#ifndef API_SERVER_H
#define API_SERVER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream> // For stringstream
#include <cstring> // For c-string manipulation if needed

// Windows specific socket headers
#include <winsock2.h>
#include <ws2tcpip.h>

// Link with ws2_32.lib (usually handled by compiler/linker flags or IDE settings)
// #pragma comment(lib, "ws2_32.lib") // This line might be needed in .cpp or build system

#include "EventManager.h" // Include the EventManager header

class ApiServer {
private:
    SOCKET serverSocket;
    EventManager& eventManager; // Reference to the event manager

    // Helper methods for HTTP request parsing and response sending
    std::string getRequestPath(const std::string& request);
    std::string parseJsonField(const std::string& json, const std::string& field);
    std::string urlDecode(const std::string& str);
    
    // Method to send HTTP responses
    void sendResponse(SOCKET clientSocket, const std::string& status, 
                      const std::string& contentType, const std::string& content, 
                      const std::string& extraHeaders = "");

    // Handler methods for specific API routes
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
    // Constructor: Takes a reference to the EventManager
    ApiServer(EventManager& manager);
    
    // Destructor: Cleans up socket resources
    ~ApiServer();

    // Starts the HTTP server loop on a given port
    void start(int port);
};

#endif // API_SERVER_H