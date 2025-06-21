Write-Host ""
Write-Host "=========================================================" -ForegroundColor Green
Write-Host "  INICIANDO O SISTEMA DE GERENCIAMENTO DE EVENTOS" -ForegroundColor Green
Write-Host "=========================================================" -ForegroundColor Green
Write-Host ""

# Etapa 1: Parar o servidor antigo
Write-Host "[1/4] Parando qualquer processo do servidor anterior..." -ForegroundColor Yellow
Stop-Process -Name "simple_cpp_server" -ErrorAction SilentlyContinue
Write-Host "     - Processo finalizado (se estava rodando)."
Write-Host ""

# Etapa 2: Compilar o servidor
Write-Host "[2/4] Compilando o servidor C++..." -ForegroundColor Yellow
g++ -o simple_cpp_server.exe simple_cpp_server.cpp -lws2_32
if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "----------------------------------------------------" -ForegroundColor Red
    Write-Host "         ! ERRO DE COMPILACAO !" -ForegroundColor Red
    Write-Host "----------------------------------------------------" -ForegroundColor Red
    Write-Host "  Nao foi possivel compilar o servidor C++."
    Write-Host "  Verifique se o compilador g++ (MinGW) esta instalado."
    Write-Host ""
    Read-Host "Pressione Enter para sair"
    exit
}
Write-Host "     - Servidor compilado com sucesso!"
Write-Host ""

# Etapa 3: Iniciar o servidor em uma nova janela (em segundo plano)
Write-Host "[3/4] Iniciando o servidor C++ em uma nova janela..." -ForegroundColor Yellow
Start-Process -FilePath ".\simple_cpp_server.exe"
Write-Host "     - Servidor iniciado. Aguardando 2 segundos..."
# Dá um tempo para o servidor subir antes de abrir o navegador
Start-Sleep -Seconds 2
Write-Host ""

# Etapa 4: Abrir o navegador
Write-Host "[4/4] Abrindo a pagina do sistema no navegador..." -ForegroundColor Yellow
Start-Process "http://localhost:8080/frontend/"
Write-Host "     - Pagina aberta!"
Write-Host ""

Write-Host "=========================================================" -ForegroundColor Green
Write-Host " TUDO PRONTO! O sistema esta rodando." -ForegroundColor Green
Write-Host " - A pagina de eventos foi aberta no seu navegador."
Write-Host " - O servidor C++ esta rodando em uma janela separada."
Write-Host " - Para parar o sistema, basta fechar a janela do servidor."
Write-Host "=========================================================" -ForegroundColor Green
Write-Host "" 