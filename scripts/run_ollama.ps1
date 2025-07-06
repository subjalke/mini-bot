# 尝试多个可能的 Ollama 安装路径
$possiblePaths = @(
    "C:\Users\天选\AppData\Local\Programs\Ollama\ollama.exe",
    "C:\Users\$env:USERNAME\AppData\Local\Programs\Ollama\ollama.exe",
    "C:\Program Files\Ollama\ollama.exe",
    "C:\Program Files (x86)\Ollama\ollama.exe",
    "ollama.exe"  # 如果在 PATH 中
)

$OllamaExe = $null
foreach ($path in $possiblePaths) {
    if (Test-Path $path) {
        $OllamaExe = $path
        Write-Host "找到 Ollama: $path" -ForegroundColor Green
        break
    }
}

if (-not $OllamaExe) {
    Write-Host "错误：找不到 Ollama 可执行文件" -ForegroundColor Red
    Write-Host "请确保 Ollama 已正确安装" -ForegroundColor Yellow
    Write-Host "下载地址：https://ollama.ai/download" -ForegroundColor Cyan
    exit 1
}

$ModelName = "deepseek-coder:6.7b"  # 你要加载的模型名

Write-Host "使用 Ollama 路径: $OllamaExe" -ForegroundColor Green

# 检查 Ollama 服务是否已在运行
$service = Get-Process | Where-Object { $_.ProcessName -like "ollama*" }
if ($service) { 
    Write-Host "✓ Ollama 服务已在运行" -ForegroundColor Green
} else {
    Write-Host "正在启动 Ollama 服务..." -ForegroundColor Yellow
    try {
        Start-Process -NoNewWindow -FilePath $OllamaExe -ArgumentList "serve" 
        Start-Sleep -Seconds 3
        Write-Host "✓ Ollama 服务已启动" -ForegroundColor Green
    } catch {
        Write-Host "✗ 启动 Ollama 服务失败: $($_.Exception.Message)" -ForegroundColor Red
        exit 1
    }
}

# 检查模型是否已安装
Write-Host "检查模型 [$ModelName]..." -ForegroundColor Yellow
try {
    $response = Invoke-WebRequest -Uri "http://localhost:11434/api/tags" -Method GET -TimeoutSec 10
    $models = $response.Content | ConvertFrom-Json
    $modelExists = $models.models | Where-Object { $_.name -eq $ModelName }
    
    if ($modelExists) {
        Write-Host "✓ 模型 [$ModelName] 已安装" -ForegroundColor Green
    } else {
        Write-Host "正在下载模型 [$ModelName]..." -ForegroundColor Yellow
        & $OllamaExe pull $ModelName
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✓ 模型 [$ModelName] 下载完成" -ForegroundColor Green
        } else {
            Write-Host "✗ 模型下载失败" -ForegroundColor Red
            exit 1
        }
    }
} catch {
    Write-Host "✗ 无法检查模型状态: $($_.Exception.Message)" -ForegroundColor Red
    Write-Host "尝试直接下载模型..." -ForegroundColor Yellow
    & $OllamaExe pull $ModelName
}

Write-Host "✓ Ollama 服务和模型准备就绪" -ForegroundColor Green
