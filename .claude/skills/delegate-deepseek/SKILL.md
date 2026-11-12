---
name: delegate-deepseek
description: Use when a jam task is long to type but simple to verify (repetitive C++ classes that follow an existing pattern, data tables, JSON/CSV content, dialogue or UI text, devlog polishing, batch renames) and could be handed to DeepSeek through OpenCode to save Claude tokens.
---

# DeepSeek에게 맡기기 (OpenCode)

OpenCode 1.19에 DeepSeek 키가 등록돼 있다. Claude가 지시문을 쓰고, DeepSeek가 파일을 고치고, Claude가 diff만 검토한다.

## 맡길 것 / 맡기지 않을 것

| 맡긴다 | 직접 한다 |
|---|---|
| 기존 패턴을 따르는 반복 클래스·컴포넌트 | 설계 판단, 새 시스템의 첫 구현 |
| 데이터 테이블, JSON/CSV, 대사·UI 문구 | 빌드 오류·버그 원인 추적 |
| 주석·문서·데브로그 다듬기 | 50줄 안쪽의 작은 수정 (지시문 쓰는 게 더 비싸다) |
| 일괄 이름 변경, 형식 맞추기 | 언리얼 에디터 조작 (→ `unreal-worker`) |

## 순서

1. **먼저 커밋한다.** 위임 결과만 diff에 남게 한다.
2. **지시문을 혼자서도 이해되게 쓴다.** DeepSeek는 이 대화를 모른다. 넣을 것:
   고칠 파일 경로, 따라 할 기존 파일 경로, 끝난 상태의 기준, 건드리면 안 되는 것.
3. **실행한다.**
   ```bash
   opencode run --pure -m deepseek/deepseek-flash --dir "<프로젝트 경로>" --auto "<지시문>"
   ```
   - 어려운 작업은 `-m deepseek/deepseek-v4-pro`.
   - `--pure`는 플러그인을 빼서 호출당 토큰을 줄인다.
   - `--auto`는 권한을 묻지 않는다. **git 저장소인 프로젝트 폴더 안에서만** 쓴다.
   - 2분을 넘길 것 같으면 백그라운드로 돌린다.
4. **검토한다.** `git diff --stat`으로 범위를 보고, 바뀐 파일만 읽는다. 지시 밖의 파일을 건드렸으면 되돌린다.
5. **C++이면 빌드한다.** 빌드가 통과한 뒤에 커밋한다.
6. **`DEVLOG.md`에 남긴다.** 무엇을 DeepSeek에 맡겼는지와 지시문 요지 (글래스 박스 부문용).

## 주의

- 결과가 두 번 연속 틀리면 그 작업은 직접 한다.
- `Content/`의 `.uasset`은 맡기지 않는다. 텍스트 파일만.
