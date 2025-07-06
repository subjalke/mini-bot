Write-Host "=== 检查 Ollama 服务状态 ===" -ForegroundColor Green

# 检查 Ollama 进程
Write-Host "1. 检查 Ollama 进程..." -ForegroundColor Yellow
$ollamaProcess = Get-Process | Where-Object { $_.ProcessName -like "*ollama*" }
if ($ollamaProcess) {
    Write-Host "✓ Ollama 进程正在运行" -ForegroundColor Green
    $ollamaProcess | Format-Table ProcessName, Id, CPU
} else {
    Write-Host "✗ Ollama 进程未运行" -ForegroundColor Red
}

# 检查端口
Write-Host "`n2. 检查端口 11434..." -ForegroundColor Yellow
$port = netstat -an | findstr ":11434"
if ($port) {
    Write-Host "✓ 端口 11434 正在监听" -ForegroundColor Green
    Write-Host $port
} else {
    Write-Host "✗ 端口 11434 未监听" -ForegroundColor Red
}

# 测试 HTTP 连接
Write-Host "`n3. 测试 HTTP 连接..." -ForegroundColor Yellow
try {
    $response = Invoke-WebRequest -Uri "http://localhost:11434/api/tags" -Method GET -TimeoutSec 5
    Write-Host "✓ HTTP 连接成功 (状态码: $($response.StatusCode))" -ForegroundColor Green
    Write-Host "响应内容: $($response.Content)" -ForegroundColor Cyan
} catch {
    Write-Host "✗ HTTP 连接失败: $($_.Exception.Message)" -ForegroundColor Red
}

# 检查模型
Write-Host "`n4. 检查模型..." -ForegroundColor Yellow
try {
    $response = Invoke-WebRequest -Uri "http://localhost:11434/api/tags" -Method GET -TimeoutSec 5
    $models = $response.Content | ConvertFrom-Json
    if ($models.models) {
        Write-Host "✓ 可用模型:" -ForegroundColor Green
        $models.models | ForEach-Object { Write-Host "  - $($_.name)" -ForegroundColor Cyan }
        
        $deepseekModel = $models.models | Where-Object { $_.name -eq "deepseek-r1:8b" }
        if ($deepseekModel) {
            Write-Host "✓ 模型 deepseek-r1:8b 已安装" -ForegroundColor Green
        } else {
            Write-Host "✗ 模型 deepseek-r1:8b 未安装" -ForegroundColor Red
            Write-Host "请运行: ollama pull deepseek-r1:8b" -ForegroundColor Yellow
        }
    }
} catch {
    Write-Host "✗ 无法获取模型列表: $($_.Exception.Message)" -ForegroundColor Red
}

Write-Host "`n=== 检查完成 ===" -ForegroundColor Green 