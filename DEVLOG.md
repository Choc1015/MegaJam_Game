# 개발 로그 — 2026 Epic AI MegaJam

주제: **Director's Cut**. 작업 단위가 끝날 때마다 아래 양식으로 추가한다 (글래스 박스 부문 제출용).

양식: 시각(KST) · 한 일 · 쓴 AI 툴 · 프롬프트 요지 · 문제와 해결

---

## Day 0 — 2026-11-12 (목), 제작 시작 전 세팅

### 04:40~06:00 툴 세팅 (게임 제작 아님)

- **한 일**
  - 잼 규정을 `MEGAJAM.md`로 정리하고, itch.io 잼 페이지와 대조해 시작·마감 시각을 확인했다 (11/12 00:00 EST = 14:00 KST, 11/15 20:59 EST = 11/16 10:59 KST).
  - Tripo·Meshy 공식 CLI를 설치했다 (`tripo-cli` 0.11.2, `meshy-cli` 0.8.5).
  - 게임 개발 스킬 10개를 골라 넣었다 (awesome-gamedev-agent-skills, Apache-2.0). 전체 79개 중 잼·언리얼·완성도 관련만.
  - OpenCode에서 DeepSeek 호출과 파일 쓰기 위임이 되는지 시험했다.
  - 3D 생성 절차(`gen-3d`)와 DeepSeek 위임 절차(`delegate-deepseek`)를 스킬로 적었다.
- **쓴 AI 툴:** Claude Code (Claude Fable 5.2), OpenCode + DeepSeek (연결 시험만)
- **프롬프트 요지:** "규정 문서를 기준으로 삼고, Tripo·Meshy를 CLI로 연결하고, 기획·토큰 최적화에 도움 되는 스킬을 찾아 적용하고, OpenCode + DeepSeek로 토큰을 줄이자."
- **문제와 해결**
  - Tripo CLI 패키지가 npm에 두 개(`tripo-cli`, `@tripo3d/cli`) 있어 어느 쪽이 공식인지 불분명했다. → Tripo 개발자 문서가 `npm install -g tripo-cli`를 안내하는 것을 확인하고 그쪽을 설치했다.
  - Tripo는 웹(Studio) 크레딧과 API 크레딧이 별개라는 것을 알았다. → 로그인 후 `tripo balance`로 CLI에서 쓸 수 있는 잔액부터 확인하기로 했다.
  - 스킬 묶음 중 Claude-Code-Game-Studios(에이전트 49개, 스킬 70여 개, 훅 15개)는 4일 잼에는 무겁고 언리얼 경로가 검증되지 않아 넣지 않았다.

### 06:00~06:30 3D 생성 경로 확정, 토큰 절약 세팅

- **한 일**
  - Tripo·Meshy CLI 잔액 확인: Tripo 1000, Meshy 1110. dry-run으로 모델당 비용을 쟀다 (Tripo 기본 P2 120, v3.1 20, Meshy 30).
  - Tripo 웹(Studio) 잔액 4235는 CLI와 별개라서, 크롬을 조작해 웹에서 생성하는 경로를 기본으로 정했다 (모델당 40). 생성~FBX 내보내기 화면 흐름만 확인했고 실제 생성은 하지 않았다.
  - Roblox MCP를 사용자 설정에서 제거했다 (세션마다 드는 도구 설명 토큰 절감).
  - 자동 압축 기준을 100만 토큰의 97%에서 20만 토큰으로 낮췄다 (`autoCompactWindow`).
  - `CLAUDE.md`에 작업별 모델 기준, 문서를 통째로 읽지 않는 규칙, 압축 시 남길 것을 적었다.
- **쓴 AI 툴:** Claude Code (Claude Fable 5.2), Claude in Chrome
- **문제와 해결**
  - Tripo CLI는 옵션 없이 돌리면 120 크레딧짜리 모델이 기본이라 8개 만에 잔액이 바닥난다. → 웹을 기본으로, CLI는 `--model tripo-v3.1`을 강제하는 예비로 돌렸다.
  - Tripo 웹의 폴리곤 수 기본값이 200만이다. → 생성 전에 2~5만으로 낮추는 단계를 절차에 넣었다.
  - 세팅 한 번에 Pro 요금제 5시간 한도의 40% 이상을 썼다 (Fable 5.2, effort high, 컨텍스트 26만 토큰). → 구현은 Sonnet 5.5로, Fable은 기획·설계에만 쓰기로 했다.

## 2026-11-12 12:21~12:40 KST · 기획 확정 (제작 시작 전, 글만)

- **한 일**
  - 사용자와 질문 하나씩 주고받으며 주제 "Director's Cut"을 한 문장으로 줄였다: 같은 집의 극장판과 감독판을 오가며, 잘려 나간 물건을 가져와 장면을 완성한다.
  - 장면 3개(안방의 약, 부엌의 쌀, 마루의 필름)와 결말, 만들 것과 뺄 것, 늘리는 순서, 일정을 `Docs/DESIGN.md`에 적었다.
  - `CLAUDE.md`의 "아트 스타일" 절과 `PRE_JAM.md`의 기획 문서 항목을 채웠다.
- **쓴 AI 툴:** Claude Code (Claude Fable 5.2), 스킬 `superpowers:brainstorming`, `game-jam`
- **프롬프트 요지:** "주제를 한 문장으로 줄이고 범위(만들 것, 뺄 것)를 정한다. 한 번에 질문 하나씩."
- **사용자가 낸 것:** "한 게임, 두 이야기"라는 해석, 극장판에서 막히면 감독판에서 물건을 가져오는 루프, 할머니와 약 장면, 사건을 하나씩 푸는 구조.
- **AI가 낸 것:** 감독판을 "할머니가 떠난 뒤의 집"으로 묶는 규칙, 장면 2·3과 결말, 범위 축소(한 집, 장면 3개), 아트 방향.
- **문제와 해결**
  - 사건마다 장소가 달라지면 4일 안에 못 끝낸다. → 한 집의 방 3개로 줄였다.
  - 분량이 짧다는 걱정(5~10분). → 세 장면이 끝까지 돌아간 뒤 내용만 늘리는 순서를 정해 두었다.

## 2026-11-12 (사전 점검)
- 잼 시작 전 툴체인 확인: 빈 FirstPerson C++ 프로젝트로 Windows Development 패키징. 첫 시도 실패(MSVC C3859, 윈도우 오류 1455 = 페이징 파일 부족). 페이징 파일 확대 후 성공, zip 533MB, 실행 확인. 게임 에셋/코드 아님(잼 제출물과 무관).

## 2026-11-12 (Day 1) — 코어 시스템 + 안방 그레이박스
- 한 일: Cut 시스템 C++ 구현 (UCutWorldSubsystem: 극장판/감독판 전환·자막·텍스트 CSV 로드, UCutVisibilityComponent, ACutTV/Interactable/Pickup/UseTarget, ACutHUD 임시 틴트·레터박스·CRT 접힘 전환). E키 상호작용, 점프 제거. 텍스트는 Content/Data/CutText.csv (데이터화, 패키징 NonUFS 스테이징 추가). 안방 두 판 그레이박스를 Lvl_FirstPerson에 배치 (방 A X=0, 방 B X=6000, TV 서로 Partner).
- 쓴 AI 툴: Claude Code (Sonnet 5.5), Unreal MCP (unreal-worker 서브에이전트로 레벨 배치).
- 문제/해결: Live Coding 중이면 UBT 빌드 불가 → 에디터 종료 후 빌드. 컴파일은 한 번에 성공.
- 남은 것: PIE 동작 검증, 퍼즐/궤/약봉지 조합, 엔딩, 타이틀, 후처리, 사운드 (11/13~11/14).

## 2026-11-12 — 판 이름 교체 (DESIGN.md 3판)
- 극장판 = TV 너머의 끝난 집(푸른 틴트·검은 띠), 감독판 = 지금의 집(따뜻함, 시작 판). 시작 글자 카드 제거, 극장판으로 처음 건너갈 때만 `Theatrical Cut`. 결말 03:40 `Director's Cut` 카드는 미구현.
- 레벨: 방 A=감독판만, 방 B=극장판만, 글 ID 접미사 교체. enum 표시 이름 한글화(감독판만/극장판만/둘 다).

### 2026-11-12 모델링 단계 시작
- Gemini(Nano Banana)로 CRT TV 레퍼런스 생성, `SourceArt/Reference/tv_ref_01.png` 저장. 흰 배경 단일 오브젝트. 이 이미지를 화풍 기준으로 삼아 다른 소품 레퍼런스에 첨부.
- 문제: 처음엔 일반 Gemini 모드로 생성되어 이미지 모드로 재생성. 배경이 벽/선반이라 "plain solid off-white background" 명시해 재생성.
- 다음: Tripo/Meshy에 이 이미지를 넣어 TV 모델 생성(gen-3d).
- Tripo 웹(H3.1, 2K, 삼각형, 폴리 30000 설정)으로 tv_ref_01.png → TV 모델 생성, 40 크레딧(웹 잔액 4195). 결과 25,312 tri. FBX(Mixamo 프리셋) 내보내기는 추가 크레딧 없음. 산출물 `SourceArt/Generated/tv_crt/`.
- 팁: 크롬 확장 업로드는 세션이 읽을 수 있는 폴더(스크래치패드)로 복사해야 함.
- 언리얼 임포트: `/Game/Art/Generated/tv_crt/` SM_TV_CRT, M_TV_CRT, T_TV_CRT_BaseColor. 크기 약 87x57x98cm, 피벗 바닥 중앙, 25,312 tri, 슬롯 1개. 뷰포트 육안 확인은 아직.

### 2026-11-12 소품 레퍼런스 이미지 17장 생성
- 도구: Gemini(Nano Banana, 이미지 모드), 같은 대화에서 TV 이미지를 스타일 기준으로 사용.
- 한 일: 소품 17종(grandma_blanket, blanket_folded, portrait_frame, incense_burner, wall_clock, desk_lamp, medicine_cabinet, notebook, result_envelope, chest, key, medicine_packet, rice_jar, calendar, phone, wardrobe, white_cloth) 레퍼런스를 `SourceArt/Reference/<이름>_ref.png`로 저장. 프롬프트는 `Docs/PROPS_PROMPTS.md`의 요지 + 스타일 문구.
- 메모: 조명 인물/사진풍으로 쏠리는 경우가 있어(desk_lamp 등) Tripo 결과를 보고 필요하면 재생성.
- 다음: Tripo 웹 H3.1로 모델 생성(개당 40크레딧), FBX 내보내기, 언리얼 임포트.

### 2026-11-12 소품 3D 모델 17개 생성 (Tripo)
- 도구: Claude in Chrome으로 Tripo Studio 웹 조작. H3.1, 이미지→3D(입력은 Gemini 레퍼런스), 2K 텍스처, 폴리곤 30000, 조명 제거. 개당 40크레딧, FBX 내보내기는 추가 크레딧 없음. 개당 약 3.5분.
- 결과: tv_crt 포함 18개 중 17개를 `SourceArt/Generated/<이름>/<이름>.fbx`로 저장. 웹 잔액 약 3515(시작 4155).
- 문제: ① 업그레이드 팝업이 생성 클릭을 가림, "Maybe Later"로 닫음. ② 내보내기 전에 페이지를 벗어나 wall_clock 내보내기가 날아가 재내보냄. ③ 폴리가 약 25~29k tri로 초안(1만 이하)보다 큼.
- 품질 메모: result_envelope는 덩어리진 형태, key는 얇은 막대, medicine_packet은 단순함. 필요하면 재생성.
- 다음: 언리얼 임포트(unreal-worker 진행 중), 레벨 배치.
- 언리얼 임포트 완료: 17개 모두 `/Game/Art/Generated/<이름>/`에 SM_/M_/T_ 3종씩. 삼각형 25.6k~29.7k, 머티리얼 BaseColor 연결 확인. 레벨 배치와 뷰포트 육안 확인은 아직.

### 2026-11-12 소품 레벨 배치
- 방 A(감독판, 지금의 집)와 방 B(극장판, 끝난 집, x+6000)에 소품 장식을 Props_A/Props_B 폴더로 배치. 사용자가 방 A의 회전을 손봤고 방 B를 그에 맞춤.
- 방 전용: A에 누운 할머니, B에 개어 놓은 이불·영정·향로·흰 천. 공용: 장롱, 약장, 벽시계, 달력, 전화기, 스탠드.
- 글 정리: CutText.csv의 Bed/Note의 .T와 .D를 맞바꿈(옛 이름 기준이어서 반대였음). Chest를 방 A로 옮기고 DirectorsOnly로 변경(약을 머리맡에 두는 단계). 약봉지는 B에 유지.
- 남은 일: TV_Test와 TV_A 겹침(TV_A 메시를 SM_TV_CRT로), 방 B 조명 어두움, DESIGN.md 옛 이름 정리, 약장/장롱 벽 파고듦.

- **2026-11-12 19:40 KST · 문서 동기화 (기획 세션):** `Docs/DESIGN.md`를 3판(15:08, 극장판·감독판 이름 맞바꿈)으로 바꿨다. 그동안 이 폴더의 사본은 2판이었고, 3판은 `C:\UnrealProject\MegaJam`에만 있었다. `CLAUDE.md` "아트 스타일" 절의 룩 한 줄도 새 이름으로 고쳤다. 코드는 커밋 8e5a3dd에서 이미 3판 이름을 따르고 있다. 쓴 툴: Claude Code.

## 2026-11-12 TV 교체
- TV_A/TV_B(ACutTV)의 박스 플레이스홀더를 SM_TV_CRT로 교체. 위치 (∓270,0,0)(+6000), yaw -90, 스케일 0.7. Partner/Presence 등 속성 그대로. 임시 TV_Test 삭제.
- 설계 문서(DESIGN.md)는 기획 세션이 이미 새 이름(감독판=지금, 극장판=끝난 세계)으로 반영함.
- 도구: unreal-worker(sonnet). 캡처로 화면이 방 안쪽을 향함을 확인.

## 2026-11-12 방 구조·그래픽 기획 (ROOM_PLAN)
- 한 일: `Docs/ROOM_PLAN.md` 작성. 시점(눈높이 135, 캡슐 키 150, FOV 95, 천장 240), 벽·바닥·천장은 큐브 + 월드 정렬 절차적 머티리얼(장판, 벽지/회벽 이단, 창호지 Two Sided Foliage), 기둥·인방·걸레받이·창·방문·형광등은 큐브. 판별 빛(감독판: 스탠드 주광원 + TV 푸른빛 + 달빛 / 극장판: 형광등 + 흐린 창 + TV 속 따뜻한 빛), 노출 고정, 방마다 bound PPV. 새 소품 P0 5·P1 7(+부엌 3)·P2 4와 프롬프트, 방 A 평면도와 시작점 이동, 컷 라인.
- 사용자 결정: 방을 키우지 말고 시야를 내리고 넓게, 방을 채운다. 마루·부엌은 안방에 이어서 짓는다.
- 도구: Claude Opus 5.5(기획), unreal-worker(sonnet)로 방 A 배치 조회, general-purpose(sonnet)로 구현자 시점 검토.
- 남은 일: 구현 세션(Sonnet 5.5)이 §8 확인(Chest_A의 AcceptItemId, TV 도착점) 뒤 단계 1부터.

## 2026-11-12 방 뼈대 구현 (ROOM_PLAN 단계 1)
- 한 일: 시점(눈높이 135, 캡슐 r30 hh75, FOV 95, 걷기 200)을 C++와 BP_FirstPersonCharacter 양쪽에 반영(C++는 아직 컴파일 안 함: Ctrl+Alt+F11). 절차적 머티리얼 M_Jangpan/M_Wall_Lower/M_Wall_Upper/M_Wood_Dark/M_Changho/M_Maru/M_Earth/M_Soot/M_Prop. 방 A·B 구조(바닥·벽·천장·기둥·인방·걸레받이·문틀·창·방문·형광등), 마루·부엌 뼈대를 큐브로 짓고 방 B는 x+6000 복제. 템플릿 지오메트리와 하늘·태양·안개·구름 제거. 빛 §5 대로 배치(스탠드 250, TV, 달빛/창, 형광등 160, 마루·부엌 약한 점광원). PPV: 전역 unbound(노출 Manual) + 방별 bound. PlayerStart (-165,-150) yaw 85.
- 도구: unreal-worker(sonnet)로 소품 배치·액터 정리, 메인 세션이 MCP 스크립트로 구조·빛.
- 문제와 해결: set_actor_transform은 빠뜨린 필드를 초기화하므로 위치·회전·스케일을 항상 다 줌. find_actors name이 부분 일치라 'Stand_A'가 소품 메시도 잡음(PointLight로 필터). 노출은 시작값(bias 6)에서 눈으로 맞춤. Live Coding은 MCP로 못 켬.
- 남은 일: PIE로 도착 지점·눈높이 확인, 방 B 스탠드 소품 숨김 여부, 빛 미세 조정.

## 2026-11-12 방 A 조명·할머니 이불 수정
- 방 A가 어두움: 천장 RectLight `FluorLight_A`(110, 3500K) 추가, Stand_A 250→450, Window_A 6→14. 노출은 Manual/Bias 6 고정(PPV_Global).
- 할머니 이불이 비균일 스케일(2.04,1.23,0.59)로 찌그러지고 침대 큐브에 묻혀 있었음 → 균일 1.3, 위치 (100,195,0).
- 방 B 이불은 사용자가 직접 배치(개어진 이불), 손대지 않음.
