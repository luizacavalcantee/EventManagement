@echo off
echo 🚀 Compilando demonstração de herança em C++...

REM Verificar se o compilador está disponível
g++ --version >nul 2>&1
if errorlevel 1 (
    echo ❌ Erro: Compilador g++ não encontrado!
    echo 📥 Baixe o MinGW ou instale o Visual Studio Build Tools
    pause
    exit /b 1
)

REM Compilar a demonstração de herança
echo 📦 Compilando demo_heranca.cpp...
g++ -std=c++17 -I. -o demo_heranca.exe demo_heranca.cpp Pessoa.cpp Participante.cpp Organizador.cpp EventoBase.cpp Evento.cpp

if errorlevel 1 (
    echo ❌ Erro na compilação!
    echo 📋 Verifique se todos os arquivos estão presentes
    pause
    exit /b 1
)

echo ✅ Demonstração compilada com sucesso!
echo 🚀 Executando demonstração de herança...
echo ==================================================
echo.

REM Executar a demonstração
demo_heranca.exe

echo.
echo ==================================================
echo ✅ Demonstração concluída!
pause 