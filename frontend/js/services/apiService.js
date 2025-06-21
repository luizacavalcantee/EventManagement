/**
 * Serviço para gerenciar chamadas da API
 */
class ApiService {
    /**
     * Faz uma requisição HTTP genérica
     * @param {string} endpoint - Endpoint da API
     * @param {Object} options - Opções da requisição
     * @returns {Promise<Object>} Resposta da API
     */
    static async request(endpoint, options = {}) {
        const url = `${CONFIG.API_BASE_URL}${endpoint}`;
        
        const defaultOptions = {
            headers: {
                'Content-Type': 'application/json',
            },
            timeout: CONFIG.REQUEST_TIMEOUT
        };

        const requestOptions = { ...defaultOptions, ...options };

        try {
            const controller = new AbortController();
            const timeoutId = setTimeout(() => controller.abort(), requestOptions.timeout);

            const response = await fetch(url, {
                ...requestOptions,
                signal: controller.signal
            });

            clearTimeout(timeoutId);

            if (!response.ok) {
                throw new Error(`HTTP error! status: ${response.status}`);
            }

            return await response.json();
        } catch (error) {
            if (error.name === 'AbortError') {
                throw new Error('Timeout na requisição');
            }
            throw error;
        }
    }

    /**
     * GET request
     * @param {string} endpoint - Endpoint da API
     * @returns {Promise<Object>} Resposta da API
     */
    static async get(endpoint) {
        return this.request(endpoint, { method: 'GET' });
    }

    /**
     * POST request
     * @param {string} endpoint - Endpoint da API
     * @param {Object} data - Dados a serem enviados
     * @returns {Promise<Object>} Resposta da API
     */
    static async post(endpoint, data) {
        return this.request(endpoint, {
            method: 'POST',
            body: JSON.stringify(data)
        });
    }

    /**
     * PUT request
     * @param {string} endpoint - Endpoint da API
     * @param {Object} data - Dados a serem enviados
     * @returns {Promise<Object>} Resposta da API
     */
    static async put(endpoint, data) {
        return this.request(endpoint, {
            method: 'PUT',
            body: JSON.stringify(data)
        });
    }

    /**
     * DELETE request
     * @param {string} endpoint - Endpoint da API
     * @returns {Promise<Object>} Resposta da API
     */
    static async delete(endpoint) {
        return this.request(endpoint, { method: 'DELETE' });
    }

    // Métodos específicos para eventos
    static async getEventos() {
        return this.get(CONFIG.ENDPOINTS.EVENTOS);
    }

    static async getEvento(id) {
        return this.get(`${CONFIG.ENDPOINTS.EVENTOS}/${id}`);
    }

    static async createEvento(evento) {
        return this.post(CONFIG.ENDPOINTS.EVENTOS, evento);
    }

    static async updateEvento(id, evento) {
        return this.put(`${CONFIG.ENDPOINTS.EVENTOS}/${id}`, evento);
    }

    static async deleteEvento(id) {
        return this.delete(`${CONFIG.ENDPOINTS.EVENTOS}/${id}`);
    }

    static async salvarEventos() {
        return this.post(CONFIG.ENDPOINTS.SALVAR);
    }

    // Métodos específicos para participantes
    static async getParticipantes(eventoId) {
        return this.get(`${CONFIG.ENDPOINTS.EVENTOS}/${eventoId}/participantes`);
    }

    static async addParticipante(eventoId, participante) {
        return this.post(`${CONFIG.ENDPOINTS.EVENTOS}/${eventoId}/participantes`, participante);
    }

    // Métodos específicos para relatórios
    static async getRelatorio() {
        return this.get(CONFIG.ENDPOINTS.RELATORIO);
    }
}

// Exportar serviço
if (typeof module !== 'undefined' && module.exports) {
    module.exports = ApiService;
} 