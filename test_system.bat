@echo off
echo ========================================
echo    TESTE DO SISTEMA DE EVENTOS
echo ========================================
echo.

echo [1/4] Verificando se o servidor esta rodando...
curl -s http://localhost:8080/health > nul
if %errorlevel% equ 0 (
    echo ✅ Servidor C++ rodando na porta 8080
) else (
    echo ❌ Servidor nao esta rodando
    echo    Execute: .\simple_cpp_server.exe
    pause
    exit /b 1
)

echo.
echo [2/4] Testando API de eventos...
curl -s http://localhost:8080/api/eventos > eventos_test.json
if %errorlevel% equ 0 (
    echo ✅ API de eventos funcionando
) else (
    echo ❌ Erro na API de eventos
)

echo.
echo [3/4] Verificando arquivos das paginas...
if exist "frontend\index.html" (
    echo ✅ Pagina de inscricoes encontrada
) else (
    echo ❌ Pagina de inscricoes nao encontrada
)

if exist "frontend\admin.html" (
    echo ✅ Pagina de administracao encontrada
) else (
    echo ❌ Pagina de administracao nao encontrada
)

echo.
echo [4/4] URLs de acesso:
echo.
echo 📱 Pagina de Inscricoes:
echo    http://localhost:8080/frontend/
echo.
echo ⚙️  Pagina de Administracao:
echo    http://localhost:8080/frontend/admin.html
echo.
echo ========================================
echo    SISTEMA PRONTO PARA USO!
echo ========================================
echo.
echo Instrucoes:
echo 1. Abra o navegador
echo 2. Acesse uma das URLs acima
echo 3. Teste as funcionalidades
echo.
echo Para parar o servidor: Ctrl+C
echo.
pause 