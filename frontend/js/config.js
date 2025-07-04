const CONFIG = {
    API_BASE_URL: 'http://localhost:8080', 
    REQUEST_TIMEOUT: 10000, 
    ITEMS_PER_PAGE: 10,
    NOTIFICATION_DURATION: 5000,
    DATE_FORMAT: 'pt-BR',
    ENDPOINTS: {
        EVENTOS: '/api/eventos',
        PARTICIPANTES: '/api/eventos',
        RELATORIO: '/api/relatorio',
        HEALTH: '/health'
    },
    SELECTORS: {
    },
    MESSAGES: {
    }
};

if (typeof module !== 'undefined' && module.exports) {
    module.exports = CONFIG;
}