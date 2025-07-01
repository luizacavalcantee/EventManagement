@echo off
echo 🚀 Sistema de Gerenciamento de Eventos - Backend C++
echo ==================================================

REM Navegar para a pasta backend para executar os scripts de compilacao
cd backend

REM Compilar o servidor
echo 📦 Compilando servidor C++...
call compile_simple.bat

if errorlevel 1 (
    echo ❌ Erro na compilacao!
    pause
    exit /b 1
)

REM Voltar para a pasta raiz do projeto para que o servidor encontre a pasta 'frontend'
cd ..

echo.
echo 🎉 Compilacao concluida!
echo 🚀 Iniciando servidor...
echo 📱 Interface disponivel em: http://localhost:8080/frontend/
echo 🔧 Para parar o servidor, pressione Ctrl+C
echo ==================================================
echo.

REM Executar o servidor a partir da raiz, especificando seu novo caminho dentro da pasta 'bin'
backend\bin\simple_cpp_server.exe
