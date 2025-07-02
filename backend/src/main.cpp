#include <iostream>
#include "GerenciadorEventos.h"

//main fazendo a chamada do menu e das funcoes do gerenciador de eventos

void exibirMenu() {
    std::cout << "\n=== Sistema de Eventos ===" << std::endl;
    std::cout << "1. Cadastrar evento" << std::endl;
    std::cout << "2. Inscrever participante" << std::endl;
    std::cout << "3. Gerar relatorio" << std::endl;
    std::cout << "4. Atualizar evento" << std::endl;
    std::cout << "5. Deletar evento" << std::endl;
    std::cout << "6. Salvar eventos em arquivo" << std::endl;
    std::cout << "7. Listar todos os eventos" << std::endl;
    std::cout << "0. Sair" << std::endl;
    std::cout << "Escolha uma opcao: ";
}

int main() {
    GerenciadorEventos gerenciador;
    int opcao;

    //carregar os eventos ao iniciar
    gerenciador.carregarEventosDoArquivo();

    do {
        exibirMenu();
        std::cin >> opcao;

        switch (opcao) {
            case 1:
                gerenciador.cadastrarEvento();
                break;
            case 2:
                gerenciador.inscreverParticipante();
                break;
            case 3:
                gerenciador.gerarRelatorio();
                break;
            case 4:
                gerenciador.atualizarEvento();
                break;
            case 5:
                gerenciador.deletarEvento();
                break;
            case 6:
                gerenciador.salvarEventosEmArquivo();
                break;
            case 7:
                gerenciador.listarEventos();
                break;
            case 0:
                //vai saalvar os eventos antes de sair
                gerenciador.salvarEventosEmArquivo();
                std::cout << "Saindo do sistema..." << std::endl;
                break;
            default:
                std::cout << "Opcao invalida!" << std::endl;
        }
    } while (opcao != 0);

    return 0;
}