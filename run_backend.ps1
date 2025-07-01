Write-Host "🚀 Sistema de Gerenciamento de Eventos - Backend C++" -ForegroundColor Green
Write-Host "==================================================" -ForegroundColor Green

# Navegar para a pasta backend
Set-Location backend

# Compilar o servidor
Write-Host "📦 Compilando servidor C++..." -ForegroundColor Yellow
& .\compile_simple.bat

if ($LASTEXITCODE -ne 0) {
    Write-Host "❌ Erro na compilação!" -ForegroundColor Red
    Read-Host "Pressione Enter para sair"
    exit 1
}

Write-Host ""
Write-Host "🎉 Compilação concluída!" -ForegroundColor Green
Write-Host "🚀 Iniciando servidor..." -ForegroundColor Yellow
Write-Host "📱 Interface disponível em: http://localhost:8080/frontend/" -ForegroundColor Cyan
Write-Host "🔧 Para parar o servidor, pressione Ctrl+C" -ForegroundColor Yellow
Write-Host "==================================================" -ForegroundColor Green
Write-Host ""

# Executar o servidor
& .\server_new.exe 