#!/bin/bash
# auto_commit.sh: 자동 빌드, 테스트, 스테이징만 자동화 (커밋/푸시는 수동)

set -e

# 1. 빌드 (uno_r4_wifi 환경)
if ! C:\\Users\\h\\.platformio\\penv\\Scripts\\platformio.exe run -e uno_r4_wifi; then
  echo "[ERROR] PlatformIO 빌드 실패!" >&2
  exit 1
fi

# 2. 유닛 테스트 (native 환경)
if ! C:\\Users\\h\\.platformio\\penv\\Scripts\\platformio.exe test -e native; then
  echo "[ERROR] 유닛 테스트 실패!" >&2
  exit 2
fi




# 4. 민감정보(시크릿) 자동 검사 (git + filesystem)
bash "$(dirname "$0")/trufflehog_gitscan.sh"

# 4. git add (모든 변경/신규 파일 포함)
git add .

# 5. 커밋/푸시는 수동으로 진행
if git diff --cached --quiet; then
  echo "[INFO] 커밋할 변경사항이 없습니다."
  exit 0
fi

echo "[INFO] 변경 파일이 스테이징되었습니다. 커밋 메시지 작성 및 git commit/push는 수동으로 진행하세요."
