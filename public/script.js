const API_URL = 'http://localhost:8080/api';

// Elementos do DOM
const eventoForm = document.getElementById('eventoForm');
const participanteForm = document.getElementById('participanteForm');
const listaEventos = document.getElementById('listaEventos');
const eventoSelect = document.getElementById('eventoSelect');

// Carregar eventos ao iniciar
document.addEventListener('DOMContentLoaded', carregarEventos);

// Event Listeners
eventoForm.addEventListener('submit', criarEvento);
participanteForm.addEventListener('submit', adicionarParticipante);

// Funções
async function carregarEventos() {
    try {
        const response = await fetch(`${API_URL}/eventos`);
        const eventos = await response.json();
        
        // Limpar listas
        listaEventos.innerHTML = '';
        eventoSelect.innerHTML = '<option value="">Selecione um evento</option>';
        
        // Preencher lista de eventos
        eventos.forEach(evento => {
            // Adicionar ao select
            const option = document.createElement('option');
            option.value = evento.nome;
            option.textContent = evento.nome;
            eventoSelect.appendChild(option);
            
            // Criar card do evento
            const eventoCard = document.createElement('div');
            eventoCard.className = 'evento-card';
            eventoCard.innerHTML = `
                <h3>${evento.nome}</h3>
                <p><strong>Data:</strong> ${evento.data}</p>
                <p><strong>Hora:</strong> ${evento.hora}</p>
                <p><strong>Local:</strong> ${evento.local}</p>
                <p><strong>Descrição:</strong> ${evento.descricao}</p>
                <div class="participante-list">
                    <h4>Participantes (${evento.participantes.length})</h4>
                    ${evento.participantes.map(p => `
                        <div class="participante-item">
                            <div>
                                <strong>${p.nome}</strong> - ${p.email}
                                ${p.telefone ? ` - ${p.telefone}` : ''}
                            </div>
                            <button 
                                class="presenca-btn ${p.presente ? 'presente' : ''}"
                                onclick="marcarPresenca('${evento.nome}', '${p.email}')"
                            >
                                ${p.presente ? 'Presente' : 'Marcar Presença'}
                            </button>
                        </div>
                    `).join('')}
                </div>
            `;
            listaEventos.appendChild(eventoCard);
        });
    } catch (error) {
        console.error('Erro ao carregar eventos:', error);
        alert('Erro ao carregar eventos. Tente novamente.');
    }
}

async function criarEvento(e) {
    e.preventDefault();
    
    const evento = {
        nome: document.getElementById('nomeEvento').value,
        data: document.getElementById('dataEvento').value,
        hora: document.getElementById('horaEvento').value,
        local: document.getElementById('localEvento').value,
        descricao: document.getElementById('descricaoEvento').value
    };
    
    try {
        const response = await fetch(`${API_URL}/eventos`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(evento)
        });
        
        if (response.ok) {
            alert('Evento criado com sucesso!');
            eventoForm.reset();
            carregarEventos();
        } else {
            throw new Error('Erro ao criar evento');
        }
    } catch (error) {
        console.error('Erro:', error);
        alert('Erro ao criar evento. Tente novamente.');
    }
}

async function adicionarParticipante(e) {
    e.preventDefault();
    
    const participante = {
        nome: document.getElementById('nomeParticipante').value,
        email: document.getElementById('emailParticipante').value,
        telefone: document.getElementById('telefoneParticipante').value
    };
    
    const nomeEvento = eventoSelect.value;
    
    try {
        const response = await fetch(`${API_URL}/eventos/${nomeEvento}/participantes`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(participante)
        });
        
        if (response.ok) {
            alert('Participante adicionado com sucesso!');
            participanteForm.reset();
            carregarEventos();
        } else {
            throw new Error('Erro ao adicionar participante');
        }
    } catch (error) {
        console.error('Erro:', error);
        alert('Erro ao adicionar participante. Tente novamente.');
    }
}

async function marcarPresenca(nomeEvento, email) {
    try {
        const response = await fetch(`${API_URL}/eventos/${nomeEvento}/presenca`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ email })
        });
        
        if (response.ok) {
            carregarEventos();
        } else {
            throw new Error('Erro ao marcar presença');
        }
    } catch (error) {
        console.error('Erro:', error);
        alert('Erro ao marcar presença. Tente novamente.');
    }
} 