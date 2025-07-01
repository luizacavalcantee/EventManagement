# Interface Web - Sistema de Gerenciamento de Eventos

Esta é uma interface web moderna e responsiva para o sistema de gerenciamento de eventos desenvolvido em C++.

## 🚀 Funcionalidades

### Dashboard
- **Cards informativos** com estatísticas em tempo real:
  - Total de eventos
  - Total de participantes
  - Eventos próximos (próximos 7 dias)
  - Eventos de hoje

### Gerenciamento de Eventos
- ✅ **Cadastrar eventos** com nome, data, hora, local e descrição
- ✏️ **Editar eventos** existentes
- 👁️ **Visualizar detalhes** completos de cada evento
- 🗑️ **Excluir eventos** com confirmação
- 📋 **Listar todos os eventos** em tabela organizada

### Gerenciamento de Participantes
- 👥 **Inscrever participantes** em eventos específicos
- 📊 **Visualizar lista de participantes** por evento
- 📧 **Dados completos**: nome, email e contato

### Relatórios
- 📄 **Gerar relatórios** em formato texto
- 💾 **Download automático** dos relatórios
- 📊 **Estatísticas detalhadas** do sistema

## 🎨 Design

- **Interface moderna** com gradientes e animações
- **Responsivo** para desktop, tablet e mobile
- **Ícones intuitivos** do Font Awesome
- **Cores temáticas** para diferentes tipos de ações
- **Animações suaves** para melhor experiência do usuário

## 🛠️ Tecnologias Utilizadas

- **HTML5** - Estrutura da página
- **CSS3** - Estilização com gradientes e animações
- **JavaScript (ES6+)** - Funcionalidades dinâmicas
- **Bootstrap 5** - Framework CSS responsivo
- **Font Awesome** - Ícones
- **Fetch API** - Comunicação com o servidor

## 📁 Estrutura de Arquivos

```
frontend/
├── index.html          # Página principal
├── app.js             # Lógica JavaScript
├── styles.css         # Estilos customizados
└── README.md          # Este arquivo
```

## 🔧 Como Usar

### 1. Preparação
Certifique-se de que o servidor API está rodando:
```bash
# Compile e execute o servidor API
g++ -o api_server api_server.cpp GerenciadorEventos.cpp Evento.cpp Participante.cpp -lhttplib -lnlohmann_json
./api_server
```

### 2. Acessar a Interface
Abra o arquivo `index.html` em seu navegador ou use um servidor local:
```bash
# Usando Python
python -m http.server 8000

# Usando Node.js
npx serve .

# Usando PHP
php -S localhost:8000
```

### 3. Configuração da API
Se necessário, ajuste a URL da API no arquivo `app.js`:
```javascript
const API_BASE_URL = 'http://localhost:8080'; // Ajuste conforme necessário
```

## 🎯 Funcionalidades Principais

### Cadastrar Evento
1. Preencha o formulário com os dados do evento
2. Clique em "Cadastrar"
3. O evento aparecerá na lista automaticamente

### Editar Evento
1. Clique no ícone de editar (lápis) na lista
2. O formulário será preenchido com os dados atuais
3. Faça as alterações necessárias
4. Clique em "Atualizar"

### Inscrever Participante
1. Selecione o evento no dropdown
2. Preencha os dados do participante
3. Clique em "Inscrever"

### Visualizar Participantes
1. Clique no ícone de usuários na coluna "Participantes"
2. Um modal mostrará a lista completa de participantes

### Gerar Relatório
1. Clique no botão "Relatório" na barra de navegação
2. O relatório será baixado automaticamente

## 🎨 Personalização

### Cores
As cores podem ser personalizadas editando as variáveis CSS no arquivo `styles.css`:

```css
/* Exemplo de personalização de cores */
:root {
    --primary-color: #3b82f6;
    --success-color: #10b981;
    --warning-color: #f59e0b;
    --danger-color: #ef4444;
}
```

### Layout
O layout é responsivo e se adapta automaticamente a diferentes tamanhos de tela. Para personalizar:

- **Desktop**: Layout em duas colunas
- **Tablet**: Layout adaptativo
- **Mobile**: Layout em coluna única

## 🔍 Troubleshooting

### Problemas Comuns

1. **Erro de conexão com API**
   - Verifique se o servidor está rodando
   - Confirme a URL da API no `app.js`
   - Verifique se não há bloqueio de CORS

2. **Interface não carrega**
   - Verifique se todos os arquivos estão na pasta correta
   - Abra o console do navegador para ver erros
   - Certifique-se de que está usando um servidor web

3. **Formulários não funcionam**
   - Verifique se o JavaScript está habilitado
   - Confirme se todos os campos obrigatórios estão preenchidos
   - Verifique a conexão com a API

### Logs e Debug
Abra o console do navegador (F12) para ver logs detalhados e mensagens de erro.

## 📱 Compatibilidade

- ✅ Chrome 80+
- ✅ Firefox 75+
- ✅ Safari 13+
- ✅ Edge 80+
- ✅ Mobile browsers

## 🤝 Contribuição

Para contribuir com melhorias:

1. Faça um fork do projeto
2. Crie uma branch para sua feature
3. Implemente as mudanças
4. Teste em diferentes navegadores
5. Envie um pull request

## 📄 Licença

Este projeto está sob a licença MIT. Veja o arquivo LICENSE para mais detalhes.

---

**Desenvolvido com ❤️ para facilitar o gerenciamento de eventos** 