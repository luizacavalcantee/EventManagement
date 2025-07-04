class ApiService {
  static async request(endpoint, options = {}) {
    const url = `${CONFIG.API_BASE_URL}${endpoint}`;

    const defaultOptions = {
      headers: {
        "Content-Type": "application/json",
      },
      timeout: CONFIG.REQUEST_TIMEOUT,
    };

    const requestOptions = { ...defaultOptions, ...options };

    try {
      const controller = new AbortController();
      const timeoutId = setTimeout(
        () => controller.abort(),
        requestOptions.timeout
      );

      const response = await fetch(url, {
        ...requestOptions,
        signal: controller.signal,
      });

      clearTimeout(timeoutId);

      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }

      return await response.json();
    } catch (error) {
      if (error.name === "AbortError") {
        throw new Error("Timeout na requisição");
      }
      throw error;
    }
  }

  static async get(endpoint) {
    return this.request(endpoint, { method: "GET" });
  }

  static async post(endpoint, data) {
    return this.request(endpoint, {
      method: "POST",
      body: JSON.stringify(data),
    });
  }

  static async put(endpoint, data) {
    return this.request(endpoint, {
      method: "PUT",
      body: JSON.stringify(data),
    });
  }

  static async delete(endpoint) {
    return this.request(endpoint, { method: "DELETE" });
  }

  static async getEventos() {
    return this.get(CONFIG.ENDPOINTS.EVENTOS);
  }

  static async getEvento(id) {
    return this.get(`${CONFIG.ENDPOINTS.EVENTOS}/${id}`);
  }

  static async createEvento(evento) {
    return this.post(CONFIG.ENDPOINTS.EVENTOS, evento);
  }

  static async updateEvento(id, evento) {
    return this.put(`${CONFIG.ENDPOINTS.EVENTOS}/${id}`, evento);
  }

  static async deleteEvento(id) {
    return this.delete(`${CONFIG.ENDPOINTS.EVENTOS}/${id}`);
  }

  static async getParticipantes(eventoId) {
    return this.get(`${CONFIG.ENDPOINTS.EVENTOS}/${eventoId}/participantes`);
  }

  static async addParticipante(eventoId, participante) {
    return this.post(
      `${CONFIG.ENDPOINTS.EVENTOS}/${eventoId}/participantes`,
      participante
    );
  }

  static async getRelatorio() {
    return this.get(CONFIG.ENDPOINTS.RELATORIO);
  }

  static async updateParticipante(eventoId, participanteId, data) {
    return this.put(
      `${CONFIG.ENDPOINTS.EVENTOS}/${eventoId}/participantes/${participanteId}`,
      data
    );
  }

  static async deleteParticipante(eventoId, participanteId) {
    return this.delete(
      `${CONFIG.ENDPOINTS.EVENTOS}/${eventoId}/participantes/${participanteId}`
    );
  }
}

if (typeof module !== "undefined" && module.exports) {
  module.exports = ApiService;
}