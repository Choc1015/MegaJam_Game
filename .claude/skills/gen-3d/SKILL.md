---
name: gen-3d
description: Use when the jam game needs a new 3D model generated with Tripo or Meshy (props, environment pieces, characters), or when checking remaining generation credits. Covers which service to use, the exact CLI commands, the credit budget, and the import + ASSETS.md logging steps.
---

# 3D 모델 생성 (Tripo / Meshy CLI)

두 CLI 모두 전역 설치돼 있다 (`tripo` 0.11.x, `meshy` 0.8.x). 로그인은 사용자가 직접 한다.
키를 채팅·코드·로그에 쓰지 않는다.

## 시작 전 확인 (세션당 한 번)

```bash
tripo doctor
tripo balance --json
meshy auth status
meshy balance
```

`tripo doctor`가 실패하거나 잔액이 0이면 멈추고 사용자에게 알린다.
Tripo는 웹(Studio) 크레딧과 API 크레딧이 별개다. 화면에 보이는 크레딧이 CLI 잔액과 다를 수 있다.

## 어느 쪽을 쓸지

2026-11-12 dry-run으로 잰 모델 하나당 비용 (텍스처 포함, 변환 단계 비용은 추정에 안 잡힘):

| 설정 | 크레딧 | 잔액으로 만들 수 있는 수 |
|---|---|---|
| Tripo `--model tripo-v3.1 -p texture_quality=standard` | 20 | 약 50개 |
| Meshy `make` (preview 20 + refine 10) | 30 | 약 37개 |
| Tripo 기본값 (P2 모델, detailed 텍스처) | 120 | 약 8개 |

| 대상 | 쓸 것 |
|---|---|
| 소품, 환경 조각, 주인공급 모델 (기본) | **Tripo 웹** H3.1 (40, 웹 잔액 4235). 아래 "Tripo 웹" 절 |
| 리깅·애니메이션이 필요한 캐릭터 | Meshy CLI (리깅 5, 애니메이션 동작당 3 추가) |
| 사용자가 자리에 없을 때의 대량 생성, 웹이 막혔을 때 | Tripo CLI v3.1 standard (20, CLI 잔액 1000) |
| Tripo CLI P2 (120) | 쓰지 않는다. 같은 품질대는 웹이 더 싸다 |

**Tripo는 `--model`을 빼면 P2(120 크레딧)가 기본이다. 반드시 `--model tripo-v3.1`을 붙인다.**

## 생성 순서

1. **비용 먼저 본다.** 크레딧이 들지 않는다.
   ```bash
   tripo make "<프롬프트>" --model tripo-v3.1 -p texture_quality=standard --then convert:fbx --dry-run --json
   meshy make "<프롬프트>" --dry-run
   ```
   `generation_credits_estimate`가 예상(20)과 다르면 멈추고 설정을 다시 본다.
2. **생성한다.** 출력은 항상 `SourceArt/Generated/<에셋이름>/` 아래.
   ```bash
   tripo make "<프롬프트>" --model tripo-v3.1 -p texture_quality=standard --then convert:fbx -o SourceArt/Generated/<이름> --json --yes --no-open
   meshy make "<프롬프트>" --max-credits 40 -o SourceArt/Generated/<이름>/ --all-assets
   ```
   - 1~3분 걸린다. 2분을 넘길 것 같으면 백그라운드로 돌린다.
   - Tripo에서 리깅까지: `--then texture,rig,convert:fbx`
   - Meshy 세부 명령은 `meshy-cli` 스킬, Tripo 세부 명령은 `tripo docs --llm`을 본다.
3. **결과를 확인한다.** 폴더에 FBX(또는 GLB)가 있는지, 크기가 비정상적으로 크지 않은지 본다.
   폴리곤이 과하면 블렌더에서 줄인 뒤 임포트한다 (`blender-worker`).
4. **언리얼로 임포트한다.** `unreal-worker`에게 맡긴다. 경로는 `/Game/Art/Generated/<이름>/`.
5. **즉시 `ASSETS.md`에 한 줄 추가한다.** 에셋명, 서비스, 프롬프트 요지, 쓴 크레딧, 날짜.

## Tripo 웹(Studio)으로 생성 — 웹 크레딧 4000 사용

사용자 결정(2026-11-12): Tripo는 CLI보다 **웹 크레딧을 먼저** 쓴다. **Claude in Chrome**(`mcp__claude-in-chrome__*`)으로 `https://studio.tripo3d.ai/ko/workspace/generate`를 조작한다. 사용자의 크롬에 로그인이 돼 있다. 내장 브라우저에는 로그인이 없으니 쓰지 않는다.

2026-11-12 05시에 확인한 것:

- 웹 잔액 **4235** (화면 오른쪽 위 동전 아이콘 옆 숫자).
- 생성 비용: AI 모델 "H3.1 – 최고 품질", 텍스처 2K 기준 **40 크레딧** (왼쪽 아래 노란 "생성" 버튼에 표시). 약 100개 분량.
- 화면이 고해상도라 글자가 작다. 읽을 때는 `zoom`이나 `find`를 쓴다.

순서:

1. 왼쪽 "모델" 패널에서 입력 탭 네 개 중 **연필 아이콘(텍스트)** 을 누르고 프롬프트를 넣는다 (1000자 이내).
2. "지오메트리 및 텍스처"를 열어 **폴리곤 수를 확인한다. 기본값이 2,000,000이라 게임에 못 쓴다.** 소품은 2~5만으로 낮춘다. 토폴로지는 삼각형, 텍스처 2K, 조명 제거 켬. 이 설정은 계정에 자동 저장된다.
3. "생성" 버튼의 크레딧을 읽는다. 40과 다르면 멈추고 설정을 다시 본다. 맞으면 누른다.
4. 완성되면 아래 도구 막대의 보라색 **"내보내기"** → 형식 **FBX**, 텍스처 해상도 2K → 노란 "내보내기".
5. 내려받은 파일을 `SourceArt/Generated/<이름>/`으로 옮긴다. 이후(임포트, `ASSETS.md` 기록)는 CLI와 같다.

지킬 것:

- **내려받기는 파일마다 사용자 승인이 필요하다.** 만들 모델 목록(이름, 프롬프트, 예상 크레딧)을 먼저 보여 주고 한 번에 승인받은 뒤, 그 목록만 생성하고 내려받는다.
- "가입/로그인" 버튼이 보이면 로그인이 풀린 것이다. 멈추고 사용자에게 알린다. 로그인은 사용자가 직접 한다.
- 오른쪽 "에셋" 패널에 있는 기존 모델(나무, 바위 등)은 **잼 이전 제작물**이다. 게임에 쓰지 않는다. 쓰려면 `PRE_JAM.md`에 적는다.
- "업그레이드", 크레딧 구매, 공개 범위 변경은 누르지 않는다.
- 아직 확인 못 한 것: 내보내기에 추가 크레딧이 드는지, 내려받은 파일이 저장되는 폴더(크롬 기본 다운로드 폴더로 예상). 첫 생성 때 확인해서 고친다.

## 프롬프트 규칙

- 기획에서 정한 **스타일 문구를 모든 프롬프트 끝에 똑같이 붙인다** (일관된 룩이 심사 기준이다).
  스타일 문구는 `CLAUDE.md`의 "아트 스타일" 항목에 적는다.
- 모델 하나에 물체 하나. 장면 전체, 바닥, 조립할 부품 묶음은 요청하지 않는다.
- 타인의 IP(캐릭터명, 브랜드, 작품명)를 프롬프트에 넣지 않는다.

## 예산

- 2026-11-12 05시 CLI 잔액: **Tripo 1000, Meshy 1110** (월 1100 + 영구 10). Tripo 웹(Studio) 크레딧은 CLI에서 쓸 수 없다. 웹 크레딧은 브라우저에서 직접 생성해 내려받는 용도로 남긴다.
- 실제 생성 한 번으로 변환(FBX) 단계까지 포함한 실제 차감액을 확인하고, 다르면 위 표를 고친다.
- 같은 모델 재생성은 2회까지. 그래도 안 되면 프롬프트를 바꾸거나 블렌더로 직접 만든다.
- 잔액이 20% 아래로 내려가면 사용자에게 알린다.
