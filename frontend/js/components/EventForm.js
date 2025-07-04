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
        const form = document.getElementById("eventForm");
        if (form) {
            form.addEventListener("submit", (e) => this.handleSubmit(e));
        }

        const cancelBtn = document.getElementById("cancelBtn");
        if (cancelBtn) {
            cancelBtn.addEventListener("click", () => {
                this.cancelarEdicao();
            });
        }
        this.updateMinDateAttribute();
    }

    updateMinDateAttribute() {
        const dataInput = document.getElementById("data");
        if (dataInput) {
            if (this.editandoEvento) {
                dataInput.removeAttribute("min");
            } else {
                dataInput.min = DateUtils.getMinDate();
            }
        }
    }

    async handleSubmit(e) {
        e.preventDefault();

        const evento = this.getFormData();

        if (!this.validateForm(evento, this.editandoEvento)) {
            return;
        }

        const confirmarSalvamento = await NotificationUtils.confirm(
            `Deseja ${this.editandoEvento ? "atualizar" : "cadastrar"} o evento "${
                evento.nome
            }"?`
        );

        if (!confirmarSalvamento) {
            return;
        }

        try {
            if (this.editandoEvento) {
                await ApiService.updateEvento(this.eventoEditandoId, evento);
                NotificationUtils.success("Evento atualizado com sucesso!");
            } else {
                await ApiService.createEvento(evento);
                NotificationUtils.success("Evento cadastrado com sucesso!");
            }

            this.limparFormulario();
            window.dispatchEvent(new CustomEvent("eventosUpdated"));
        } catch (error) {
            console.error(
                "DEBUG (EventForm.handleSubmit): Erro ao salvar evento:",
                error
            );
            let errorMessage = "Erro ao salvar evento. Por favor, tente novamente.";
            if (error.message) {
                errorMessage = error.message;
            }
            NotificationUtils.error(errorMessage);
        }
    }

    getFormData() {
        const rawDate = document.getElementById("data").value;
        const formattedDate = DateUtils.formatDate(rawDate);
        const hora = document.getElementById("hora").value;
        const local = document.getElementById("local").value.trim();
        const descricao = document.getElementById("descricao").value.trim();
        const nome = document.getElementById("nome").value.trim();

        const eventoData = {
            nome: nome,
            data: formattedDate,
            hora: hora,
            local: local,
            descricao: descricao,
        };

        return eventoData;
    }

    validateForm(evento, isEditing = false) {
        if (!evento.nome) {
            NotificationUtils.warning("Por favor, informe o nome do evento.");
            return false;
        }
        if (evento.nome.length < 5) {
            NotificationUtils.warning(
                "O nome do evento deve ter pelo menos 5 caracteres."
            );
            return false;
        }
        if (!evento.data) {
            NotificationUtils.warning("Por favor, informe a data do evento.");
            return false;
        }
        if (!evento.local) {
            NotificationUtils.warning("Por favor, informe o local do evento.");
            return false;
        }
        if (!evento.descricao) {
            NotificationUtils.warning("Por favor, informe a descrição do evento.");
            return false;
        }

        const [dia, mes, ano] = evento.data.split("/");

        const dataHoraCompletaStr = `${mes}/${dia}/${ano} ${
            evento.hora || "00:00"
        }`;

        const eventDateTime = new Date(dataHoraCompletaStr);

        if (isNaN(eventDateTime.getTime())) {
            NotificationUtils.warning(
                "Formato de data ou hora inválido no formulário."
            );
            return false;
        }

        const now = new Date();

        if (!isEditing) {
            if (eventDateTime.getTime() < now.getTime()) {
                NotificationUtils.warning(
                    "A data e hora de um novo evento devem ser no futuro."
                );
                return false;
            }
        }
        return true;
    }

    preencherParaEdicao(evento) {
        this.eventoOriginal = { ...evento };

        document.getElementById("nome").value = evento.nome;
        document.getElementById("data").value = DateUtils.formatDateForInput(
            evento.data
        );
        document.getElementById("hora").value = evento.hora || "";
        document.getElementById("local").value = evento.local;
        document.getElementById("descricao").value = evento.descricao || "";

        this.editandoEvento = true;
        this.eventoEditandoId = evento.id;

        this.updateFormUI();
        this.updateMinDateAttribute();

        document.getElementById("formTitle").scrollIntoView({ behavior: "smooth" });
    }

    updateFormUI() {
        const formTitle = document.getElementById("formTitle");
        const submitBtn = document.getElementById("submitBtn");
        const cancelBtn = document.getElementById("cancelBtn");

        if (this.editandoEvento) {
            formTitle.textContent = "Editar Evento";
            submitBtn.innerHTML = '<i class="fas fa-save me-1"></i>Atualizar';
            cancelBtn.style.display = "block";
        } else {
            formTitle.textContent = "Cadastrar Evento";
            submitBtn.innerHTML = '<i class="fas fa-save me-1"></i>Cadastrar';
            cancelBtn.style.display = "none";
        }
    }

    limparFormulario() {
        document.getElementById("eventForm").reset();
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
            eventoEditandoId: this.eventoEditandoId,
        };
    }
}

if (typeof module !== "undefined" && module.exports) {
    module.exports = EventForm;
}
