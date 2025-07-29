### TemperatureSensorManager 클래스 설계

- **역할:**
	- 여러 DS18B20 센서를 자동 탐지하고, 각 센서의 주소를 관리하며, 온도 측정/수집/갱신 기능을 제공하는 도메인 계층의 핵심 매니저 클래스입니다.
	- 각 센서별로 사용자 정의 ID를 저장/조회/중복 체크하며, 센서 에러(-127.0 등) 및 예외 상황을 일관되게 처리합니다.

- **주요 책임:**
	- 센서 자동 탐지 및 주소 목록 관리
	- 센서별 온도 측정 및 데이터 갱신
	- 센서별 사용자 데이터(ID) 저장/조회/중복 체크
	- 에러 및 예외 상황 관리

- **주요 메서드 시그니처(예시):**
	```cpp
	class TemperatureSensorManager {
	public:
			TemperatureSensorManager();
			~TemperatureSensorManager();

			void discoverSensors(); // 센서 자동 탐지
			size_t getSensorCount() const;
			bool getSensorAddress(size_t index, uint8_t* address) const;
			float readTemperature(size_t index);
			bool setSensorId(size_t index, const std::string& id);
			std::string getSensorId(size_t index) const;
			bool isIdDuplicated(const std::string& id) const;
			// ... 기타 예외/에러 처리 메서드
	};
	```

- **설계 시 고려사항:**
	- ITemperatureSensor 인터페이스를 활용하여 하드웨어 독립적 설계
	- 센서 주소와 사용자 ID의 매핑 구조(예: std::map 등) 활용
	- 예외 상황(센서 미연결, 에러값 등) 처리 정책 명확화

---
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

### ITemperatureSensor 인터페이스 정의

 #### 개발 및 자동화 과정 기록

 - **파일 생성 위치:**
	 - `src/domain/ITemperatureSensor.h` 파일을 생성하여 DS18B20 센서 추상화를 위한 인터페이스를 정의함.

 - **빌드 및 테스트:**
	 - PlatformIO 환경에서 `C:\Users\h\.platformio\penv\Scripts\platformio.exe run -e uno_r4_wifi` 명령으로 빌드.
	 - `C:\Users\h\.platformio\penv\Scripts\platformio.exe test -e native` 명령으로 유닛테스트를 수행하여 인터페이스 정의의 적합성 및 프로젝트 정상 동작을 검증함.

 - **자동화 스크립트 도입 배경:**
	 - 반복적인 빌드, 테스트, git add/commit/push 과정을 자동화하고, 각 단계별 에러 발생 시 즉시 중단 및 명확한 메시지 출력을 위해 `tools/auto_commit.sh` 스크립트를 도입함.
	 - 해당 스크립트는 빌드, 테스트, git add/commit/push를 일괄 처리하며, 변경사항이 없을 경우 커밋을 생략하고, 에러 발생 시 원인별로 종료 코드를 반환함.
	 - Windows 환경에서는 Git Bash 또는 WSL에서 `bash tools/auto_commit.sh`로 실행 가능하며, 프로젝트의 일관된 품질 관리와 개발 효율성 향상에 기여함.

 ---
 

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
