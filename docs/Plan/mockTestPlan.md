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

## 6. 문제 해결 및 최종 접근법: 헤더 온리 (Header-Only) Mock

### 6.1. 문제점: 링커 오류 (Linker Error)

초기 `platformio.ini` 설정(`build_src_filter` 사용)에도 불구하고 `pio test -e native` 실행 시 `undefined reference to ...` 링커 오류가 지속적으로 발생했습니다. 이는 PlatformIO의 빌드 시스템이 `test/mocks/` 폴더에 있는 `.cpp` 파일들을 컴파일은 했지만, 최종 실행 파일을 빌드할 때 오브젝트 파일(`*.o`)을 제대로 링크하지 못해 발생한 문제입니다.

### 6.2. 해결책: 헤더 온리 (Header-Only) 패턴 적용

이 문제를 근본적으로 해결하기 위해, 모든 Mock 객체 구현을 `.cpp` 파일에서 `.h` 파일로 이전하는 **헤더 온리(Header-Only)** 패턴을 채택했습니다.

- **구현**: Mock 클래스의 모든 멤버 함수 구현을 헤더 파일(`.h`) 내로 옮기고, `inline` 키워드를 추가했습니다.
- **장점**:
    1.  **링커 문제 원천 차단**: `#include` 지시문만으로 선언과 구현이 동시에 제공되므로, 빌드 시스템이 별도의 소스 파일을 찾아 링크할 필요가 없어집니다.
    2.  **독립성 강화**: 테스트에 필요한 모든 Mock 구현이 헤더 파일 하나에 포함되어, 의존성 관리가 더욱 명확하고 간결해집니다.
    3.  **컴파일 타임 최적화**: `inline` 함수는 컴파일러에 의해 최적화될 가능성이 있습니다.

### 6.3. 최종 디렉토리 및 `platformio.ini` 설정

헤더 온리 패턴을 적용함에 따라 `.cpp` 파일이 불필요해졌고, `build_src_filter` 설정도 단순화되었습니다.

**최종 디렉토리 구조:**
```
/test
|-- /mocks
|   |-- MockTemperatureSensor.h         // 선언과 구현 포함
|   |-- MockTemperatureSensorManager.h  // 선언과 구현 포함
|-- /interfaces
|   |-- ITemperatureSensor.h
|-- test_complete_mock_system.cpp       // 통합 테스트 스위트
```

**최종 `platformio.ini` 설정:**
```ini
[env:native]
platform = native
test_framework = unity
build_flags =
    -std=c++17
    -Wall
    -I test  ; test 폴더를 include 경로로 추가하여 <mocks/Mock*.h> 사용 가능
build_src_filter = 
    +<test/*.cpp> ; test 루트의 테스트 스위트만 빌드
    -<src/*>
    -<main.cpp>
lib_deps =
    throwtheswitch/Unity@^2.6.0
```

이 접근법은 `frimePlan.md`에 명시된 **클린 아키텍처**와 **의존성 역전 원칙(DIP)**을 완벽하게 지원하며, 안정적이고 독립적인 테스트 환경을 성공적으로 구축하는 결과를 가져왔습니다.


