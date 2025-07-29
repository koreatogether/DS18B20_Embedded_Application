# 02. 핵심 기능 설계 및 구현

이 문서는 DS18B20 Embedded Application의 핵심 기능(센서 관리, 메모리 분석, 통신, 의존성 주입 등) 설계 및 구현 과정을 기록합니다.

## 목차
1. DS18B20 센서 관리 (Domain Layer)
2. 메모리 분석 시스템 (Application Layer)
3. 통신 및 명령어 처리 (Infrastructure Layer)
4. 의존성 주입 및 통합 (Main Layer)
5. 설계 의사결정/이슈/토의

---

## 1. DS18B20 센서 관리 (Domain Layer)
- ITemperatureSensor 인터페이스 정의
- TemperatureSensorManager 클래스 설계
- 센서 자동 탐지 및 주소 관리 로직
- 센서별 온도 측정/수집/갱신 메서드
- 센서별 사용자 데이터(ID) 저장/조회/중복 체크
- 에러 처리(-127.0 등) 및 예외 상황 관리

## 2. 메모리 분석 시스템 (Application Layer)
- IMemoryAnalyzer 인터페이스 정의
- MemoryMonitorService 클래스 구현
- 초기화 시 메모리 상태 측정/로깅
- 주기적(10초) 메모리 모니터링
- 런타임/스트레스/구조 메모리 테스트
- CSV/마크다운 로그 출력 형식 정의

## 3. 통신 및 명령어 처리 (Infrastructure Layer)
- ICommandProcessor 인터페이스 정의
- SerialCommandHandler 클래스 구현
- memtest/memstress/memstruct/memfree/memtoggle 명령어
- help/menu 등 사용자 편의 명령어
- 명령어 파싱 및 응답 처리

## 4. 의존성 주입 및 통합 (Main Layer)
- 각 계층 간 의존성 주입 구조 설계
- main.cpp에서 객체 생성 및 초기화
- 시스템 동작 플로우 통합

## 5. 설계 의사결정/이슈/토의
- 주요 설계 결정, 대안, 이슈, 토의 내용 기록
- 예시: 인터페이스 설계 이유, 테스트 전략, 확장성 고려 등

---

> 각 항목별로 설계/구현/예제/이슈/토의 내용을 자유롭게 추가·수정해 주세요.
