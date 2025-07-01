@echo off
echo 🚀 Compilando servidor C++ completo (API)...

:: (A linha "cd .." foi REMOVIDA daqui)

:: Cria a pasta de build se não existir
if not exist build mkdir build

:: Verificar se o compilador está disponível
g++ --version >nul 2>&1
if errorlevel 1 (
    echo ❌ Erro: Compilador g++ nao encontrado!
    pause
    exit /b 1
)

:: Compilar o servidor completo
echo 📦 Compilando a API do servidor...
g++ -std=c++17 -I./src -o ./build/api_server.exe ./src/simple_cpp_server.cpp ./src/Evento.cpp ./src/Participante.cpp ./src/GerenciadorEventos.cpp -lws2_32

if errorlevel 1 (
    echo ❌ Erro na compilacao!
    pause
    exit /b 1
)

echo ✅ Servidor API compilado com sucesso!
echo 🚀 Salvo em: backend/build/api_server.exe
pause