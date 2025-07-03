/**
 * Configurações da aplicação
 */
const CONFIG = {
    // URL base da API
    API_BASE_URL: 'http://localhost:8080',
    
    // Configurações de timeout
    REQUEST_TIMEOUT: 10000,
    
    // Configurações de paginação
    ITEMS_PER_PAGE: 10,
    
    // Configurações de notificações
    NOTIFICATION_DURATION: 5000,
    
    // Configurações de data
    DATE_FORMAT: 'pt-BR',
    
    // Endpoints da API
    ENDPOINTS: {
        EVENTOS: '/api/eventos',
        PARTICIPANTES: '/api/participantes',
        RELATORIO: '/api/relatorio',
        SALVAR: '/api/eventos'
    }
};

// Exportar configuração
if (typeof module !== 'undefined' && module.exports) {
    module.exports = CONFIG;
} 