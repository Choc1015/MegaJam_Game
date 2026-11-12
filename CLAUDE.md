# 2026 Epic AI MegaJam 작업 규칙

규정·일정·제출 요건의 기준 문서는 [MEGAJAM.md](MEGAJAM.md)다. 애매하면 그 문서를 먼저 보고, 최종 판단은 itch.io 잼 페이지를 따른다.

## 고정 사실

- **주제:** Director's Cut (감독판)
- **제작 시작:** 2026-11-12(목) 14:00 KST. 그 전에는 게임 코드·에셋·레벨을 만들지 않는다. 툴·MCP·스킬 세팅만 한다.
- **제출 마감:** 2026-11-16(월) 10:59 KST. 목표는 09:00까지 제출 완료.
- **기능 동결:** 2026-11-15(일) 10:59 KST부터 새 기능 금지. 버그 수정과 폴리싱만.
- **빌드:** UE 5.8, Windows, **Development** 빌드. zip은 `TeamName_GameName.zip`, 1GB 미만.
- **테스트 패키징:** 11/13(Day 2)에 반드시 한 번.

## 금지

- 다른 개인 프로젝트(`C:\UnrealProject`의 다른 폴더 등)의 코드·에셋을 가져오지 않는다.
- 타인의 IP를 쓰지 않는다. 3D 생성 프롬프트에도 넣지 않는다.
- WeLoveIndies 사운드는 비상업용 전용이다. 쓰면 `ASSETS.md`에 그 사실을 적는다.

## 기록 (미루지 않는다)

| 파일 | 언제 | 무엇을 |
|---|---|---|
| [DEVLOG.md](DEVLOG.md) | 작업 단위가 끝날 때마다 | 날짜, 한 일, 쓴 AI 툴, 프롬프트 요지, 문제와 해결 |
| [ASSETS.md](ASSETS.md) | 에셋을 들이는 즉시 | 에셋명, 출처, 라이선스, 생성 툴 |
| [PRE_JAM.md](PRE_JAM.md) | 잼 시작 전 제작물을 쓸 때 | 무엇을, 언제 만들었는지 |

커밋은 자주, 마일스톤마다 태그.

## 누구에게 맡길지 (토큰 절약)

| 작업 | 맡길 곳 |
|---|---|
| 언리얼 에디터 조작 (액터, 에셋, 블루프린트) | `unreal-worker` 서브에이전트 |
| 블렌더 조작 | `blender-worker` 서브에이전트 |
| 3D 모델 생성 (Tripo, Meshy) | `gen-3d` 스킬 |
| 길지만 단순한 텍스트 작업 | `delegate-deepseek` 스킬 |
| 설계, 새 시스템, 디버깅, 리뷰 | 메인 세션이 직접 |

서브에이전트를 직접 띄울 때(검색, 파일 읽기, 단순 확인)는 `model`을 `sonnet`이나 `haiku`로 지정한다. 메인 모델을 물려받게 두지 않는다.

## 모델 (Pro 요금제, 5시간 한도가 빠듯하다)

세션이 자기 모델을 바꿀 수는 없다. 아래 기준과 현재 모델이 다르면 **작업을 시작하기 전에 한 줄로** 사용자에게 모델 메뉴에서 바꾸라고 권한다.

| 작업 | 모델 | effort |
|---|---|---|
| 구현, 세팅, 문서, 에디터 조작 지시, 패키징 | Sonnet 5.5 | medium |
| 기획 확정, 새 시스템 설계, 두 번 시도해도 안 풀리는 버그 | Fable 5.2 또는 Opus 5.5 | high |
| 단순 조회, 한 줄 수정 | Haiku 5.5 | low |

DeepSeek는 OpenCode로 쓸 수 있다(`delegate-deepseek` 스킬). 모델을 권할 때는 Claude 모델 셋에 DeepSeek 위임까지 넣어 비교한다: 길지만 검증이 쉬운 작업(반복 클래스, 데이터 표, 텍스트)은 DeepSeek에 맡기고, 판단이 필요한 건 Claude에서 한다.

비싼 모델은 짧은 세션으로 쓰고, 결론을 `CLAUDE.md`나 `Docs/`에 적은 뒤 Sonnet 세션에서 구현한다.

## 컨텍스트 관리

- 자동 압축은 20만 토큰에서 일어난다 (`~/.claude/settings.json`의 `autoCompactWindow`).
- 세션은 기능 단위로 새로 연다. 한 기능이 끝나면 커밋하고 새 세션.
- **문서는 통째로 읽지 않는다.**
  - `DEVLOG.md`, `ASSETS.md`: 읽지 않고 끝에 덧붙인다 (`cat >> 파일 <<'EOF'`). 확인은 `Grep`이나 끝 30줄만.
  - `MEGAJAM.md`: 필요한 절만 `Grep`으로 찾아 읽는다. 일정·빌드 요건은 이 파일 위쪽에 이미 있다.
  - 스킬의 `references/`는 그 주제가 실제로 필요할 때만 연다.
- **문서가 커지면 스스로 줄인다.** 묻지 않고 한다.
  - `CLAUDE.md`가 120줄을 넘으면: 세부 절차를 `.claude/skills/`나 `Docs/`로 옮기고 여기엔 한 줄 링크만 남긴다.
  - `DEVLOG.md`가 300줄을 넘으면: 지난 날짜분을 `Docs/Devlog/day-N.md`로 옮기고 본 파일엔 오늘 것만 둔다.
  - 끝난 일의 메모, 중복 설명, 더는 맞지 않는 내용은 지운다.

## 압축할 때 남길 것 (Compact Instructions)

요약에 반드시 남긴다: 지금 하던 작업과 다음 한 단계, 사용자가 내린 결정, 고친 파일 경로, 아직 안 한 것과 검증 안 된 것, 마감까지 남은 일정, 크레딧 잔액. 버린다: 도구 출력 원문, 스크린샷, 조사 과정, 이미 파일에 적힌 내용.

## 스킬

- 범위·일정·컷: `game-jam`, `prototype-fast`
- 언리얼: `unreal-cpp-gameplay`, `unreal-blueprints`, `unreal-enhanced-input`, `unreal-packaging`
- 완성도: `game-feel`, `level-design`, `audio-design`
- 제출: `itch-publish`
- 3D 생성: `gen-3d`, `meshy-cli`
- 기획 브레인스토밍: `superpowers:brainstorming`

## 아트 스타일

3D 생성 프롬프트 끝에 이 한 줄을 똑같이 붙인다 (기획은 [Docs/DESIGN.md](Docs/DESIGN.md)):

`stylized low-poly, hand-painted flat colors, warm muted palette, soft rounded shapes, prop from an old Korean countryside house, single object, plain background, no text, no logo`

극장판과 감독판은 같은 모델을 쓰고, 차이는 후처리로만 낸다 (극장판: 따뜻하고 선명 / 감독판: 색 빠진 푸른빛, 필름 노이즈, 검은 띠).

## 범위 원칙

완성된 재밌는 게임이 야심차지만 망가진 게임보다 낫다. 압박이 오면 기본값은 "뺀다".

## 프로젝트 구조

- 프로젝트: `C:\UnrealProject\WhatWasCut` (WhatWasCut.uproject), UE 5.8, First Person C++ 템플릿에서 시작. Variant_Horror/Shooter, Weapons는 제거함.
- 원격: https://github.com/Choc1015/MegaJam_Game (git + LFS). 시작 레벨: `Content/FirstPerson/Lvl_FirstPerson`.
- Unreal MCP: 포트 8010 (`Config/DefaultEditorPerProjectUserSettings.ini`에서 자동 시작). 도구 목록 캐시: `.claude/unreal-tools.md`.
- 기획: [Docs/DESIGN.md](Docs/DESIGN.md). 블렌더 내보내기: `SourceArt/Export/`.
