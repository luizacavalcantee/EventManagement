/**
 * Script principal da página de inscrições
 */
class InscricoesApp {
    constructor() {
        this.eventoList = new EventoList();
        this.eventos = [];
        this.eventoSelecionado = null;
        this.filtros = {
            status: '',
            local: '',
            ordenacao: 'data',
            busca: ''
        };
        this.init();
    }

    /**
     * Inicializa a aplicação
     */
    async init() {
        try {
            // Configurar event listeners
            this.setupEventListeners();
            
            // Carregar eventos
            await this.carregarEventos();
            
        } catch (error) {
            console.error('Erro ao inicializar aplicação:', error);
            NotificationUtils.error('Erro ao carregar eventos. Verifique se o servidor está rodando.');
        }
    }

    /**
     * Configura os event listeners
     */
    setupEventListeners() {
        // Busca
        const searchInput = document.getElementById('searchInput');
        if (searchInput) {
            searchInput.addEventListener('input', (e) => {
                this.filtros.busca = e.target.value;
                this.aplicarFiltros();
            });
        }

        // Filtros
        const filterStatus = document.getElementById('filterStatus');
        if (filterStatus) {
            filterStatus.addEventListener('change', (e) => {
                this.filtros.status = e.target.value;
                this.aplicarFiltros();
            });
        }

        const filterLocal = document.getElementById('filterLocal');
        if (filterLocal) {
            filterLocal.addEventListener('change', (e) => {
                this.filtros.local = e.target.value;
                this.aplicarFiltros();
            });
        }

        const sortBy = document.getElementById('sortBy');
        if (sortBy) {
            sortBy.addEventListener('change', (e) => {
                this.filtros.ordenacao = e.target.value;
                this.aplicarFiltros();
            });
        }
    }

    /**
     * Carrega os eventos do servidor
     */
    async carregarEventos() {
        try {
            this.mostrarLoading(true);
            
            const eventos = await ApiService.getEventos();
            this.eventos = eventos;
            
            this.atualizarFiltros(eventos);
            this.aplicarFiltros();
            this.atualizarContador();
            
        } catch (error) {
            console.error('Erro ao carregar eventos:', error);
            NotificationUtils.error('Erro ao carregar eventos. Verifique se o servidor está rodando.');
            this.mostrarNoEventos();
        } finally {
            this.mostrarLoading(false);
        }
    }

    /**
     * Mostra/esconde o loading
     */
    mostrarLoading(mostrar) {
        const loading = document.getElementById('loadingEventos');
        const lista = document.getElementById('eventosList');
        const noEventos = document.getElementById('noEventos');
        
        if (mostrar) {
            loading.style.display = 'block';
            lista.style.display = 'none';
            noEventos.style.display = 'none';
        } else {
            loading.style.display = 'none';
            lista.style.display = 'block';
        }
    }

    /**
     * Mostra mensagem de "nenhum evento"
     */
    mostrarNoEventos() {
        const noEventos = document.getElementById('noEventos');
        const lista = document.getElementById('eventosList');
        
        noEventos.style.display = 'block';
        lista.style.display = 'none';
    }

    /**
     * Atualiza os filtros disponíveis
     */
    atualizarFiltros(eventos) {
        // Atualizar filtro de locais
        const filterLocal = document.getElementById('filterLocal');
        if (filterLocal) {
            const locais = [...new Set(eventos.map(e => e.local))].sort();
            
            // Manter a opção atual se ainda existir
            const valorAtual = filterLocal.value;
            
            filterLocal.innerHTML = '<option value="">Todos os locais</option>';
            locais.forEach(local => {
                const option = document.createElement('option');
                option.value = local;
                option.textContent = local;
                if (local === valorAtual) {
                    option.selected = true;
                }
                filterLocal.appendChild(option);
            });
        }
    }

    /**
     * Aplica os filtros aos eventos
     */
    aplicarFiltros() {
        let eventosFiltrados = [...this.eventos];

        // Filtro de busca
        if (this.filtros.busca) {
            const busca = this.filtros.busca.toLowerCase();
            eventosFiltrados = eventosFiltrados.filter(evento => 
                evento.nome.toLowerCase().includes(busca) ||
                evento.local.toLowerCase().includes(busca) ||
                (evento.descricao && evento.descricao.toLowerCase().includes(busca))
            );
        }

        // Filtro de status
        if (this.filtros.status) {
            const hoje = new Date().toISOString().split('T')[0];
            const proximo = new Date(Date.now() + 7 * 24 * 60 * 60 * 1000).toISOString().split('T')[0];
            
            switch (this.filtros.status) {
                case 'hoje':
                    eventosFiltrados = eventosFiltrados.filter(e => e.data === hoje);
                    break;
                case 'proximos':
                    eventosFiltrados = eventosFiltrados.filter(e => 
                        e.data >= hoje && e.data <= proximo
                    );
                    break;
                case 'futuros':
                    eventosFiltrados = eventosFiltrados.filter(e => e.data > proximo);
                    break;
            }
        }

        // Filtro de local
        if (this.filtros.local) {
            eventosFiltrados = eventosFiltrados.filter(e => e.local === this.filtros.local);
        }

        // Ordenação
        eventosFiltrados.sort((a, b) => {
            switch (this.filtros.ordenacao) {
                case 'nome':
                    return a.nome.localeCompare(b.nome);
                case 'local':
                    return a.local.localeCompare(b.local);
                case 'data':
                default:
                    return new Date(a.data) - new Date(b.data);
            }
        });

        // Renderizar eventos filtrados
        this.renderizarEventos(eventosFiltrados);
        this.atualizarContador(eventosFiltrados.length);
    }

    /**
     * Renderiza a lista de eventos
     */
    renderizarEventos(eventos) {
        if (eventos.length === 0) {
            this.mostrarNoEventos();
            return;
        }

        this.eventoList.renderizarEventos(eventos, {
            mostrarAcoes: false,
            mostrarParticipantes: false,
            onInscrever: (evento) => this.abrirModalInscricao(evento)
        });
    }

    /**
     * Atualiza o contador de eventos
     */
    atualizarContador(total = null) {
        const contador = document.getElementById('totalEventos');
        if (contador) {
            contador.textContent = total !== null ? total : this.eventos.length;
        }
    }

    /**
     * Abre o modal de inscrição
     */
    abrirModalInscricao(evento) {
        this.eventoSelecionado = evento;
        this.preencherInfoEvento(evento);
        
        const modal = new bootstrap.Modal(document.getElementById('inscricaoModal'));
        modal.show();
    }

    /**
     * Preenche as informações do evento no modal
     */
    preencherInfoEvento(evento) {
        const container = document.getElementById('eventoInfo');
        
        container.innerHTML = `
            <div class="card border-primary">
                <div class="card-body">
                    <h6 class="card-title text-primary">
                        <i class="fas fa-calendar me-2"></i>${evento.nome}
                    </h6>
                    <div class="row">
                        <div class="col-md-6">
                            <p class="mb-1">
                                <i class="fas fa-map-marker-alt me-2 text-muted"></i>
                                <strong>Local:</strong> ${evento.local}
                            </p>
                            <p class="mb-1">
                                <i class="fas fa-calendar-day me-2 text-muted"></i>
                                <strong>Data:</strong> ${DateUtils.formatarData(evento.data)}
                            </p>
                        </div>
                        <div class="col-md-6">
                            <p class="mb-1">
                                <i class="fas fa-clock me-2 text-muted"></i>
                                <strong>Hora:</strong> ${evento.hora || 'Não informada'}
                            </p>
                            <p class="mb-1">
                                <i class="fas fa-users me-2 text-muted"></i>
                                <strong>Inscritos:</strong> ${evento.numParticipantes}
                            </p>
                        </div>
                    </div>
                    ${evento.descricao ? `
                        <hr>
                        <p class="mb-0">
                            <i class="fas fa-info-circle me-2 text-muted"></i>
                            <strong>Descrição:</strong> ${evento.descricao}
                        </p>
                    ` : ''}
                </div>
            </div>
        `;
    }

    /**
     * Inscreve um participante no evento
     */
    async inscreverParticipante() {
        if (!this.eventoSelecionado) {
            NotificationUtils.error('Nenhum evento selecionado.');
            return;
        }

        const nome = document.getElementById('nomeParticipante').value.trim();
        const email = document.getElementById('emailParticipante').value.trim();
        const contato = document.getElementById('contatoParticipante').value.trim();

        if (!nome || !email) {
            NotificationUtils.warning('Por favor, preencha nome e email.');
            return;
        }

        if (!this.validarEmail(email)) {
            NotificationUtils.warning('Por favor, insira um email válido.');
            return;
        }

        try {
            const participante = { nome, email, contato };
            await ApiService.addParticipante(this.eventoSelecionado.id, participante);
            
            NotificationUtils.success('Inscrição realizada com sucesso!');
            
            // Fechar modal e limpar formulário
            const modal = bootstrap.Modal.getInstance(document.getElementById('inscricaoModal'));
            modal.hide();
            
            document.getElementById('inscricaoForm').reset();
            
            // Recarregar eventos para atualizar contadores
            await this.carregarEventos();
            
        } catch (error) {
            console.error('Erro ao inscrever participante:', error);
            NotificationUtils.error('Erro ao realizar inscrição. Tente novamente.');
        }
    }

    /**
     * Valida formato de email
     */
    validarEmail(email) {
        const re = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
        return re.test(email);
    }
}

// Funções globais
function carregarEventos() {
    if (window.inscricoesApp) {
        window.inscricoesApp.carregarEventos();
    }
}

function limparFiltros() {
    if (window.inscricoesApp) {
        // Limpar campos
        document.getElementById('searchInput').value = '';
        document.getElementById('filterStatus').value = '';
        document.getElementById('filterLocal').value = '';
        document.getElementById('sortBy').value = 'data';
        
        // Limpar filtros
        window.inscricoesApp.filtros = {
            status: '',
            local: '',
            ordenacao: 'data',
            busca: ''
        };
        
        // Aplicar filtros
        window.inscricoesApp.aplicarFiltros();
    }
}

function inscreverParticipante() {
    if (window.inscricoesApp) {
        window.inscricoesApp.inscreverParticipante();
    }
}

// Inicializar aplicação quando o DOM estiver pronto
document.addEventListener('DOMContentLoaded', () => {
    window.inscricoesApp = new InscricoesApp();
}); 