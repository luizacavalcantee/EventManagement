@echo off
echo 🚀 Sistema de Gerenciamento de Eventos - Backend C++
echo ==================================================

REM **INÍCIO DA PARTE DE COMPILAÇÃO (antes era compile_simple.bat)**
echo 📦 Compilando servidor C++ simplificado...

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

:: Compilar o servidor simplificado
echo 📦 Compilando src/simple_cpp_server.cpp...
g++ -std=c++17 -I./src -o ./build/server_new.exe ./src/simple_cpp_server.cpp -lws2_32

if errorlevel 1 (
    echo ❌ Erro na compilacao!
    pause
    exit /b 1
)

echo ✅ Servidor simplificado compilado com sucesso!
echo 🚀 Salvo em: backend/build/server_new.exe

:: O 'pause' após a compilação pode ser útil para ver a mensagem,
:: mas pode ser removido se o objetivo é apenas continuar.
:: pause

REM **FIM DA PARTE DE COMPILAÇÃO**

echo.
echo 🎉 Compilação concluída!

REM VOLTAR PARA A PASTA RAIZ ANTES DE INICIAR O SERVIDOR
cd ..

echo 🚀 Iniciando servidor a partir da raiz do projeto...
echo 📱 Interface disponível em: http://localhost:8080/frontend/
echo 🔧 Para parar o servidor, pressione Ctrl+C
echo ==================================================
echo.

REM Executar o servidor usando seu caminho a partir da raiz
backend\build\server_new.exe