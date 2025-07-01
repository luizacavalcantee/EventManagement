@echo off
echo 🚀 Compilando servidor C++ simplificado...

REM Verificar se o compilador está disponível
g++ --version >nul 2>&1
if errorlevel 1 (
    echo ❌ Erro: Compilador g++ não encontrado!
    echo 📥 Baixe o MinGW ou instale o Visual Studio Build Tools
    pause
    exit /b 1
)

REM Remover arquivo antigo se existir
if exist server_new.exe del server_new.exe

REM Compilar apenas o servidor simplificado (sem dependências externas)
echo 📦 Compilando simple_cpp_server.cpp...
g++ -std=c++17 -I. -o server_new.exe simple_cpp_server.cpp -lws2_32

if errorlevel 1 (
    echo ❌ Erro na compilação!
    echo 📋 Verifique se o compilador está instalado corretamente
    pause
    exit /b 1
)

echo ✅ Servidor simplificado compilado com sucesso!
echo 🚀 Para executar: server_new.exe
echo 📱 Interface: http://localhost:8080/frontend/
echo 📝 Este servidor usa apenas bibliotecas padrão do C++
pause 