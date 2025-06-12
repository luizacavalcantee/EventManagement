#include <iostream>
#include <string>
#include <fstream> // novo: para salvar e carregar arquivos
using namespace std;

class Participante {
    string nome;
    string email;

public:
    Participante(string n, string e) {
        nome = n;
        email = e;
    }

    string getNome() { return nome; }
    string getEmail() { return email; }

    // novo: salvar participante em arquivo
    string toString() {
        return nome + "," + email;
    }
};

class Evento {
    string nome;
    string data;
    string local;
    string descricao;
    Participante* participantes[100];
    int numParticipantes;

public:
    Evento(string n, string d, string l, string desc) {
        nome = n;
        data = d;
        local = l;
        descricao = desc;
        numParticipantes = 0;
    }

    void adicionarParticipante(string nome, string email) {
        if (numParticipantes < 100) {
            participantes[numParticipantes] = new Participante(nome, email);
            numParticipantes++;
            cout << "Participante adicionado com sucesso!" << endl;
        } else {
            cout << "Evento lotado!" << endl;
        }
    }

    string getNome() { return nome; }
    string getData() { return data; }
    string getLocal() { return local; }
    string getDescricao() { return descricao; }
    int getNumParticipantes() { return numParticipantes; }
    Participante* getParticipante(int i) { return participantes[i]; }

    // novo: atualizar dados do evento
    void atualizarEvento(string n, string d, string l, string desc) {
        nome = n;
        data = d;
        local = l;
        descricao = desc;
    }

    // novo: salvar evento e participantes em arquivo
    void salvarEvento(ofstream &arquivo) {
        arquivo << nome << "," << data << "," << local << "," << descricao << endl;
        for (int i = 0; i < numParticipantes; i++) {
            arquivo << "PARTICIPANTE," << participantes[i]->toString() << endl;
        }
    }
};

class GerenciadorEventos {
    Evento* eventos[10];
    int numEventos;

public:
    GerenciadorEventos() {
        numEventos = 0;
    }

    void cadastrarEvento() {
        if (numEventos < 10) {
            string nome, data, local, descricao;
            cin.ignore();
            cout << "\n=== Cadastro de Evento ===" << endl;
            cout << "Nome do evento: "; getline(cin, nome);
            cout << "Data: "; getline(cin, data);
            cout << "Local: "; getline(cin, local);
            cout << "Descricao: "; getline(cin, descricao);

            eventos[numEventos] = new Evento(nome, data, local, descricao);
            numEventos++;
            cout << "Evento cadastrado com sucesso!" << endl;
        } else {
            cout << "Limite de eventos atingido!" << endl;
        }
    }

    void inscreverParticipante() {
        if (numEventos == 0) {
            cout << "Sem eventos cadastrados!" << endl;
            return;
        }

        string nomeEvento, nomeParticipante, email;
        cin.ignore();
        cout << "\nNome do evento: "; getline(cin, nomeEvento);

        for (int i = 0; i < numEventos; i++) {
            if (eventos[i]->getNome() == nomeEvento) {
                cout << "Nome do participante: "; getline(cin, nomeParticipante);
                cout << "Email do participante: "; getline(cin, email);
                eventos[i]->adicionarParticipante(nomeParticipante, email);
                return;
            }
        }
        cout << "Evento nao encontrado!" << endl;
    }

    void gerarRelatorio() {
        if (numEventos == 0) {
            cout << "Nao ha eventos cadastrados!" << endl;
            return;
        }

        string nomeEvento;
        cin.ignore();
        cout << "\nNome do evento para relatorio: "; getline(cin, nomeEvento);

        for (int i = 0; i < numEventos; i++) {
            if (eventos[i]->getNome() == nomeEvento) {
                cout << "\n=== Relatorio do Evento ===" << endl;
                cout << "Nome: " << eventos[i]->getNome() << endl;
                cout << "Data: " << eventos[i]->getData() << endl;
                cout << "Local: " << eventos[i]->getLocal() << endl;
                cout << "Descricao: " << eventos[i]->getDescricao() << endl;
                cout << "\nParticipantes:" << endl;
                cout << "------------------------" << endl;
                for (int j = 0; j < eventos[i]->getNumParticipantes(); j++) {
                    Participante* p = eventos[i]->getParticipante(j);
                    cout << "Nome: " << p->getNome() << endl;
                    cout << "Email: " << p->getEmail() << endl;
                    cout << "------------------------" << endl;
                }
                return;
            }
        }
        cout << "Evento nao encontrado!" << endl;
    }

    // novo: atualizar evento
    void atualizarEvento() {
        string nomeAtual, novoNome, novaData, novoLocal, novaDescricao;
        cin.ignore();
        cout << "\nNome do evento que deseja atualizar: "; getline(cin, nomeAtual);

        for (int i = 0; i < numEventos; i++) {
            if (eventos[i]->getNome() == nomeAtual) {
                cout << "Novo nome: "; getline(cin, novoNome);
                cout << "Nova data: "; getline(cin, novaData);
                cout << "Novo local: "; getline(cin, novoLocal);
                cout << "Nova descricao: "; getline(cin, novaDescricao);
                eventos[i]->atualizarEvento(novoNome, novaData, novoLocal, novaDescricao);
                cout << "Evento atualizado com sucesso!" << endl;
                return;
            }
        }
        cout << "Evento nao encontrado!" << endl;
    }

    // novo: excluir evento
    void deletarEvento() {
        string nomeEvento;
        cin.ignore();
        cout << "\nNome do evento que deseja deletar: "; getline(cin, nomeEvento);

        for (int i = 0; i < numEventos; i++) {
            if (eventos[i]->getNome() == nomeEvento) {
                delete eventos[i];
                for (int j = i; j < numEventos - 1; j++) {
                    eventos[j] = eventos[j + 1];
                }
                numEventos--;
                cout << "Evento deletado com sucesso!" << endl;
                return;
            }
        }
        cout << "Evento nao encontrado!" << endl;
    }

    // novo: salvar tudo em arquivo
    void salvarEventosEmArquivo() {
        ofstream arquivo("eventos.txt");
        for (int i = 0; i < numEventos; i++) {
            eventos[i]->salvarEvento(arquivo);
        }
        arquivo.close();
        cout << "Eventos salvos no arquivo com sucesso!" << endl;
    }
};

int main() {
    GerenciadorEventos gerenciador;
    int opcao;

    do {
        cout << "\n=== Sistema de Eventos ===" << endl;
        cout << "1. Cadastrar evento" << endl;
        cout << "2. Inscrever participante" << endl;
        cout << "3. Gerar relatorio" << endl;
        cout << "4. Atualizar evento" << endl; // novo
        cout << "5. Deletar evento" << endl;   // novo
        cout << "6. Salvar eventos em arquivo" << endl; // novo
        cout << "0. Sair" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;

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
            case 0:
                cout << "Saindo do sistema..." << endl;
                break;
            default:
                cout << "Opcao invalida!" << endl;
        }
    } while (opcao != 0);

    return 0;
}
