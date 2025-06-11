#include <iostream>
#include <string>
using namespace std;
class Participante {
    string nome;
    string email;

public:
    //construtor
    Participante(string n, string e) {
        nome = n;
        email = e;
    }
    //acessar informações do participante
    string getNome() { return nome; }
    string getEmail() { return email; }
};

class Evento {
    string nome;
    string data;
    string local;
    string descricao;
    Participante* participantes[100]; //arranjo de participantes
    int numParticipantes;

public:
    //construtor
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

    //informações do evento
    string getNome() { return nome; }
    string getData() { return data; }
    string getLocal() { return local; }
    string getDescricao() { return descricao; }
    int getNumParticipantes() { return numParticipantes; }
    Participante* getParticipante(int i) { return participantes[i]; }
};

class GerenciadorEventos {
    Evento* eventos[10]; // arranjo dos eventos
    int numEventos;

public:
    //construtor
    GerenciadorEventos() {
        numEventos = 0;
    }

    void cadastrarEvento() {
        if (numEventos < 10) {
            string nome, data, local, descricao;
            
            cout << "\n=== Cadastro de Evento ===" << endl;
            cout << "Nome do evento: ";
            getline(cin, nome);
            cout << "Data: ";
            getline(cin, data);
            cout << "Local: ";
            getline(cin, local);
            cout << "Descricao: ";
            getline(cin, descricao);

            eventos[numEventos] = new Evento(nome, data, local, descricao);
            numEventos++;
            cout << "Evento cadastrado com sucesso!" << endl;
        } else {
            cout << "Limite de eventos atingido!" << endl;
        }
    }

    //inscrição dos participantes
    void inscreverParticipante() {
        if (numEventos == 0) {
            cout << "Sem eventos cadastrados!" << endl;
            return;
        }

        string nomeEvento, nomeParticipante, email;
        cout << "\nNome do evento: ";
        getline(cin, nomeEvento);

        for (int i = 0; i < numEventos; i++) {
            if (eventos[i]->getNome() == nomeEvento) {
                cout << "Nome do participante: ";
                getline(cin, nomeParticipante);
                cout << "Email do participante: ";
                getline(cin, email);
                eventos[i]->adicionarParticipante(nomeParticipante, email);
                return;
            }
        }
        cout << "Evento não encontrado!" << endl;
    }

    //gerar relatorio
    void gerarRelatorio() {
        if (numEventos == 0) {
            cout << "Nao ha eventos cadastrados!" << endl;
            return;
        }

        string nomeEvento;
        cout << "\nNome do evento para relatorio: ";
        getline(cin, nomeEvento);

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
};

int main() {
    GerenciadorEventos gerenciador;
    int opcao;

    do {
        cout << "\n=== Sistema de Eventos ===" << endl;
        cout << "1. Cadastrar evento" << endl;
        cout << "2. Inscrever participante" << endl;
        cout << "3. Gerar relatorio" << endl;
        cout << "0. Sair" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        cin.ignore(); //limpar o buffer

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
            case 0:
                cout << "Saindo do sistema..." << endl;
                break;
            default:
                cout << "Opcao invalida!" << endl;
        }
    } while (opcao != 0);

    return 0;
} 