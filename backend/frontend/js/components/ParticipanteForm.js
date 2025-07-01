/**
 * Componente ParticipanteForm - Gerencia o formulário de inscrição de participantes
 */
class ParticipanteForm {
    constructor() {
        this.init();
    }

    /**
     * Inicializa o componente
     */
    init() {
        this.setupEventListeners();
    }

    /**
     * Configura os event listeners
     */
    setupEventListeners() {
        const form = document.getElementById('participanteForm');
        if (form) {
            form.addEventListener('submit', (e) => this.handleSubmit(e));
        }
    }

    /**
     * Manipula o envio do formulário
     * @param {Event} e - Evento de submit
     */
    async handleSubmit(e) {
        e.preventDefault();
        
        const participante = this.getFormData();
        
        if (!this.validateForm(participante)) {
            return;
        }

        try {
            await ApiService.addParticipante(participante.eventoId, participante);
            NotificationUtils.success('Participante inscrito com sucesso!');
            this.limparFormulario();
            
            // Disparar evento para atualizar a lista
            window.dispatchEvent(new CustomEvent('eventosUpdated'));
            
        } catch (error) {
            console.error('Erro ao inscrever participante:', error);
            NotificationUtils.error('Erro ao inscrever participante. Por favor, tente novamente.');
        }
    }

    /**
     * Obtém os dados do formulário
     * @returns {Object} Dados do participante
     */
    getFormData() {
        return {
            eventoId: document.getElementById('eventoSelect').value,
            nome: document.getElementById('participanteNome').value.trim(),
            email: document.getElementById('participanteEmail').value.trim(),
            contato: document.getElementById('participanteContato').value.trim()
        };
    }

    /**
     * Valida os dados do formulário
     * @param {Object} participante - Dados do participante
     * @returns {boolean} True se válido
     */
    validateForm(participante) {
        if (!participante.eventoId) {
            NotificationUtils.warning('Por favor, selecione um evento.');
            return false;
        }

        if (!participante.nome) {
            NotificationUtils.warning('Por favor, informe o nome do participante.');
            return false;
        }

        if (!participante.email) {
            NotificationUtils.warning('Por favor, informe o email do participante.');
            return false;
        }

        if (!this.isValidEmail(participante.email)) {
            NotificationUtils.warning('Por favor, informe um email válido.');
            return false;
        }

        return true;
    }

    /**
     * Valida formato de email
     * @param {string} email - Email a ser validado
     * @returns {boolean} True se válido
     */
    isValidEmail(email) {
        const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
        return emailRegex.test(email);
    }

    /**
     * Limpa o formulário
     */
    limparFormulario() {
        document.getElementById('participanteForm').reset();
    }

    /**
     * Atualiza a lista de eventos no select
     * @param {Array} eventos - Lista de eventos
     */
    atualizarSelectEventos(eventos) {
        const select = document.getElementById('eventoSelect');
        if (!select) return;

        select.innerHTML = '<option value="">Selecione um evento</option>';
        
        eventos.forEach(evento => {
            const option = document.createElement('option');
            option.value = evento.id;
            option.textContent = `${evento.nome} - ${DateUtils.formatDate(evento.data)}`;
            select.appendChild(option);
        });
    }

    /**
     * Obtém o evento selecionado
     * @returns {Object|null} Evento selecionado ou null
     */
    getEventoSelecionado() {
        const eventoId = document.getElementById('eventoSelect').value;
        if (!eventoId) return null;

        // Buscar o evento na lista global
        if (window.eventoList && window.eventoList.eventos) {
            return window.eventoList.eventos.find(e => e.id == eventoId);
        }
        
        return null;
    }

    /**
     * Verifica se o evento selecionado está cheio
     * @returns {boolean} True se estiver cheio
     */
    isEventoCheio() {
        const evento = this.getEventoSelecionado();
        if (!evento) return false;

        // Assumindo que cada evento tem uma capacidade máxima de 100 participantes
        const capacidadeMaxima = evento.capacidade || 100;
        return (evento.numParticipantes || 0) >= capacidadeMaxima;
    }

    /**
     * Mostra aviso se o evento estiver cheio
     */
    verificarCapacidade() {
        if (this.isEventoCheio()) {
            NotificationUtils.warning('Este evento está com a capacidade máxima atingida.');
        }
    }
}

// Exportar componente
if (typeof module !== 'undefined' && module.exports) {
    module.exports = ParticipanteForm;
} 