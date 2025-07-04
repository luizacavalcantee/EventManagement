#include <iostream>
#include <stdexcept> // For std::runtime_error and other exceptions

#include "EventManager.h" // Your core business logic manager
#include "ApiServer.h"    // Your HTTP server implementation

int main() {
    // Define the port for the API server
    const int API_PORT = 8080;

    try {
        // 1. Initialize the EventManager (loads data from file)
        std::cout << "Initializing Event Manager..." << std::endl;
        EventManager eventManager;
        std::cout << "Event Manager initialized successfully." << std::endl;

        // 2. Initialize the API Server, passing the EventManager instance
        std::cout << "Initializing API Server..." << std::endl;
        ApiServer apiServer(eventManager);
        std::cout << "API Server initialized successfully." << std::endl;

        // 3. Start the API Server
        std::cout << "Starting API Server on port " << API_PORT << "..." << std::endl;
        apiServer.start(API_PORT);

    } catch (const std::exception& e) {
        // Catch any unhandled exceptions during setup or server runtime
        std::cerr << "CRITICAL ERROR: An unhandled exception occurred: " << e.what() << std::endl;
        std::cerr << "Server is shutting down." << std::endl;
        return 1; // Indicate error
    } catch (...) {
        // Catch any other unknown exceptions
        std::cerr << "CRITICAL ERROR: An unknown exception occurred." << std::endl;
        std::cerr << "Server is shutting down." << std::endl;
        return 1; // Indicate error
    }

    return 0; // Successful exit (though server loop is infinite)
}