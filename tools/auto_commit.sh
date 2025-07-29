#!/bin/bash
# auto_commit.sh: 자동 빌드, 테스트, 커밋, 푸시 스크립트 (에러 대처 포함)

set -e

# 1. 빌드
if ! C:\\Users\\h\\.platformio\\penv\\Scripts\\platformio.exe run -e uno_r4_wifi; then
  echo "[ERROR] PlatformIO 빌드 실패!" >&2
  exit 1
fi

# 2. 유닛 테스트
if ! C:\\Users\\h\\.platformio\\penv\\Scripts\\platformio.exe test -e native; then
  echo "[ERROR] 유닛 테스트 실패!" >&2
  exit 2
fi

# 3. git add (모든 변경/신규 파일 포함)
git add .



# 4. 커밋/푸시 단계는 수동으로 진행 (자동화는 스테이징까지)
if git diff --cached --quiet; then
  echo "[INFO] 커밋할 변경사항이 없습니다."
  exit 0
fi

echo "[INFO] 변경 파일이 스테이징되었습니다. 커밋 메시지 작성 및 git commit/push는 수동으로 진행하세요."

# 5. git push
if ! git push; then
  echo "[ERROR] git push 실패! 네트워크/권한/충돌 확인 필요" >&2
  exit 3
fi

echo "[SUCCESS] 자동 빌드/테스트/커밋/푸시 완료!"
