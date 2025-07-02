@echo off
echo 🚀 Sistema de Gerenciamento de Eventos - Backend C++
echo ==================================================

REM 1. Navegar para a pasta backend para compilar
cd backend

REM 2. Compilar o servidor
echo 📦 Compilando servidor C++...
call scripts/compile_simple.bat

if errorlevel 1 (
    echo ❌ Erro na compilação!
    pause
    exit /b 1
)

echo.
echo 🎉 Compilação concluída!

REM 3. VOLTAR PARA A PASTA RAIZ ANTES DE INICIAR O SERVIDOR
cd ..

echo 🚀 Iniciando servidor a partir da raiz do projeto...
echo 📱 Interface disponível em: http://localhost:8080/frontend/
echo 🔧 Para parar o servidor, pressione Ctrl+C
echo ==================================================
echo.

REM 4. Executar o servidor usando seu caminho a partir da raiz
backend\build\server_new.exe