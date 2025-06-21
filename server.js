const express = require('express');
const cors = require('cors');
const path = require('path');

const app = express();
const PORT = 8080;

// Middleware
app.use(cors());
app.use(express.json());
app.use('/frontend', express.static(path.join(__dirname, 'frontend')));

// Dados em memória
let eventos = [];
let participantes = {};
let nextId = 1;

// Criar eventos de exemplo
function criarEventosExemplo() {
    const hoje = new Date();
    const eventosExemplo = [
        {
            id: nextId++,
            nome: "Workshop de JavaScript",
            data: new Date(hoje.getTime() + 5 * 24 * 60 * 60 * 1000).toISOString().split('T')[0],
            hora: "14:00",
            local: "Auditório Principal",
            descricao: "Workshop prático sobre JavaScript moderno",
            numParticipantes: 0
        },
        {
            id: nextId++,
            nome: "Palestra sobre React",
            data: new Date(hoje.getTime() + 10 * 24 * 60 * 60 * 1000).toISOString().split('T')[0],
            hora: "19:00",
            local: "Sala de Conferências",
            descricao: "Introdução ao React e seus conceitos fundamentais",
            numParticipantes: 0
        },
        {
            id: nextId++,
            nome: "Meetup de Desenvolvedores",
            data: new Date(hoje.getTime() - 2 * 24 * 60 * 60 * 1000).toISOString().split('T')[0],
            hora: "18:30",
            local: "Espaço Coworking",
            descricao: "Networking e troca de experiências",
            numParticipantes: 0
        }
    ];
    
    eventos = eventosExemplo;
    eventos.forEach(evento => {
        participantes[evento.id] = [];
    });
}

// Rotas da API
app.get('/eventos', (req, res) => {
    res.json(eventos);
});

app.get('/eventos/:id', (req, res) => {
    const evento = eventos.find(e => e.id == req.params.id);
    if (evento) {
        res.json(evento);
    } else {
        res.status(404).json({ error: "Evento não encontrado" });
    }
});

app.post('/eventos', (req, res) => {
    const novoEvento = {
        id: nextId++,
        nome: req.body.nome || "",
        data: req.body.data || "",
        hora: req.body.hora || "",
        local: req.body.local || "",
        descricao: req.body.descricao || "",
        numParticipantes: 0
    };
    
    eventos.push(novoEvento);
    participantes[novoEvento.id] = [];
    
    res.status(201).json(novoEvento);
});

app.put('/eventos/:id', (req, res) => {
    const evento = eventos.find(e => e.id == req.params.id);
    if (!evento) {
        return res.status(404).json({ error: "Evento não encontrado" });
    }
    
    Object.assign(evento, {
        nome: req.body.nome || evento.nome,
        data: req.body.data || evento.data,
        hora: req.body.hora || evento.hora,
        local: req.body.local || evento.local,
        descricao: req.body.descricao || evento.descricao
    });
    
    res.json(evento);
});

app.delete('/eventos/:id', (req, res) => {
    const index = eventos.findIndex(e => e.id == req.params.id);
    if (index === -1) {
        return res.status(404).json({ error: "Evento não encontrado" });
    }
    
    eventos.splice(index, 1);
    delete participantes[req.params.id];
    
    res.json({ message: "Evento deletado com sucesso" });
});

app.get('/eventos/:id/participantes', (req, res) => {
    const eventoId = parseInt(req.params.id);
    res.json(participantes[eventoId] || []);
});

app.post('/eventos/:id/participantes', (req, res) => {
    const eventoId = parseInt(req.params.id);
    if (!participantes[eventoId]) {
        return res.status(404).json({ error: "Evento não encontrado" });
    }
    
    const novoParticipante = {
        nome: req.body.nome || "",
        email: req.body.email || "",
        contato: req.body.contato || ""
    };
    
    participantes[eventoId].push(novoParticipante);
    
    // Atualizar contador
    const evento = eventos.find(e => e.id === eventoId);
    if (evento) {
        evento.numParticipantes = participantes[eventoId].length;
    }
    
    res.status(201).json(novoParticipante);
});

app.get('/relatorio', (req, res) => {
    const hoje = new Date();
    const proximo = new Date(hoje.getTime() + 7 * 24 * 60 * 60 * 1000);
    
    const eventosHoje = eventos.filter(e => e.data === hoje.toISOString().split('T')[0]);
    const eventosProximos = eventos.filter(e => {
        const dataEvento = new Date(e.data);
        return dataEvento >= hoje && dataEvento <= proximo;
    });
    
    const totalParticipantes = eventos.reduce((total, e) => total + e.numParticipantes, 0);
    
    const relatorio = {
        totalEventos: eventos.length,
        totalParticipantes,
        eventosProximos: eventosProximos.length,
        eventos,
        eventosPorStatus: {
            passados: eventos.filter(e => new Date(e.data) < hoje).length,
            hoje: eventosHoje.length,
            proximos: eventosProximos.length,
            futuros: eventos.filter(e => new Date(e.data) > proximo).length
        }
    };
    
    res.json(relatorio);
});

app.post('/eventos/salvar', (req, res) => {
    res.json({ message: "Eventos salvos com sucesso" });
});

app.get('/health', (req, res) => {
    res.json({ status: "ok", message: "Servidor funcionando!" });
});

// Rota principal
app.get('/', (req, res) => {
    res.redirect('/frontend/');
});

// Inicializar
criarEventosExemplo();

console.log('🚀 Iniciando servidor de eventos...');
console.log('📊 Criando eventos de exemplo...');
console.log(`✅ ${eventos.length} eventos criados`);
console.log(`🌐 Servidor rodando em http://localhost:${PORT}`);
console.log('📱 Interface disponível em http://localhost:8080/frontend/');
console.log('🔧 Para parar o servidor, pressione Ctrl+C');
console.log('-'.repeat(50));

app.listen(PORT, () => {
    console.log(`✅ Servidor iniciado na porta ${PORT}`);
}); 