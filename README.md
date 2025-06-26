# 🎉 Sistema de Gerenciamento de Eventos

Um sistema completo para gerenciar eventos, participantes e relatórios com interface web moderna e backend em C++.

## 🚀 **Tecnologias**

- **Backend**: C++ puro com Winsock (Windows)
- **Frontend**: JavaScript vanilla + Bootstrap
- **Arquitetura**: Modular e responsiva
- **Persistência**: Arquivo texto (eventos.txt)

## 📋 **Funcionalidades**

### **Dashboard**
- 📊 Estatísticas em tempo real
- 📅 Eventos de hoje e próximos
- 👥 Total de participantes

### **Gerenciamento de Eventos**
- ➕ Criar novos eventos
- ✏️ Editar eventos existentes
- 🗑️ Excluir eventos
- 👁️ Visualizar detalhes

### **Gerenciamento de Participantes**
- 📝 Inscrever participantes
- 📋 Listar participantes
- 🔢 Controle de capacidade

### **Relatórios**
- 📄 Gerar relatórios em texto
- 💾 Download automático
- 📈 Estatísticas detalhadas

## 🛠️ **Como Rodar**

### **Pré-requisitos**
- Compilador C++ (MinGW, Visual Studio, ou GCC)
- Navegador web moderno
- Windows (para Winsock)

### **Instalação e Execução**
```bash
# Opção 1: Script automático (recomendado)
run_backend.bat

# Opção 2: Manual
cd backend
compile_simple.bat
simple_cpp_server.exe

# 3. Acessar interface
# http://localhost:8080/frontend/
```

### **Compilação Manual**
```bash
# Windows com MinGW
cd backend
g++ -std=c++17 -o simple_cpp_server.exe simple_cpp_server.cpp -lws2_32

# Linux/macOS (sem Winsock)
cd backend
g++ -std=c++17 -o simple_cpp_server simple_cpp_server.cpp
```

## 📁 **Estrutura do Projeto**

```
EventManagement/
├── backend/              # Backend C++ organizado
│   ├── simple_cpp_server.cpp  # Servidor principal
│   ├── simple_cpp_server.exe  # Executável
│   ├── Evento.h/.cpp         # Classe Evento
│   ├── Participante.h/.cpp   # Classe Participante
│   ├── GerenciadorEventos.h/.cpp # Gerenciador principal
│   ├── eventos.txt           # Dados persistentes
│   └── compile_simple.bat    # Script de compilação
├── frontend/              # Interface web
│   ├── index.html         # Página principal
│   ├── styles.css         # Estilos
│   ├── app.js            # Aplicação principal
│   └── js/               # Componentes modulares
│       ├── config.js     # Configurações
│       ├── services/     # Serviços da API
│       ├── components/   # Componentes da interface
│       └── utils/        # Utilitários
├── run_backend.bat       # Script principal de execução
└── README.md            # Este arquivo
```

## 🌐 **URLs da API**

| Endpoint | Método | Descrição |
|----------|--------|-----------|
| `/health` | GET | Status do servidor |
| `/api/eventos` | GET | Listar todos os eventos |
| `/api/eventos` | POST | Criar novo evento |
| `/api/relatorio` | GET | Gerar relatório |

## 📊 **Dados de Exemplo**

O sistema vem com eventos de exemplo carregados do arquivo `backend/eventos.txt`:
- Workshop de JavaScript
- Palestra sobre React
- Meetup de Desenvolvedores

## 🔧 **Configuração**

### **Porta do Servidor**
Edite `backend/simple_cpp_server.cpp`:
```cpp
serverAddr.sin_port = htons(8080); // Mude para a porta desejada
```

### **URL da API**
Edite `frontend/js/config.js`:
```javascript
const API_BASE_URL = 'http://localhost:8080'; // Mude se necessário
```

## 🚀 **Comandos Úteis**

| Comando | Descrição |
|---------|-----------|
| `run_backend.bat` | Compilar e executar (recomendado) |
| `backend/compile_simple.bat` | Compilar servidor C++ |
| `backend/simple_cpp_server.exe` | Executar servidor |
| `test_system.bat` | Testar sistema completo |

## 📞 **Suporte**

Para mais detalhes sobre a integração C++, consulte o arquivo `INTEGRACAO_C++.md`.

---

**🎉 Sistema pronto para uso com backend C++ puro organizado!**
