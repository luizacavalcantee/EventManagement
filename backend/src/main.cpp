#include <iostream>      // Inclui a biblioteca para entrada e saída padrão (cout, cerr)
#include <stdexcept>     // Inclui a biblioteca para exceções padrão (std::exception)

#include "EventManager.h" // Inclui a definição da classe EventManager
#include "ApiServer.h"    // Inclui a definição da classe ApiServer

int main() {
    const int API_PORT = 8080; // Define a porta na qual o servidor API irá rodar

    try {
        // Inicializa o gerenciador de eventos
        std::cout << "Initializing Event Manager..." << std::endl;
        EventManager eventManager;
        std::cout << "Event Manager initialized successfully." << std::endl;

        // Inicializa o servidor da API, passando o gerenciador de eventos
        std::cout << "Initializing API Server..." << std::endl;
        ApiServer apiServer(eventManager);
        std::cout << "API Server initialized successfully." << std::endl;

        // Inicia o servidor da API na porta especificada
        std::cout << "Starting API Server on port " << API_PORT << "..." << std::endl;
        apiServer.start(API_PORT);

    } catch (const std::exception& e) {
        // Captura exceções padrão e exibe mensagem de erro crítica
        std::cerr << "CRITICAL ERROR: An unhandled exception occurred: " << e.what() << std::endl;
        std::cerr << "Server is shutting down." << std::endl;
        return 1;
    } catch (...) {
        // Captura qualquer outra exceção desconhecida
        std::cerr << "CRITICAL ERROR: An unknown exception occurred." << std::endl;
        std::cerr << "Server is shutting down." << std::endl;
        return 1;
    }

    return 0; // Retorna 0 indicando execução bem-sucedida
}