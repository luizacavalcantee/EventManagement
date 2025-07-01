/**
 * Utilitários para notificações e alertas
 */
class NotificationUtils {
    /**
     * Mostra uma notificação toast
     * @param {string} message - Mensagem a ser exibida
     * @param {string} type - Tipo da notificação (success, danger, warning, info)
     * @param {number} duration - Duração em milissegundos
     */
    static showNotification(message, type = 'info', duration = CONFIG.NOTIFICATION_DURATION) {
        const alertDiv = document.createElement('div');
        alertDiv.className = `alert alert-${type} alert-dismissible fade show position-fixed`;
        alertDiv.style.cssText = 'top: 20px; right: 20px; z-index: 9999; min-width: 300px; max-width: 400px;';
        alertDiv.innerHTML = `
            <div class="d-flex align-items-center">
                <i class="fas ${this.getIconForType(type)} me-2"></i>
                <span>${message}</span>
            </div>
            <button type="button" class="btn-close" data-bs-dismiss="alert"></button>
        `;
        
        document.body.appendChild(alertDiv);
        
        // Auto-remove após o tempo especificado
        setTimeout(() => {
            if (alertDiv.parentNode) {
                alertDiv.remove();
            }
        }, duration);
    }

    /**
     * Mostra uma notificação de sucesso
     * @param {string} message - Mensagem a ser exibida
     */
    static success(message) {
        this.showNotification(message, 'success');
    }

    /**
     * Mostra uma notificação de erro
     * @param {string} message - Mensagem a ser exibida
     */
    static error(message) {
        this.showNotification(message, 'danger');
    }

    /**
     * Mostra uma notificação de aviso
     * @param {string} message - Mensagem a ser exibida
     */
    static warning(message) {
        this.showNotification(message, 'warning');
    }

    /**
     * Mostra uma notificação informativa
     * @param {string} message - Mensagem a ser exibida
     */
    static info(message) {
        this.showNotification(message, 'info');
    }

    /**
     * Mostra uma confirmação
     * @param {string} message - Mensagem de confirmação
     * @returns {Promise<boolean>} Promise que resolve para true se confirmado
     */
    static confirm(message) {
        return new Promise((resolve) => {
            const result = confirm(message);
            resolve(result);
        });
    }

    /**
     * Obtém o ícone apropriado para cada tipo de notificação
     * @param {string} type - Tipo da notificação
     * @returns {string} Classe do ícone
     */
    static getIconForType(type) {
        const icons = {
            success: 'fa-check-circle',
            danger: 'fa-exclamation-circle',
            warning: 'fa-exclamation-triangle',
            info: 'fa-info-circle'
        };
        return icons[type] || icons.info;
    }

    /**
     * Limpa todas as notificações
     */
    static clearAll() {
        const notifications = document.querySelectorAll('.alert.position-fixed');
        notifications.forEach(notification => notification.remove());
    }
}

// Exportar utilitário
if (typeof module !== 'undefined' && module.exports) {
    module.exports = NotificationUtils;
} 