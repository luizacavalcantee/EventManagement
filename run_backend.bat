@echo off
echo 🚀 Sistema de Gerenciamento de Eventos - Backend C++
echo ==================================================

REM Navegar para a pasta backend
cd backend

REM Compilar o servidor
echo 📦 Compilando servidor C++...
call compile_simple.bat

if errorlevel 1 (
    echo ❌ Erro na compilação!
    pause
    exit /b 1
)

echo.
echo 🎉 Compilação concluída!
echo 🚀 Iniciando servidor...
echo 📱 Interface disponível em: http://localhost:8080/frontend/
echo 🔧 Para parar o servidor, pressione Ctrl+C
echo ==================================================
echo.

REM Executar o servidor
simple_cpp_server.exe 