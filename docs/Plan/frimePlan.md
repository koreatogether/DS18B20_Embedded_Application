# DS18B20 Embedded Application: Master Plan

이 문서는 DS18B20 온도 센서 + 메모리 분석 통합 시스템의 개발, 테스트, 운영, 품질보증, 문서화 전 과정을 아우르는 마스터 체크리스트입니다.

## 🎯 프로젝트 목표
- Arduino UNO R4 WiFi 기반 DS18B20 다중 센서 관리 시스템
- 실시간 메모리 분석 및 모니터링 통합
- Clean Architecture + SOLID + DIP 원칙 적용
- 유닛 테스트 기반 품질 보증 및 자동화
- Arduino IDE와의 호환성 유지

---

## [v] 1. 프로젝트 구조 및 환경 구축
### 1.1 기본 설정
- [v] PlatformIO 프로젝트 초기화 및 보드 설정 (uno_r4_wifi)
- [v] platformio.ini 설정 (라이브러리[v], 테스트 환경[v], 빌드 옵션[v])
- [v] .gitignore, .vscode 설정

### 1.2 계층적 디렉토리 구조 정립

- [v] docs/ (설계, 분석, 사용자 가이드)
- [v] src/domain/ (하드웨어 독립적 비즈니스 로직)
- [v] src/application/ (유스케이스 구현)
- [v] src/infrastructure/ (하드웨어 인터페이스, 통신)
- [v] test(PC 기반 유닛 테스트)
- [v] test/integration/ (하드웨어 기반 통합 테스트)
- [v] tools/ (자동화 도구, 스크립트)

### 1.3 의존성 관리
hand written Mock 방식 ( Mock 객체를 직접 구현하여 의존성 분리)

### 1.4 CI/CD 및 버전 관리
- [v] GitHub 저장소 설정
- [v] GitHub Actions 워크플로우 (.github/workflows/main.yml)
- [v] 자동 빌드/테스트/정적분석 파이프라인

## [ ] 2. 핵심 기능 설계 및 구현
### 2.1 DS18B20 센서 관리 (Domain Layer)
- [v] ITemperatureSensor 인터페이스 정의
- [v] TemperatureSensorManager 클래스 설계
- [v] 센서 자동 탐지 및 주소 관리 로직
- [v] 센서별 온도 측정/수집/갱신 메서드
- [v] 센서별 사용자 데이터(ID) 저장/조회/중복 체크
- [v] 에러 처리(-127.0 등) 및 예외 상황 관리

### 2.2 통신 및 명령어 처리 (Infrastructure Layer)
- [ ] ICommandProcessor 인터페이스 정의
- [ ] SerialCommandHandler 클래스 구현
- [ ] memtest/memstress/memstruct/memfree/memtoggle 명령어
- [ ] help/menu 등 사용자 편의 명령어
- [ ] 명령어 파싱 및 응답 처리

### 2.3 메모리 분석 시스템 (Application Layer)
- [ ] IMemoryAnalyzer 인터페이스 정의
- [ ] MemoryMonitorService 클래스 구현
- [ ] 초기화 시 메모리 상태 측정/로깅
- [ ] 주기적(10초) 메모리 모니터링
- [ ] 런타임/스트레스/구조 메모리 테스트
- [ ] CSV/마크다운 로그 출력 형식 정의

### 2.4 의존성 주입 및 통합 (Main Layer)
- [ ] 각 계층 간 의존성 주입 구조 설계
- [ ] main.cpp에서 객체 생성 및 초기화
- [ ] 시스템 동작 플로우 통합

## [ ] 3. 테스트 및 품질 보증
### 3.1 유닛 테스트 구현
- [ ] 각 클래스별 유닛 테스트 작성
- [ ] Mock 객체를 활용한 의존성 분리 테스트
- [ ] PC 환경에서 실행 가능한 테스트 구조
- [ ] 테스트 커버리지 목표 달성

### 3.2 통합 테스트
- [ ] 하드웨어 기반 실제 센서 연동 테스트
- [ ] 메모리 분석 기능 통합 테스트
- [ ] 시리얼 명령어 End-to-End 테스트

### 3.3 자동화 및 정적 분석
- [v] CI/CD 파이프라인에서 자동 테스트 실행
- [v] cppcheck, clang-tidy 정적 분석 통합
- [ ] 메모리 사용량 변화 추적 및 리포트
- [ ] 코드 품질 메트릭 모니터링

### 3.4 성능 및 안정성 테스트
- [ ] 장시간 동작 안정성 검증
- [ ] 메모리 누수 탐지 테스트
- [ ] 스트레스 테스트 시나리오 실행

## [ ] 4. 문서화 및 자동화 도구
### 4.1 프로젝트 문서
- [ ] README.md (프로젝트 개요, 빌드/실행 방법)
- [ ] 설계 문서 (docs/design/) - 아키텍처, 모듈 인터페이스
- [ ] 사용자 가이드 (docs/user/) - 명령어, 시나리오
- [ ] 기여 가이드 (CONTRIBUTING.md)

### 4.2 Living Documentation
- [ ] 코드 변경 시 자동 문서 업데이트 프로세스
- [ ] Doxygen/유사 도구로 API 문서 자동 생성
- [ ] CI/CD에서 문서 빌드 및 배포 자동화

### 4.3 변경 추적
- [ ] CHANGELOG.md 관리
- [ ] 커밋 메시지 표준화
- [ ] 버전 태깅 및 릴리스 노트

### 4.4 Python 분석 도구
- [ ] memory_serial_log/ 디렉토리 구성
- [ ] save_serial_log.py (실시간 로그 수집)
- [ ] analyze_memory_logs.py (로그 분석 및 리포트)
- [ ] 분석 도구 사용법 문서화

## [ ] 5. 운영 및 유지보수
### 5.1 배포 및 운영
- [ ] Arduino IDE 호환성 검증
- [ ] 다양한 환경에서 빌드/실행 테스트
- [ ] 장기 동작/안정성 검증
- [ ] 메모리 사용량 최적화

### 5.2 이슈 관리
- [ ] GitHub Issues 템플릿 설정
- [ ] 버그 리포트 및 기능 요청 프로세스
- [ ] 우선순위 및 라벨링 시스템

### 5.3 코드 리뷰 및 개선
- [ ] Pull Request 템플릿 및 리뷰 가이드라인
- [ ] AI 자가 리뷰 프로세스 적용
- [ ] 코드 품질 지속적 개선

---

## 📋 추진 우선순위
1. **1단계**: 프로젝트 구조 구축 (1.1 ~ 1.4)
2. **2단계**: 핵심 기능 TDD 개발 (2.1 ~ 2.4)
3. **3단계**: 테스트 및 품질 보증 (3.1 ~ 3.4)
4. **4단계**: 문서화 및 도구 (4.1 ~ 4.4)
5. **5단계**: 운영 및 유지보수 (5.1 ~ 5.3)

각 항목은 작업 완료 시 [x]로 체크하며, 모든 개발/운영/문서화 활동의 기준이 됩니다.
