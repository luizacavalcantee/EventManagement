#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct Evento {
    int id;
    string nome;
    string data;
    string hora;
    string local;
    string descricao;
    int numParticipantes;
    
    Evento(int i, string n, string d, string h, string l, string desc) 
        : id(i), nome(n), data(d), hora(h), local(l), descricao(desc), numParticipantes(0) {}
};

struct Participante {
    string nome;
    string email;
    string contato;
    
    Participante(string n, string e, string c) : nome(n), email(e), contato(c) {}
};

class GerenciadorEventos {
private:
    vector<Evento> eventos;
    map<int, vector<Participante>> participantes;
    int nextId;
    
public:
    GerenciadorEventos() : nextId(1) {
        carregarEventos();
    }
    
    void carregarEventos() {
        ifstream arquivo("eventos.txt");
        if (!arquivo.is_open()) {
            cout << "Criando arquivo de eventos..." << endl;
            return;
        }
        
        string linha;
        int eventoAtual = -1;
        
        while (getline(arquivo, linha)) {
            if (linha.find("PARTICIPANTE") == 0) {
                if (eventoAtual != -1) {
                    stringstream ss(linha);
                    string tipo, nome, email, contato;
                    getline(ss, tipo, ',');
                    getline(ss, nome, ',');
                    getline(ss, email, ',');
                    getline(ss, contato, ',');
                    participantes[eventoAtual].push_back(Participante(nome, email, contato));
                }
            } else {
                stringstream ss(linha);
                string idStr, nome, data, hora, local, descricao;
                getline(ss, idStr, ',');
                getline(ss, nome, ',');
                getline(ss, data, ',');
                getline(ss, hora, ',');
                getline(ss, local, ',');
                getline(ss, descricao, ',');
                
                int id = stoi(idStr);
                eventos.push_back(Evento(id, nome, data, hora, local, descricao));
                eventoAtual = id;
                if (id >= nextId) nextId = id + 1;
            }
        }
        arquivo.close();
    }
    
    void salvarEventos() {
        ofstream arquivo("eventos.txt");
        for (const auto& evento : eventos) {
            arquivo << evento.id << "," << evento.nome << "," << evento.data << "," 
                   << evento.hora << "," << evento.local << "," << evento.descricao << endl;
            
            for (const auto& participante : participantes[evento.id]) {
                arquivo << "PARTICIPANTE," << participante.nome << "," 
                       << participante.email << "," << participante.contato << endl;
            }
        }
        arquivo.close();
    }
    
    string listarEventos() {
        string json = "[";
        for (size_t i = 0; i < eventos.size(); i++) {
            if (i > 0) json += ",";
            json += "{";
            json += "\"id\":" + to_string(eventos[i].id) + ",";
            json += "\"nome\":\"" + eventos[i].nome + "\",";
            json += "\"data\":\"" + eventos[i].data + "\",";
            json += "\"hora\":\"" + eventos[i].hora + "\",";
            json += "\"local\":\"" + eventos[i].local + "\",";
            json += "\"descricao\":\"" + eventos[i].descricao + "\",";
            json += "\"numParticipantes\":" + to_string(participantes[eventos[i].id].size());
            json += "}";
        }
        json += "]";
        return json;
    }
    
    string criarEvento(const string& nome, const string& data, const string& hora, 
                      const string& local, const string& descricao) {
        Evento evento(nextId++, nome, data, hora, local, descricao);
        eventos.push_back(evento);
        participantes[evento.id] = vector<Participante>();
        salvarEventos();
        
        string json = "{";
        json += "\"id\":" + to_string(evento.id) + ",";
        json += "\"nome\":\"" + evento.nome + "\",";
        json += "\"data\":\"" + evento.data + "\",";
        json += "\"hora\":\"" + evento.hora + "\",";
        json += "\"local\":\"" + evento.local + "\",";
        json += "\"descricao\":\"" + evento.descricao + "\",";
        json += "\"numParticipantes\":0";
        json += "}";
        return json;
    }
    
    string getRelatorio() {
        int totalEventos = eventos.size();
        int totalParticipantes = 0;
        int eventosProximos = 0;
        int eventosHoje = 0;

        for (const auto& par : participantes) {
            totalParticipantes += par.second.size();
        }

        eventosProximos = totalEventos; 

        string json = "{";
        json += "\"totalEventos\":" + to_string(totalEventos) + ",";
        json += "\"totalParticipantes\":" + to_string(totalParticipantes) + ",";
        json += "\"eventosProximos\":" + to_string(eventosProximos) + ",";
        json += "\"eventosHoje\":" + to_string(eventosHoje);
        json += "}";
        return json;
    }

    string inscreverParticipante(int eventoId, const string& nome, const string& email, const string& contato) {
        if (participantes.find(eventoId) != participantes.end()) {
            participantes[eventoId].push_back(Participante(nome, email, contato));
            salvarEventos();
            return "{\"status\":\"success\",\"message\":\"Participante inscrito com sucesso\"}";
        } else {
            return "{\"status\":\"error\",\"message\":\"Evento nao encontrado\"}";
        }
    }

    string listarParticipantes(int eventoId) {
        string json = "[";
        if (participantes.find(eventoId) != participantes.end()) {
            for (const auto& participante : participantes[eventoId]) {
                if (json.length() > 1) json += ",";
                json += "{";
                json += "\"nome\":\"" + participante.nome + "\",";
                json += "\"email\":\"" + participante.email + "\",";
                json += "\"contato\":\"" + participante.contato + "\"";
                json += "}";
            }
        }
        json += "]";
        return json;
    }

    bool atualizarEvento(int eventoId, const string& nome, const string& data, const string& hora, const string& local, const string& descricao) {
        for (auto& evento : eventos) {
            if (evento.id == eventoId) {
                evento.nome = nome;
                evento.data = data;
                evento.hora = hora;
                evento.local = local;
                evento.descricao = descricao;
                salvarEventos();
                return true;
            }
        }
        return false;
    }
};

string urlDecode(const string& str) {
    string result;
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '%' && i + 2 < str.length()) {
            int val;
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &val);
            result += static_cast<char>(val);
            i += 2;
        } else if (str[i] == '+') {
            result += ' ';
        } else {
            result += str[i];
        }
    }
    return result;
}

string parseJsonField(const string& json, const string& field) {
    size_t pos = json.find("\"" + field + "\":\"");
    if (pos == string::npos) return "";
    pos += field.length() + 4;
    size_t end = json.find("\"", pos);
    if (end == string::npos) return "";
    return json.substr(pos, end - pos);
}

string getRequestPath(const string& request) {
    size_t startPos = request.find(" ") + 1;
    size_t endPos = request.find(" ", startPos);
    if (startPos == string::npos || endPos == string::npos) {
        return "";
    }
    return request.substr(startPos, endPos - startPos);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Erro ao inicializar Winsock" << endl;
        return 1;
    }
    
    GerenciadorEventos gerenciador;
    
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cout << "Erro ao criar socket" << endl;
        WSACleanup();
        return 1;
    }
    
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);
    
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Erro ao fazer bind" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        cout << "Erro ao escutar" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    
    cout << "🚀 Servidor C++ iniciado na porta 8080..." << endl;
    cout << "📱 Interface disponível em: http://localhost:8080/frontend/" << endl;
    cout << "🔧 Para parar o servidor, pressione Ctrl+C" << endl;
    cout << "--------------------------------------------------" << endl;
    
    while (true) {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            cout << "Erro ao aceitar conexão" << endl;
            continue;
        }
        
        char buffer[4096];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            string request(buffer);
            
            cout << "==================================================" << endl << flush;
            cout << ">>> REQUISICAO RECEBIDA:" << endl << flush;
            cout << request.substr(0, 400) << "..." << endl << flush;

            string method = request.substr(0, request.find(" "));
            string path = getRequestPath(request);
            
            cout << "--> METODO: [" << method << "]" << endl << flush;
            cout << "--> CAMINHO: [" << path << "]" << endl << flush;

            string response;
            string content;
            string status = "200 OK";

            cout << "--> ROTEAMENTO: " << flush;
            if (method == "OPTIONS") {
                cout << "OPTIONS (CORS Preflight)" << endl << flush;
                status = "204 No Content";
            }
            else if (path == "/health") {
                cout << "Rota /health" << endl << flush;
                content = "{\"status\":\"ok\",\"message\":\"Servidor funcionando!\"}";
            }
            else if (path == "/api/eventos" && method == "GET") {
                cout << "Rota /api/eventos (GET)" << endl << flush;
                content = gerenciador.listarEventos();
            }
            else if ((path == "/api/eventos" || path == "/api/eventos/") && method == "POST") {
                cout << "Rota /api/eventos (POST)" << endl << flush;
                size_t bodyPos = request.find("\r\n\r\n");
                if (bodyPos != string::npos) {
                    string body = request.substr(bodyPos + 4);
                    string nome = parseJsonField(body, "nome");
                    string data = parseJsonField(body, "data");
                    string hora = parseJsonField(body, "hora");
                    string local = parseJsonField(body, "local");
                    string descricao = parseJsonField(body, "descricao");
                    content = gerenciador.criarEvento(nome, data, hora, local, descricao);
                    status  = "201 Created";
                } else {
                    status = "400 Bad Request";
                    content = "{\"error\":\"Corpo da requisicao ausente\"}";
                }
            }
            else if (path == "/api/relatorio" && method == "GET") {
                cout << "Rota /api/relatorio (GET)" << endl << flush;
                content = gerenciador.getRelatorio();
            }
            else if (path.find("/api/eventos/") == 0 && path.find("/participantes") != string::npos && method == "POST") {
                cout << "Rota /api/eventos/{id}/participantes (POST)" << endl << flush;
                size_t startPos = path.find("/api/eventos/") + 13;
                size_t endPos = path.find("/participantes");
                if (startPos != string::npos && endPos != string::npos) {
                    string idStr = path.substr(startPos, endPos - startPos);
                    int eventoId = stoi(idStr);

                    size_t bodyPos = request.find("\r\n\r\n");
                    if (bodyPos != string::npos) {
                        string body = request.substr(bodyPos + 4);
                        string nome = parseJsonField(body, "nome");
                        string email = parseJsonField(body, "email");
                        string contato = parseJsonField(body, "contato");

                        if (!nome.empty() && !email.empty()) {
                            content = gerenciador.inscreverParticipante(eventoId, nome, email, contato);
                        } else {
                            status = "400 Bad Request";
                            content = "{\"error\":\"Nome e email sao obrigatorios\"}";
                        }
                    } else {
                        status = "400 Bad Request";
                        content = "{\"error\":\"Corpo da requisicao ausente\"}";
                    }
                } else {
                    status = "400 Bad Request";
                    content = "{\"error\":\"ID do evento invalido\"}";
                }
            }
            else if (path.find("/api/eventos/") == 0 && path.find("/participantes") != string::npos && method == "GET") {
                cout << "Rota /api/eventos/{id}/participantes (GET)" << endl << flush;
                size_t startPos = path.find("/api/eventos/") + 13;
                size_t endPos = path.find("/participantes");
                if (startPos != string::npos && endPos != string::npos) {
                    string idStr = path.substr(startPos, endPos - startPos);
                    int eventoId = stoi(idStr);

                    content = gerenciador.listarParticipantes(eventoId);
                } else {
                    status = "400 Bad Request";
                    content = "{\"error\":\"ID do evento invalido\"}";
                }
            }
            else if (path.find("/api/eventos/") == 0 && method == "PUT") {
                cout << "Rota /api/eventos/{id} (PUT)" << endl << flush;
                string idStr = path.substr(13);
                int eventoId = stoi(idStr);

                size_t bodyPos = request.find("\r\n\r\n");
                if (bodyPos != string::npos) {
                    string body = request.substr(bodyPos + 4);
                    string nome = parseJsonField(body, "nome");
                    string data = parseJsonField(body, "data");
                    string hora = parseJsonField(body, "hora");
                    string local = parseJsonField(body, "local");
                    string descricao = parseJsonField(body, "descricao");

                    bool atualizado = gerenciador.atualizarEvento(eventoId, nome, data, hora, local, descricao);
                    if (atualizado) {
                        content = "{\"status\":\"success\",\"message\":\"Evento atualizado com sucesso\"}";
                        status = "200 OK";
                    } else {
                        status = "404 Not Found";
                        content = "{\"error\":\"Evento nao encontrado\"}";
                    }
                } else {
                    status = "400 Bad Request";
                    content = "{\"error\":\"Corpo da requisicao ausente\"}";
                }
            }
            else if (path.rfind("/frontend/", 0) == 0 || path == "/") {
                cout << "Rota de Arquivo Estatico" << endl << flush;
                string filePath = (path == "/") ? "frontend/index.html" : path.substr(1);
                size_t queryPos = filePath.find("?");
                if (queryPos != string::npos) {
                    filePath = filePath.substr(0, queryPos);
                }
                if (!filePath.empty() && filePath.back() == '/') {
                    filePath += "index.html";
                }
                ifstream file(filePath, ios::binary);
                if (file) {
                    stringstream fileBuffer;
                    fileBuffer << file.rdbuf();
                    content = fileBuffer.str();

                    string contentType = "text/html";
                    if (filePath.find(".css") != string::npos) contentType = "text/css";
                    if (filePath.find(".js") != string::npos) contentType = "application/javascript";

                    response = "HTTP/1.1 200 OK\r\n";
                    response += "Content-Type: " + contentType + "\r\n";
                } else {
                    status = "404 Not Found";
                    content = "<h1>404 Not Found</h1><p>O arquivo solicitado '" + filePath + "' nao foi encontrado.</p>";
                    response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n";
                }
                response += "Access-Control-Allow-Origin: *\r\n";
                response += "Content-Length: " + to_string(content.length()) + "\r\n\r\n";
                response += content;
                send(clientSocket, response.c_str(), response.length(), 0);
                closesocket(clientSocket);
                continue;
            }
            else {
                cout << "404 Nao Encontrado" << endl << flush;
                status = "404 Not Found";
                content = "{\"error\":\"Rota nao encontrada\"}";
            }

            cout << "--> CONTEUDO (parcial): " << content.substr(0, 150) << "..." << endl << flush;
            cout << "==================================================" << endl << endl << flush;

            response = "HTTP/1.1 " + status + "\r\n";
            response += "Content-Type: application/json\r\n";
            response += "Access-Control-Allow-Origin: *\r\n";
            response += "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n";
            response += "Access-Control-Allow-Headers: Content-Type\r\n";
            response += "Content-Length: " + to_string(content.length()) + "\r\n";
            response += "\r\n";
            response += content;
            
            send(clientSocket, response.c_str(), response.length(), 0);
        }
        
        closesocket(clientSocket);
    }
    
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
