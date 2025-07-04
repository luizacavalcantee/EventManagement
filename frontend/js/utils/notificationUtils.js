class NotificationUtils {
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
        
        setTimeout(() => {
            if (alertDiv.parentNode) {
                alertDiv.remove();
            }
        }, duration);
    }

    static success(message) {
        this.showNotification(message, 'success');
    }

    static error(message) {
        this.showNotification(message, 'danger');
    }

    static warning(message) {
        this.showNotification(message, 'warning');
    }

    static info(message) {
        this.showNotification(message, 'info');
    }

    static confirm(message) {
        return new Promise((resolve) => {
            const result = confirm(message);
            resolve(result);
        });
    }

    static getIconForType(type) {
        const icons = {
            success: 'fa-check-circle',
            danger: 'fa-exclamation-circle',
            warning: 'fa-exclamation-triangle',
            info: 'fa-info-circle'
        };
        return icons[type] || icons.info;
    }

    static clearAll() {
        const notifications = document.querySelectorAll('.alert.position-fixed');
        notifications.forEach(notification => notification.remove());
    }
}

if (typeof module !== 'undefined' && module.exports) {
    module.exports = NotificationUtils;
}
