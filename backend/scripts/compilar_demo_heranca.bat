@echo off
echo 🚀 Compilando e executando a demonstracao de heranca...

:: (A linha "cd .." foi REMOVIDA daqui)

:: Cria a pasta de build se não existir
if not exist build mkdir build

:: Verificar se o compilador está disponível
g++ --version >nul 2>nul
if errorlevel 1 (
    echo ❌ Erro: Compilador g++ nao encontrado!
    pause
    exit /b 1
)

:: Compilar a demonstração de herança
echo 📦 Compilando src/demo_heranca.cpp e dependencias...
g++ -std=c++17 -I./src -o ./build/demo_heranca.exe ./src/demo_heranca.cpp ./src/Pessoa.cpp ./src/Participante.cpp ./src/Organizador.cpp ./src/EventoBase.cpp ./src/Evento.cpp

if errorlevel 1 (
    echo ❌ Erro na compilacao!
    pause
    exit /b 1
)

echo ✅ Demonstracao compilada com sucesso!
echo 🚀 Executando a demonstracao...
echo ==================================================
echo.

:: Executar a demonstração a partir da pasta de build
build/demo_heranca.exe

echo.
echo ==================================================
echo ✅ Demonstracao concluida!
pause