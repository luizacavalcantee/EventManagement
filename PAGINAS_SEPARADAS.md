# 📄 Páginas Separadas - Sistema de Eventos

Agora o sistema tem **duas páginas separadas** para diferentes tipos de usuários:

## 🎯 **Estrutura das Páginas**

### **1. 📱 Página de Inscrições (`index.html`)**
- **Público**: Qualquer pessoa que queira se inscrever em eventos
- **Funcionalidades**:
  - ✅ Visualizar eventos disponíveis
  - ✅ Buscar e filtrar eventos
  - ✅ Inscrever-se em eventos
  - ✅ Ver detalhes dos eventos

### **2. ⚙️ Página de Administração (`admin.html`)**
- **Público**: Administradores/organizadores
- **Funcionalidades**:
  - ✅ Dashboard com estatísticas
  - ✅ Cadastrar novos eventos
  - ✅ Editar eventos existentes
  - ✅ Deletar eventos
  - ✅ Ver lista de participantes
  - ✅ Gerenciar inscrições

---

## 🚀 **Como Usar**

### **Para Administradores:**
1. **Acesse**: `http://localhost:8080/frontend/admin.html`
2. **Cadastre eventos** usando o formulário
3. **Gerencie eventos** na lista (editar, deletar)
4. **Veja participantes** clicando no botão de participantes
5. **Monitore estatísticas** no dashboard

### **Para Participantes:**
1. **Acesse**: `http://localhost:8080/frontend/`
2. **Navegue pelos eventos** disponíveis
3. **Use filtros** para encontrar eventos específicos
4. **Inscreva-se** clicando em "Inscrever-se"
5. **Preencha seus dados** no modal

---

## 🔗 **Navegação Entre Páginas**

### **Da página de inscrições para administração:**
- Clique em "Administração" no menu superior

### **Da página de administração para inscrições:**
- Clique em "Página de Inscrições" no menu superior

---

## 📋 **Funcionalidades Detalhadas**

### **Página de Inscrições (`index.html`)**

#### **🔍 Busca e Filtros**
- **Busca por texto**: Nome, local ou descrição
- **Filtro por status**: Hoje, próximos, futuros
- **Filtro por local**: Todos os locais disponíveis
- **Ordenação**: Por data, nome ou local

#### **📅 Visualização de Eventos**
- **Cards responsivos** com informações completas
- **Status visual** (hoje, próximo, passado)
- **Contador de inscritos** em tempo real
- **Botão de inscrição** em cada evento

#### **📝 Processo de Inscrição**
1. Clique em "Inscrever-se"
2. Modal com informações do evento
3. Preencha: Nome, Email, Telefone (opcional)
4. Confirme a inscrição
5. Receba confirmação

### **Página de Administração (`admin.html`)**

#### **📊 Dashboard**
- **Total de eventos** cadastrados
- **Total de participantes** inscritos
- **Eventos próximos** (próximos 7 dias)
- **Eventos de hoje**

#### **➕ Cadastro de Eventos**
- **Formulário completo**: Nome, data, hora, local, descrição
- **Validação automática** de campos obrigatórios
- **Data mínima** definida como hoje
- **Modo de edição** para eventos existentes

#### **📋 Gerenciamento de Eventos**
- **Lista em cards** com ações rápidas
- **Editar evento**: Preenche formulário automaticamente
- **Deletar evento**: Confirmação antes de excluir
- **Ver participantes**: Modal com lista completa

#### **👥 Visualização de Participantes**
- **Modal dedicado** para cada evento
- **Tabela organizada**: Nome, email, contato
- **Avatar com inicial** do nome
- **Contador total** de participantes
- **Estado vazio** quando não há inscritos

---

## 🎨 **Interface e Design**

### **Cores e Temas**
- **Página de Inscrições**: Verde (sucesso, inscrição)
- **Página de Administração**: Azul (profissional, gestão)

### **Responsividade**
- **Mobile-first**: Funciona em todos os dispositivos
- **Cards adaptativos**: Se ajustam ao tamanho da tela
- **Navegação otimizada**: Menus e botões touch-friendly

### **Animações e Transições**
- **Hover effects**: Cards e botões
- **Loading states**: Indicadores de carregamento
- **Smooth transitions**: Transições suaves

---

## 🔧 **Configuração e Personalização**

### **URLs das Páginas**
```javascript
// Configuração em js/config.js
const CONFIG = {
    API_BASE_URL: 'http://localhost:8080',
    // URLs das páginas
    PAGES: {
        INSCRICOES: '/frontend/',
        ADMIN: '/frontend/admin.html'
    }
};
```

### **Personalização de Cores**
```css
/* Em styles.css */
/* Página de inscrições */
.navbar.bg-success { /* Verde */ }

/* Página de administração */
.navbar.bg-primary { /* Azul */ }
```

---

## 🐛 **Solução de Problemas**

### **Eventos não salvam**
1. Verifique se o servidor C++ está rodando
2. Abra o console do navegador (F12)
3. Teste a API: `http://localhost:8080/health`
4. Verifique permissões de escrita no arquivo

### **Inscrições não funcionam**
1. Verifique se o evento existe
2. Confirme se os campos obrigatórios estão preenchidos
3. Verifique o formato do email
4. Teste a conexão com o servidor

### **Páginas não carregam**
1. Verifique se todos os arquivos estão na pasta `frontend/`
2. Confirme se o servidor está servindo arquivos estáticos
3. Verifique se não há erros no console

---

## 📱 **URLs de Acesso**

| Página | URL | Descrição |
|--------|-----|-----------|
| **Inscrições** | `http://localhost:8080/frontend/` | Página principal para inscrições |
| **Administração** | `http://localhost:8080/frontend/admin.html` | Painel de administração |

---

## 🎉 **Benefícios da Separação**

### **Para Administradores:**
- ✅ Interface focada em gestão
- ✅ Dashboard com estatísticas
- ✅ Controle total sobre eventos
- ✅ Visualização detalhada de participantes

### **Para Participantes:**
- ✅ Interface limpa e focada
- ✅ Processo de inscrição simplificado
- ✅ Busca e filtros intuitivos
- ✅ Experiência otimizada

### **Para o Sistema:**
- ✅ Separação clara de responsabilidades
- ✅ Segurança melhorada
- ✅ Manutenção facilitada
- ✅ Escalabilidade

---

**🚀 Sistema pronto com páginas separadas e funcionalidades completas!** 