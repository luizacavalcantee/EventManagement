/**
 * Componente EventoForm - Gerencia o formulário de criação/edição de eventos
 */
class EventoForm {
    constructor() {
        this.editandoEvento = false;
        this.eventoEditandoId = null;
        this.init();
    }

    /**
     * Inicializa o componente
     */
    init() {
        this.setupEventListeners();
        this.setMinDate();
    }

    /**
     * Configura os event listeners
     */
    setupEventListeners() {
        const form = document.getElementById('eventoForm');
        if (form) {
            form.addEventListener('submit', (e) => this.handleSubmit(e));
        }
    }

    /**
     * Define a data mínima como hoje
     */
    setMinDate() {
        const dataInput = document.getElementById('data');
        if (dataInput) {
            dataInput.min = DateUtils.getMinDate();
        }
    }

    /**
     * Manipula o envio do formulário
     * @param {Event} e - Evento de submit
     */
    async handleSubmit(e) {
        e.preventDefault();
        
        const evento = this.getFormData();
        
        if (!this.validateForm(evento)) {
            return;
        }

        // Confirmar se deseja salvar o evento
        const confirmarSalvamento = await NotificationUtils.confirm(
            `Deseja ${this.editandoEvento ? 'atualizar' : 'cadastrar'} o evento "${evento.nome}"?`
        );
        
        if (!confirmarSalvamento) {
            return;
        }

        try {
            if (this.editandoEvento) {
                await ApiService.updateEvento(this.eventoEditandoId, evento);
                NotificationUtils.success('Evento atualizado com sucesso!');
            } else {
                await ApiService.createEvento(evento);
                NotificationUtils.success('Evento cadastrado com sucesso!');
            }
            
            // Mostrar popup de confirmação
            const confirmacao = await NotificationUtils.confirm(
                'Evento salvo com sucesso! Deseja atualizar a página para ver as mudanças?'
            );
            
            if (confirmacao) {
                // Atualizar a página
                window.location.reload();
            } else {
                // Limpar formulário e disparar evento para atualizar a lista
                this.limparFormulario();
                window.dispatchEvent(new CustomEvent('eventosUpdated'));
            }
            
        } catch (error) {
            console.error('Erro ao salvar evento:', error);
            NotificationUtils.error('Erro ao salvar evento. Por favor, tente novamente.');
        }
    }

    /**
     * Obtém os dados do formulário
     * @returns {Object} Dados do evento
     */
    getFormData() {
        return {
            nome: document.getElementById('nome').value.trim(),
            data: document.getElementById('data').value,
            hora: document.getElementById('hora').value,
            local: document.getElementById('local').value.trim(),
            descricao: document.getElementById('descricao').value.trim()
        };
    }

    /**
     * Valida os dados do formulário
     * @param {Object} evento - Dados do evento
     * @returns {boolean} True se válido
     */
    validateForm(evento) {
        if (!evento.nome) {
            NotificationUtils.warning('Por favor, informe o nome do evento.');
            return false;
        }

        if (!evento.data) {
            NotificationUtils.warning('Por favor, informe a data do evento.');
            return false;
        }

        if (!evento.local) {
            NotificationUtils.warning('Por favor, informe o local do evento.');
            return false;
        }

        // Validar se a data não é passada
        if (DateUtils.isPast(evento.data)) {
            NotificationUtils.warning('A data do evento não pode ser no passado.');
            return false;
        }

        return true;
    }

    /**
     * Preenche o formulário para edição
     * @param {Object} evento - Dados do evento
     */
    preencherParaEdicao(evento) {
        document.getElementById('nome').value = evento.nome;
        document.getElementById('data').value = evento.data;
        document.getElementById('hora').value = evento.hora || '';
        document.getElementById('local').value = evento.local;
        document.getElementById('descricao').value = evento.descricao || '';

        this.editandoEvento = true;
        this.eventoEditandoId = evento.id;
        
        this.updateFormUI();
        
        // Scroll para o formulário
        document.getElementById('eventoForm').scrollIntoView({ behavior: 'smooth' });
    }

    /**
     * Atualiza a interface do formulário
     */
    updateFormUI() {
        const formTitle = document.getElementById('formTitle');
        const submitBtn = document.getElementById('submitBtn');
        const cancelBtn = document.getElementById('cancelBtn');

        if (this.editandoEvento) {
            formTitle.textContent = 'Editar Evento';
            submitBtn.innerHTML = '<i class="fas fa-save me-1"></i>Atualizar';
            cancelBtn.style.display = 'block';
        } else {
            formTitle.textContent = 'Cadastrar Evento';
            submitBtn.innerHTML = '<i class="fas fa-save me-1"></i>Cadastrar';
            cancelBtn.style.display = 'none';
        }
    }

    /**
     * Limpa o formulário
     */
    limparFormulario() {
        document.getElementById('eventoForm').reset();
        this.editandoEvento = false;
        this.eventoEditandoId = null;
        this.updateFormUI();
    }

    /**
     * Cancela a edição
     */
    cancelarEdicao() {
        this.limparFormulario();
    }

    /**
     * Obtém o estado atual do formulário
     * @returns {Object} Estado do formulário
     */
    getState() {
        return {
            editandoEvento: this.editandoEvento,
            eventoEditandoId: this.eventoEditandoId
        };
    }
}

// Exportar componente
if (typeof module !== 'undefined' && module.exports) {
    module.exports = EventoForm;
} 