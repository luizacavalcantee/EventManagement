@echo off
echo 🚀 Compilando servidor C++ simplificado...

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

REM Compilar o servidor simplificado, especificando os caminhos de origem e destino
echo 📦 Compilando src\simple_cpp_server.cpp...
g++ -std=c++17 -o bin\simple_cpp_server.exe src\simple_cpp_server.cpp -lws2_32

if errorlevel 1 (
    echo ❌ Erro na compilacao!
    echo 📋 Verifique se o compilador esta instalado corretamente
    pause
    exit /b 1
)

echo ✅ Servidor simplificado compilado com sucesso em 'bin\'!
echo 🚀 Para executar, use o script 'run_backend.bat' ou execute 'backend\bin\simple_cpp_server.exe' da raiz do projeto.
pause
