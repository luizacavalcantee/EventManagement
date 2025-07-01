@echo off
echo 🚀 Compilando servidor C++ completo...

REM Garante que as pastas de saida existam
if not exist bin mkdir bin

REM Verificar se o compilador g++ esta disponivel
g++ --version >nul 2>&1
if errorlevel 1 (
    echo ❌ Erro: Compilador g++ nao encontrado!
    echo 📥 Baixe o MinGW ou instale o Visual Studio Build Tools
    pause
    exit /b 1
)

REM Compilar o servidor completo, juntando todos os fontes da pasta 'src'
echo 📦 Compilando todos os fontes de 'src\'...
g++ -std=c++17 -Isrc -o bin\programa.exe src\main.cpp src\Evento.cpp src\Participante.cpp src\GerenciadorEventos.cpp -lws2_32

REM Nota: Se voce tiver um 'api_server.cpp' separado, use a linha abaixo em vez da de cima.
REM g++ -std=c++17 -Isrc -o bin\api_server.exe src\api_server.cpp src\Evento.cpp src\Participante.cpp src\GerenciadorEventos.cpp -lws2_32


if errorlevel 1 (
    echo ❌ Erro na compilacao!
    echo 📋 Verifique se todas as bibliotecas e arquivos de codigo estao corretos.
    pause
    exit /b 1
)

echo ✅ Servidor completo compilado com sucesso em 'bin\'!
echo 🚀 Para executar, use o script 'run_backend.bat' ou execute 'backend\bin\programa.exe' da raiz do projeto.
pause
