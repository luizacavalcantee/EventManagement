#include "GerenciadorEventos.h"
#include <iostream>
#include <fstream>
#include <sstream>

// sobre a classe GerenciadorEventos

GerenciadorEventos::GerenciadorEventos() {
    numEventos = 0;
}

GerenciadorEventos::~GerenciadorEventos() {
    for (int i = 0; i < numEventos; i++) {
        delete eventos[i];
    }
}

void GerenciadorEventos::cadastrarEvento() {
    if (numEventos < 10) {
        std::string nome, data, hora, local, descricao;
        std::cin.ignore();
        std::cout << "\n=== Cadastro de Evento ===" << std::endl;
        std::cout << "Nome do evento: "; std::getline(std::cin, nome);
        std::cout << "Data: "; std::getline(std::cin, data);
        std::cout << "Hora: "; std::getline(std::cin, hora);
        std::cout << "Local: "; std::getline(std::cin, local);
        std::cout << "Descricao: "; std::getline(std::cin, descricao);

        eventos[numEventos] = new Evento(nome, data, hora, local, descricao);
        numEventos++;
        std::cout << "Evento cadastrado com sucesso!" << std::endl;
    } else {
        std::cout << "Limite de eventos atingido!" << std::endl;
    }
}

void GerenciadorEventos::inscreverParticipante() {
    if (numEventos == 0) {
        std::cout << "Sem eventos cadastrados!" << std::endl;
        return;
    }

    std::string nomeEvento, nomeParticipante, email, contato;
    std::cin.ignore();
    std::cout << "\nNome do evento: "; std::getline(std::cin, nomeEvento);

    for (int i = 0; i < numEventos; i++) {
        if (eventos[i]->getNome() == nomeEvento) {
            std::cout << "Nome do participante: "; std::getline(std::cin, nomeParticipante);
            std::cout << "Email do participante: "; std::getline(std::cin, email);
            std::cout << "Contato do participante: "; std::getline(std::cin, contato);
            eventos[i]->adicionarParticipante(nomeParticipante, email, contato);
            return;
        }
    }
    std::cout << "Evento nao encontrado!" << std::endl;
}

void GerenciadorEventos::gerarRelatorio() {
    if (numEventos == 0) {
        std::cout << "Nao ha eventos cadastrados!" << std::endl;
        return;
    }

    std::string nomeEvento;
    std::cin.ignore();
    std::cout << "\nNome do evento para relatorio: "; std::getline(std::cin, nomeEvento);

    for (int i = 0; i < numEventos; i++) {
        if (eventos[i]->getNome() == nomeEvento) {
            std::cout << "\n=== Relatorio do Evento ===" << std::endl;
            std::cout << "Nome: " << eventos[i]->getNome() << std::endl;
            std::cout << "Data: " << eventos[i]->getData() << std::endl;
            std::cout << "Hora: " << eventos[i]->getHora() << std::endl;
            std::cout << "Local: " << eventos[i]->getLocal() << std::endl;
            std::cout << "Descricao: " << eventos[i]->getDescricao() << std::endl;
            std::cout << "\nParticipantes:" << std::endl;
            std::cout << "------------------------" << std::endl;
            for (int j = 0; j < eventos[i]->getNumParticipantes(); j++) {
                Participante* p = eventos[i]->getParticipante(j);
                std::cout << "Nome: " << p->getNome() << std::endl;
                std::cout << "Email: " << p->getEmail() << std::endl;
                std::cout << "Contato: " << p->getContato() << std::endl;
                std::cout << "------------------------" << std::endl;
            }
            return;
        }
    }
    std::cout << "Evento nao encontrado!" << std::endl;
}

void GerenciadorEventos::atualizarEvento() {
    std::string nomeAtual, novoNome, novaData, novaHora, novoLocal, novaDescricao;
    std::cin.ignore();
    std::cout << "\nNome do evento que deseja atualizar: "; std::getline(std::cin, nomeAtual);

    for (int i = 0; i < numEventos; i++) {
        if (eventos[i]->getNome() == nomeAtual) {
            std::cout << "Novo nome: "; std::getline(std::cin, novoNome);
            std::cout << "Nova data: "; std::getline(std::cin, novaData);
            std::cout << "Nova hora: "; std::getline(std::cin, novaHora);
            std::cout << "Novo local: "; std::getline(std::cin, novoLocal);
            std::cout << "Nova descricao: "; std::getline(std::cin, novaDescricao);
            eventos[i]->atualizarEvento(novoNome, novaData, novaHora, novoLocal, novaDescricao);
            std::cout << "Evento atualizado com sucesso!" << std::endl;
            return;
        }
    }
    std::cout << "Evento nao encontrado!" << std::endl;
}

void GerenciadorEventos::deletarEvento() {
    std::string nomeEvento;
    std::cin.ignore();
    std::cout << "\nNome do evento que deseja deletar: "; std::getline(std::cin, nomeEvento);

    for (int i = 0; i < numEventos; i++) {
        if (eventos[i]->getNome() == nomeEvento) {
            delete eventos[i];
            for (int j = i; j < numEventos - 1; j++) {
                eventos[j] = eventos[j + 1];
            }
            numEventos--;
            std::cout << "Evento deletado com sucesso!" << std::endl;
            return;
        }
    }
    std::cout << "Evento nao encontrado!" << std::endl;
}

void GerenciadorEventos::salvarEventosEmArquivo() {
    std::ofstream arquivo("eventos.txt");
    for (int i = 0; i < numEventos; i++) {
        eventos[i]->salvarEvento(arquivo);
    }
    arquivo.close();
    std::cout << "Eventos salvos no arquivo com sucesso!" << std::endl;
}

void GerenciadorEventos::carregarEventosDoArquivo() {
    std::ifstream arquivo("eventos.txt");
    if (!arquivo.is_open()) {
        std::cout << "Nenhum arquivo de eventos encontrado." << std::endl;
        return;
    }

    std::string linha;
    Evento* eventoAtual = nullptr;

    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);
        std::string tipo;
        std::getline(ss, tipo, ',');

        if (tipo == "PARTICIPANTE") {
            std::string nome, email, contato;
            std::getline(ss, nome, ',');
            std::getline(ss, email, ',');
            std::getline(ss, contato, ',');
            if (eventoAtual) {
                eventoAtual->adicionarParticipante(nome, email, contato);
            }
        } else {
            std::string nome, data, hora, local, descricao;
            nome = tipo;
            std::getline(ss, data, ',');
            std::getline(ss, hora, ',');
            std::getline(ss, local, ',');
            std::getline(ss, descricao, ',');

            if (numEventos < 10) {
                eventos[numEventos] = new Evento(nome, data, hora, local, descricao);
                eventoAtual = eventos[numEventos];
                numEventos++;
            }
        }
    }
    arquivo.close();
    std::cout << "Eventos carregados com sucesso!" << std::endl;
}

void GerenciadorEventos::listarEventos() {
    if (numEventos == 0) {
        std::cout << "Nao ha eventos cadastrados!" << std::endl;
        return;
    }

    std::cout << "\n=== Lista de Eventos ===" << std::endl;
    for (int i = 0; i < numEventos; i++) {
        std::cout << "\nEvento " << (i + 1) << ":" << std::endl;
        std::cout << "Nome: " << eventos[i]->getNome() << std::endl;
        std::cout << "Data: " << eventos[i]->getData() << std::endl;
        std::cout << "Hora: " << eventos[i]->getHora() << std::endl;
        std::cout << "Local: " << eventos[i]->getLocal() << std::endl;
        std::cout << "Descricao: " << eventos[i]->getDescricao() << std::endl;
        std::cout << "Numero de participantes: " << eventos[i]->getNumParticipantes() << std::endl;
        std::cout << "------------------------" << std::endl;
    }
} 
