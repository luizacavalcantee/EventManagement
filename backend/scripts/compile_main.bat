@echo off
echo 🚀 Compilando sistema principal de eventos (console)...

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

:: Compilar o sistema principal
echo 📦 Compilando src/main.cpp e dependencias...
g++ -std=c++17 -I./src -o ./build/programa.exe ./src/main.cpp ./src/GerenciadorEventos.cpp ./src/Evento.cpp ./src/EventoBase.cpp ./src/Participante.cpp ./src/Pessoa.cpp

if errorlevel 1 (
    echo ❌ Erro na compilacao!
    pause
    exit /b 1
)

echo ✅ Sistema principal compilado com sucesso!
echo 🚀 Salvo em: backend/build/programa.exe
pause