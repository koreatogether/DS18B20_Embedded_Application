#!/bin/bash

# DS18B20 Embedded Application - Automated Quality Monitoring Script
# ==================================================================

set -e  # 에러 발생 시 즉시 종료

echo "🔍 DS18B20 Quality Monitoring Started"
echo "=================================="

# 현재 시간 기록
START_TIME=$(date +"%Y-%m-%d %H:%M:%S")
echo "⏰ Start Time: $START_TIME"

# 프로젝트 루트 디렉토리로 이동
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
cd "$PROJECT_ROOT"

echo "📁 Project Root: $PROJECT_ROOT"

# 로그 디렉토리 생성
mkdir -p logs/quality
mkdir -p logs/comfile
mkdir -p test/logs

echo ""
echo "🔨 Step 1: Building and Testing"
echo "==============================="

# 1. 빌드 실행 및 로그 저장
echo "⚙️ Building for Arduino UNO R4 WiFi..."
if pio run -e uno_r4_wifi -v 2>&1 | tee logs/comfile/compile_results.txt; then
    echo "✅ Build completed successfully"
else
    echo "❌ Build failed"
    exit 1
fi

# 2. 테스트 실행 및 로그 저장
echo ""
echo "🧪 Running unit tests..."
if pio test -e native -v 2>&1 | tee test/logs/test_results_clean.txt; then
    echo "✅ Tests completed"
else
    echo "❌ Tests failed"
    exit 1
fi

echo ""
echo "📊 Step 2: Quality Metrics Analysis"
echo "=================================="

# 3. 품질 메트릭 실행
echo "📈 Running quality metrics analysis..."
if python tools/quality_metrics/code_metrics.py; then
    echo "✅ Quality metrics completed"
else
    echo "❌ Quality metrics failed"
    exit 1
fi

# 4. 트렌드 분석 (데이터가 충분한 경우에만)
echo ""
echo "📈 Running trend analysis..."
if python tools/quality_metrics/trend_analyzer.py; then
    echo "✅ Trend analysis completed"
else
    echo "⚠️ Trend analysis failed (this is okay for first run)"
fi

echo ""
echo "📋 Step 3: Summary Report"
echo "========================"

# 최신 품질 메트릭 결과 표시
LATEST_METRICS=$(ls -t logs/quality/metrics_*.json | head -1)
if [ -f "$LATEST_METRICS" ]; then
    QUALITY_SCORE=$(python -c "
import json
with open('$LATEST_METRICS', 'r') as f:
    data = json.load(f)
    print(f\"{data.get('quality_score', 0):.1f}\")
")
    
    TEST_SUCCESS=$(python -c "
import json
with open('$LATEST_METRICS', 'r') as f:
    data = json.load(f)
    print(f\"{data.get('test_metrics', {}).get('test_success_rate', 0):.1f}\")
")
    
    RAM_USAGE=$(python -c "
import json
with open('$LATEST_METRICS', 'r') as f:
    data = json.load(f)
    print(f\"{data.get('build_metrics', {}).get('ram_usage_percent', 0):.1f}\")
")
    
    FLASH_USAGE=$(python -c "
import json
with open('$LATEST_METRICS', 'r') as f:
    data = json.load(f)
    print(f\"{data.get('build_metrics', {}).get('flash_usage_percent', 0):.1f}\")
")
    
    echo "📊 Quality Summary:"
    echo "  Overall Quality Score: $QUALITY_SCORE/100"
    echo "  Test Success Rate: $TEST_SUCCESS%"
    echo "  RAM Usage: $RAM_USAGE%"
    echo "  Flash Usage: $FLASH_USAGE%"
    
    # 품질 점수에 따른 결과 판정
    if (( $(echo "$QUALITY_SCORE >= 90" | bc -l) )); then
        echo "🎉 Excellent quality! Keep up the good work!"
    elif (( $(echo "$QUALITY_SCORE >= 80" | bc -l) )); then
        echo "✅ Good quality. Minor improvements possible."
    elif (( $(echo "$QUALITY_SCORE >= 70" | bc -l) )); then
        echo "⚠️ Acceptable quality. Consider improvements."
    else
        echo "🚨 Quality needs improvement!"
    fi
else
    echo "❌ No quality metrics found"
fi

# 종료 시간 기록
END_TIME=$(date +"%Y-%m-%d %H:%M:%S")
echo ""
echo "⏰ End Time: $END_TIME"
echo "✅ Quality monitoring completed successfully!"

# 최신 리포트 파일 경로 출력
LATEST_REPORT=$(ls -t logs/quality/quality_report_*.md | head -1 2>/dev/null || echo "No report found")
echo "📄 Latest report: $LATEST_REPORT"
