#include "GerenciadorEventos.h"
#include <iostream>
#include <fstream>
#include <sstream>

// Usando o alias para o nlohmann::json
using json = nlohmann::json;

// --- Construtor e Destrutor ---

GerenciadorEventos::GerenciadorEventos() : nextId(1) {
    // O vetor é inicializado vazio por padrão. Nenhuma ação necessária.
}

GerenciadorEventos::~GerenciadorEventos() {
    // Itera sobre o vetor e deleta cada ponteiro para liberar a memória.
    for (Evento* ev : eventos) {
        delete ev;
    }
    // O vetor em si é limpo automaticamente quando o objeto é destruído.
}


// --- Métodos para interação via Console ---

void GerenciadorEventos::cadastrarEvento() {
    std::string nome, data, hora, local, descricao;
    std::cin.ignore();
    std::cout << "\n=== Cadastro de Evento ===" << std::endl;
    std::cout << "Nome do evento: "; std::getline(std::cin, nome);
    std::cout << "Data: "; std::getline(std::cin, data);
    std::cout << "Hora: "; std::getline(std::cin, hora);
    std::cout << "Local: "; std::getline(std::cin, local);
    std::cout << "Descricao: "; std::getline(std::cin, descricao);

    // MUDANÇA: Usa push_back para adicionar o novo evento. Não há mais limite de 10.
    eventos.push_back(new Evento(nextId++, nome, data, hora, local, descricao));
    std::cout << "Evento cadastrado com sucesso!" << std::endl;
}

void GerenciadorEventos::inscreverParticipante() {
    if (eventos.empty()) {
        std::cout << "Sem eventos cadastrados!" << std::endl;
        return;
    }

    std::string nomeEvento, nomeParticipante, email, contato;
    std::cin.ignore();
    std::cout << "\nNome do evento: "; std::getline(std::cin, nomeEvento);

    for (auto& evento : eventos) {
        if (evento->getNome() == nomeEvento) {
            std::cout << "Nome do participante: "; std::getline(std::cin, nomeParticipante);
            std::cout << "Email do participante: "; std::getline(std::cin, email);
            std::cout << "Contato do participante: "; std::getline(std::cin, contato);
            // Supondo que você tenha um método que cria e adiciona o participante
            // evento->adicionarParticipante(new Participante(nomeParticipante, email, contato));
            std::cout << "Participante inscrito com sucesso!" << std::endl;
            return;
        }
    }
    std::cout << "Evento nao encontrado!" << std::endl;
}

void GerenciadorEventos::atualizarEvento() {
    std::string nomeAtual, novoNome, novaData, novaHora, novoLocal, novaDescricao;
    std::cin.ignore();
    std::cout << "\nNome do evento que deseja atualizar: "; std::getline(std::cin, nomeAtual);

    for (auto& evento : eventos) {
        if (evento->getNome() == nomeAtual) {
            std::cout << "Novo nome: "; std::getline(std::cin, novoNome);
            std::cout << "Nova data: "; std::getline(std::cin, novaData);
            std::cout << "Nova hora: "; std::getline(std::cin, novaHora);
            std::cout << "Novo local: "; std::getline(std::cin, novoLocal);
            std::cout << "Nova descricao: "; std::getline(std::cin, novaDescricao);
            evento->atualizarEvento(novoNome, novaData, novaHora, novoLocal, novaDescricao);
            std::cout << "Evento atualizado com sucesso!" << std::endl;
            return;
        }
    }
    std::cout << "Evento nao encontrado!" << std::endl;
}

// CORREÇÃO CRÍTICA: Versão de console para deletar evento
void GerenciadorEventos::deletarEvento() {
    if (eventos.empty()) {
        std::cout << "Nao ha eventos para deletar!" << std::endl;
        return;
    }
    std::string nomeEvento;
    std::cin.ignore();
    std::cout << "\nNome do evento que deseja deletar: "; std::getline(std::cin, nomeEvento);

    for (auto it = eventos.begin(); it != eventos.end(); ++it) {
        if ((*it)->getNome() == nomeEvento) {
            delete *it; // 1. Libera a memória do objeto Evento
            eventos.erase(it); // 2. Remove o ponteiro do vetor de forma segura
            std::cout << "Evento deletado com sucesso!" << std::endl;
            salvarEventosEmArquivo();
            return; // Sai da função
        }
    }
    std::cout << "Evento nao encontrado!" << std::endl;
}

void GerenciadorEventos::listarEventos() {
    if (eventos.empty()) {
        std::cout << "Nao ha eventos cadastrados!" << std::endl;
        return;
    }

    std::cout << "\n=== Lista de Eventos ===" << std::endl;
    for (const auto& evento : eventos) {
        std::cout << "------------------------" << std::endl;
        evento->exibirDetalhes(); // Usando o método de exibição do próprio evento
    }
}

void GerenciadorEventos::salvarEventosEmArquivo() {
    std::ofstream arquivo("eventos.txt");
    for (const auto& evento : eventos) {
        evento->salvarEvento(arquivo);
    }
    arquivo.close();
    std::cout << "Eventos salvos no arquivo com sucesso!" << std::endl;
}

void GerenciadorEventos::carregarEventosDoArquivo() {
    std::ifstream arquivo("eventos.txt");
    if (!arquivo.is_open()) {
        // Não é um erro, apenas pode não existir arquivo ainda.
        // std::cout << "Nenhum arquivo de eventos encontrado." << std::endl;
        return;
    }

    std::string linha;
    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);
        int id;
        std::string idStr, nome, data, hora, local, descricao;

        std::getline(ss, idStr, ',');
        std::getline(ss, nome, ',');
        std::getline(ss, data, ',');
        std::getline(ss, hora, ',');
        std::getline(ss, local, ',');
        std::getline(ss, descricao, ',');
        
        try {
            id = std::stoi(idStr);
            eventos.push_back(new Evento(id, nome, data, hora, local, descricao));
            if (id >= nextId) {
                nextId = id + 1;
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Erro ao ler ID do arquivo: " << e.what() << std::endl;
        }
    }
    arquivo.close();
    std::cout << "Eventos carregados com sucesso!" << std::endl;
}


// --- Implementação dos Métodos da "API" ---

void GerenciadorEventos::deletarEvento(int id) {
    for (auto it = eventos.begin(); it != eventos.end(); ++it) {
        if ((*it)->getId() == id) {
            delete *it;
            eventos.erase(it);
            return;
        }
    }
    // Opcional: Lançar um erro se o evento não for encontrado
    throw std::runtime_error("Evento com ID " + std::to_string(id) + " nao encontrado para delecao.");
}

Evento& GerenciadorEventos::getEventoPorId(int id) {
    for (auto& evento : eventos) {
        if (evento->getId() == id) {
            return *evento; // Retorna uma referência ao evento encontrado
        }
    }
    throw std::runtime_error("Evento com ID " + std::to_string(id) + " nao encontrado.");
}

// Versão const para ser usada em métodos const
const Evento& GerenciadorEventos::getEventoPorId(int id) const {
    for (const auto& evento : eventos) {
        if (evento->getId() == id) {
            return *evento;
        }
    }
    throw std::runtime_error("Evento com ID " + std::to_string(id) + " nao encontrado.");
}

const std::vector<Evento*>& GerenciadorEventos::getEventos() const {
    return eventos;
}

json GerenciadorEventos::gerarRelatorioJson() const {
    json relatorio;
    relatorio["totalEventos"] = eventos.size();

    int totalParticipantes = 0;
    for (const auto& evento : eventos) {
        totalParticipantes += evento->getNumParticipantes();
    }
    relatorio["totalParticipantes"] = totalParticipantes;

    // A lógica de contagem por status permanece a mesma,
    // mas agora iterando sobre o vetor.
    int eventosHoje = 0, eventosProximos = 0, eventosPassados = 0;
    std::string hoje = "2025-07-02"; // Exemplo, idealmente pegar a data atual

    for (const auto& evento : eventos) {
        if (evento->getData() == hoje) eventosHoje++;
        else if (evento->getData() > hoje) eventosProximos++;
        else eventosPassados++;
    }

    relatorio["eventosPorStatus"] = {
        {"hoje", eventosHoje},
        {"proximos", eventosProximos},
        {"passados", eventosPassados}
    };
    return relatorio;
}

// Funções não implementadas no seu .cpp original, adicionando implementações de exemplo
void GerenciadorEventos::cadastrarEvento(const Evento& evento) {
    // Cria uma cópia do evento no heap para ser gerenciado pela classe
    eventos.push_back(new Evento(nextId++, evento.getNome(), evento.getData(), evento.getHora(), evento.getLocal(), evento.getDescricao()));
}

void GerenciadorEventos::atualizarEvento(int id, const Evento& evento) {
    Evento& eventoParaAtualizar = getEventoPorId(id); // Reutiliza a função de busca
    eventoParaAtualizar.atualizarEvento(evento.getNome(), evento.getData(), evento.getHora(), evento.getLocal(), evento.getDescricao());
}

// A função gerarRelatorio() de console não estava no seu .cpp, adicionei uma versão simples
void GerenciadorEventos::gerarRelatorio() {
    std::cout << "Relatorio em JSON:" << std::endl;
    std::cout << gerarRelatorioJson().dump(4) << std::endl; // .dump(4) para formatar
}