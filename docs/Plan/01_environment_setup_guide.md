# DS18B20 프로젝트 환경 구축 가이드

이 문서는 DS18B20 임베디드 애플리케이션 프로젝트의 환경 구축 과정을 단계별로 설명합니다. 각 항목은 프로젝트 계획(frimePlan.md)의 체크리스트 항목에 해당합니다.

## 1. 프로젝트 기본 환경 구축

### 1.1 PlatformIO 초기화 및 보드 설정

Arduino UNO R4 WiFi 보드를 위한 PlatformIO 프로젝트를 초기화하는 과정입니다.

```bash
# PlatformIO 프로젝트 초기화 명령
pio project init --board uno_r4_wifi
```

생성된 `platformio.ini` 파일에서 기본 보드 설정을 확인할 수 있습니다:

```ini
[env:uno_r4_wifi]
platform = renesas-ra
board = uno_r4_wifi
framework = arduino
```

### 1.2 라이브러리 의존성 관리

프로젝트에 필요한 라이브러리(DS18B20 센서 사용을 위한 OneWire, DallasTemperature)를 설정합니다.

```ini
# platformio.ini에 라이브러리 의존성 추가
lib_deps =
    milesburton/DallasTemperature
    paulstoffregen/OneWire
```

### 1.3 빌드 옵션 설정

Arduino IDE 기본 빌드 수준에 맞는 옵션을 추가합니다.

```ini
# platformio.ini에 빌드 플래그 추가
build_flags =
    -Wall
    -Os
    -std=gnu++17
```

### 1.4 테스트 환경 구성

Unity 프레임워크 기반의 테스트 환경을 구축합니다. Native(PC) 환경에서 테스트를 실행할 수 있도록 설정합니다.

```ini
# platformio.ini에 테스트 설정 추가
[test]
test_framework = unity
test_build_src = yes
test_ignore = mocks

[env:native]
platform = native
build_flags =
    -std=gnu++17
    -Wall
```

테스트 코드는 `test` 폴더에 작성하며, Unity 인클루드를 다음과 같이 사용합니다:

```c
// test/test_sample.c
#include <unity.h>

void test_addition(void) {
    TEST_ASSERT_EQUAL_INT(4, 2 + 2);
}

void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_addition);
    return UNITY_END();
}
```

### 1.5 테스트 실행

PlatformIO IDE의 테스트 버튼을 사용하거나, 아래 명령어로 테스트를 실행합니다:

```bash
# Native 환경에서 테스트 실행
pio test -e native
```

### 1.6 .gitignore 및 .vscode 설정

#### .gitignore 설정

PlatformIO, VS Code, 빌드 산출물 등 불필요한 파일이 Git에 포함되지 않도록 `.gitignore` 파일을 아래와 같이 작성합니다.

```gitignore
# PlatformIO
.pio/
.vscode/
build/
libdeps/
*.pyc
__pycache__/
*.elf
*.bin
*.hex

# OS/Editor
.DS_Store
Thumbs.db

# 기타
*.log
```

#### .vscode 설정

VS Code에서 프로젝트를 일관되게 관리하기 위해 `.vscode` 폴더에 아래와 같은 파일을 둡니다.

- `extensions.json`: 추천 확장 목록
- `settings.json`: 워크스페이스별 설정(선택)

예시: `.vscode/extensions.json`
```json
{
  "recommendations": [
    "platformio.platformio-ide",
    "ms-vscode.cpptools"
  ],
  "unwantedRecommendations": [
    "ms-vscode.cpptools-extension-pack"
  ]
}
```

예시: `.vscode/settings.json`
```json
{
  "files.exclude": {
    "**/.pio": true,
    "**/libdeps": true
  }
}
```

이렇게 하면 팀원이나 다른 개발자가 프로젝트를 열었을 때 불필요한 파일이 Git에 포함되지 않고, VS Code 환경도 일관되게 유지할 수 있습니다.

## 2. Git 버전 관리 설정

프로젝트의 버전 관리를 위한 Git 저장소를 초기화하고 GitHub에 연결합니다.

```bash
# Git 초기화
git init

# GitHub 원격 저장소 연결
git remote add origin https://github.com/username/DS18B20_Embedded_Application.git

# 첫 커밋 및 푸시
git add .
git commit -m "Initial commit"
git branch -M main
git push -u origin main
```

## 3. 프로젝트 구조 설정

이 프로젝트는 클린 아키텍처 및 SOLID 원칙을 따르는 계층적 구조를 사용합니다. 다음 단계는 아래 폴더 구조를 정립하고 구현을 시작하는 것입니다:

docs/ (설계, 분석, 사용자 가이드)
src/domain/ (하드웨어 독립적 비즈니스 로직)
src/application/ (유스케이스 구현)
src/infrastructure/ (하드웨어 인터페이스, 통신)
test(PC 기반 유닛 테스트)
test/integration/ (하드웨어 기반 통합 테스트)
tools/ (자동화 도구, 스크립트)

## 4. 의존성 설정
이 단계는 본 프로젝트의 규모로는 hand written mock 방식 (mock 직접 구현 ) 으로 충분함.

## 5. CI/CD 파이프라인 설정

목적 : CI/CD 파이프라인을 설정하여 코드 변경 시 자동으로 빌드 및 테스트를 실행합니다. 
GitHub Actions를 사용하여 다음과 같은 워크플로우를 설정할 수 있습니다. (앞서 github 설정을 끝낸 상황 필요)

