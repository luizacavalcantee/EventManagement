class EventForm {
    constructor() {
        this.editandoEvento = false;
        this.eventoEditandoId = null;
        this.eventoOriginal = null;
        this.init();
    }

    init() {
        this.setupEventListeners();
    }

    setupEventListeners() {
        const form = document.getElementById('eventForm');
        if (form) {
            form.addEventListener('submit', (e) => this.handleSubmit(e));
        }

        const cancelBtn = document.getElementById('cancelBtn');
        if (cancelBtn) {
            cancelBtn.addEventListener('click', () => {
                this.cancelarEdicao();
            });
        }
        this.limparFormulario();
    }

    updateMinDateAttribute() {
        const dataInput = document.getElementById('data');
        if (dataInput) {
            if (this.editandoEvento) {
                dataInput.removeAttribute('min');
            } else {
                dataInput.min = DateUtils.getMinDate();
            }
        }
    }

    async handleSubmit(e) {
        e.preventDefault();

        const evento = this.getFormData();

        if (!this.validateForm(evento)) {
            return;
        }

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

            const confirmacao = await NotificationUtils.confirm(
                'Evento salvo com sucesso! Deseja atualizar a página para ver as mudanças?'
            );

            if (confirmacao) {
                window.location.reload();
            } else {
                this.limparFormulario();
                window.dispatchEvent(new CustomEvent('eventosUpdated'));
            }

        } catch (error) {
            console.error('Erro ao salvar evento:', error);
            NotificationUtils.error('Erro ao salvar evento. Por favor, tente novamente.');
        }
    }

    getFormData() {
        return {
            nome: document.getElementById('nome').value.trim(),
            data: document.getElementById('data').value,
            hora: document.getElementById('hora').value,
            local: document.getElementById('local').value.trim(),
            descricao: document.getElementById('descricao').value.trim()
        };
    }

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

        const dataDoFormularioObj = DateUtils.parseDate(evento.data);
        const today = new Date();
        today.setHours(0, 0, 0, 0);

        if (dataDoFormularioObj && dataDoFormularioObj.getTime() < today.getTime()) {
            if (!this.editandoEvento) {
                NotificationUtils.warning('A data de um novo evento não pode ser no passado.');
                return false;
            } else {
                const dataOriginalObj = this.eventoOriginal ? DateUtils.parseDate(this.eventoOriginal.data) : null;

                if (dataOriginalObj && dataOriginalObj.getTime() >= today.getTime()) {
                    NotificationUtils.warning('Não é possível mover um evento futuro para uma data passada.');
                    return false;
                }
            }
        }

        return true;
    }

    preencherParaEdicao(evento) {
        this.eventoOriginal = { ...evento };

        document.getElementById('nome').value = evento.nome;
        document.getElementById('data').value = DateUtils.formatDateForInput(evento.data);
        document.getElementById('hora').value = evento.hora || '';
        document.getElementById('local').value = evento.local;
        document.getElementById('descricao').value = evento.descricao || '';

        this.editandoEvento = true;
        this.eventoEditandoId = evento.id;

        this.updateFormUI();
        this.updateMinDateAttribute();

        document.getElementById('formTitle').scrollIntoView({ behavior: 'smooth' });
    }

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

    limparFormulario() {
        document.getElementById('eventForm').reset();
        this.editandoEvento = false;
        this.eventoEditandoId = null;
        this.eventoOriginal = null;
        this.updateFormUI();
        this.updateMinDateAttribute();
    }

    cancelarEdicao() {
        this.limparFormulario();
    }

    getState() {
        return {
            editandoEvento: this.editandoEvento,
            eventoEditandoId: this.eventoEditandoId
        };
    }
}

if (typeof module !== 'undefined' && module.exports) {
    module.exports = EventForm;
}
