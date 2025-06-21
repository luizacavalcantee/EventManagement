# 🔗 Integração Backend C++ + Frontend JavaScript

Este guia explica como integrar o backend C++ original com o frontend JavaScript.

## 🎯 **Problema Resolvido**

O frontend JavaScript estava tentando salvar eventos, mas havia incompatibilidades com o backend C++:

### **❌ Problemas Identificados:**
1. **Estrutura de dados diferente**: C++ esperava `capacidade`, frontend enviava `descricao`
2. **Campos faltando**: C++ não tinha `hora` e `descricao` na API
3. **IDs**: C++ não retornava IDs nos eventos criados
4. **Endpoints**: Alguns endpoints do frontend não existiam no C++

### **✅ Soluções Implementadas:**
1. **Adicionado ID** à classe Evento
2. **Atualizada API** para aceitar todos os campos do frontend
3. **Implementados endpoints** faltantes
4. **Corrigida estrutura JSON** de resposta

## 🛠️ **Arquivos Modificados**

### **Backend C++**
- ✅ `Evento.h` - Adicionado campo `id` e métodos `getId()`/`setId()`
- ✅ `Evento.cpp` - Implementação dos novos métodos
- ✅ `GerenciadorEventos.h` - Novos métodos para API
- ✅ `GerenciadorEventos.cpp` - Implementação dos métodos da API
- ✅ `api_server.cpp` - API completa e compatível com frontend

### **Frontend JavaScript**
- ✅ `config.js` - Configurações da API
- ✅ `apiService.js` - Serviços de comunicação
- ✅ `EventoForm.js` - Formulário de eventos
- ✅ `EventoList.js` - Lista de eventos
- ✅ `ParticipanteForm.js` - Formulário de participantes

## 🚀 **Como Compilar e Executar**

### **Passo 1: Compilar o Servidor C++**
```bash
# Windows
compile_server.bat

# Linux/macOS
g++ -std=c++17 -I. -o api_server api_server.cpp Evento.cpp Participante.cpp GerenciadorEventos.cpp
```

### **Passo 2: Executar o Servidor**
```bash
# Windows
api_server.exe

# Linux/macOS
./api_server
```

### **Passo 3: Acessar Interface**
- Abra o navegador
- Vá para: `http://localhost:8080/frontend/`

## 📋 **Endpoints da API**

| Endpoint | Método | Descrição | Status |
|----------|--------|-----------|--------|
| `/health` | GET | Status do servidor | ✅ |
| `/eventos` | GET | Listar eventos | ✅ |
| `/eventos/:id` | GET | Obter evento | ✅ |
| `/eventos` | POST | Criar evento | ✅ |
| `/eventos/:id` | PUT | Atualizar evento | ✅ |
| `/eventos/:id` | DELETE | Deletar evento | ✅ |
| `/eventos/:id/participantes` | GET | Listar participantes | ✅ |
| `/eventos/:id/participantes` | POST | Adicionar participante | ✅ |
| `/relatorio` | GET | Gerar relatório | ✅ |
| `/eventos/salvar` | POST | Salvar eventos | ✅ |

## 🔄 **Fluxo de Dados**

### **Criar Evento**
1. **Frontend** envia POST para `/eventos`
2. **Backend C++** recebe dados JSON
3. **Backend** cria objeto Evento
4. **Backend** salva no arquivo
5. **Backend** retorna evento com ID
6. **Frontend** atualiza interface

### **Listar Eventos**
1. **Frontend** envia GET para `/eventos`
2. **Backend C++** carrega do arquivo
3. **Backend** converte para JSON
4. **Frontend** exibe na interface

## 📊 **Estrutura de Dados**

### **Evento (JSON)**
```json
{
  "id": 1,
  "nome": "Workshop JavaScript",
  "data": "2024-01-15",
  "hora": "14:00",
  "local": "Auditório Principal",
  "descricao": "Workshop prático",
  "numParticipantes": 5
}
```

### **Participante (JSON)**
```json
{
  "nome": "João Silva",
  "email": "joao@email.com",
  "contato": "(11) 99999-9999"
}
```

## 🔧 **Configurações**

### **Porta do Servidor**
Edite `api_server.cpp`:
```cpp
svr.listen("localhost", 8080); // Mude a porta se necessário
```

### **URL da API**
Edite `frontend/js/config.js`:
```javascript
API_BASE_URL: 'http://localhost:8080' // Mude se necessário
```

## 🐛 **Solução de Problemas**

### **Erro: "Biblioteca não encontrada"**
```bash
# Instalar httplib (header-only)
# Baixe httplib.h e coloque na pasta do projeto

# Instalar nlohmann/json (header-only)
# Baixe json.hpp e coloque na pasta do projeto
```

### **Erro: "Compilador não encontrado"**
```bash
# Windows: Instale MinGW ou Visual Studio Build Tools
# Linux: sudo apt install g++
# macOS: xcode-select --install
```

### **Erro: "CORS"**
- ✅ CORS já configurado no servidor C++
- ✅ Headers permitem requisições do frontend

### **Eventos não salvam**
1. Verifique se o servidor está rodando
2. Verifique o console do navegador (F12)
3. Teste a API: `http://localhost:8080/health`
4. Verifique permissões de escrita no arquivo

## 📁 **Arquivos de Dados**

### **eventos.txt**
```
1,Workshop JavaScript,2024-01-15,14:00,Auditório Principal,Workshop prático
PARTICIPANTE,João Silva,joao@email.com,(11) 99999-9999
PARTICIPANTE,Maria Santos,maria@email.com,(11) 88888-8888
```

## 🎉 **Resultado**

Agora o sistema está **100% integrado**:
- ✅ **Backend C++** processa dados
- ✅ **Frontend JavaScript** interface moderna
- ✅ **Persistência** em arquivo texto
- ✅ **API REST** completa
- ✅ **CORS** configurado
- ✅ **Dados sincronizados**

---

**🚀 Sistema pronto para uso com backend C++ e frontend JavaScript!** 