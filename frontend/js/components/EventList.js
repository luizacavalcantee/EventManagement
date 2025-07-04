class EventList {
    constructor() {
        this.eventos = [];
        this.init();
    }

    init() {
        this.setupEventListeners();
    }

    setupEventListeners() {
        window.addEventListener('eventosUpdated', () => {
            this.carregarEventos();
        });
    }

    async carregarEventos() {
        try {
            this.eventos = await ApiService.getEventos();
            this.renderizarTabela();
            this.atualizarSelectEventos();
            if (window.dashboard) {
                window.dashboard.updateData(this.eventos);
            }
        } catch (error) {
            console.error('Erro ao carregar eventos:', error);
            NotificationUtils.error('Erro ao carregar eventos. Verifique se o servidor está rodando.');
        }
    }

    renderizarEventos(eventos, options = {}) {
        this.eventos = eventos || [];
        if (options.modo === 'cards' || options.mostrarAcoes === false) {
            this.renderizarCards(eventos, options);
        } else {
            this.renderizarTabela(options);
        }
    }

    renderizarTabela() {
        const tbody = document.getElementById('eventosLista');
        if (!tbody) return;

        tbody.innerHTML = '';

        if (this.eventos.length === 0) {
            tbody.innerHTML = this.getEmptyStateHTML();
            return;
        }

        this.eventos.forEach(evento => {
            const tr = this.criarLinhaEvento(evento);
            tbody.appendChild(tr);
        });
    }

    criarLinhaEvento(evento) {
        const tr = document.createElement('tr');
        const statusClass = this.getStatusClass(evento.data);
        const eventoJson = JSON.stringify(evento).replace(/"/g, '&quot;');
        tr.className = statusClass;
        tr.innerHTML = `
            <td>
                <strong>${evento.nome}</strong>
                ${evento.descricao ? `<br><small class="text-muted">${evento.descricao}</small>` : ''}
            </td>
            <td>${DateUtils.formatDate(evento.data)}</td>
            <td>${evento.hora || '--:--'}</td>
            <td>${evento.local}</td>
            <td>
                <span class="badge bg-primary">${evento.numParticipantes || 0}</span>
            </td>
            <td class="action-buttons">
                <div class="btn-group">
                    <button class="btn btn-sm btn-outline-info" onclick="window.adminApp.verParticipantes(${eventoJson})" title="Ver participantes">
                        <i class="fas fa-users"></i>
                    </button>
                    <button class="btn btn-sm btn-outline-warning" onclick="window.adminApp.editarEvento(${eventoJson})" title="Editar">
                        <i class="fas fa-edit"></i>
                    </button>
                    <button class="btn btn-sm btn-outline-danger" onclick="window.adminApp.deletarEvento(${eventoJson})" title="Excluir">
                        <i class="fas fa-trash"></i>
                    </button>
                </div>
            </td>
        `;
        return tr;
    }

    getStatusClass(data) {
        if (DateUtils.isPast(data)) {
            return 'table-secondary';
        } else if (DateUtils.isUpcoming(data)) {
            return 'table-warning';
        }
        return '';
    }

    getStatusBadgeClass(data) {
        if (DateUtils.isPast(data)) {
            return 'bg-secondary';
        } else if (DateUtils.isToday(data)) {
            return 'bg-success';
        } else if (DateUtils.isUpcoming(data)) {
            return 'bg-warning';
        }
        return 'bg-primary';
    }

    getStatusText(data) {
        if (DateUtils.isPast(data)) {
            return 'Passado';
        } else if (DateUtils.isToday(data)) {
            return 'Hoje';
        } else if (DateUtils.isUpcoming(data)) {
            return 'Próximo';
        }
        return 'Futuro';
    }

    getEmptyCardsHTML() {
        return `
            <div class="col-12 text-center py-5">
                <i class="fas fa-calendar-times fa-3x text-muted mb-3"></i>
                <h5 class="text-muted">Nenhum evento encontrado</h5>
                <p class="text-muted">Não há eventos disponíveis no momento.</p>
            </div>
        `;
    }

    getEmptyStateHTML() {
        return `
            <tr>
                <td colspan="6" class="text-center text-muted py-5">
                    <i class="fas fa-calendar-times fa-3x mb-3"></i>
                    <br>
                    <h5>Nenhum evento cadastrado</h5>
                    <p class="mb-0">Comece cadastrando seu primeiro evento!</p>
                </td>
            </tr>
        `;
    }

    atualizarSelectEventos() {
        const select = document.getElementById('eventoSelect');
        if (!select) return;

        select.innerHTML = '<option value="">Selecione um evento</option>';

        this.eventos.forEach(evento => {
            const option = document.createElement('option');
            option.value = evento.id;
            option.textContent = `${evento.nome} - ${DateUtils.formatDate(evento.data)}`;
            select.appendChild(option);
        });
    }

    async editarEvento(id) {
        try {
            const evento = this.eventos.find(e => e.id === id);
            if (!evento) {
                throw new Error('Evento não encontrado');
            }

            if (window.eventForm) {
                window.eventForm.preencherParaEdicao(evento);
            }
        } catch (error) {
            console.error('Erro ao carregar evento:', error);
            NotificationUtils.error('Erro ao carregar evento. Por favor, tente novamente.');
        }
    }

    async deletarEvento(id) {
        const confirmed = await NotificationUtils.confirm('Tem certeza que deseja excluir este evento?');
        if (!confirmed) return;

        try {
            await ApiService.deleteEvento(id);
            NotificationUtils.success('Evento excluído com sucesso!');
            this.carregarEventos();
        } catch (error) {
            console.error('Erro ao excluir evento:', error);
            NotificationUtils.error('Erro ao excluir evento. Por favor, tente novamente.');
        }
    }

    async verDetalhes(id) {
        try {
            const evento = this.eventos.find(e => e.id === id);
            if (!evento) {
                throw new Error('Evento não encontrado');
            }

            const modal = new bootstrap.Modal(document.getElementById('detalhesModal'));
            document.getElementById('detalhesEvento').innerHTML = this.getDetalhesHTML(evento);
            modal.show();
        } catch (error) {
            console.error('Erro ao carregar detalhes:', error);
            NotificationUtils.error('Erro ao carregar detalhes. Por favor, tente novamente.');
        }
    }

    getDetalhesHTML(evento) {
        return `
            <div class="row">
                <div class="col-md-6">
                    <h5>${evento.nome}</h5>
                    <p class="text-muted">${evento.descricao || 'Sem descrição'}</p>
                </div>
                <div class="col-md-6">
                    <ul class="list-unstyled">
                        <li><i class="fas fa-calendar me-2"></i>${DateUtils.formatDate(evento.data)}</li>
                        <li><i class="fas fa-clock me-2"></i>${evento.hora || 'Não informada'}</li>
                        <li><i class="fas fa-map-marker-alt me-2"></i>${evento.local}</li>
                        <li><i class="fas fa-users me-2"></i>${evento.numParticipantes || 0} participantes</li>
                    </ul>
                </div>
            </div>
        `;
    }

    async verParticipantes(eventoId) {
        try {
            const participantes = await ApiService.getParticipantes(eventoId);
            const modal = new bootstrap.Modal(document.getElementById('participantesModal'));
            document.getElementById('participantesList').innerHTML = this.getParticipantesHTML(participantes);
            modal.show();
        } catch (error) {
            console.error('Erro ao carregar participantes:', error);
            NotificationUtils.error('Erro ao carregar participantes. Por favor, tente novamente.');
        }
    }

    getParticipantesHTML(participantes) {
        if (participantes.length === 0) {
            return `
                <div class="text-center py-4">
                    <i class="fas fa-users fa-3x text-muted mb-3"></i>
                    <h5 class="text-muted">Nenhum participante inscrito</h5>
                    <p class="text-muted">Ainda não há inscrições para este evento.</p>
                </div>
            `;
        }

        let html = '<div class="table-responsive"><table class="table table-hover">';
        html += '<thead><tr><th>Nome</th><th>Email</th><th>Contato</th></tr></thead><tbody>';

        participantes.forEach(participante => {
            html += `
                <tr>
                    <td>${participante.nome}</td>
                    <td>${participante.email}</td>
                    <td>${participante.contato || 'Não informado'}</td>
                </tr>
            `;
        });

        html += '</tbody></table></div>';
        return html;
    }

    async salvarEventos() {
        try {
            await ApiService.salvarEventos();
            NotificationUtils.success('Eventos salvos com sucesso!');
        } catch (error) {
            console.error('Erro ao salvar eventos:', error);
            NotificationUtils.error('Erro ao salvar eventos. Por favor, tente novamente.');
        }
    }

    renderizarCards(eventos, options = {}) {
        const container = document.getElementById('eventosList');
        if (!container) return;

        container.innerHTML = '';

        if (eventos.length === 0) {
            container.innerHTML = this.getEmptyCardsHTML();
            return;
        }

        eventos.forEach(evento => {
            const card = this.criarCardEvento(evento, options);
            container.appendChild(card);
        });
    }

    criarCardEvento(evento, options = {}) {
        const col = document.createElement('div');
        col.className = 'col-md-6 col-lg-4 mb-4';

        const statusClass = this.getStatusBadgeClass(evento.data);
        const statusText = this.getStatusText(evento.data);

        col.innerHTML = `
            <div class="card h-100 shadow-sm">
                <div class="card-header bg-light">
                    <div class="d-flex justify-content-between align-items-start">
                        <h6 class="card-title mb-0 text-primary">${evento.nome}</h6>
                        <span class="badge ${statusClass}">${statusText}</span>
                    </div>
                </div>
                <div class="card-body">
                    <div class="mb-3">
                        <p class="card-text text-muted small">
                            ${evento.descricao || 'Sem descrição disponível'}
                        </p>
                    </div>

                    <div class="row text-center mb-3">
                        <div class="col-6">
                            <div class="border-end">
                                <i class="fas fa-calendar-day text-primary mb-1"></i>
                                <div class="small text-muted">Data</div>
                                <div class="fw-bold">${DateUtils.formatDate(evento.data)}</div>
                            </div>
                        </div>
                        <div class="col-6">
                            <i class="fas fa-clock text-primary mb-1"></i>
                            <div class="small text-muted">Hora</div>
                            <div class="fw-bold">${evento.hora || '--:--'}</div>
                        </div>
                    </div>

                    <div class="mb-3">
                        <div class="d-flex align-items-center text-muted">
                            <i class="fas fa-map-marker-alt me-2"></i>
                            <span class="small">${evento.local}</span>
                        </div>
                    </div>

                    <div class="d-flex justify-content-between align-items-center">
                        <div class="text-muted small">
                            <i class="fas fa-users me-1"></i>
                            ${evento.numParticipantes || 0} inscritos
                        </div>
                        ${options.onInscrever ? `
                            <button class="btn btn-success btn-sm" onclick="window.inscricoesApp.abrirModalInscricao(${JSON.stringify(evento).replace(/"/g, '&quot;')})">
                                <i class="fas fa-user-plus me-1"></i>Inscrever-se
                            </button>
                        ` : ''}
                    </div>
                </div>
            </div>
        `;

        return col;
    }
}

if (typeof module !== 'undefined' && module.exports) {
    module.exports = EventList;
}