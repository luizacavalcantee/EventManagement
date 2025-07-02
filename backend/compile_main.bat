@echo off
echo 🚀 Compilando sistema principal de eventos...

REM Verificar se o compilador está disponível
g++ --version >nul 2>&1
if errorlevel 1 (
    echo ❌ Erro: Compilador g++ não encontrado!
    echo 📥 Baixe o MinGW ou instale o Visual Studio Build Tools
    pause
    exit /b 1
)

REM Remover arquivo antigo se existir
if exist programa.exe del programa.exe

REM Compilar o sistema principal
echo 📦 Compilando main.cpp...
g++ -std=c++17 -I. -o programa.exe main.cpp GerenciadorEventos.cpp Evento.cpp EventoBase.cpp Participante.cpp Pessoa.cpp

if errorlevel 1 (
    echo ❌ Erro na compilação!
    echo 📋 Verifique se todos os arquivos estão presentes
    pause
    exit /b 1
)

echo ✅ Sistema principal compilado com sucesso!
echo 🚀 Para executar: programa.exe
echo 📝 Sistema de gerenciamento de eventos via console
pause 