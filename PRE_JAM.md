# 잼 이전 제작물 신고

제작 시작(2026-11-12 14:00 KST) 전에 만들었거나 준비한 것. 제출 양식의 "잼 이전 제작물" 항목에 그대로 옮긴다.
게임 코드·에셋·레벨은 여기에 없다. 전부 툴 세팅이다.

## 직접 만든 것 (2026-11-12 04:40~06:00 KST, Claude Code로 작성)

| 항목 | 종류 | 내용 |
|---|---|---|
| `CLAUDE.md` | 프롬프트 | 잼 작업 규칙 (일정, 금지 사항, 기록 방식, 작업 위임 기준) |
| `MEGAJAM.md` | 문서 | 잼 규정 요약 |
| `.claude/skills/gen-3d/` | 프롬프트 | Tripo·Meshy CLI로 3D 모델을 생성하는 절차 |
| `.claude/skills/delegate-deepseek/` | 프롬프트 | OpenCode + DeepSeek에 단순 작업을 맡기는 절차 |
| `DEVLOG.md`, `ASSETS.md`, `PRE_JAM.md` | 문서 | 빈 기록 양식 |

## 기획 문서 (2026-11-12 12:21~12:40 KST, Claude Code로 사용자와 논의해 작성)

제작 시작 전에 쓴 것은 글로 된 기획뿐이다. 게임 코드·에셋·레벨·프로젝트 파일은 만들지 않았다.

| 항목 | 종류 | 내용 |
|---|---|---|
| `Docs/DESIGN.md` | 문서 | 한 문장 콘셉트, 핵심 루프, 장면 3개, 만들 것과 뺄 것, 일정 (12:35 KST 작성) |
| `CLAUDE.md`의 "아트 스타일" 절 | 프롬프트 | 3D 생성 프롬프트에 붙일 스타일 문구 한 줄 (12:38 KST 작성) |

## 잼 이전부터 쓰던 개인 세팅

| 항목 | 종류 | 내용 |
|---|---|---|
| `unreal-project-bootstrap` 스킬 | 프롬프트 | 새 언리얼 프로젝트에 MCP·git·데브로그 양식을 까는 개인 스킬과 템플릿 |
| `unreal-worker`, `blender-worker` | 프롬프트 | 언리얼·블렌더 MCP 조작을 맡는 서브에이전트 정의 |

## 남이 만든 것 (툴 목록에도 기재)

| 항목 | 출처 | 라이선스 |
|---|---|---|
| 스킬 10개: game-jam, prototype-fast, itch-publish, level-design, game-feel, audio-design, unreal-cpp-gameplay, unreal-blueprints, unreal-enhanced-input, unreal-packaging | github.com/gamedev-skills/awesome-gamedev-agent-skills (커밋 25eec57) | Apache-2.0 |
| `meshy-cli` 스킬 | npm `meshy-cli` 0.8.5에 포함 | MIT |
| superpowers, unreal-engine-skills-for-claude-code, code-review, commit-commands | Claude Code 공식 플러그인 | 각 플러그인 라이선스 |

## 게임에 들어간 잼 이전 에셋·코드

없음. (생기면 여기에 추가)
