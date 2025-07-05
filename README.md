# Sistema de Gerenciamento de Eventos

Backend em C++ e frontend em JavaScript vanilla para gerenciamento de eventos.

## **Demonstração Visual**

<div align="center">
  <img src="assets/foto1.png" alt="Dashboard Principal" width="600"/>
  <p><em>Dashboard principal com estatísticas em tempo real</em></p>
  
  <img src="assets/foto3.png" alt="Painel de Administração" width="600"/>
  <p><em>Painel completo de administração de eventos</em></p>
  
  <img src="assets/foto5.png" alt="Inscrição de Participantes" width="600"/>
  <p><em>Interface de inscrição para participantes</em></p>
  
  <img src="assets/foto2.png" alt="Gestão de Participantes" width="600"/>
  <p><em>Visualização e gestão de participantes</em></p>
  
  <img src="assets/foto4.png" alt="Edição de Participantes" width="600"/>
  <p><em>Modal para edição de dados de participantes</em></p>
</div>

## Funcionalidades

- Dashboard com estatísticas
- Gestão de eventos (criar, editar, excluir)
- Sistema de inscrições de participantes
- Interface responsiva
- API RESTful
- Armazenamento em arquivo de texto

## Tecnologias

- **Backend**: C++ (Winsock)
- **Frontend**: JavaScript vanilla
- **Estilização**: Bootstrap 5
- **Banco**: Arquivo de texto (`eventos.txt`)

## Estrutura do Projeto

```
EventManagement/
├── assets/
├── backend/
│   ├── src/
│   │   ├── include/
│   │   │   ├── json/
│   │   │   │   └── json.hpp
│   │   │   ├── ApiServer.h
│   │   │   ├── EventManager.h
│   │   │   ├── Event.h
│   │   │   ├── Participant.h
│   │   │   └── Person.h
│   │   ├── ApiServer.cpp
│   │   ├── EventManager.cpp
│   │   ├── Event.cpp
│   │   ├── Participant.cpp
│   │   ├── Person.cpp
│   │   └── main.cpp
│   └── build/
│       └── backend.exe
├── frontend/
│   ├── css/
│   │   └── styles.css
│   ├── js/
│   │   ├── components/
│   │   │   ├── Dashboard.js
│   │   │   ├── EventForm.js
│   │   │   ├── EventList.js
│   │   │   └── ParticipantForm.js
│   │   ├── services/
│   │   │   └── apiService.js
│   │   ├── utils/
│   │   │   ├── dateUtils.js
│   │   │   └── notificationUtils.js
│   │   ├── admin.js
│   │   ├── inscricoes.js
│   │   └── config.js
│   ├── admin.html
│   └── index.html
├── eventos.txt
├── run_backend.bat
└── README.md
```

## Como Executar

### Pré-requisitos
- Windows 10/11
- MinGW-w64 ou Visual Studio (compilador C++)
- Navegador web

### Execução
```bash
cd EventManagement
run_backend.bat
```

O script compila o código C++ e inicia o servidor na porta 8080.

### Acesso
- **Admin**: http://localhost:8080/frontend/admin.html
- **Inscrições**: http://localhost:8080/frontend/index.html

### Banco de Dados
- Arquivo: `eventos.txt` (criado automaticamente na raiz)
- Dados são preservados entre execuções

## API

### Eventos
- `GET /api/eventos` - Lista eventos
- `POST /api/eventos` - Cria evento
- `PUT /api/eventos/{id}` - Atualiza evento
- `DELETE /api/eventos/{id}` - Remove evento
- `GET /api/relatorio` - Relatório geral

### Participantes
- `GET /api/eventos/{id}/participantes` - Lista participantes
- `POST /api/eventos/{id}/participantes` - Adiciona participante
- `PUT /api/eventos/{id}/participantes/{id}` - Atualiza participante
- `DELETE /api/eventos/{id}/participantes/{id}` - Remove participante

## Funcionalidades Detalhadas

### Dashboard
- Estatísticas em tempo real
- Calendário visual de eventos
- Ações rápidas (criar, editar, excluir)
- Busca e filtros

### Gestão de Eventos
- Validação de dados obrigatórios
- Controle de datas e horários
- Informações detalhadas (local, descrição, capacidade)

### Participantes
- Controle de capacidade por evento
- Validação de email
- Dados de contato
- Edição de informações

## Testando

### Cenários de Teste
1. Criar um evento e verificar no dashboard
2. Inscrever participantes e validar capacidade
3. Editar dados de eventos e participantes
4. Excluir eventos
5. Testar validações com dados inválidos

## Problemas Comuns

| Problema | Solução |
|----------|---------|
| Erro de compilação | Verifique se o MinGW está no PATH |
| Porta 8080 ocupada | Feche outros serviços |
| Arquivo não encontrado | Execute o script de compilação |
| Erro de conexão | Verifique se o servidor está rodando |

### Debug
- Logs no terminal
- Console do navegador para frontend
- Arquivo `eventos.txt` na raiz

---

<div align="center">
  <p><strong>Desenvolvido com ❤️ pelos primos para facilitar a gestão de eventos</strong></p>
</div>
