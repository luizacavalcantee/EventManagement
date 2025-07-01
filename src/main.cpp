#include <iostream>
#include <string>
#include <fstream> // novo: para salvar e carregar arquivos
#include <sstream>
#include <httplib.h>
#include <nlohmann/json.hpp>
using namespace std;
using json = nlohmann::json;

class Entidade {
protected:
    string nome;
    string descricao;

public:
    Entidade(string n, string d) {
        nome = n;
        descricao = d;
    }

    string getNome() { return nome; }
    string getDescricao() { return descricao; }
    void setNome(string n) { nome = n; }
    void setDescricao(string d) { descricao = d; }
};

class Participante : public Entidade {
    string email;
    string telefone;
    bool presente;

public:
    Participante(string n, string e, string t = "") : Entidade(n, "") {
        email = e;
        telefone = t;
        presente = false;
    }

    string getEmail() { return email; }
    string getTelefone() { return telefone; }
    bool getPresenca() { return presente; }
    void setEmail(string e) { email = e; }
    void setTelefone(string t) { telefone = t; }
    void marcarPresenca() { presente = true; }

    string toString() {
        return nome + "," + email + "," + telefone + "," + (presente ? "PRESENTE" : "AUSENTE");
    }
};

class Evento : public Entidade {
    string data;
    string hora;
    string local;
    Participante* participantes[100];
    int numParticipantes;

public:
    Evento(string n, string d, string h, string l, string desc) : Entidade(n, desc) {
        data = d;
        hora = h;
        local = l;
        numParticipantes = 0;
    }

    void adicionarParticipante(string nome, string email, string telefone = "") {
        if (numParticipantes < 100) {
            participantes[numParticipantes] = new Participante(nome, email, telefone);
            numParticipantes++;
            cout << "Participante adicionado com sucesso!" << endl;
        } else {
            cout << "Evento lotado!" << endl;
        }
    }

    void marcarPresenca(string email) {
        for (int i = 0; i < numParticipantes; i++) {
            if (participantes[i]->getEmail() == email) {
                participantes[i]->marcarPresenca();
                cout << "Presenca marcada com sucesso!" << endl;
                return;
            }
        }
        cout << "Participante nao encontrado!" << endl;
    }

    string getData() { return data; }
    string getHora() { return hora; }
    string getLocal() { return local; }
    int getNumParticipantes() { return numParticipantes; }
    Participante* getParticipante(int i) { return participantes[i]; }

    void atualizarEvento(string n, string d, string h, string l, string desc) {
        setNome(n);
        data = d;
        hora = h;
        local = l;
        setDescricao(desc);
    }

    void salvarEvento(ofstream &arquivo) {
        arquivo << "EVENTO," << getNome() << "," << data << "," << hora << "," 
                << local << "," << getDescricao() << "," << numParticipantes << endl;
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
        carregarEventos(); // Carrega eventos do arquivo ao iniciar
    }

    ~GerenciadorEventos() {
        for (int i = 0; i < numEventos; i++) {
            delete eventos[i];
        }
    }

    void cadastrarEvento() {
        if (numEventos < 10) {
            string nome, data, hora, local, descricao;
            cin.ignore();
            cout << "\n=== Cadastro de Evento ===" << endl;
            cout << "Nome do evento: "; getline(cin, nome);
            cout << "Data: "; getline(cin, data);
            cout << "Hora: "; getline(cin, hora);
            cout << "Local: "; getline(cin, local);
            cout << "Descricao: "; getline(cin, descricao);

            eventos[numEventos] = new Evento(nome, data, hora, local, descricao);
            numEventos++;
            cout << "Evento cadastrado com sucesso!" << endl;
            
            salvarEventosEmArquivo();
        } else {
            cout << "Limite de eventos atingido!" << endl;
        }
    }

    void carregarEventos() {
        ifstream arquivo("eventos.txt");
        if (!arquivo.is_open()) {
            cout << "Nenhum arquivo de eventos encontrado." << endl;
            return;
        }

        string linha;
        while (getline(arquivo, linha) && numEventos < 10) {
            stringstream ss(linha);
            string tipo, nome, data, hora, local, descricao, numPart;
            
            getline(ss, tipo, ',');
            if (tipo == "EVENTO") {
                getline(ss, nome, ',');
                getline(ss, data, ',');
                getline(ss, hora, ',');
                getline(ss, local, ',');
                getline(ss, descricao, ',');
                getline(ss, numPart, ',');
                
                eventos[numEventos] = new Evento(nome, data, hora, local, descricao);
                numEventos++;
            }
        }
        arquivo.close();
    }

    void inscreverParticipante() {
        if (numEventos == 0) {
            cout << "Sem eventos cadastrados!" << endl;
            return;
        }

        string nomeEvento, nomeParticipante, email, telefone;
        cin.ignore();
        cout << "\nNome do evento: "; getline(cin, nomeEvento);

        for (int i = 0; i < numEventos; i++) {
            if (eventos[i]->getNome() == nomeEvento) {
                cout << "Nome do participante: "; getline(cin, nomeParticipante);
                cout << "Email do participante: "; getline(cin, email);
                cout << "Telefone (opcional): "; getline(cin, telefone);
                eventos[i]->adicionarParticipante(nomeParticipante, email, telefone);
                salvarEventosEmArquivo();
                return;
            }
        }
        cout << "Evento nao encontrado!" << endl;
    }

    void marcarPresenca() {
        if (numEventos == 0) {
            cout << "Sem eventos cadastrados!" << endl;
            return;
        }

        string nomeEvento, email;
        cin.ignore();
        cout << "\nNome do evento: "; getline(cin, nomeEvento);
        cout << "Email do participante: "; getline(cin, email);

        for (int i = 0; i < numEventos; i++) {
            if (eventos[i]->getNome() == nomeEvento) {
                eventos[i]->marcarPresenca(email);
                salvarEventosEmArquivo();
                return;
            }
        }
        cout << "Evento nao encontrado!" << endl;
    }

    void gerarRelatorio() {
        if (numEventos == 0) {
            cout << "Nao tem eventos cadastrados!" << endl;
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
                cout << "Hora: " << eventos[i]->getHora() << endl;
                cout << "Local: " << eventos[i]->getLocal() << endl;
                cout << "Descricao: " << eventos[i]->getDescricao() << endl;
                cout << "\nLista de Participantes:" << endl;
                cout << "------------------------" << endl;
                for (int j = 0; j < eventos[i]->getNumParticipantes(); j++) {
                    Participante* p = eventos[i]->getParticipante(j);
                    cout << "Nome: " << p->getNome() << endl;
                    cout << "Email: " << p->getEmail() << endl;
                    if (!p->getTelefone().empty()) {
                        cout << "Telefone: " << p->getTelefone() << endl;
                    }
                    cout << "Presenca: " << (p->getPresenca() ? "Presente" : "Ausente") << endl;
                    cout << "------------------------" << endl;
                }
                return;
            }
        }
        cout << "Evento nao encontrado!" << endl;
    }

    // novo: atualizar evento
    void atualizarEvento() {
        string nomeAtual, novoNome, novaData, novaHora, novoLocal, novaDescricao;
        cin.ignore();
        cout << "\nNome do evento que deseja atualizar: "; getline(cin, nomeAtual);

        for (int i = 0; i < numEventos; i++) {
            if (eventos[i]->getNome() == nomeAtual) {
                cout << "Novo nome: "; getline(cin, novoNome);
                cout << "Nova data: "; getline(cin, novaData);
                cout << "Nova hora: "; getline(cin, novaHora);
                cout << "Novo local: "; getline(cin, novoLocal);
                cout << "Nova descricao: "; getline(cin, novaDescricao);
                eventos[i]->atualizarEvento(novoNome, novaData, novaHora, novoLocal, novaDescricao);
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

    void listarEventos() {
        if (numEventos == 0) {
            cout << "Nao tem eventos cadastrados!" << endl;
            return;
        }

        cout << "\n=== Lista de Eventos ===" << endl;
        for (int i = 0; i < numEventos; i++) {
            cout << "\nEvento " << (i + 1) << ":" << endl;
            cout << "Nome: " << eventos[i]->getNome() << endl;
            cout << "Data: " << eventos[i]->getData() << endl;
            cout << "Hora: " << eventos[i]->getHora() << endl;
            cout << "Local: " << eventos[i]->getLocal() << endl;
            cout << "Descrição: " << eventos[i]->getDescricao() << endl;
            cout << "Numero de participantes: " << eventos[i]->getNumParticipantes() << endl;
            cout << "------------------------" << endl;
        }
    }

    // New method to get all events as JSON
    string getEventosJson() {
        json eventosJson = json::array();
        for (int i = 0; i < numEventos; i++) {
            json eventoJson;
            eventoJson["nome"] = eventos[i]->getNome();
            eventoJson["data"] = eventos[i]->getData();
            eventoJson["hora"] = eventos[i]->getHora();
            eventoJson["local"] = eventos[i]->getLocal();
            eventoJson["descricao"] = eventos[i]->getDescricao();
            eventoJson["numParticipantes"] = eventos[i]->getNumParticipantes();
            
            json participantesJson = json::array();
            for (int j = 0; j < eventos[i]->getNumParticipantes(); j++) {
                Participante* p = eventos[i]->getParticipante(j);
                json participanteJson;
                participanteJson["nome"] = p->getNome();
                participanteJson["email"] = p->getEmail();
                participanteJson["telefone"] = p->getTelefone();
                participanteJson["presente"] = p->getPresenca();
                participantesJson.push_back(participanteJson);
            }
            eventoJson["participantes"] = participantesJson;
            eventosJson.push_back(eventoJson);
        }
        return eventosJson.dump();
    }

    // New method to add event from JSON
    bool adicionarEventoJson(const json& eventoJson) {
        if (numEventos < 10) {
            string nome = eventoJson["nome"];
            string data = eventoJson["data"];
            string hora = eventoJson["hora"];
            string local = eventoJson["local"];
            string descricao = eventoJson["descricao"];

            eventos[numEventos] = new Evento(nome, data, hora, local, descricao);
            numEventos++;
            salvarEventosEmArquivo();
            return true;
        }
        return false;
    }

    // New method to add participant from JSON
    bool adicionarParticipanteJson(const string& nomeEvento, const json& participanteJson) {
        for (int i = 0; i < numEventos; i++) {
            if (eventos[i]->getNome() == nomeEvento) {
                string nome = participanteJson["nome"];
                string email = participanteJson["email"];
                string telefone = participanteJson["telefone"];
                eventos[i]->adicionarParticipante(nome, email, telefone);
                salvarEventosEmArquivo();
                return true;
            }
        }
        return false;
    }
};

int main() {
    GerenciadorEventos gerenciador;
    httplib::Server svr;

    // Enable CORS
    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });

    // Get all events
    svr.Get("/api/eventos", [&](const httplib::Request&, httplib::Response& res) {
        res.set_content(gerenciador.getEventosJson(), "application/json");
    });

    // Add new event
    svr.Post("/api/eventos", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json eventoJson = json::parse(req.body);
            if (gerenciador.adicionarEventoJson(eventoJson)) {
                res.status = 201;
                res.set_content("{\"message\": \"Evento criado com sucesso\"}", "application/json");
            } else {
                res.status = 400;
                res.set_content("{\"error\": \"Não foi possível criar o evento\"}", "application/json");
            }
        } catch (const exception& e) {
            res.status = 400;
            res.set_content("{\"error\": \"Dados inválidos\"}", "application/json");
        }
    });

    // Add participant to event
    svr.Post("/api/eventos/:nome/participantes", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            string nomeEvento = req.path_params.at("nome");
            json participanteJson = json::parse(req.body);
            if (gerenciador.adicionarParticipanteJson(nomeEvento, participanteJson)) {
                res.status = 201;
                res.set_content("{\"message\": \"Participante adicionado com sucesso\"}", "application/json");
            } else {
                res.status = 400;
                res.set_content("{\"error\": \"Não foi possível adicionar o participante\"}", "application/json");
            }
        } catch (const exception& e) {
            res.status = 400;
            res.set_content("{\"error\": \"Dados inválidos\"}", "application/json");
        }
    });

    // Mark presence
    svr.Post("/api/eventos/:nome/presenca", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            string nomeEvento = req.path_params.at("nome");
            json data = json::parse(req.body);
            string email = data["email"];
            
            for (int i = 0; i < gerenciador.numEventos; i++) {
                if (gerenciador.eventos[i]->getNome() == nomeEvento) {
                    gerenciador.eventos[i]->marcarPresenca(email);
                    gerenciador.salvarEventosEmArquivo();
                    res.status = 200;
                    res.set_content("{\"message\": \"Presença marcada com sucesso\"}", "application/json");
                    return;
                }
            }
            res.status = 404;
            res.set_content("{\"error\": \"Evento não encontrado\"}", "application/json");
        } catch (const exception& e) {
            res.status = 400;
            res.set_content("{\"error\": \"Dados inválidos\"}", "application/json");
        }
    });

    // Serve static files
    svr.set_mount_point("/", "./public");

    cout << "Servidor rodando em http://localhost:8080" << endl;
    svr.listen("localhost", 8080);

    return 0;
}