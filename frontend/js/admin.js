class AdminApp {
    constructor() {
        this.dashboard = new Dashboard();
        this.eventForm = new EventForm();
        this.eventList = new EventList();
        this.init();
    }

    async init() {
        try {
            await this.carregarDados();
            this.setupEventListeners();
        } catch (error) {
            console.error('Erro ao inicializar aplicação:', error);
            NotificationUtils.error('Erro ao carregar dados. Verifique se o servidor está rodando.');
        }
    }

    setupEventListeners() {
        window.addEventListener('eventosUpdated', () => {
            this.carregarDados();
        });

        const cancelBtn = document.getElementById('cancelBtn');
        if (cancelBtn) {
            cancelBtn.addEventListener('click', () => {
                this.eventForm.cancelarEdicao();
            });
        }
    }

    setMinDate() {
        const dataInput = document.getElementById('data');
        if (dataInput) {
            dataInput.min = DateUtils.getMinDate();
        }
    }

    async carregarDados() {
        try {
            const [relatorio, eventos] = await Promise.all([
                ApiService.getRelatorio(),
                ApiService.getEventos()
            ]);

            this.dashboard.atualizarDashboard(relatorio);

            this.eventList.renderizarEventos(eventos, {
                mostrarAcoes: true,
                mostrarParticipantes: true,
                onEditar: (evento) => this.editarEvento(evento),
                onDeletar: (evento) => this.deletarEvento(evento),
                onVerParticipantes: (evento) => this.verParticipantes(evento)
            });

        } catch (error) {
            console.error('Erro ao carregar dados:', error);
            NotificationUtils.error('Erro ao carregar dados. Verifique se o servidor está rodando.');
        }
    }

    editarEvento(evento) {
        this.eventForm.preencherParaEdicao(evento);
    }

    async deletarEvento(evento) {
        if (confirm(`Tem certeza que deseja deletar o evento "${evento.nome}"?`)) {
            try {
                await ApiService.deleteEvento(evento.id);
                NotificationUtils.success('Evento deletado com sucesso!');
                this.carregarDados();
            } catch (error) {
                console.error('Erro ao deletar evento:', error);
                NotificationUtils.error('Erro ao deletar evento. Tente novamente.');
            }
        }
    }

    async verParticipantes(evento) {
        try {
            const participantes = await ApiService.getParticipantes(evento.id);
            this.renderizarParticipantes(evento, participantes);

            const modal = new bootstrap.Modal(document.getElementById('participantesModal'));
            modal.show();

        } catch (error) {
            console.error('Erro ao carregar participantes:', error);
            NotificationUtils.error('Erro ao carregar participantes. Tente novamente.');
        }
    }

    renderizarParticipantes(evento, participantes) {
        const container = document.getElementById('participantesList');

        if (participantes.length === 0) {
            container.innerHTML = `
                <div class="text-center py-4">
                    <i class="fas fa-users fa-3x text-muted mb-3"></i>
                    <h5 class="text-muted">Nenhum participante inscrito</h5>
                    <p class="text-muted">Ainda não há inscrições para este evento.</p>
                </div>
            `;
            return;
        }

        let html = `
            <div class="mb-3">
                <h6 class="text-primary">
                    <i class="fas fa-calendar me-2"></i>${evento.nome}
                </h6>
                <p class="text-muted mb-3">
                    <i class="fas fa-map-marker-alt me-1"></i>${evento.local} | 
                    <i class="fas fa-clock me-1"></i>${evento.data} ${evento.hora}
                </p>
            </div>
            <div class="table-responsive">
                <table class="table table-hover">
                    <thead class="table-light">
                        <tr>
                            <th><i class="fas fa-user me-1"></i>Nome</th>
                            <th><i class="fas fa-envelope me-1"></i>Email</th>
                            <th><i class="fas fa-phone me-1"></i>Contato</th>
                        </tr>
                    </thead>
                    <tbody>
        `;

        participantes.forEach((participante, index) => {
            html += `
                <tr>
                    <td>
                        <div class="d-flex align-items-center">
                            <div class="avatar-sm bg-primary text-white rounded-circle d-flex align-items-center justify-content-center me-2">
                                ${participante.nome.charAt(0).toUpperCase()}
                            </div>
                            ${participante.nome}
                        </div>
                    </td>
                    <td>
                        <a href="mailto:${participante.email}" class="text-decoration-none">
                            ${participante.email}
                        </a>
                    </td>
                    <td>
                        ${participante.contato || '<span class="text-muted">Não informado</span>'}
                    </td>
                </tr>
            `;
        });

        html += `
                    </tbody>
                </table>
            </div>
            <div class="mt-3">
                <span class="badge bg-primary">${participantes.length} participante(s)</span>
            </div>
        `;

        container.innerHTML = html;
    }
}

function carregarEventos() {
    if (window.adminApp) {
        window.adminApp.carregarDados();
    }
}

document.addEventListener('DOMContentLoaded', () => {
    window.adminApp = new AdminApp();
});