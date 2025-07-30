/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "DS18B20 Embedded Application", "index.html", [
    [ "DS18B20 프로젝트 환경 구축 가이드", "md_docs_2_plan_201__environment__setup__guide.html", [
      [ "1. 프로젝트 기본 환경 구축", "md_docs_2_plan_201__environment__setup__guide.html#autotoc_md1", [
        [ "1.1 PlatformIO 초기화 및 보드 설정", "md_docs_2_plan_201__environment__setup__guide.html#autotoc_md2", null ],
        [ "1.2 라이브러리 의존성 관리", "md_docs_2_plan_201__environment__setup__guide.html#autotoc_md3", null ],
        [ "1.3 빌드 옵션 설정", "md_docs_2_plan_201__environment__setup__guide.html#autotoc_md4", null ],
        [ "1.4 테스트 환경 구성", "md_docs_2_plan_201__environment__setup__guide.html#autotoc_md5", null ],
        [ "1.5 테스트 실행", "md_docs_2_plan_201__environment__setup__guide.html#autotoc_md6", null ],
        [ "1.6 .gitignore 및 .vscode 설정", "md_docs_2_plan_201__environment__setup__guide.html#autotoc_md7", [
          [ ".gitignore 설정", "md_docs_2_plan_201__environment__setup__guide.html#autotoc_md8", null ],
          [ ".vscode 설정", "md_docs_2_plan_201__environment__setup__guide.html#autotoc_md9", null ]
        ] ]
      ] ],
      [ "2. Git 버전 관리 설정", "md_docs_2_plan_201__environment__setup__guide.html#autotoc_md10", null ],
      [ "3. 프로젝트 구조 설정", "md_docs_2_plan_201__environment__setup__guide.html#autotoc_md11", null ],
      [ "4. 의존성 설정", "md_docs_2_plan_201__environment__setup__guide.html#autotoc_md12", null ],
      [ "5. CI/CD 파이프라인 개요", "md_docs_2_plan_201__environment__setup__guide.html#autotoc_md13", null ],
      [ "6. GitHub Actions 적용 이후 자동화 과정", "md_docs_2_plan_201__environment__setup__guide.html#autotoc_md14", [
        [ "6.1 워크플로우(main.yml) 파일 생성 및 커밋", "md_docs_2_plan_201__environment__setup__guide.html#autotoc_md15", null ],
        [ "6.2 자동화 결과 확인 및 활용", "md_docs_2_plan_201__environment__setup__guide.html#autotoc_md16", null ],
        [ "6.3 요약", "md_docs_2_plan_201__environment__setup__guide.html#autotoc_md17", null ]
      ] ]
    ] ],
    [ "센서별 사용자 데이터(ID) 저장/조회/중복 체크 ✅ 완료", "md_docs_2_plan_202__core__feature__design.html", [
      [ "DS18B20 Domain Layer Core Feature Progress (계획-설계-구현-테스트-문서화-자동화 동기화)", "md_docs_2_plan_202__core__feature__design.html#autotoc_md20", [
        [ "에러 처리(-127.0 등) 및 예외 상황 관리 ✅ 완료", "md_docs_2_plan_202__core__feature__design.html#autotoc_md19", null ],
        [ "장시간 동작 안정성(롱런) 테스트 전략 (Application/Test/QA)", "md_docs_2_plan_202__core__feature__design.html#autotoc_md22", null ],
        [ "5. 각 단계별 완료 시, 문서 상단에 ✅ 및 요약 추가, 계획과 문서의 일치 보장", "md_docs_2_plan_202__core__feature__design.html#autotoc_md23", [
          [ "ITemperatureSensor 인터페이스 정의 ✅ 완료", "md_docs_2_plan_202__core__feature__design.html#autotoc_md24", null ],
          [ "TemperatureSensorManager 클래스 설계 ✅ 완료", "md_docs_2_plan_202__core__feature__design.html#autotoc_md25", null ],
          [ "센서 자동 탐지 및 주소 관리 로직 ✅ 완료", "md_docs_2_plan_202__core__feature__design.html#autotoc_md26", null ],
          [ "센서별 온도 측정/수집/갱신 메서드 ✅ 완료", "md_docs_2_plan_202__core__feature__design.html#autotoc_md27", null ],
          [ "센서별 온도 측정/수집/갱신 메서드", "md_docs_2_plan_202__core__feature__design.html#autotoc_md29", null ],
          [ "센서 자동 탐지 및 주소 관리 로직", "md_docs_2_plan_202__core__feature__design.html#autotoc_md31", null ],
          [ "TemperatureSensorManager 클래스 설계", "md_docs_2_plan_202__core__feature__design.html#autotoc_md33", null ]
        ] ]
      ] ],
      [ "02. 핵심 기능 설계 및 구현", "md_docs_2_plan_202__core__feature__design.html#autotoc_md35", [
        [ "목차", "md_docs_2_plan_202__core__feature__design.html#autotoc_md36", null ],
        [ "1. DS18B20 센서 관리 (Domain Layer)", "md_docs_2_plan_202__core__feature__design.html#autotoc_md38", [
          [ "ITemperatureSensor 인터페이스 정의", "md_docs_2_plan_202__core__feature__design.html#autotoc_md39", [
            [ "개발 및 자동화 과정 기록", "md_docs_2_plan_202__core__feature__design.html#autotoc_md40", null ]
          ] ]
        ] ],
        [ "2. 메모리 분석 시스템 (Application Layer)", "md_docs_2_plan_202__core__feature__design.html#autotoc_md42", [
          [ "IMemoryAnalyzer 인터페이스 정의 ✅ 완료", "md_docs_2_plan_202__core__feature__design.html#autotoc_md43", null ],
          [ "IMemoryTracker 인터페이스 정의 ✅ 완료", "md_docs_2_plan_202__core__feature__design.html#autotoc_md44", null ],
          [ "MemoryMonitorService 클래스 구현 ✅ 완료", "md_docs_2_plan_202__core__feature__design.html#autotoc_md45", null ],
          [ "MemoryTrackerService 클래스 구현 ✅ 완료", "md_docs_2_plan_202__core__feature__design.html#autotoc_md46", null ],
          [ "통합 메모리 시스템 아키텍처 ✅ 완료", "md_docs_2_plan_202__core__feature__design.html#autotoc_md47", null ],
          [ "로그 시스템 구조화 ✅ 완료", "md_docs_2_plan_202__core__feature__design.html#autotoc_md48", null ]
        ] ],
        [ "3. 통신 및 명령어 처리 (Infrastructure Layer)", "md_docs_2_plan_202__core__feature__design.html#autotoc_md50", [
          [ "ICommandProcessor 인터페이스 정의 ✅ 완료", "md_docs_2_plan_202__core__feature__design.html#autotoc_md51", null ],
          [ "SerialCommandHandler 클래스 구현 ✅ 완료", "md_docs_2_plan_202__core__feature__design.html#autotoc_md52", null ],
          [ "명령어 파싱 및 응답 처리 ✅ 완료", "md_docs_2_plan_202__core__feature__design.html#autotoc_md53", null ]
        ] ],
        [ "4. 의존성 주입 및 통합 (Main Layer)", "md_docs_2_plan_202__core__feature__design.html#autotoc_md55", [
          [ "HAL(Hardware Abstraction Layer) 구현 ✅ 완료", "md_docs_2_plan_202__core__feature__design.html#autotoc_md56", null ],
          [ "통합 의존성 주입 구조 ✅ 완료", "md_docs_2_plan_202__core__feature__design.html#autotoc_md57", null ],
          [ "컴파일 및 테스트 검증 ✅ 완료", "md_docs_2_plan_202__core__feature__design.html#autotoc_md58", null ]
        ] ],
        [ "5. 설계 의사결정/이슈/토의", "md_docs_2_plan_202__core__feature__design.html#autotoc_md60", [
          [ "메모리 추적 시스템 설계 결정 사항", "md_docs_2_plan_202__core__feature__design.html#autotoc_md61", [
            [ "1. RTTI 없는 환경에서의 타입 캐스팅 문제 해결", "md_docs_2_plan_202__core__feature__design.html#autotoc_md62", null ],
            [ "2. Header-Only Mock 패턴 확장", "md_docs_2_plan_202__core__feature__design.html#autotoc_md63", null ],
            [ "3. 메모리 추적 데이터 구조 설계", "md_docs_2_plan_202__core__feature__design.html#autotoc_md64", null ],
            [ "4. 원형 버퍼를 활용한 메모리 효율성", "md_docs_2_plan_202__core__feature__design.html#autotoc_md65", null ],
            [ "5. 로그 시스템 구조화", "md_docs_2_plan_202__core__feature__design.html#autotoc_md66", null ]
          ] ],
          [ "향후 확장 고려사항", "md_docs_2_plan_202__core__feature__design.html#autotoc_md67", [
            [ "1. 실시간 메모리 시각화", "md_docs_2_plan_202__core__feature__design.html#autotoc_md68", null ],
            [ "2. 메모리 누수 탐지", "md_docs_2_plan_202__core__feature__design.html#autotoc_md69", null ],
            [ "3. 다중 센서 연동", "md_docs_2_plan_202__core__feature__design.html#autotoc_md70", null ]
          ] ]
        ] ],
        [ "4. 코드 품질 메트릭 모니터링 시스템 ✅ 완료", "md_docs_2_plan_202__core__feature__design.html#autotoc_md72", [
          [ "설계 목표 및 구현 상태", "md_docs_2_plan_202__core__feature__design.html#autotoc_md73", [
            [ "자동화된 품질 분석 시스템 구축", "md_docs_2_plan_202__core__feature__design.html#autotoc_md74", null ]
          ] ],
          [ "주요 구현 컴포넌트", "md_docs_2_plan_202__core__feature__design.html#autotoc_md75", [
            [ "1. 코드 품질 메트릭 수집기 (<span class=\"tt\">code_metrics.py</span>)", "md_docs_2_plan_202__core__feature__design.html#autotoc_md76", null ],
            [ "2. 트렌드 분석 시스템 (<span class=\"tt\">trend_analyzer.py</span>)", "md_docs_2_plan_202__core__feature__design.html#autotoc_md77", null ],
            [ "3. 자동화 스크립트", "md_docs_2_plan_202__core__feature__design.html#autotoc_md78", null ]
          ] ],
          [ "현재 품질 점수 상세 분석", "md_docs_2_plan_202__core__feature__design.html#autotoc_md79", [
            [ "📊 전체 품질 점수: 90.9/100 (탁월한 품질)", "md_docs_2_plan_202__core__feature__design.html#autotoc_md80", null ]
          ] ],
          [ "CI/CD 통합", "md_docs_2_plan_202__core__feature__design.html#autotoc_md81", [
            [ "GitHub Actions 워크플로우 확장", "md_docs_2_plan_202__core__feature__design.html#autotoc_md82", null ],
            [ "자동 품질 게이트", "md_docs_2_plan_202__core__feature__design.html#autotoc_md83", null ]
          ] ],
          [ "생성되는 보고서", "md_docs_2_plan_202__core__feature__design.html#autotoc_md84", [
            [ "1. 품질 메트릭 리포트 (<span class=\"tt\">quality_report_*.md</span>)", "md_docs_2_plan_202__core__feature__design.html#autotoc_md85", null ],
            [ "2. 트렌드 분석 리포트 (<span class=\"tt\">trend_report_*.md</span>)", "md_docs_2_plan_202__core__feature__design.html#autotoc_md86", null ],
            [ "3. JSON 데이터 (<span class=\"tt\">metrics_*.json</span>, <span class=\"tt\">trend_analysis_*.json</span>)", "md_docs_2_plan_202__core__feature__design.html#autotoc_md87", null ]
          ] ],
          [ "품질 개선 가이드라인", "md_docs_2_plan_202__core__feature__design.html#autotoc_md88", [
            [ "개발 워크플로우 권장사항", "md_docs_2_plan_202__core__feature__design.html#autotoc_md89", null ],
            [ "품질 점수 기준", "md_docs_2_plan_202__core__feature__design.html#autotoc_md90", null ]
          ] ],
          [ "향후 확장 계획", "md_docs_2_plan_202__core__feature__design.html#autotoc_md91", [
            [ "1. 정적 분석 도구 확장", "md_docs_2_plan_202__core__feature__design.html#autotoc_md92", null ],
            [ "2. 커버리지 분석 고도화", "md_docs_2_plan_202__core__feature__design.html#autotoc_md93", null ],
            [ "3. 성능 메트릭 추가", "md_docs_2_plan_202__core__feature__design.html#autotoc_md94", null ]
          ] ],
          [ "주요 성과 요약", "md_docs_2_plan_202__core__feature__design.html#autotoc_md96", null ]
        ] ]
      ] ]
    ] ],
    [ "DS18B20 Embedded Application: Master Plan", "md_docs_2_plan_2frime_plan.html", [
      [ "🎯 프로젝트 목표", "md_docs_2_plan_2frime_plan.html#autotoc_md99", null ],
      [ "[v] 1. 프로젝트 구조 및 환경 구축", "md_docs_2_plan_2frime_plan.html#autotoc_md101", [
        [ "1.1 기본 설정", "md_docs_2_plan_2frime_plan.html#autotoc_md102", null ],
        [ "1.2 계층적 디렉토리 구조 정립", "md_docs_2_plan_2frime_plan.html#autotoc_md103", null ],
        [ "1.3 의존성 관리", "md_docs_2_plan_2frime_plan.html#autotoc_md104", null ],
        [ "1.4 CI/CD 및 버전 관리", "md_docs_2_plan_2frime_plan.html#autotoc_md105", null ]
      ] ],
      [ "[ ] 2. 핵심 기능 설계 및 구현", "md_docs_2_plan_2frime_plan.html#autotoc_md106", [
        [ "2.1 DS18B20 센서 관리 (Domain Layer)", "md_docs_2_plan_2frime_plan.html#autotoc_md107", null ],
        [ "2.2 통신 및 명령어 처리 (Infrastructure Layer)", "md_docs_2_plan_2frime_plan.html#autotoc_md108", null ],
        [ "2.3 메모리 분석 시스템 (Application Layer)", "md_docs_2_plan_2frime_plan.html#autotoc_md109", null ],
        [ "2.4 의존성 주입 및 통합 (Main Layer)", "md_docs_2_plan_2frime_plan.html#autotoc_md110", null ]
      ] ],
      [ "[v] 3. 테스트 및 품질 보증", "md_docs_2_plan_2frime_plan.html#autotoc_md111", [
        [ "3.1 유닛 테스트 구현", "md_docs_2_plan_2frime_plan.html#autotoc_md112", null ],
        [ "3.2 통합 테스트", "md_docs_2_plan_2frime_plan.html#autotoc_md113", null ],
        [ "3.3 자동화 및 정적 분석", "md_docs_2_plan_2frime_plan.html#autotoc_md114", null ],
        [ "3.4 성능 및 안정성 테스트", "md_docs_2_plan_2frime_plan.html#autotoc_md115", null ]
      ] ],
      [ "[ ] 4. 문서화 및 자동화 도구", "md_docs_2_plan_2frime_plan.html#autotoc_md116", [
        [ "4.1 프로젝트 문서", "md_docs_2_plan_2frime_plan.html#autotoc_md117", null ],
        [ "4.2 Living Documentation", "md_docs_2_plan_2frime_plan.html#autotoc_md118", null ],
        [ "4.3 변경 추적", "md_docs_2_plan_2frime_plan.html#autotoc_md119", null ],
        [ "4.4 코드 품질 및 분석 도구", "md_docs_2_plan_2frime_plan.html#autotoc_md120", null ]
      ] ],
      [ "[ ] 5. 운영 및 유지보수", "md_docs_2_plan_2frime_plan.html#autotoc_md121", [
        [ "5.1 배포 및 운영", "md_docs_2_plan_2frime_plan.html#autotoc_md122", null ],
        [ "5.2 이슈 관리", "md_docs_2_plan_2frime_plan.html#autotoc_md123", null ],
        [ "5.3 코드 리뷰 및 개선", "md_docs_2_plan_2frime_plan.html#autotoc_md124", null ]
      ] ],
      [ "📋 추진 우선순위", "md_docs_2_plan_2frime_plan.html#autotoc_md126", [
        [ "🎯 현재 상태 (2025-07-30)", "md_docs_2_plan_2frime_plan.html#autotoc_md127", null ]
      ] ],
      [ "📅 향후 개발 계획", "md_docs_2_plan_2frime_plan.html#autotoc_md129", [
        [ "즉시 착수 가능 항목", "md_docs_2_plan_2frime_plan.html#autotoc_md130", null ],
        [ "중장기 발전 방향", "md_docs_2_plan_2frime_plan.html#autotoc_md131", null ],
        [ "🏆 달성 성과", "md_docs_2_plan_2frime_plan.html#autotoc_md132", null ]
      ] ]
    ] ],
    [ "장시간 동작 안정성(Mock Long Run Stability) 테스트", "md_docs_2_plan_2mock_test_plan.html", [
      [ "목적", "md_docs_2_plan_2mock_test_plan.html#autotoc_md134", null ],
      [ "구조 및 방식", "md_docs_2_plan_2mock_test_plan.html#autotoc_md135", null ],
      [ "주요 시나리오", "md_docs_2_plan_2mock_test_plan.html#autotoc_md136", null ],
      [ "장점", "md_docs_2_plan_2mock_test_plan.html#autotoc_md137", null ],
      [ "Mock 기반 테스트 환경 구축 계획 및 절차", "md_docs_2_plan_2mock_test_plan.html#autotoc_md139", [
        [ "1. 목적", "md_docs_2_plan_2mock_test_plan.html#autotoc_md140", null ],
        [ "2. 폴더 구조 설계 [ 완료 ]", "md_docs_2_plan_2mock_test_plan.html#autotoc_md142", null ],
        [ "3. PlatformIO 설정 [ 완료 ]", "md_docs_2_plan_2mock_test_plan.html#autotoc_md144", null ],
        [ "4. Mock 클래스 작성", "md_docs_2_plan_2mock_test_plan.html#autotoc_md145", null ],
        [ "5. 유닛 테스트 코드 작성", "md_docs_2_plan_2mock_test_plan.html#autotoc_md146", null ],
        [ "6. 빌드 및 테스트", "md_docs_2_plan_2mock_test_plan.html#autotoc_md147", null ],
        [ "7. 자동화 및 문서화", "md_docs_2_plan_2mock_test_plan.html#autotoc_md148", null ]
      ] ],
      [ "6. 문제 해결 및 최종 접근법: 헤더 온리 (Header-Only) Mock", "md_docs_2_plan_2mock_test_plan.html#autotoc_md150", [
        [ "6.1. 문제점: 링커 오류 (Linker Error)", "md_docs_2_plan_2mock_test_plan.html#autotoc_md151", null ],
        [ "6.2. 해결책: 헤더 온리 (Header-Only) 패턴 적용", "md_docs_2_plan_2mock_test_plan.html#autotoc_md152", null ],
        [ "6.3. 최종 디렉토리 및 <span class=\"tt\">platformio.ini</span> 설정", "md_docs_2_plan_2mock_test_plan.html#autotoc_md153", null ]
      ] ],
      [ "7. Infrastructure Layer 유닛 테스트: <span class=\"tt\">SerialCommandHandler</span>", "md_docs_2_plan_2mock_test_plan.html#autotoc_md155", [
        [ "7.1. 목적", "md_docs_2_plan_2mock_test_plan.html#autotoc_md156", null ],
        [ "7.2. 접근 방식", "md_docs_2_plan_2mock_test_plan.html#autotoc_md157", null ],
        [ "7.3. <span class=\"tt\">platformio.ini</span> 설정", "md_docs_2_plan_2mock_test_plan.html#autotoc_md158", null ],
        [ "7.4. 결과: 성공", "md_docs_2_plan_2mock_test_plan.html#autotoc_md159", null ]
      ] ],
      [ "8. 메모리 분석 시스템(Application Layer) 테스트 플랜 및 진행 현황", "md_docs_2_plan_2mock_test_plan.html#autotoc_md161", [
        [ "8.1. 목표", "md_docs_2_plan_2mock_test_plan.html#autotoc_md162", null ],
        [ "8.2. 진행 현황", "md_docs_2_plan_2mock_test_plan.html#autotoc_md163", null ],
        [ "8.3. 테스트 환경 및 방법", "md_docs_2_plan_2mock_test_plan.html#autotoc_md164", null ]
      ] ],
      [ "9. HAL(Hardware Abstraction Layer) 도입 및 완전한 Header-Only 테스트 환경 구축 완료", "md_docs_2_plan_2mock_test_plan.html#autotoc_md166", [
        [ "9.1. HAL 패턴 도입 배경", "md_docs_2_plan_2mock_test_plan.html#autotoc_md167", null ],
        [ "9.2. HAL 구조 설계", "md_docs_2_plan_2mock_test_plan.html#autotoc_md168", null ],
        [ "9.3. 의존성 주입 구조 개선", "md_docs_2_plan_2mock_test_plan.html#autotoc_md169", null ],
        [ "9.4. 완전한 Header-Only Mock 테스트 환경 구축", "md_docs_2_plan_2mock_test_plan.html#autotoc_md170", [
          [ "9.4.1. 설계 원칙", "md_docs_2_plan_2mock_test_plan.html#autotoc_md171", null ],
          [ "9.4.2. 구현된 Mock 클래스들", "md_docs_2_plan_2mock_test_plan.html#autotoc_md172", null ],
          [ "9.4.3. platformio.ini 최종 설정", "md_docs_2_plan_2mock_test_plan.html#autotoc_md173", null ]
        ] ],
        [ "9.5. 테스트 결과 (최종 업데이트: 2025-07-30)", "md_docs_2_plan_2mock_test_plan.html#autotoc_md174", [
          [ "9.5.1. 테스트 통계 (최신)", "md_docs_2_plan_2mock_test_plan.html#autotoc_md175", null ],
          [ "9.5.2. 테스트된 기능 목록 (확장됨)", "md_docs_2_plan_2mock_test_plan.html#autotoc_md176", null ]
        ] ],
        [ "9.6. 달성된 목표", "md_docs_2_plan_2mock_test_plan.html#autotoc_md177", [
          [ "9.6.1. 아키텍처 품질", "md_docs_2_plan_2mock_test_plan.html#autotoc_md178", null ],
          [ "9.6.2. 개발 효율성", "md_docs_2_plan_2mock_test_plan.html#autotoc_md179", null ],
          [ "9.6.3. 유지보수성", "md_docs_2_plan_2mock_test_plan.html#autotoc_md180", null ]
        ] ]
      ] ],
      [ "10. 코드 품질 메트릭 모니터링 시스템 통합 ✅ 완료", "md_docs_2_plan_2mock_test_plan.html#autotoc_md182", [
        [ "10.1. 테스트 품질 관리 체계 구축", "md_docs_2_plan_2mock_test_plan.html#autotoc_md183", [
          [ "10.1.1. 품질 메트릭 시스템 개요", "md_docs_2_plan_2mock_test_plan.html#autotoc_md184", null ],
          [ "10.1.2. 구현된 도구들", "md_docs_2_plan_2mock_test_plan.html#autotoc_md185", null ]
        ] ],
        [ "10.2. 테스트 품질 메트릭 상세 분석", "md_docs_2_plan_2mock_test_plan.html#autotoc_md186", [
          [ "10.2.1. Mock 테스트 환경의 품질 지표", "md_docs_2_plan_2mock_test_plan.html#autotoc_md187", null ],
          [ "10.2.2. 아키텍처 품질 평가", "md_docs_2_plan_2mock_test_plan.html#autotoc_md188", null ]
        ] ],
        [ "10.3. CI/CD 통합 및 자동화", "md_docs_2_plan_2mock_test_plan.html#autotoc_md189", [
          [ "10.3.1. GitHub Actions 확장", "md_docs_2_plan_2mock_test_plan.html#autotoc_md190", null ],
          [ "10.3.2. 자동 품질 게이트", "md_docs_2_plan_2mock_test_plan.html#autotoc_md191", null ]
        ] ],
        [ "10.4. 품질 모니터링 워크플로우", "md_docs_2_plan_2mock_test_plan.html#autotoc_md192", [
          [ "10.4.1. 개발자 워크플로우", "md_docs_2_plan_2mock_test_plan.html#autotoc_md193", null ],
          [ "10.4.2. 지속적 개선 프로세스", "md_docs_2_plan_2mock_test_plan.html#autotoc_md194", null ]
        ] ],
        [ "10.5. Mock 테스트 환경의 성과 요약", "md_docs_2_plan_2mock_test_plan.html#autotoc_md195", [
          [ "10.5.1. 달성된 목표", "md_docs_2_plan_2mock_test_plan.html#autotoc_md196", null ],
          [ "10.5.2. 메모리 누수 탐지 테스트 성공 사례", "md_docs_2_plan_2mock_test_plan.html#autotoc_md197", null ],
          [ "10.5.3. 스트레스 테스트 Mock 통합 및 자동화 사례", "md_docs_2_plan_2mock_test_plan.html#autotoc_md198", null ],
          [ "10.5.2. 품질 메트릭 기여도", "md_docs_2_plan_2mock_test_plan.html#autotoc_md199", null ]
        ] ],
        [ "10.6. 모범 사례 (Best Practices)", "md_docs_2_plan_2mock_test_plan.html#autotoc_md200", [
          [ "10.6.1. Mock 설계 원칙", "md_docs_2_plan_2mock_test_plan.html#autotoc_md201", null ],
          [ "10.6.2. 테스트 구조화", "md_docs_2_plan_2mock_test_plan.html#autotoc_md202", null ],
          [ "10.6.3. 단언문 패턴", "md_docs_2_plan_2mock_test_plan.html#autotoc_md203", null ]
        ] ],
        [ "10.7. 결론", "md_docs_2_plan_2mock_test_plan.html#autotoc_md204", [
          [ "최종 성과 요약:", "md_docs_2_plan_2mock_test_plan.html#autotoc_md205", null ]
        ] ]
      ] ]
    ] ],
    [ "DS18B20 Embedded Application 아키텍처 설계", "md_docs_2design_2architecture.html", [
      [ "계층 구조", "md_docs_2design_2architecture.html#autotoc_md207", null ],
      [ "주요 설계 원칙", "md_docs_2design_2architecture.html#autotoc_md208", null ],
      [ "시스템 구성도", "md_docs_2design_2architecture.html#autotoc_md209", null ],
      [ "데이터 흐름", "md_docs_2design_2architecture.html#autotoc_md210", null ]
    ] ],
    [ "DS18B20 Embedded Application 모듈 인터페이스", "md_docs_2design_2module__interfaces.html", [
      [ "Domain Layer", "md_docs_2design_2module__interfaces.html#autotoc_md213", null ],
      [ "Application Layer", "md_docs_2design_2module__interfaces.html#autotoc_md214", null ],
      [ "Infrastructure Layer", "md_docs_2design_2module__interfaces.html#autotoc_md215", null ],
      [ "HAL Layer", "md_docs_2design_2module__interfaces.html#autotoc_md216", null ],
      [ "테스트/Mock", "md_docs_2design_2module__interfaces.html#autotoc_md217", null ]
    ] ],
    [ "클래스", "annotated.html", [
      [ "클래스 목록", "annotated.html", "annotated_dup" ],
      [ "클래스 색인", "classes.html", null ],
      [ "클래스 계통도", "hierarchy.html", "hierarchy" ],
      [ "클래스 멤버", "functions.html", [
        [ "모두", "functions.html", null ],
        [ "함수", "functions_func.html", null ]
      ] ]
    ] ],
    [ "파일들", "files.html", [
      [ "파일 목록", "files.html", "files_dup" ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"_arduino_hal_8h_source.html",
"md_docs_2_plan_2mock_test_plan.html#autotoc_md197"
];

var SYNCONMSG = '패널 동기화를 비활성화하기 위해 클릭하십시오';
var SYNCOFFMSG = '패널 동기화를 활성화하기 위해 클릭하십시오';
var LISTOFALLMEMBERS = '모든 멤버 목록';