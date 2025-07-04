#include "ApiServer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <regex>
#include "json/json.hpp"
#pragma comment(lib, "ws2_32.lib")

std::string ApiServer::getRequestPath(const std::string& request) {
    size_t startPos = request.find(" ") + 1;
    size_t endPos = request.find(" ", startPos);
    if (startPos == std::string::npos || endPos == std::string::npos) {
        return "";
    }
    return request.substr(startPos, endPos - startPos);
}

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

void ApiServer::sendResponse(SOCKET clientSocket, const std::string& status, 
                             const std::string& contentType, const std::string& content, 
                             const std::string& extraHeaders) {
    std::string response = "HTTP/1.1 " + status + "\r\n";
    response += "Content-Type: " + contentType + "\r\n";
    response += "Access-Control-Allow-Origin: *\r\n";
    response += "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n";
    response += "Access-Control-Allow-Headers: Content-Type, Authorization\r\n";
    response += extraHeaders;
    response += "Content-Length: " + std::to_string(content.length()) + "\r\n";
    response += "\r\n";
    response += content;
    send(clientSocket, response.c_str(), response.length(), 0);
}

void ApiServer::handleGetEvents(SOCKET clientSocket) {
    json eventsArray = json::array();
    for (const auto& eventPtr : eventManager.getAllEvents()) {
        json eventJson;
        eventJson["id"] = eventPtr->getId();
        eventJson["nome"] = eventPtr->getName();
        eventJson["data"] = eventPtr->getDate();
        eventJson["hora"] = eventPtr->getTime();
        eventJson["local"] = eventPtr->getLocation();
        eventJson["descricao"] = eventPtr->getDescription();
        eventJson["numParticipantes"] = eventPtr->getNumParticipants();
        eventsArray.push_back(eventJson);
    }
    sendResponse(clientSocket, "200 OK", "application/json", eventsArray.dump());
}

void ApiServer::handlePostEvent(SOCKET clientSocket, const std::string& requestBody) {
    try {
        json requestJson = json::parse(requestBody);
        std::string name = requestJson.at("nome").get<std::string>();
        std::string date = requestJson.at("data").get<std::string>();
        std::string time = requestJson.at("hora").get<std::string>();
        std::string location = requestJson.at("local").get<std::string>();
        std::string description = requestJson.at("descricao").get<std::string>();
        json createdEvent = eventManager.addEvent(name, date, time, location, description);
        sendResponse(clientSocket, "201 Created", "application/json", createdEvent.dump());
    } catch (const json::parse_error& e) {
        sendResponse(clientSocket, "400 Bad Request", "application/json", 
                     "{\"error\":\"Invalid JSON format: " + std::string(e.what()) + "\"}", "");
    } catch (const json::out_of_range& e) {
        sendResponse(clientSocket, "400 Bad Request", "application/json", 
                     "{\"error\":\"Missing required JSON field: " + std::string(e.what()) + "\"}", "");
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
    try {
        json requestJson = json::parse(requestBody);
        std::string name = requestJson.at("nome").get<std::string>();
        std::string email = requestJson.at("email").get<std::string>();
        std::string contact = requestJson.value("contato", "");
        json createdParticipant = eventManager.addParticipantToEvent(eventId, name, email, contact);
        sendResponse(clientSocket, "201 Created", "application/json", createdParticipant.dump());
    } catch (const json::parse_error& e) {
        sendResponse(clientSocket, "400 Bad Request", "application/json", 
                     "{\"error\":\"Invalid JSON format: " + std::string(e.what()) + "\"}", "");
    } catch (const json::out_of_range& e) {
        sendResponse(clientSocket, "400 Bad Request", "application/json", 
                     "{\"error\":\"Missing required JSON field: " + std::string(e.what()) + "\"}", "");
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

void ApiServer::handleGetEventById(SOCKET clientSocket, int eventId) {
    try {
        const Event& event = eventManager.getEventById(eventId);
        json eventJson;
        eventJson["id"] = event.getId();
        eventJson["nome"] = event.getName();
        eventJson["data"] = event.getDate();
        eventJson["hora"] = event.getTime();
        eventJson["local"] = event.getLocation();
        eventJson["descricao"] = event.getDescription();
        eventJson["numParticipantes"] = event.getNumParticipants();
        sendResponse(clientSocket, "200 OK", "application/json", eventJson.dump());
    } catch (const std::runtime_error& e) {
        sendResponse(clientSocket, "404 Not Found", "application/json", 
                     "{\"error\":\"" + std::string(e.what()) + "\"}", "");
    }
}

void ApiServer::handlePutEvent(SOCKET clientSocket, int eventId, const std::string& requestBody) {
    try {
        json requestJson = json::parse(requestBody);
        std::string name = requestJson.at("nome").get<std::string>();
        std::string data = requestJson.at("data").get<std::string>();
        std::string hora = requestJson.at("hora").get<std::string>();
        std::string local = requestJson.at("local").get<std::string>();
        std::string descricao = requestJson.at("descricao").get<std::string>();
        bool updated = eventManager.updateEvent(eventId, name, data, hora, local, descricao);
        if (updated) {
            sendResponse(clientSocket, "200 OK", "application/json", 
                         "{\"status\":\"success\",\"message\":\"Event updated successfully.\"}", "");
        } else {
            sendResponse(clientSocket, "404 Not Found", "application/json", 
                         "{\"error\":\"Event not found.\"}", "");
        }
    } catch (const json::parse_error& e) {
        sendResponse(clientSocket, "400 Bad Request", "application/json", 
                     "{\"error\":\"Invalid JSON format: " + std::string(e.what()) + "\"}", "");
    } catch (const json::out_of_range& e) {
        sendResponse(clientSocket, "400 Bad Request", "application/json", 
                     "{\"error\":\"Missing required JSON field: " + std::string(e.what()) + "\"}", "");
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
    try {
        json requestJson = json::parse(requestBody);
        std::string newName = requestJson.at("nome").get<std::string>();
        std::string newEmail = requestJson.at("email").get<std::string>();
        std::string newContact = requestJson.value("contato", "");
        bool updated = eventManager.updateParticipantInEvent(eventId, participantId, newName, newEmail, newContact);
        if (updated) {
            sendResponse(clientSocket, "200 OK", "application/json", 
                         "{\"status\":\"success\",\"message\":\"Participant updated successfully.\"}", "");
        } else {
            sendResponse(clientSocket, "404 Not Found", "application/json", 
                         "{\"error\":\"Event or Participant not found.\"}", "");
        }
    } catch (const json::parse_error& e) {
        sendResponse(clientSocket, "400 Bad Request", "application/json", 
                     "{\"error\":\"Invalid JSON format: " + std::string(e.what()) + "\"}", "");
    } catch (const json::out_of_range& e) {
        sendResponse(clientSocket, "400 Bad Request", "application/json", 
                     "{\"error\":\"Missing required JSON field: " + std::string(e.what()) + "\"}", "");
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

ApiServer::ApiServer(EventManager& manager) : eventManager(manager), serverSocket(INVALID_SOCKET) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Erro ao inicializar Winsock" << std::endl;
        exit(1);
    }
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Erro ao criar socket" << std::endl;
        WSACleanup();
        exit(1);
    }
    int optval = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) == SOCKET_ERROR) {
        std::cerr << "Erro ao definir SO_REUSEADDR: " << WSAGetLastError() << std::endl;
    }
}

ApiServer::~ApiServer() {
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
    }
    WSACleanup();
}

void ApiServer::start(int port) {
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Erro ao fazer bind na porta " << port << ": " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(1);
    }
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Erro ao escutar na porta " << port << ": " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(1);
    }
    std::cout << "🚀 API Server started on port " << port << "..." << std::endl;
    std::cout << "📱 Frontend available at: http://localhost:" << port << "/frontend/" << std::endl;
    std::cout << "🔧 To stop the server, pressione Ctrl+C" << std::endl;
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
            if (!path.empty() && path.back() == '/' && path.length() > 1) {
                path.pop_back();
            }
            std::cout << "--> METHOD: [" << method << "]" << std::endl;
            std::cout << "--> PATH: [" << path << "]" << std::endl;
            std::string requestBody;
            size_t bodyPos = request.find("\r\n\r\n");
            if (bodyPos != std::string::npos) {
                requestBody = request.substr(bodyPos + 4);
            }
            std::cout << "--> ROUTING: ";
            try {
                if (method == "OPTIONS") {
                    std::cout << "OPTIONS (CORS Preflight)" << std::endl;
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
                else if (method == "GET" && path.rfind("/api/eventos/", 0) == 0 && path.find("/participantes") == std::string::npos) {
                    try {
                        size_t idStart = path.find("/api/eventos/") + 13;
                        int eventId = std::stoi(path.substr(idStart));
                        std::cout << "Route /api/eventos/{id} (GET) for ID: " << eventId << std::endl;
                        handleGetEventById(clientSocket, eventId);
                    } catch (const std::invalid_argument& e) {
                        sendResponse(clientSocket, "400 Bad Request", "application/json", "{\"error\":\"Invalid Event ID format.\"}", "");
                    } catch (const std::out_of_range& e) {
                        sendResponse(clientSocket, "400 Bad Request", "application/json", "{\"error\":\"Event ID out of range.\"}", "");
                    }
                }
                else if ((method == "PUT" || method == "DELETE") && path.rfind("/api/eventos/", 0) == 0 && path.find("/participantes") == std::string::npos) {
                    try {
                        size_t idStart = path.find("/api/eventos/") + 13;
                        int eventId = std::stoi(path.substr(idStart));
                        if (method == "PUT") {
                            std::cout << "Route /api/eventos/{id} (PUT) for ID: " << eventId << std::endl;
                            handlePutEvent(clientSocket, eventId, requestBody);
                        } else {
                            std::cout << "Route /api/eventos/{id} (DELETE) for ID: " << eventId << std::endl;
                            handleDeleteEvent(clientSocket, eventId);
                        }
                    } catch (const std::invalid_argument& e) {
                        sendResponse(clientSocket, "400 Bad Request", "application/json", "{\"error\":\"Invalid Event ID format.\"}", "");
                    } catch (const std::out_of_range& e) {
                        sendResponse(clientSocket, "400 Bad Request", "application/json", "{\"error\":\"Event ID out of range.\"}", "");
                    }
                }
                else if (path.rfind("/api/eventos/", 0) == 0 && path.find("/participantes") != std::string::npos) {
                    size_t eventIdStart = path.find("/api/eventos/") + 13;
                    size_t eventIdEnd = path.find("/participantes", eventIdStart);
                    if (eventIdEnd == std::string::npos) { 
                         sendResponse(clientSocket, "400 Bad Request", "application/json", "{\"error\":\"Invalid participants route format.\"}", "");
                         closesocket(clientSocket);
                         continue;
                    }
                    int eventId = 0;
                    try {
                        eventId = std::stoi(path.substr(eventIdStart, eventIdEnd - eventIdStart));
                        size_t participants_segment_pos = path.find("/participantes", eventIdEnd);
                        size_t specific_participant_id_start_pos = path.find("/", participants_segment_pos + std::string("/participantes").length());
                        if (specific_participant_id_start_pos != std::string::npos && 
                            specific_participant_id_start_pos < path.length() -1 ) {
                            try {
                                int participantId = std::stoi(path.substr(specific_participant_id_start_pos + 1));
                                if (method == "PUT") {
                                    handlePutParticipant(clientSocket, eventId, participantId, requestBody);
                                } else if (method == "DELETE") {
                                    handleDeleteParticipant(clientSocket, eventId, participantId);
                                } else {
                                    sendResponse(clientSocket, "405 Method Not Allowed", "application/json", "{\"error\":\"Method not allowed for this participant route.\"}", "Allow: PUT, DELETE, OPTIONS\r\n");
                                }
                            } catch (const std::invalid_argument& e) {
                                sendResponse(clientSocket, "400 Bad Request", "application/json", "{\"error\":\"Invalid participant ID format in route.\"}", "");
                            } catch (const std::out_of_range& e) {
                                sendResponse(clientSocket, "400 Bad Request", "application/json", "{\"error\":\"Participant ID out of range.\"}", "");
                            }
                        } else {
                            if (method == "POST") {
                                handlePostParticipant(clientSocket, eventId, requestBody);
                            } else if (method == "GET") {
                                handleGetParticipants(clientSocket, eventId);
                            } else {
                                sendResponse(clientSocket, "405 Method Not Allowed", "application/json", "{\"error\":\"Method not allowed for this route.\"}", "Allow: GET, POST, OPTIONS\r\n");
                            }
                        }
                    } catch (const std::invalid_argument& e) {
                        sendResponse(clientSocket, "400 Bad Request", "application/json", "{\"error\":\"Invalid Event ID format in participant route.\"}", "");
                    } catch (const std::out_of_range& e) {
                        sendResponse(clientSocket, "400 Bad Request", "application/json", "{\"error\":\"Event ID out of range.\"}", "");
                    }
                }
                else if (path == "/" || path == "/frontend" || path.rfind("/frontend/", 0) == 0) {
                    std::cout << "Static File Route" << std::endl;
                    std::string localFilePath;
                    if (path == "/" || path == "/frontend") {
                        localFilePath = "frontend/index.html";
                    } else {
                        localFilePath = path.substr(1);
                    }
                    size_t queryPos = localFilePath.find("?");
                    if (queryPos != std::string::npos) {
                        localFilePath = localFilePath.substr(0, queryPos);
                    }
                    std::ifstream file(localFilePath, std::ios::binary);
                    if (file) {
                        std::stringstream fileBuffer;
                        fileBuffer << file.rdbuf();
                        std::string content = fileBuffer.str();
                        std::string contentType = "application/octet-stream";
                        if (localFilePath.find(".html") != std::string::npos) contentType = "text/html";
                        else if (localFilePath.find(".css") != std::string::npos) contentType = "text/css";
                        else if (localFilePath.find(".js") != std::string::npos) contentType = "application/javascript";
                        else if (localFilePath.find(".png") != std::string::npos) contentType = "image/png";
                        else if (localFilePath.find(".jpg") != std::string::npos || localFilePath.find(".jpeg") != std::string::npos) contentType = "image/jpeg";
                        else if (localFilePath.find(".ico") != std::string::npos) contentType = "image/x-icon";
                        sendResponse(clientSocket, "200 OK", contentType, content);
                    } else {
                        std::cout << "File not found: " << localFilePath << std::endl;
                        sendResponse(clientSocket, "404 Not Found", "text/html", 
                                     "<h1>404 Not Found</h1><p>The requested file '" + localFilePath + "' was not found.</p>");
                    }
                }
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
