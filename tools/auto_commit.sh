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


# 4. 커밋 메시지 자동 생성 (instructions 규칙 기반)
if git diff --cached --quiet; then
  echo "[INFO] 커밋할 변경사항이 없습니다."
  exit 0
fi

# 변경 파일 목록 추출
CHANGED=$(git diff --cached --name-only)

MSG=""
if echo "$CHANGED" | grep -qE '^src/domain/'; then
  MSG="impl(domain): update domain logic"
fi
if echo "$CHANGED" | grep -qE '^src/application/'; then
  MSG="impl(application): update application logic"
fi
if echo "$CHANGED" | grep -qE '^src/infrastructure/'; then
  MSG="impl(infrastructure): update infrastructure logic"
fi
if echo "$CHANGED" | grep -qE '^test/'; then
  MSG="test(unit): update or add unit tests"
fi
if echo "$CHANGED" | grep -qE '^docs/Plan/02_core_feature_design.md'; then
  MSG="docs(plan): update core feature design documentation"
fi
if echo "$CHANGED" | grep -qE '^docs/Plan/frimePlan.md'; then
  MSG="plan: update frimePlan checklist"
fi
if echo "$CHANGED" | grep -qE '^tools/'; then
  MSG="ci: update automation scripts"
fi

# 여러 영역이 동시에 변경된 경우 메시지 합치기
if [ $(echo "$CHANGED" | grep -cE '^src/domain/') -gt 0 ] && [ $(echo "$CHANGED" | grep -cE '^test/') -gt 0 ]; then
  MSG="impl(domain): update domain logic & test(unit): update or add unit tests"
fi
if [ -z "$MSG" ]; then
  MSG="chore: update project files"
fi

git commit -m "$MSG"

# 5. git push
if ! git push; then
  echo "[ERROR] git push 실패! 네트워크/권한/충돌 확인 필요" >&2
  exit 3
fi

echo "[SUCCESS] 자동 빌드/테스트/커밋/푸시 완료!"
