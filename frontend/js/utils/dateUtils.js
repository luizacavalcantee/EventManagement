/**
 * Utilitários para manipulação de datas
 */
class DateUtils {
    /**
     * Converte uma string de data (DD/MM/YYYY ou YYYY-MM-DD) para um objeto Date
     * @param {string} dateString - String da data (ex: "07/08/2025" ou "2025-08-07")
     * @returns {Date} Objeto Date
     */
    static parseDate(dateString) {
        if (!dateString) return null;

        // Tenta parsear como DD/MM/YYYY
        if (dateString.includes('/')) {
            const parts = dateString.split('/');
            // new Date(year, monthIndex, day) - monthIndex é 0-based
            const date = new Date(parseInt(parts[2]), parseInt(parts[1]) - 1, parseInt(parts[0]));
            if (!isNaN(date.getTime())) return date;
        } 
        
        // Tenta parsear como YYYY-MM-DD (ISO 8601)
        const date = new Date(dateString);
        if (!isNaN(date.getTime())) return date;
        
        return null; // Não conseguiu parsear
    }

    /**
     * Formata uma data para o formato local (ex: brasileiro DD/MM/YYYY)
     * @param {string} dateString - String da data (DD/MM/YYYY do backend ou YYYY-MM-DD do input)
     * @param {string} locale - Localidade para formatação (ex: 'pt-BR')
     * @returns {string} Data formatada
     */
    static formatDate(dateString, locale = 'pt-BR') {
        const date = DateUtils.parseDate(dateString); // Usa o novo parseDate
        if (!date) return '--/--/----';
        
        return date.toLocaleDateString(locale);
    }

    /**
     * NOVO: Converte uma data do formato DD/MM/YYYY (do backend) para YYYY-MM-DD (para input type="date")
     * @param {string} dateString - String da data no formato DD/MM/YYYY
     * @returns {string} Data no formato YYYY-MM-DD
     */
    static formatDateForInput(dateString) {
        if (!dateString) return '';
        const parts = dateString.split('/');
        if (parts.length === 3) {
            return `${parts[2]}-${parts[1]}-${parts[0]}`; // YYYY-MM-DD
        }
        return dateString; // Retorna original se não for DD/MM/YYYY
    }

    /**
     * Formata uma data e hora
     * @param {string} dateString - String da data
     * @param {string} timeString - String da hora
     * @param {string} locale - Localidade para formatação
     * @returns {string} Data e hora formatadas
     */
    static formatDateTime(dateString, timeString, locale = 'pt-BR') {
        const date = this.formatDate(dateString, locale);
        const time = timeString || '--:--';
        return `${date} às ${time}`;
    }

    /**
     * Verifica se uma data é hoje
     * @param {string} dateString - String da data (YYYY-MM-DD ou DD/MM/YYYY)
     * @returns {boolean} True se for hoje
     */
    static isToday(dateString) {
        const date = DateUtils.parseDate(dateString);
        if (!date) return false;
        
        const today = new Date();
        // Zera as horas para comparar apenas a data
        date.setHours(0, 0, 0, 0);
        today.setHours(0, 0, 0, 0);
        return date.getTime() === today.getTime();
    }

    /**
     * Verifica se uma data está no futuro (qualquer dia após hoje)
     * @param {string} dateString - String da data (YYYY-MM-DD ou DD/MM/YYYY)
     * @returns {boolean} True se estiver no futuro
     */
    static isFuture(dateString) {
        const date = DateUtils.parseDate(dateString);
        if (!date) return false;

        const today = new Date();
        // Zera as horas para comparar apenas a data
        date.setHours(0, 0, 0, 0);
        today.setHours(0, 0, 0, 0);
        return date.getTime() > today.getTime();
    }

    /**
     * Verifica se uma data é considerada "próxima" (futura, nos próximos 7 dias)
     * @param {string} dateString - String da data (YYYY-MM-DD ou DD/MM/YYYY)
     * @returns {boolean} True se for próxima (hoje + 7 dias)
     */
    static isUpcoming(dateString) {
        const date = DateUtils.parseDate(dateString);
        if (!date) return false;

        const today = new Date();
        const nextWeek = new Date();
        nextWeek.setDate(today.getDate() + 7);
        
        // Zera as horas para comparar apenas a data
        date.setHours(0, 0, 0, 0);
        today.setHours(0, 0, 0, 0);
        nextWeek.setHours(0, 0, 0, 0);
        
        return date.getTime() >= today.getTime() && date.getTime() <= nextWeek.getTime();
    }

    /**
     * Verifica se uma data já passou (qualquer dia antes de hoje)
     * @param {string} dateString - String da data (YYYY-MM-DD ou DD/MM/YYYY)
     * @returns {boolean} True se já passou
     */
    static isPast(dateString) {
        const date = DateUtils.parseDate(dateString);
        if (!date) return false;

        const today = new Date();
        // Zera as horas para comparar apenas a data
        date.setHours(0, 0, 0, 0);
        today.setHours(0, 0, 0, 0);
        return date.getTime() < today.getTime();
    }

    /**
     * Obtém a data mínima para inputs (hoje) no formato YYYY-MM-DD
     * @returns {string} Data no formato YYYY-MM-DD
     */
    static getMinDate() {
        return new Date().toISOString().split('T')[0];
    }

    /**
     * Calcula a diferença em dias entre duas datas
     * @param {string} date1String - Primeira data (YYYY-MM-DD ou DD/MM/YYYY)
     * @param {string} date2String - Segunda data (YYYY-MM-DD ou DD/MM/YYYY)
     * @returns {number} Diferença em dias (número absoluto)
     */
    static getDaysDifference(date1String, date2String) {
        const d1 = DateUtils.parseDate(date1String);
        const d2 = DateUtils.parseDate(date2String);
        if (!d1 || !d2) return NaN;

        const diffTime = Math.abs(d2.getTime() - d1.getTime());
        return Math.ceil(diffTime / (1000 * 60 * 60 * 24));
    }
}

// Exportar utilitário
if (typeof module !== 'undefined' && module.exports) {
    module.exports = DateUtils;
}