# DS18B20 Embedded Application - Automated Quality Monitoring Script (PowerShell)
# =============================================================================

param(
    [switch]$SkipBuild,
    [switch]$SkipTest,
    [switch]$Verbose
)

$ErrorActionPreference = "Stop"

Write-Host "🔍 DS18B20 Quality Monitoring Started" -ForegroundColor Cyan
Write-Host "======================================"

# 현재 시간 기록
$StartTime = Get-Date
Write-Host "⏰ Start Time: $($StartTime.ToString('yyyy-MM-dd HH:mm:ss'))"

# 프로젝트 루트 디렉토리로 이동
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$ProjectRoot = Resolve-Path (Join-Path $ScriptDir "../..")
Set-Location $ProjectRoot

Write-Host "📁 Project Root: $ProjectRoot"

# 로그 디렉토리 생성
@("logs/quality", "logs/comfile", "test/logs") | ForEach-Object {
    if (!(Test-Path $_)) {
        New-Item -ItemType Directory -Path $_ -Force | Out-Null
    }
}

if (!$SkipBuild -and !$SkipTest) {
    Write-Host ""
    Write-Host "🔨 Step 1: Building and Testing" -ForegroundColor Yellow
    Write-Host "==============================="
}

# 1. 빌드 실행 및 로그 저장
if (!$SkipBuild) {
    Write-Host "⚙️ Building for Arduino UNO R4 WiFi..."
    try {
        $BuildOutput = pio run -e uno_r4_wifi -v 2>&1
        $BuildOutput | Out-File -FilePath "logs/comfile/compile_results.txt" -Encoding Unicode
        if ($Verbose) { $BuildOutput }
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✅ Build completed successfully" -ForegroundColor Green
        } else {
            throw "Build failed with exit code $LASTEXITCODE"
        }
    }
    catch {
        Write-Host "❌ Build failed: $_" -ForegroundColor Red
        exit 1
    }
}

# 2. 테스트 실행 및 로그 저장
if (!$SkipTest) {
    Write-Host ""
    Write-Host "🧪 Running unit tests..."
    try {
        $TestOutput = pio test -e native -v 2>&1
        $TestOutput | Out-File -FilePath "test/logs/test_results_clean.txt" -Encoding Unicode
        if ($Verbose) { $TestOutput }
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✅ Tests completed" -ForegroundColor Green
        } else {
            throw "Tests failed with exit code $LASTEXITCODE"
        }
    }
    catch {
        Write-Host "❌ Tests failed: $_" -ForegroundColor Red
        exit 1
    }
}

Write-Host ""
Write-Host "📊 Step 2: Quality Metrics Analysis" -ForegroundColor Yellow
Write-Host "=================================="

# 3. 품질 메트릭 실행
Write-Host "📈 Running quality metrics analysis..."
try {
    $MetricsOutput = python tools/quality_metrics/code_metrics.py 2>&1
    if ($Verbose) { $MetricsOutput }
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✅ Quality metrics completed" -ForegroundColor Green
    } else {
        throw "Quality metrics failed with exit code $LASTEXITCODE"
    }
}
catch {
    Write-Host "❌ Quality metrics failed: $_" -ForegroundColor Red
    exit 1
}

# 4. 트렌드 분석 (데이터가 충분한 경우에만)
Write-Host ""
Write-Host "📈 Running trend analysis..."
try {
    $TrendOutput = python tools/quality_metrics/trend_analyzer.py 2>&1
    if ($Verbose) { $TrendOutput }
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✅ Trend analysis completed" -ForegroundColor Green
    } else {
        Write-Host "⚠️ Trend analysis failed (this is okay for first run)" -ForegroundColor Yellow
    }
}
catch {
    Write-Host "⚠️ Trend analysis failed: $_ (this is okay for first run)" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "📋 Step 3: Summary Report" -ForegroundColor Yellow
Write-Host "========================"

# 최신 품질 메트릭 결과 표시
$LatestMetrics = Get-ChildItem "logs/quality/metrics_*.json" | Sort-Object LastWriteTime -Descending | Select-Object -First 1

if ($LatestMetrics) {
    try {
        $MetricsData = Get-Content $LatestMetrics.FullName -Encoding UTF8 | ConvertFrom-Json
        
        $QualityScore = [math]::Round($MetricsData.quality_score, 1)
        $TestSuccess = [math]::Round($MetricsData.test_metrics.test_success_rate, 1)
        $RamUsage = [math]::Round($MetricsData.build_metrics.ram_usage_percent, 1)
        $FlashUsage = [math]::Round($MetricsData.build_metrics.flash_usage_percent, 1)
        
        Write-Host "📊 Quality Summary:"
        Write-Host "  Overall Quality Score: $QualityScore/100"
        Write-Host "  Test Success Rate: $TestSuccess%"
        Write-Host "  RAM Usage: $RamUsage%"
        Write-Host "  Flash Usage: $FlashUsage%"
        
        # 품질 점수에 따른 결과 판정
        if ($QualityScore -ge 90) {
            Write-Host "🎉 Excellent quality! Keep up the good work!" -ForegroundColor Green
        } elseif ($QualityScore -ge 80) {
            Write-Host "✅ Good quality. Minor improvements possible." -ForegroundColor Green
        } elseif ($QualityScore -ge 70) {
            Write-Host "⚠️ Acceptable quality. Consider improvements." -ForegroundColor Yellow
        } else {
            Write-Host "🚨 Quality needs improvement!" -ForegroundColor Red
        }
    }
    catch {
        Write-Host "❌ Error reading quality metrics: $_" -ForegroundColor Red
    }
} else {
    Write-Host "❌ No quality metrics found" -ForegroundColor Red
}

# 종료 시간 기록
$EndTime = Get-Date
$Duration = $EndTime - $StartTime

Write-Host ""
Write-Host "⏰ End Time: $($EndTime.ToString('yyyy-MM-dd HH:mm:ss'))"
Write-Host "⏱️ Duration: $($Duration.ToString('mm\:ss'))"
Write-Host "✅ Quality monitoring completed successfully!" -ForegroundColor Green

# 최신 리포트 파일 경로 출력
$LatestReport = Get-ChildItem "logs/quality/quality_report_*.md" -ErrorAction SilentlyContinue | Sort-Object LastWriteTime -Descending | Select-Object -First 1
if ($LatestReport) {
    Write-Host "📄 Latest report: $($LatestReport.FullName)"
} else {
    Write-Host "📄 Latest report: No report found"
}
