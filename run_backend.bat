@echo off
REM Define a página de código do console para UTF-8 para exibir caracteres especiais corretamente.
chcp 65001 > nul
cls

echo 🚀 Sistema de Gerenciamento de Eventos - Backend C++
echo ==================================================

echo 📦 Compilando backend C++...

:: 1. Navegar para a pasta backend para compilar
cd backend

:: Cria a pasta de build se não existir
if not exist build mkdir build

:: Verificar se o compilador está disponível
g++ --version >nul 2>&1
if errorlevel 1 (
    echo ❌ Erro: Compilador g++ nao encontrado!
    pause
    exit /b 1
)

:: Compilar todos os arquivos .cpp
:: -std=c++17: Usa o padrão C++17
:: -I./src/include: Adiciona o diretório include para buscar cabeçalhos (como json.hpp e os seus .h)
:: -o ./build/backend.exe: Define o nome do executável de saída
:: ./src/*.cpp: Compila todos os arquivos .cpp dentro de src/
:: -lws2_32: Linka a biblioteca Winsock no Windows
echo 📦 Compilando arquivos fonte...
g++ -std=c++17 -I./src/include -o ./build/backend.exe ./src/ApiServer.cpp ./src/Event.cpp ./src/EventManager.cpp ./src/main.cpp ./src/Participant.cpp ./src/Person.cpp -lws2_32

if errorlevel 1 (
    echo ❌ Erro na compilacao!
    pause
    exit /b 1
)

echo ✅ Backend compilado com sucesso!
echo 🚀 Salvo em: backend/build/backend.exe

echo.
echo 🎉 Compilação concluída!

REM VOLTAR PARA A PASSTA RAIZ ANTES DE INICIAR O SERVIDOR
cd ..

echo 🚀 Iniciando servidor a partir da raiz do projeto...
echo 📱 Interface disponível em: http://localhost:8080/frontend/
echo 🔧 Para parar o servidor, pressione Ctrl+C
echo ==================================================
echo.

REM Executar o servidor usando seu caminho a partir da raiz
backend\build\backend.exe