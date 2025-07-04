// Arquivo: js/admin.js

class AdminApp {
    constructor() {
        this.dashboard = new Dashboard();
        this.eventForm = new EventForm();
        this.eventList = new EventList();
        // Guarda o evento cujo modal de participantes está aberto
        this.currentEventInModal = null;
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

        // Listener para o botão "Salvar Alterações" do modal de edição
        const saveEditBtn = document.getElementById('saveEditParticipanteBtn');
        if (saveEditBtn) {
            saveEditBtn.addEventListener('click', () => this.salvarEdicaoParticipante());
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
                NotificationUtils.error(error.message || 'Erro ao deletar evento.');
            }
        }
    }

    async verParticipantes(evento) {
        try {
            // Guarda o evento atual para saber qual lista recarregar depois
            this.currentEventInModal = evento;
            const participantes = await ApiService.getParticipantes(evento.id);
            this.renderizarParticipantes(evento, participantes);
            const modal = new bootstrap.Modal(document.getElementById('participantesModal'));
            modal.show();
        } catch (error) {
            console.error('Erro ao carregar participantes:', error);
            NotificationUtils.error(error.message || 'Erro ao carregar participantes.');
        }
    }

    // --- NOVA LÓGICA DE EDIÇÃO DE PARTICIPANTE ---

    /**
     * Passo 1: Abre o modal de edição com os dados do participante.
     */
    async editarParticipante(eventoId, participanteId) {
        try {
            const participante = await ApiService.getParticipanteById(eventoId, participanteId);

            // Preenche os campos do formulário de edição
            document.getElementById('editParticipanteEventoId').value = eventoId;
            document.getElementById('editParticipanteId').value = participante.id;
            document.getElementById('editNomeParticipante').value = participante.nome;
            document.getElementById('editEmailParticipante').value = participante.email;
            document.getElementById('editContatoParticipante').value = participante.contato || '';

            // Abre o modal de edição
            const editModal = new bootstrap.Modal(document.getElementById('editParticipanteModal'));
            editModal.show();
        } catch (error) {
            console.error('Erro ao buscar participante para edição:', error);
            NotificationUtils.error(error.message || 'Não foi possível carregar os dados do participante.');
        }
    }

    // Arquivo: js/admin.js

    // Arquivo: js/admin.js

    async salvarEdicaoParticipante() {
        console.log("--- DEBUG: A função salvarEdicaoParticipante FOI CHAMADA ---");
        
        try {
            const eventoIdInput = document.getElementById('editParticipanteEventoId');
            const participanteIdInput = document.getElementById('editParticipanteId');

            console.log("--- DEBUG: Verificando inputs ocultos ---");
            console.log("Elemento do Evento ID:", eventoIdInput);
            console.log("Elemento do Participante ID:", participanteIdInput);

            if (!eventoIdInput || !participanteIdInput) {
                console.error("ERRO CRÍTICO: Não foi possível encontrar os inputs ocultos #editParticipanteEventoId ou #editParticipanteId no HTML!");
                NotificationUtils.error("Erro de interface: IDs do formulário não encontrados.");
                return;
            }

            const eventoId = eventoIdInput.value;
            const participanteId = participanteIdInput.value; // Declaração da variável

            console.log(`--- DEBUG: Valores lidos -> eventoId: ${eventoId}, participanteId: ${participanteId}`);

            const dadosAtualizados = {
                nome: document.getElementById('editNomeParticipante').value.trim(),
                email: document.getElementById('editEmailParticipante').value.trim(),
                contato: document.getElementById('editContatoParticipante').value.trim()
            };
            
            console.log("--- DEBUG: Dados a serem enviados:", dadosAtualizados);

            if (!dadosAtualizados.nome || !dadosAtualizados.email) {
                NotificationUtils.warning('Nome e Email são obrigatórios.');
                return;
            }

            await ApiService.updateParticipante(eventoId, participanteId, dadosAtualizados);
            NotificationUtils.success('Participante atualizado com sucesso!');

            const editModalEl = document.getElementById('editParticipanteModal');
            const editModalInstance = bootstrap.Modal.getInstance(editModalEl);
            if (editModalInstance) {
                editModalInstance.hide();
            }
            
            this.refreshParticipantsModal();

        } catch (error) {
            console.error("--- ERRO OCORREU DENTRO DE salvarEdicaoParticipante ---", error);
            NotificationUtils.error(error.message || 'Falha ao salvar. Tente novamente.');
        }
    }
    /**
     * Passo 3: Helper para recarregar a lista de participantes no modal.
     */
    async refreshParticipantsModal() {
        if (this.currentEventInModal) {
            try {
                const participantes = await ApiService.getParticipantes(this.currentEventInModal.id);
                this.renderizarParticipantes(this.currentEventInModal, participantes);
            } catch (error) {
                console.error('Erro ao recarregar lista de participantes:', error);
            }
        }
    }

    // --- FIM DA NOVA LÓGICA ---

    async deletarParticipante(eventoId, participanteId, participanteNome) {
        if (confirm(`Tem certeza que deseja remover o participante "${participanteNome}"?`)) {
            try {
                await ApiService.deleteParticipante(eventoId, participanteId);
                NotificationUtils.success('Participante removido com sucesso!');
                this.refreshParticipantsModal(); // Atualiza o modal de participantes
                this.carregarDados(); // Atualiza a contagem na lista de eventos
            } catch (error) {
                console.error('Erro ao remover participante:', error);
                NotificationUtils.error(error.message || 'Erro ao remover participante.');
            }
        }
    }

    renderizarParticipantes(evento, participantes) {
        const container = document.getElementById('participantesList');

        if (!participantes || participantes.length === 0) {
            container.innerHTML = `
                <div class="text-center py-4">
                    <i class="fas fa-users fa-3x text-muted mb-3"></i>
                    <h5 class="text-muted">Nenhum participante inscrito</h5>
                </div>`;
            return;
        }

        const participantesHtml = participantes.map(p => `
            <tr>
                <td>
                    <div class="d-flex align-items-center">
                        <div class="avatar-sm bg-primary text-white rounded-circle d-flex align-items-center justify-content-center me-2">
                            ${p.nome.charAt(0).toUpperCase()}
                        </div>
                        ${p.nome}
                    </div>
                </td>
                <td><a href="mailto:${p.email}" class="text-decoration-none">${p.email}</a></td>
                <td>${p.contato || '<span class="text-muted">Não informado</span>'}</td>
                <td class="action-buttons">
                    <div class="btn-group">
                        <button class="btn btn-sm btn-outline-warning" 
                            onclick="window.adminApp.editarParticipante('${evento.id}', '${p.id}')" 
                            title="Editar Participante">
                            <i class="fas fa-edit"></i>
                        </button>
                        <button class="btn btn-sm btn-outline-danger" 
                            onclick="window.adminApp.deletarParticipante('${evento.id}', '${p.id}', '${p.nome.replace(/'/g, "\\'")}')" 
                            title="Excluir Participante">
                            <i class="fas fa-trash"></i>
                        </button>
                    </div>
                </td>
            </tr>
        `).join('');

        container.innerHTML = `
            <div class="mb-3">
                <h6 class="text-primary"><i class="fas fa-calendar me-2"></i>${evento.nome}</h6>
            </div>
            <div class="table-responsive">
                <table class="table table-hover">
                    <thead class="table-light">
                        <tr>
                            <th><i class="fas fa-user me-1"></i>Nome</th>
                            <th><i class="fas fa-envelope me-1"></i>Email</th>
                            <th><i class="fas fa-phone me-1"></i>Contato</th>
                            <th>Ações</th>
                        </tr>
                    </thead>
                    <tbody>${participantesHtml}</tbody>
                </table>
            </div>
            <div class="mt-3">
                <span class="badge bg-primary">${participantes.length} participante(s)</span>
            </div>`;
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