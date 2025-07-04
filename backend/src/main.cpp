#include <iostream>
#include <stdexcept>

#include "EventManager.h"
#include "ApiServer.h"

int main() {
    const int API_PORT = 8080;

    try {
        std::cout << "Initializing Event Manager..." << std::endl;
        EventManager eventManager;
        std::cout << "Event Manager initialized successfully." << std::endl;

        std::cout << "Initializing API Server..." << std::endl;
        ApiServer apiServer(eventManager);
        std::cout << "API Server initialized successfully." << std::endl;

        std::cout << "Starting API Server on port " << API_PORT << "..." << std::endl;
        apiServer.start(API_PORT);

    } catch (const std::exception& e) {
        std::cerr << "CRITICAL ERROR: An unhandled exception occurred: " << e.what() << std::endl;
        std::cerr << "Server is shutting down." << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "CRITICAL ERROR: An unknown exception occurred." << std::endl;
        std::cerr << "Server is shutting down." << std::endl;
        return 1;
    }

    return 0;
}