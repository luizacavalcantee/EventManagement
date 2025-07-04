class DateUtils {
    static parseDate(dateString) {
        if (!dateString) {
            return null;
        }

        if (dateString.includes('-') && dateString.split('-').length === 3) {
            const parts = dateString.split('-');
            const year = parseInt(parts[0]);
            const month = parseInt(parts[1]) - 1;
            const day = parseInt(parts[2]);
            const date = new Date(year, month, day);
            if (!isNaN(date.getTime())) {
                return date;
            }
        }

        if (dateString.includes('/') && dateString.split('/').length === 3) {
            const parts = dateString.split('/');
            const day = parseInt(parts[0]);
            const month = parseInt(parts[1]) - 1;
            const year = parseInt(parts[2]);
            const date = new Date(year, month, day);
            if (!isNaN(date.getTime())) {
                return date;
            }
        }

        const date = new Date(dateString);
        if (!isNaN(date.getTime())) {
            return date;
        }
        return null;
    }

    static formatDate(dateString, locale = 'pt-BR') {
        const date = DateUtils.parseDate(dateString);
        if (!date) {
            return '--/--/----';
        }
        return date.toLocaleDateString(locale);
    }

    static formatDateForInput(dateString) {
        const date = DateUtils.parseDate(dateString);
        if (!date) return '';
        const year = date.getFullYear();
        const month = (date.getMonth() + 1).toString().padStart(2, '0');
        const day = date.getDate().toString().padStart(2, '0');
        return `${year}-${month}-${day}`;
    }

    static formatDateTime(dateString, timeString, locale = 'pt-BR') {
        const date = this.formatDate(dateString, locale);
        const time = timeString || '--:--';
        return `${date} às ${time}`;
    }

    static isToday(dateString) {
        const date = DateUtils.parseDate(dateString);
        if (!date) return false;
        const today = new Date();
        date.setHours(0, 0, 0, 0);
        today.setHours(0, 0, 0, 0);
        return date.getTime() === today.getTime();
    }

    static isFuture(dateString) {
        const date = DateUtils.parseDate(dateString);
        if (!date) return false;
        const today = new Date();
        date.setHours(0, 0, 0, 0);
        today.setHours(0, 0, 0, 0);
        return date.getTime() > today.getTime();
    }

    static isUpcoming(dateString) {
        const date = DateUtils.parseDate(dateString);
        if (!date) return false;
        const today = new Date();
        const nextWeek = new Date();
        nextWeek.setDate(today.getDate() + 7);
        date.setHours(0, 0, 0, 0);
        today.setHours(0, 0, 0, 0);
        nextWeek.setHours(0, 0, 0, 0);
        return date.getTime() >= today.getTime() && date.getTime() <= nextWeek.getTime();
    }

    static isPast(dateString) {
        const date = DateUtils.parseDate(dateString);
        if (!date) return false;
        const today = new Date();
        date.setHours(0, 0, 0, 0);
        today.setHours(0, 0, 0, 0);
        return date.getTime() < today.getTime();
    }

    static getMinDate() {
        return new Date().toISOString().split('T')[0];
    }

    static getDaysDifference(date1String, date2String) {
        const d1 = DateUtils.parseDate(date1String);
        const d2 = DateUtils.parseDate(date2String);
        if (!d1 || !d2) return NaN;
        const diffTime = Math.abs(d2.getTime() - d1.getTime());
        return Math.ceil(diffTime / (1000 * 60 * 60 * 24));
    }
}

if (typeof module !== 'undefined' && module.exports) {
    module.exports = DateUtils;
}
