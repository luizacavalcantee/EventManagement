class ParticipantForm {
    constructor() {
        this.init();
    }

    init() {
        this.setupEventListeners();
    }

    setupEventListeners() {
        const form = document.getElementById('participanteForm');
        if (form) {
            form.addEventListener('submit', (e) => this.handleSubmit(e));
        }
    }

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
            window.dispatchEvent(new CustomEvent('eventosUpdated'));
        } catch (error) {
            console.error('Erro ao inscrever participante:', error);
            NotificationUtils.error('Erro ao inscrever participante. Por favor, tente novamente.');
        }
    }

    getFormData() {
        return {
            eventoId: document.getElementById('eventoSelect').value,
            nome: document.getElementById('participanteNome').value.trim(),
            email: document.getElementById('participanteEmail').value.trim(),
            contato: document.getElementById('participanteContato').value.trim()
        };
    }

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

    isValidEmail(email) {
        const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
        return emailRegex.test(email);
    }

    limparFormulario() {
        document.getElementById('participanteForm').reset();
    }

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

    getEventoSelecionado() {
        const eventoId = document.getElementById('eventoSelect').value;
        if (!eventoId) return null;
        if (window.eventList && window.eventList.eventos) {
            return window.eventList.eventos.find(e => e.id == eventoId);
        }
        return null;
    }

    isEventoCheio() {
        const evento = this.getEventoSelecionado();
        if (!evento) return false;
        const capacidadeMaxima = evento.capacidade || 100;
        return (evento.numParticipantes || 0) >= capacidadeMaxima;
    }

    verificarCapacidade() {
        if (this.isEventoCheio()) {
            NotificationUtils.warning('Este evento está com a capacidade máxima atingida.');
        }
    }
}

if (typeof module !== 'undefined' && module.exports) {
    module.exports = ParticipantForm;
}