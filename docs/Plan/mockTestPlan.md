근거 및 체크 결과
핸드메이드 Mock 방식 명시

plan 1.3: "Mock 객체를 직접 구현하여 의존성 분리" → 실제 하드웨어/라이브러리와 무관하게 테스트 가능
폴더 구조

test/ 폴더에 mock, unit, integration 등 계층 분리
src/와 test/가 명확히 분리되어 있음
ini 설정

build_src_filter = +<test/*> +<test/**/*> -<src/*> -<src/**/*> -<main.cpp> → src의 실제 구현체, main.cpp, 외부 라이브러리 등은 빌드에서 완전히 제외
-I src ... -I test로 mock 헤더/소스만 인클루드 가능
Mock 객체 활용

MockTemperatureSensor.h 등 mock 구현체 존재
테스트 코드에서 mock만 사용하면 실제 의존성 없이 테스트 가능
Unity 테스트 프레임워크

PC(native) 환경에서 실행 가능, 하드웨어/플랫폼 종속성 없음

의 내용을 근거로 테스트 전용 플랜으로 컴파일을 따라한 환경을 구축을 한다.

---

## Mock 기반 테스트 환경 구축 계획 및 절차

### 1. 목적
- 실제 하드웨어/외부 라이브러리와 완전히 분리된 mock 객체만으로 테스트 가능한 환경을 구축한다.
- 테스트 코드와 mock 소스만으로 컴파일/테스트가 가능하도록 한다.


----

### 2. 폴더 구조 설계 [ 완료 ]
- `test/mocks/` : mock 클래스 및 관련 소스/헤더 파일
- `test/unit/` : mock 객체를 활용한 유닛 테스트 코드
- `src/domain/ITemperatureSensor.h` : 테스트 대상 인터페이스(의존성 주입용)
- `src/` 및 `main.cpp`는 빌드에서 제외
  
  -----------


### 3. PlatformIO 설정 [ 완료 ]
- `platformio.ini`의 `[env:native]` 환경에 아래 옵션을 적용
  - `build_src_filter = +<test/*> +<test/**/*> -<src/*> -<src/**/*> -<main.cpp>`
  - `build_flags`에 `-I test` 등 mock 경로 추가
  - `lib_deps`에 `Unity`만 포함

### 4. Mock 클래스 작성
- 예시: `test/mocks/MockTemperatureSensor.h/.cpp`
  - 테스트에서 사용할 인터페이스(예: `ITemperatureSensor`)를 상속
  - 실제 하드웨어 동작을 시뮬레이션하는 메서드 구현

### 5. 유닛 테스트 코드 작성
- `test/unit/`에 mock 객체만 사용하는 테스트 코드 작성
- 테스트 대상 클래스가 mock 인터페이스만 참조하도록 설계

### 6. 빌드 및 테스트
- `pio test -e native` 명령으로 mock 기반 테스트만 실행
- src/의 실제 구현체, 외부 라이브러리, main.cpp 등은 빌드에 포함되지 않음

### 7. 자동화 및 문서화
- 위 절차를 문서화하여 프로젝트 내 공유
- CI에서도 mock 테스트만 별도 실행 가능하도록 워크플로우 분리

---


