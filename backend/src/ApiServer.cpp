#include "ApiServer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> // For std::remove_if
#include <iomanip>   // For std::put_time, etc.
#include <regex>     // For more robust path parsing if needed, but sticking to find/substr for now.

// Link with ws2_32.lib for Windows Sockets
#pragma comment(lib, "ws2_32.lib")

// --- Helper Functions (moved into class as private methods) ---

// Extracts the request path from an HTTP request string
std::string ApiServer::getRequestPath(const std::string& request) {
    size_t startPos = request.find(" ") + 1;
    size_t endPos = request.find(" ", startPos);
    if (startPos == std::string::npos || endPos == std::string::npos) {
        return "";
    }
    return request.substr(startPos, endPos - startPos);
}

// Parses a specific string field from a simple JSON string
std::string ApiServer::parseJsonField(const std::string& json, const std::string& field) {
    std::string search_str = "\"" + field + "\":\"";
    size_t pos = json.find(search_str);
    if (pos == std::string::npos) {
        // Handle numeric fields (e.g., "id":123) if necessary
        search_str = "\"" + field + "\":";
        pos = json.find(search_str);
        if (pos == std::string::npos) return ""; // Field not found
        pos += search_str.length();
        size_t end = json.find_first_of(",}", pos); // Find comma or closing brace
        return json.substr(pos, end - pos);
    }
    pos += search_str.length();
    size_t end = json.find("\"", pos);
    if (end == std::string::npos) return "";
    return json.substr(pos, end - pos);
}

// Decodes URL-encoded strings
std::string ApiServer::urlDecode(const std::string& str) {
    std::string result;
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '%' && i + 2 < str.length()) {
            int val;
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &val);
            result += static_cast<char>(val);
            i += 2;
        } else if (str[i] == '+') {
            result += ' ';
        } else {
            result += str[i];
        }
    }
    return result;
}

// Sends an HTTP response to the client
void ApiServer::sendResponse(SOCKET clientSocket, const std::string& status, 
                             const std::string& contentType, const std::string& content, 
                             const std::string& extraHeaders) {
    std::string response = "HTTP/1.1 " + status + "\r\n";
    response += "Content-Type: " + contentType + "\r\n";
    // CORS headers - essential for frontend development serving from different origin
    response += "Access-Control-Allow-Origin: *\r\n";
    response += "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n";
    response += "Access-Control-Allow-Headers: Content-Type, Authorization\r\n"; // Added Authorization for completeness
    response += extraHeaders; // Add any custom headers
    response += "Content-Length: " + std::to_string(content.length()) + "\r\n";
    response += "\r\n"; // End of headers
    response += content; // Response body
    
    send(clientSocket, response.c_str(), response.length(), 0);
}


// --- API Handler Implementations ---

void ApiServer::handleGetEvents(SOCKET clientSocket) {
    json eventsArray = json::array();
    for (const auto& eventPtr : eventManager.getAllEvents()) {
        json eventJson;
        eventJson["id"] = eventPtr->getId();
        eventJson["nome"] = eventPtr->getName(); // Frontend expects "nome"
        eventJson["data"] = eventPtr->getDate();
        eventJson["hora"] = eventPtr->getTime();
        eventJson["local"] = eventPtr->getLocation();
        eventJson["descricao"] = eventPtr->getDescription();
        eventJson["numParticipantes"] = eventPtr->getNumParticipants(); // Frontend expects "numParticipantes"
        eventsArray.push_back(eventJson);
    }
    sendResponse(clientSocket, "200 OK", "application/json", eventsArray.dump());
}

void ApiServer::handlePostEvent(SOCKET clientSocket, const std::string& requestBody) {
    std::string name = parseJsonField(requestBody, "nome");
    std::string date = parseJsonField(requestBody, "data");
    std::string time = parseJsonField(requestBody, "hora");
    std::string location = parseJsonField(requestBody, "local");
    std::string description = parseJsonField(requestBody, "descricao");

    if (name.empty() || date.empty() || time.empty() || location.empty() || description.empty()) {
        sendResponse(clientSocket, "400 Bad Request", "application/json", 
                     "{\"error\":\"Missing required fields for event.\"}", "");
        return;
    }

    try {
        json createdEvent = eventManager.addEvent(name, date, time, location, description);
        sendResponse(clientSocket, "201 Created", "application/json", createdEvent.dump());
    } catch (const std::runtime_error& e) {
        sendResponse(clientSocket, "500 Internal Server Error", "application/json", 
                     "{\"error\":\"" + std::string(e.what()) + "\"}", "");
    }
}

void ApiServer::handleGetDashboardStats(SOCKET clientSocket) {
    json stats = eventManager.getDashboardStats();
    sendResponse(clientSocket, "200 OK", "application/json", stats.dump());
}

void ApiServer::handlePostParticipant(SOCKET clientSocket, int eventId, const std::string& requestBody) {
    std::string name = parseJsonField(requestBody, "nome");
    std::string email = parseJsonField(requestBody, "email");
    std::string contact = parseJsonField(requestBody, "contato");

    if (name.empty() || email.empty()) {
        sendResponse(clientSocket, "400 Bad Request", "application/json", 
                     "{\"error\":\"Nome and email are required for participant.\"}", "");
        return;
    }

    try {
        json createdParticipant = eventManager.addParticipantToEvent(eventId, name, email, contact);
        sendResponse(clientSocket, "201 Created", "application/json", createdParticipant.dump());
    } catch (const std::runtime_error& e) {
        sendResponse(clientSocket, "404 Not Found", "application/json", 
                     "{\"error\":\"" + std::string(e.what()) + "\"}", "");
    }
}

void ApiServer::handleGetParticipants(SOCKET clientSocket, int eventId) {
    try {
        json participantsArray = eventManager.getParticipantsForEvent(eventId);
        sendResponse(clientSocket, "200 OK", "application/json", participantsArray.dump());
    } catch (const std::runtime_error& e) {
        sendResponse(clientSocket, "404 Not Found", "application/json", 
                     "{\"error\":\"" + std::string(e.what()) + "\"}", "");
    }
}

void ApiServer::handlePutEvent(SOCKET clientSocket, int eventId, const std::string& requestBody) {
    std::string name = parseJsonField(requestBody, "nome");
    std::string data = parseJsonField(requestBody, "data");
    std::string hora = parseJsonField(requestBody, "hora");
    std::string local = parseJsonField(requestBody, "local");
    std::string descricao = parseJsonField(requestBody, "descricao");

    if (name.empty() || data.empty() || hora.empty() || local.empty() || descricao.empty()) {
        sendResponse(clientSocket, "400 Bad Request", "application/json", 
                     "{\"error\":\"Missing required fields for event update.\"}", "");
        return;
    }

    try {
        bool updated = eventManager.updateEvent(eventId, name, data, hora, local, descricao);
        if (updated) {
            sendResponse(clientSocket, "200 OK", "application/json", 
                         "{\"status\":\"success\",\"message\":\"Event updated successfully.\"}", "");
        } else {
            sendResponse(clientSocket, "404 Not Found", "application/json", 
                         "{\"error\":\"Event not found.\"}", "");
        }
    } catch (const std::runtime_error& e) {
        sendResponse(clientSocket, "500 Internal Server Error", "application/json", 
                     "{\"error\":\"" + std::string(e.what()) + "\"}", "");
    }
}

void ApiServer::handleDeleteEvent(SOCKET clientSocket, int eventId) {
    try {
        bool deleted = eventManager.deleteEvent(eventId);
        if (deleted) {
            sendResponse(clientSocket, "200 OK", "application/json", 
                         "{\"status\":\"success\",\"message\":\"Event deleted successfully.\"}", "");
        } else {
            sendResponse(clientSocket, "404 Not Found", "application/json", 
                         "{\"error\":\"Event not found.\"}", "");
        }
    } catch (const std::runtime_error& e) {
        sendResponse(clientSocket, "500 Internal Server Error", "application/json", 
                     "{\"error\":\"" + std::string(e.what()) + "\"}", "");
    }
}

void ApiServer::handlePutParticipant(SOCKET clientSocket, int eventId, int participantId, const std::string& requestBody) {
    std::string newName = parseJsonField(requestBody, "nome");
    std::string newEmail = parseJsonField(requestBody, "email");
    std::string newContact = parseJsonField(requestBody, "contato");

    if (newName.empty() || newEmail.empty()) { // Contact can be empty
        sendResponse(clientSocket, "400 Bad Request", "application/json", 
                     "{\"error\":\"Nome and email are required for participant update.\"}", "");
        return;
    }

    try {
        bool updated = eventManager.updateParticipantInEvent(eventId, participantId, newName, newEmail, newContact);
        if (updated) {
            sendResponse(clientSocket, "200 OK", "application/json", 
                         "{\"status\":\"success\",\"message\":\"Participant updated successfully.\"}", "");
        } else {
            sendResponse(clientSocket, "404 Not Found", "application/json", 
                         "{\"error\":\"Event or Participant not found.\"}", "");
        }
    } catch (const std::runtime_error& e) {
        sendResponse(clientSocket, "500 Internal Server Error", "application/json", 
                     "{\"error\":\"" + std::string(e.what()) + "\"}", "");
    }
}

void ApiServer::handleDeleteParticipant(SOCKET clientSocket, int eventId, int participantId) {
    try {
        bool deleted = eventManager.removeParticipantFromEvent(eventId, participantId);
        if (deleted) {
            sendResponse(clientSocket, "200 OK", "application/json", 
                         "{\"status\":\"success\",\"message\":\"Participant deleted successfully.\"}", "");
        } else {
            sendResponse(clientSocket, "404 Not Found", "application/json", 
                         "{\"error\":\"Event or Participant not found.\"}", "");
        }
    } catch (const std::runtime_error& e) {
        sendResponse(clientSocket, "500 Internal Server Error", "application/json", 
                     "{\"error\":\"" + std::string(e.what()) + "\"}", "");
    }
}


// --- Constructor and Destructor ---

ApiServer::ApiServer(EventManager& manager) : eventManager(manager), serverSocket(INVALID_SOCKET) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Erro ao inicializar Winsock" << std::endl;
        exit(1); // Exit if Winsock fails to initialize
    }
    
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Erro ao criar socket" << std::endl;
        WSACleanup();
        exit(1);
    }
    
    // Set socket option to reuse address (SO_REUSEADDR) - useful for quick restarts
    int optval = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) == SOCKET_ERROR) {
        std::cerr << "Erro ao definir SO_REUSEADDR: " << WSAGetLastError() << std::endl;
    }

    // Set a receive timeout (optional, but can prevent blocking indefinitely)
    // int timeout = 5000; // 5 seconds
    // setsockopt(serverSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
}

ApiServer::~ApiServer() {
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
    }
    WSACleanup();
}

// --- Main Server Loop ---

void ApiServer::start(int port) {
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    serverAddr.sin_port = htons(port);
    
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Erro ao fazer bind na porta " << port << ": " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(1);
    }
    
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) { // SOMAXCONN is max pending connections
        std::cerr << "Erro ao escutar na porta " << port << ": " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(1);
    }
    
    std::cout << "🚀 API Server started on port " << port << "..." << std::endl;
    std::cout << "📱 Frontend available at: http://localhost:" << port << "/frontend/" << std::endl;
    std::cout << "🔧 To stop the server, press Ctrl+C" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    
    while (true) {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Erro ao aceitar conexão: " << WSAGetLastError() << std::endl;
            continue;
        }
        
        char buffer[4096];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::string request(buffer);
            
            std::cout << "==================================================" << std::endl;
            std::cout << ">>> REQUISICAO RECEBIDA:" << std::endl;
            std::cout << request.substr(0, std::min((int)request.length(), 400)) << "..." << std::endl;

            std::string method = request.substr(0, request.find(" "));
            std::string path = getRequestPath(request);
            
            std::cout << "--> METHOD: [" << method << "]" << std::endl;
            std::cout << "--> PATH: [" << path << "]" << std::endl;

            // Extract request body for POST/PUT requests
            std::string requestBody;
            size_t bodyPos = request.find("\r\n\r\n");
            if (bodyPos != std::string::npos) {
                requestBody = request.substr(bodyPos + 4);
            }

            // --- Routing Logic ---
            std::cout << "--> ROUTING: ";
            try {
                if (method == "OPTIONS") {
                    std::cout << "OPTIONS (CORS Preflight)" << std::endl;
                    // For OPTIONS, we just send necessary headers, no content
                    sendResponse(clientSocket, "204 No Content", "text/plain", "");
                }
                else if (path == "/health") {
                    std::cout << "Route /health" << std::endl;
                    sendResponse(clientSocket, "200 OK", "application/json", "{\"status\":\"ok\",\"message\":\"Server is running!\"}");
                }
                else if (path == "/api/eventos" && method == "GET") {
                    std::cout << "Route /api/eventos (GET)" << std::endl;
                    handleGetEvents(clientSocket);
                }
                else if ((path == "/api/eventos" || path == "/api/eventos/") && method == "POST") {
                    std::cout << "Route /api/eventos (POST)" << std::endl;
                    handlePostEvent(clientSocket, requestBody);
                }
                else if (path == "/api/relatorio" && method == "GET") {
                    std::cout << "Route /api/relatorio (GET)" << std::endl;
                    handleGetDashboardStats(clientSocket);
                }
                // --- Event Specific Routes (PUT/DELETE) ---
                else if (path.rfind("/api/eventos/", 0) == 0 && path.find("/participantes") == std::string::npos) {
                    // This covers /api/eventos/{id}
                    try {
                        size_t idStart = path.find("/api/eventos/") + 13;
                        int eventId = std::stoi(path.substr(idStart));
                        
                        if (method == "PUT") {
                            std::cout << "Route /api/eventos/{id} (PUT) for ID: " << eventId << std::endl;
                            handlePutEvent(clientSocket, eventId, requestBody);
                        } else if (method == "DELETE") {
                            std::cout << "Route /api/eventos/{id} (DELETE) for ID: " << eventId << std::endl;
                            handleDeleteEvent(clientSocket, eventId);
                        } else {
                            sendResponse(clientSocket, "405 Method Not Allowed", "application/json", "{\"error\":\"Method not allowed for this route.\"}", "Allow: GET, POST, PUT, DELETE, OPTIONS\r\n");
                        }
                    } catch (const std::invalid_argument& e) {
                        sendResponse(clientSocket, "400 Bad Request", "application/json", "{\"error\":\"Invalid Event ID format.\"}", "");
                    } catch (const std::out_of_range& e) {
                        sendResponse(clientSocket, "400 Bad Request", "application/json", "{\"error\":\"Event ID out of range.\"}", "");
                    }
                }
                // --- Participant Specific Routes ---
                else if (path.rfind("/api/eventos/", 0) == 0 && path.find("/participantes") != std::string::npos) {
                    // Handles /api/eventos/{eventId}/participantes or /api/eventos/{eventId}/participantes/{participantId}
                    size_t eventIdStart = path.find("/api/eventos/") + 13;
                    size_t eventIdEnd = path.find("/participantes", eventIdStart);
                    
                    if (eventIdEnd == std::string::npos) { // Should not happen with correct path structure
                         sendResponse(clientSocket, "400 Bad Request", "application/json", "{\"error\":\"Invalid participants route format.\"}", "");
                         closesocket(clientSocket);
                         continue;
                    }

                    try {
                        int eventId = std::stoi(path.substr(eventIdStart, eventIdEnd - eventIdStart));
                        
                        // Check if there's a participant ID in the path
                        size_t participantIdStart = path.find("/participantes/", eventIdEnd) + std::string("/participantes/").length();
                        
                        if (participantIdStart != std::string::npos && participantIdStart < path.length()) {
                            // Route: /api/eventos/{eventId}/participantes/{participantId}
                            int participantId = std::stoi(path.substr(participantIdStart));
                            std::cout << "Route /api/eventos/" << eventId << "/participantes/" << participantId << " (Method: " << method << ")" << std::endl;
                            
                            if (method == "PUT") {
                                handlePutParticipant(clientSocket, eventId, participantId, requestBody);
                            } else if (method == "DELETE") {
                                handleDeleteParticipant(clientSocket, eventId, participantId);
                            } else {
                                sendResponse(clientSocket, "405 Method Not Allowed", "application/json", "{\"error\":\"Method not allowed for this participant route.\"}", "Allow: PUT, DELETE, OPTIONS\r\n");
                            }

                        } else {
                            // Route: /api/eventos/{eventId}/participantes
                            std::cout << "Route /api/eventos/" << eventId << "/participantes (Method: " << method << ")" << std::endl;
                            if (method == "POST") {
                                handlePostParticipant(clientSocket, eventId, requestBody);
                            } else if (method == "GET") {
                                handleGetParticipants(clientSocket, eventId);
                            } else {
                                sendResponse(clientSocket, "405 Method Not Allowed", "application/json", "{\"error\":\"Method not allowed for this route.\"}", "Allow: GET, POST, OPTIONS\r\n");
                            }
                        }
                    } catch (const std::invalid_argument& e) {
                        sendResponse(clientSocket, "400 Bad Request", "application/json", "{\"error\":\"Invalid ID format in participant route.\"}", "");
                    } catch (const std::out_of_range& e) {
                        sendResponse(clientSocket, "400 Bad Request", "application/json", "{\"error\":\"ID out of range in participant route.\"}", "");
                    }
                }
                // --- Static File Serving ---
                else if (path.rfind("/frontend/", 0) == 0 || path == "/") {
                    std::cout << "Static File Route" << std::endl;
                    std::string filePath = (path == "/") ? "frontend/index.html" : path.substr(1);
                    size_t queryPos = filePath.find("?");
                    if (queryPos != std::string::npos) {
                        filePath = filePath.substr(0, queryPos); // Remove query parameters
                    }
                    if (!filePath.empty() && filePath.back() == '/') {
                        filePath += "index.html"; // Default to index.html for directory requests
                    }
                    
                    std::ifstream file(filePath, std::ios::binary);
                    if (file) {
                        std::stringstream fileBuffer;
                        fileBuffer << file.rdbuf();
                        std::string content = fileBuffer.str();

                        std::string contentType = "text/html";
                        if (filePath.find(".css") != std::string::npos) contentType = "text/css";
                        else if (filePath.find(".js") != std::string::npos) contentType = "application/javascript";
                        else if (filePath.find(".png") != std::string::npos) contentType = "image/png"; // Example for images
                        else if (filePath.find(".jpg") != std::string::npos || filePath.find(".jpeg") != std::string::npos) contentType = "image/jpeg";
                        else if (filePath.find(".ico") != std::string::npos) contentType = "image/x-icon";

                        sendResponse(clientSocket, "200 OK", contentType, content);
                    } else {
                        std::cout << "File not found: " << filePath << std::endl;
                        sendResponse(clientSocket, "404 Not Found", "text/html", 
                                     "<h1>404 Not Found</h1><p>The requested file '" + filePath + "' was not found.</p>");
                    }
                }
                // --- 404 Not Found for API routes ---
                else {
                    std::cout << "404 Not Found (API or unhandled route)" << std::endl;
                    sendResponse(clientSocket, "404 Not Found", "application/json", 
                                 "{\"error\":\"Route not found or method not supported for path: " + path + "\"}", "");
                }
            } catch (const std::exception& e) {
                std::cerr << "Unhandled exception during request processing: " << e.what() << std::endl;
                sendResponse(clientSocket, "500 Internal Server Error", "application/json", 
                             "{\"error\":\"Internal server error: " + std::string(e.what()) + "\"}", "");
            }
            std::cout << "==================================================" << std::endl << std::endl;
        } else if (bytesReceived == 0) {
            std::cout << "Client disconnected." << std::endl;
        } else {
            std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
        }
        closesocket(clientSocket);
    }
}