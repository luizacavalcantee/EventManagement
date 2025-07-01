@echo off
echo 🚀 Compilando servidor C++ simplificado...

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
pause