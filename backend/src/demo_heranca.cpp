#include <iostream>
#include <vector>
#include "Pessoa.h"
#include "Participante.h"
#include "Organizador.h"
#include "EventoBase.h"
#include "Evento.h"

// Demonstra polimorfismo: objetos derivados tratados como classe base
void exibirPessoas(const std::vector<Pessoa*>& pessoas) {
    std::cout << "\n=== Lista de Pessoas (Polimorfismo) ===" << std::endl;
    for (size_t i = 0; i < pessoas.size(); i++) {
        std::cout << i + 1 << ". " << pessoas[i]->getTipo() << ": " 
                  << pessoas[i]->toString() << std::endl;
    }
}

// Demonstra polimorfismo com eventos
void exibirEventos(const std::vector<EventoBase*>& eventos) {
    std::cout << "\n=== Lista de Eventos (Polimorfismo) ===" << std::endl;
    for (size_t i = 0; i < eventos.size(); i++) {
        std::cout << i + 1 << ". " << eventos[i]->getTipoEvento() << ": " 
                  << eventos[i]->toString() << std::endl;
        std::cout << "   Preço: R$ " << eventos[i]->calcularPreco() << std::endl;
        std::cout << "   Inscrições abertas: " << (eventos[i]->podeInscricao() ? "Sim" : "Não") << std::endl;
    }
}

int main() {
    std::cout << "=== Demonstração de Herança em C++ ===" << std::endl;
    
    // Criando objetos das classes derivadas
    std::cout << "\n1. Criando objetos..." << std::endl;
    
    Participante* p1 = new Participante("João Silva", "joao@email.com", "11999999999", "VIP");
    Participante* p2 = new Participante("Maria Santos", "maria@email.com", "11888888888");
    
    Organizador* o1 = new Organizador("Carlos Coordenador", "carlos@empresa.com", "11777777777", "Coordenador", "TI");
    Organizador* o2 = new Organizador("Ana Assistente", "ana@empresa.com", "11666666666");
    
    Evento* e1 = new Evento(1, "Workshop de C++", "2024-01-15", "14:00", "Auditório A", "Aprenda C++ do zero");
    Evento* e2 = new Evento(2, "Conferência de IA", "2024-01-20", "09:00", "Sala B", "Tendências em Inteligência Artificial");
    
    // Demonstrando polimorfismo com vetor de ponteiros para classe base
    std::cout << "\n2. Demonstrando herança..." << std::endl;
    
    std::vector<Pessoa*> pessoas;
    pessoas.push_back(p1);
    pessoas.push_back(p2);
    pessoas.push_back(o1);
    pessoas.push_back(o2);
    
    exibirPessoas(pessoas);
    
    // Testando métodos específicos das classes derivadas
    std::cout << "\n3. Funcionalidades específicas..." << std::endl;
    
    p1->confirmarPresenca();
    std::cout << "Participante VIP: " << p1->toString() << std::endl;
    
    o1->setNivelAcesso(4);
    std::cout << "Organizador: " << o1->toString() << std::endl;
    std::cout << "Pode editar evento: " << (o1->podeEditarEvento() ? "Sim" : "Não") << std::endl;
    std::cout << "Pode excluir evento: " << (o1->podeExcluirEvento() ? "Sim" : "Não") << std::endl;
    
    // Configurando e testando eventos
    std::cout << "\n4. Trabalhando com eventos..." << std::endl;
    
    e1->setPreco(50.0);
    e1->setCapacidadeMaxima(30);
    e1->setCategoria("Workshop");
    
    e2->setPreco(100.0);
    e2->setCapacidadeMaxima(100);
    e2->setCategoria("Conferência");
    
    e1->adicionarParticipante(p1);
    e1->adicionarParticipante(p2);
    e1->adicionarOrganizador(o1);
    
    e2->adicionarOrganizador(o2);
    
    std::vector<EventoBase*> eventos;
    eventos.push_back(e1);
    eventos.push_back(e2);
    
    exibirEventos(eventos);
    
    // Testando métodos virtuais
    std::cout << "\n5. Métodos virtuais..." << std::endl;
    
    for (auto evento : eventos) {
        std::cout << "\nDetalhes do evento:" << std::endl;
        evento->exibirDetalhes();
    }
    
    // Limpeza de memória
    std::cout << "\n6. Limpando memória..." << std::endl;
    
    delete p1;
    delete p2;
    delete o1;
    delete o2;
    
    delete e1;
    delete e2;
    
    std::cout << "\n=== Demonstração concluída! ===" << std::endl;
    
    return 0;
} 