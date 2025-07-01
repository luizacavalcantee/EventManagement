@echo off
echo 🚀 Compilando servidor C++...

REM Verificar se o compilador está disponível
g++ --version >nul 2>&1
if errorlevel 1 (
    echo ❌ Erro: Compilador g++ não encontrado!
    echo 📥 Baixe o MinGW ou instale o Visual Studio Build Tools
    pause
    exit /b 1
)

REM Compilar o servidor
echo 📦 Compilando api_server.cpp...
g++ -std=c++17 -I. -o api_server.exe simple_cpp_server.cpp Evento.cpp Participante.cpp GerenciadorEventos.cpp -lws2_32

if errorlevel 1 (
    echo ❌ Erro na compilação!
    echo 📋 Verifique se todas as bibliotecas estão instaladas:
    echo    - httplib
    echo    - nlohmann/json
    pause
    exit /b 1
)

echo ✅ Servidor compilado com sucesso!
echo 🚀 Para executar: api_server.exe
echo 📱 Interface: http://localhost:8080/frontend/
pause 