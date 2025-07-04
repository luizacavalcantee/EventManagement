class Dashboard {
    atualizarDashboard(relatorio) {
        if (!relatorio) {
            console.warn('Dashboard: Relatório nulo ou indefinido recebido.');
            return;
        }
        
        document.getElementById('totalEventos').textContent = relatorio.totalEventos || 0;
        document.getElementById('totalParticipantes').textContent = relatorio.totalParticipantes || 0;
        document.getElementById('eventosProximos').textContent = relatorio.eventosProximos || 0;
        document.getElementById('eventosHoje').textContent = relatorio.eventosHoje || 0;
    }
}

if (typeof module !== 'undefined' && module.exports) {
    module.exports = Dashboard;
}