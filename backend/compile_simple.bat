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

REM Compilar o servidor simplificado
echo 📦 Compilando simple_cpp_server.cpp...
g++ -std=c++17 -o simple_cpp_server.exe simple_cpp_server.cpp -lws2_32

if errorlevel 1 (
    echo ❌ Erro na compilação!
    echo 📋 Verifique se o compilador está instalado corretamente
    pause
    exit /b 1
)

echo ✅ Servidor simplificado compilado com sucesso!
echo 🚀 Para executar: simple_cpp_server.exe
echo 📱 Interface: http://localhost:8080/frontend/
echo 📝 Este servidor usa apenas bibliotecas padrão do C++
pause 