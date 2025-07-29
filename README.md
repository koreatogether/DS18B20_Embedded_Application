# DS18B20_Embedded_Application

Arduino UNO R4 WiFi 기반 DS18B20 온도 센서 + 메모리 분석 통합 시스템

## 주요 특징
- 다중 DS18B20 온도 센서 자동 탐지 및 실시간 데이터 수집
- 센서별 ID 및 사용자 데이터 관리
- 실시간 메모리 사용량 분석 및 주기적 모니터링
- 시리얼 명령어 기반 메모리 테스트 및 CSV/마크다운 로그 출력
- Python 스크립트 기반 로그 수집 및 분석 자동화
- 클린 아키텍처(SOLID, DIP) 기반 계층적 구조
- PlatformIO 및 Arduino IDE 호환
- 유닛 테스트 및 CI/CD 자동화 지원

## 폴더 구조
```
├── .gitignore
├── .pio/                        # PlatformIO 빌드 산출물
├── .vscode/                     # VSCode 설정
├── build/                       # 워크스페이스/빌드 관련 파일
├── docs/                        # 설계, 분석, 가이드 문서
├── DS18B20_Embedded_Application.ino # 아두이노 스케치 파일
├── include/                     # 헤더 파일
├── lib/                         # 외부 라이브러리
├── platformio.ini               # PlatformIO 설정
├── README.md                    # 프로젝트 개요(본 파일)
├── src/                         # 메인 소스 코드(main.cpp 등)
├── test/                        # 유닛/통합 테스트
│   ├── intergration/            # 하드웨어 통합 테스트
│   ├── mocks/                   # Mock 객체
│   └── unit/                    # PC 기반 유닛 테스트
├── tools/                       # (예비) 도구 폴더
```

## 주요 명령어 예시
- `memtest`, `memstress`, `memstruct`, `memfree`, `memtoggle` (메모리 분석)
- `help`, `menu` (사용자 편의)

## 빌드 및 실행
1. PlatformIO 또는 Arduino IDE에서 프로젝트 열기
2. 보드: Arduino UNO R4 WiFi
3. 빌드 및 업로드

## 참고 문서

---
