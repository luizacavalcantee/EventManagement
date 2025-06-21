# 🎉 Sistema de Gerenciamento de Eventos

Um sistema completo para gerenciar eventos, participantes e relatórios com interface web moderna.

## 🚀 **Tecnologias**

- **Backend**: Node.js + Express.js
- **Frontend**: JavaScript vanilla + Bootstrap
- **Arquitetura**: Modular e responsiva

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
- Node.js 16.x ou superior
- Navegador web moderno

### **Instalação**
```bash
# 1. Instalar dependências
npm install

# 2. Iniciar servidor
npm start

# 3. Acessar interface
# http://localhost:8080/frontend/
```

### **Desenvolvimento**
```bash
# Auto-reload para desenvolvimento
npm run dev
```

## 📁 **Estrutura do Projeto**

```
EventManagement/
├── frontend/              # Interface web
│   ├── index.html         # Página principal
│   ├── styles.css         # Estilos
│   ├── app.js            # Aplicação principal
│   └── js/               # Componentes modulares
│       ├── config.js     # Configurações
│       ├── services/     # Serviços da API
│       ├── components/   # Componentes da interface
│       └── utils/        # Utilitários
├── server.js             # Servidor Node.js
├── package.json          # Dependências e scripts
└── COMO_RODAR.md         # Guia detalhado
```

## 🌐 **URLs da API**

| Endpoint | Método | Descrição |
|----------|--------|-----------|
| `/eventos` | GET | Listar todos os eventos |
| `/eventos/:id` | GET | Obter evento específico |
| `/eventos` | POST | Criar novo evento |
| `/eventos/:id` | PUT | Atualizar evento |
| `/eventos/:id` | DELETE | Deletar evento |
| `/eventos/:id/participantes` | GET | Listar participantes |
| `/eventos/:id/participantes` | POST | Adicionar participante |
| `/relatorio` | GET | Gerar relatório |
| `/health` | GET | Status do servidor |

## 📊 **Dados de Exemplo**

O sistema vem com 3 eventos de exemplo:
- Workshop de JavaScript
- Palestra sobre React
- Meetup de Desenvolvedores

## 🔧 **Configuração**

### **Porta do Servidor**
Edite `server.js`:
```javascript
const PORT = 8080; // Mude para a porta desejada
```

### **URL da API**
Edite `frontend/js/config.js`:
```javascript
const API_BASE_URL = 'http://localhost:8080'; // Mude se necessário
```

## 🚀 **Comandos Úteis**

| Comando | Descrição |
|---------|-----------|
| `npm start` | Iniciar servidor de produção |
| `npm run dev` | Iniciar servidor de desenvolvimento |
| `npm install` | Instalar dependências |
| `npm test` | Executar testes |

## 📞 **Suporte**

Para mais detalhes, consulte o arquivo `COMO_RODAR.md`.

---

**🎉 Sistema pronto para uso!**
