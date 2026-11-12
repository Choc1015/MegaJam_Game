# 확인된 언리얼 MCP 도구

unreal-worker가 `describe_toolset`으로 확인한 도구를 한 줄씩 추가한다. 여기 있는 도구는 다시 조회하지 않고 바로 `call_tool`로 쓴다.
형식: `- <toolset_name> / <tool_name>(<핵심 인자>) — 용도`

## 자주 쓰는 툴셋 (이름 확인됨)
- `editor_toolset.toolsets.scene.SceneTools` — 레벨 로드, 액터 배치·삭제, 카메라, 아웃라이너
- `editor_toolset.toolsets.actor.ActorTools` — 액터 트랜스폼·라벨·부모·컴포넌트
- `editor_toolset.toolsets.asset.AssetTools` — 에셋 조회·임포트·저장
- `editor_toolset.toolsets.blueprint.BlueprintTools` — 블루프린트 생성·편집
- `editor_toolset.toolsets.object.ObjectTools` — 프로퍼티 조회·수정, 클래스 탐색
- `editor_toolset.toolsets.material.MaterialTools` / `material_instance.MaterialInstanceTools`
- `editor_toolset.toolsets.static_mesh.StaticMeshTools` / `skeletal_mesh.SkeletalMeshTools`
- `editor_toolset.toolsets.programmatic.ProgrammaticToolset` — 여러 도구 호출을 파이썬 스크립트 하나로 묶어 실행
- `EditorToolset.EditorAppToolset` — 콘솔 변수, 선택, 뷰포트 카메라, PIE 제어
- `EditorToolset.LogsToolset` — 출력 로그 읽기
- `aimodule_toolset.toolsets.behavior_tree.BehaviorTreeTools` / `state_tree_toolset.toolsets.state_tree.StateTreeTools`
- `ToolsetRegistry.AgentSkillToolset` — 프로젝트 스킬 목록·읽기

## 도구 상세
(공통: 액터·에셋 객체 인자는 `{"refPath": "..."}` 형식, xform은 `{location:{x,y,z}, rotation:{pitch,yaw,roll}, scale:{x,y,z}}`, 생략한 필드는 변경 안 함)
- SceneTools / save_actor(actor) — 액터를 디스크에 저장
- SceneTools / is_checked_out(actor) — 액터가 현재 사용자에게 체크아웃됐는지
- SceneTools / can_edit(actor) — 액터 편집 가능 여부(소스컨트롤 잠금 확인)
- SceneTools / commit_level_instance(level_instance, discard=false) — 레벨 인스턴스 편집 종료(저장/폐기)
- SceneTools / edit_level_instance(level_instance) — 레벨 인스턴스 편집 모드 진입
- SceneTools / create_level_instance(level_path, name, xform, parent) — 레벨 에셋 참조 Level Instance 액터 생성
- SceneTools / merge_actors(actors[], output_path, name, destroy_source_actors=false) — StaticMesh 액터들을 하나의 메시로 병합
- SceneTools / trace_world(start, end) — 라인 트레이스, 첫 히트까지 거리 반환(없으면 None)
- SceneTools / delete_folder(folder_path) — 아웃라이너 폴더 삭제(액터는 부모 폴더로)
- SceneTools / rename_folder(old_path, new_path) — 아웃라이너 폴더 이름 변경/병합
- SceneTools / set_actor_folder(actor, folder_path) — 액터를 아웃라이너 폴더에 배정(빈 문자열=루트)
- SceneTools / get_actors_in_folder(folder_path, recursive=false) — 폴더 안 액터 목록
- SceneTools / get_folders() — 사용 중인 아웃라이너 폴더 경로 전체
- SceneTools / remove_from_scene(actor) — 액터 삭제
- SceneTools / add_to_scene_from_asset(asset_path, name, xform, parent, snap_to_ground=false) — 에셋으로 액터 스폰
- SceneTools / add_to_scene_from_class(actor_type, name, xform, parent, snap_to_ground=false) — 클래스로 액터 스폰
- SceneTools / find_actors(name, tag, actor_type, root, bounds, collision_channels) — 조건으로 액터 검색(name/tag/collision_channels는 필수 키, 빈 값 가능)
- SceneTools / get_collision_channels() — find_actors용 콜리전 채널 목록
- SceneTools / get_current_level() — 현재 레벨 에셋 경로
- SceneTools / load_level(level_path) — 레벨 로드
- ActorTools / remove_component(component) — 컴포넌트 제거
- ActorTools / add_component(owner, component_type, name) — 액터/블루프린트/씬컴포넌트에 컴포넌트 추가
- ActorTools / get_components(actor, component_type) — 액터의 컴포넌트 목록
- ActorTools / get_actor_bounds(actor) — 월드 바운딩 박스
- ActorTools / set_parent_component(component, parent) — 씬 컴포넌트 부모 변경(None=분리)
- ActorTools / get_parent_component(component) — 부모 씬 컴포넌트
- ActorTools / get_component_actor(component) — 컴포넌트 소유 액터
- ActorTools / get_root_component(actor) — 루트 컴포넌트
- ActorTools / look_at(actor, target) — 액터가 월드 좌표를 바라보게 회전
- ActorTools / set_actor_transform(actor, xform, worldspace=true) — 위치/회전/스케일 설정
- ActorTools / get_actor_transform(actor) — 월드 트랜스폼 조회
- ActorTools / add_tag(actor, tag) — 태그 추가
- ActorTools / remove_tag(actor, tag) — 태그 제거
- ActorTools / has_tag(actor, tag) — 태그 보유 여부
- ActorTools / get_tags(actor) — 태그 목록
- ActorTools / set_label(actor, label) — 아웃라이너 표시 이름 설정
- ActorTools / get_label(actor) — 표시 이름 조회
- AssetTools / write_file(file_path, content) — /Game, 플러그인 Content, Saved 아래 텍스트 파일 쓰기(덮어씀)
- AssetTools / read_file(file_path) — 위 경로의 텍스트 파일 읽기
- AssetTools / get_plugin_content_paths(include_engine=false) — 콘텐츠 있는 플러그인 루트 경로
- AssetTools / get_dependencies(asset_path) — 에셋이 의존하는 에셋 목록
- AssetTools / get_referencers(asset_path) — 에셋을 참조하는 에셋 목록
- AssetTools / is_checked_out(asset_path) — 체크아웃 여부
- AssetTools / can_edit_asset(asset_path) — 편집 가능 여부
- AssetTools / is_dirty(asset_path) — 저장 안 된 변경 여부
- AssetTools / save_assets(asset_paths[]) — 에셋 저장(빈 리스트=모든 dirty 에셋)
- AssetTools / load_asset(asset_path) — 에셋 로드, 객체 참조 반환
- AssetTools / update_metadata_tags(asset_path, set_tags, remove_tags) — 메타데이터 태그 설정/삭제
- AssetTools / get_metadata_tags(asset_path) — 메타데이터 태그 조회
- AssetTools / get_asset_class(asset_path) — 에셋 클래스 이름
- AssetTools / get_asset_tags(asset_path) — 에셋 레지스트리 태그 조회
- AssetTools / find_assets(folder_path, name, asset_type, recursive=true, tags) — 에셋 검색(folder_path ""=전체)
- AssetTools / delete(path) — 에셋/폴더 삭제
- AssetTools / move(path, new_path) — 에셋/폴더 이동·이름변경
- AssetTools / duplicate(path, new_path) — 에셋/폴더 복제
- AssetTools / exists(path) — 에셋/폴더 존재 여부
- AssetTools / list_folders(root_path, recursive=true) — 하위 폴더 목록
- AssetTools / create_folder(path) — 폴더 생성
- StaticMeshTools / import_file(folder_path, asset_name, source_file, import_materials, import_textures, combine_meshes=true) — FBX 등을 스태틱 메시로 임포트(머티리얼/텍스처는 원래 이름으로 생성되니 move로 개명). 반환엔 메시만 나옴
- StaticMeshTools / get_bounds(mesh) — 로컬 바운딩 박스(cm)
- StaticMeshTools / get_triangle_count(mesh, lod_index=0) / get_vertex_count — 삼각형/정점 수
- StaticMeshTools / get_material_slots(mesh), get_material(mesh, slot_name), set_material(mesh, slot_name, material) — 머티리얼 슬롯
- StaticMeshTools / generate_lods, generate_convex_collisions, set_nanite_enabled, remove_lods, remove_collisions — LOD/콜리전/나나이트
- 임포트 절차(검증됨): StaticMeshTools/import_file(import_materials/textures=true)가 머티리얼 BaseColor에 TextureSample을 자동 연결하고 슬롯에도 할당함. 이후 find_assets로 폴더 조회 → AssetTools/move로 M_/T_ 개명(이동해도 연결 유지). 메시 refPath는 `/경로/SM_X.SM_X` 형식 필요(`/경로/SM_X`만 쓰면 invalid object path)
- MaterialTools / get_property_input(material{refPath}, material_property "MP_BaseColor") — BaseColor에 연결된 노드 확인(expression None이면 미연결)
- AssetTools / get_asset_class(asset_path) — 클래스 판별용(texture/material 구분)
- AssetTools / get_dependencies — 의존성 없으면 오류('NoneType' not iterable) 가능
- EditorAppToolset / CaptureViewport — gridSpacing 0, gridExtent 0, gridHeight 0, maxLabelDistance 0, maxLabels 0, classFilter {refPath:"/Script/Engine.Actor"} 인자 필요
- ProgrammaticToolset / get_execution_environment() — 스크립트 환경 안내(스크립트 쓰기 전 1회 호출 필수)
- ProgrammaticToolset / execute_tool_script(script) — 파이썬 스크립트 실행. `def run()` 이 dict 반환, 내부에서 `execute_tool("<전체 도구명>", json.dumps({...}))["returnValue"]` 로 호출(실패 시 RuntimeError), import 가능: json, math, datetime, copy, re, time

## 소품 배치 중 추가된 메모
- editor_toolset.toolsets.scene.SceneTools / add_to_scene_from_asset(asset_path,name,xform{location,rotation,scale}) — 메시 에셋 경로로 StaticMeshActor 배치
- editor_toolset.toolsets.scene.SceneTools / set_actor_folder(actor,folder_path) — 아웃라이너 폴더
- editor_toolset.toolsets.scene.SceneTools / find_actors(name,tag,collision_channels) — name만 채우고 tag:"" collision_channels:[] 필수
- editor_toolset.toolsets.actor.ActorTools / set_label(actor,label), get_label, get_actor_bounds(월드 AABB)
- editor_toolset.toolsets.actor.ActorTools / set_actor_transform(actor,xform) — 빠진 필드는 "유지"가 아니라 identity로 리셋됨. location/rotation/scale 전부 넘길 것
- editor_toolset.toolsets.asset.AssetTools / save_assets(asset_paths=[]) — 빈 리스트 = 전체 저장
- EditorToolset.EditorAppToolset / CaptureViewport(captureTransform, annotations{gridSpacing:0,gridExtent:0,gridHeight:0,maxLabelDistance:0,maxLabels:0}) — annotations 필수. 결과는 base64 JSON이 ~1MB라 파일로만 저장됨 (Bash 없으면 이미지 못 봄)
- editor_toolset.toolsets.object.ObjectTools / get_properties(instance{refPath},properties[]), set_properties(instance,values=JSON문자열) — 컴포넌트 refPath는 "<액터refPath>.CutVisibility", 열거값은 문자열
- editor_toolset.toolsets.actor.ActorTools / get_components(actor), get_actor_transform(actor)
- editor_toolset.toolsets.programmatic.ProgrammaticToolset / execute_tool_script — execute_tool(name, json.dumps(args))
