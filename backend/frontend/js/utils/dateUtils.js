/**
 * Utilitários para manipulação de datas
 */
class DateUtils {
    /**
     * Formata uma data para o formato brasileiro
     * @param {string} dateString - String da data
     * @returns {string} Data formatada
     */
    static formatDate(dateString) {
        if (!dateString) return '--/--/----';
        
        const date = new Date(dateString);
        if (isNaN(date.getTime())) return '--/--/----';
        
        return date.toLocaleDateString('pt-BR');
    }

    /**
     * Formata uma data e hora
     * @param {string} dateString - String da data
     * @param {string} timeString - String da hora
     * @returns {string} Data e hora formatadas
     */
    static formatDateTime(dateString, timeString) {
        const date = this.formatDate(dateString);
        const time = timeString || '--:--';
        return `${date} às ${time}`;
    }

    /**
     * Verifica se uma data é hoje
     * @param {string} dateString - String da data
     * @returns {boolean} True se for hoje
     */
    static isToday(dateString) {
        const date = new Date(dateString);
        const today = new Date();
        return date.toDateString() === today.toDateString();
    }

    /**
     * Verifica se uma data é próxima (próximos 7 dias)
     * @param {string} dateString - String da data
     * @returns {boolean} True se for próxima
     */
    static isUpcoming(dateString) {
        const date = new Date(dateString);
        const today = new Date();
        const nextWeek = new Date();
        nextWeek.setDate(today.getDate() + 7);
        
        return date >= today && date <= nextWeek;
    }

    /**
     * Verifica se uma data já passou
     * @param {string} dateString - String da data
     * @returns {boolean} True se já passou
     */
    static isPast(dateString) {
        const date = new Date(dateString);
        const today = new Date();
        return date < today;
    }

    /**
     * Obtém a data mínima para inputs (hoje)
     * @returns {string} Data no formato YYYY-MM-DD
     */
    static getMinDate() {
        return new Date().toISOString().split('T')[0];
    }

    /**
     * Calcula a diferença em dias entre duas datas
     * @param {string} date1 - Primeira data
     * @param {string} date2 - Segunda data
     * @returns {number} Diferença em dias
     */
    static getDaysDifference(date1, date2) {
        const d1 = new Date(date1);
        const d2 = new Date(date2);
        const diffTime = Math.abs(d2 - d1);
        return Math.ceil(diffTime / (1000 * 60 * 60 * 24));
    }
}

// Exportar utilitário
if (typeof module !== 'undefined' && module.exports) {
    module.exports = DateUtils;
} 