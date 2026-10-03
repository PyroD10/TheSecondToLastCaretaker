# 06 — Game reference (names found in dumps; update on every re-dump)

Game version: `__________` (Dan: main menu) · UE4SS `v3.0.1 Beta #0, Git SHA 527a483b` (confirmed from UE4SS.log 2026-10-03)
Dumps: `ue4ss/` → symlink to `E:\SteamLibrary\steamapps\common\Voyage\Voyage\Binaries\Win64\ue4ss\`.
Last object dump: 2026-10-03 14:29 — **taken in main menu**, lacks in-game instances. Re-dump needed.

## Dump formats
- `UE4SS_ObjectDump.txt`: one object per line, `[addr] <Class> /Path/To.Object [n: …] [c: …] [or: …]`.
  Assets look like `SkeletalMesh /Game/…/SK_X.SK_X`; live actors like `BP_Foo_C /Game/Maps/<Map>.<Map>:PersistentLevel.BP_Foo_C_0`.
  `tools/grep_dump.py <regex>` searches it.
- `CXXHeaderDump/<Module|Blueprint>.hpp`: one header per module / Blueprint class, with member offsets and UFunction signatures.
  The game module is `Voyage.hpp` (~17 k lines). Blueprint classes get their own file, e.g. `BP_Base_Character.hpp`.
- `<timestamp>-ue4ss_actor_data.csv`: Live View actor export — class, location, rotation, scale, static meshes. In-game snapshot.

## Player
| What | Name | Notes |
|---|---|---|
| Menu pawn | `/Game/Blueprints/Game/BP_VoyageMenuPawn.BP_VoyageMenuPawn_C` | position 0/0/0 → "not in world" |
| Player character | `/Game/Blueprints/BP_FirstPersonCharacter_New.BP_FirstPersonCharacter_New_C` | class chain: `ABP_FirstPersonCharacter_New_C : ABP_Base_Character_C : AVoyageCharacter : AVoyageBaseCharacter : (ACharacter?)` — verify base in `Voyage.hpp` |
| Third person | `ToggleThirdPerson()` on the character, `ThirdPersonCamera`, `SpringArm_0` | the game has a full third-person body → there is a body mesh + anim BP we can reuse for ghosts |
| First-person mesh parts | components `SK_Head`, `SK_RightArm`, `SK_LeftArm`, `SK_Foot` (`USkeletalMeshComponent`) on `ABP_Base_Character_C` | |
| Character art | `/Game/Characters/Mechanical_Light_01_A/Mesh/SK_Mechanical_Light_01_A_{Body,Head,LeftHand,RightHand,Shoes}_01_A` | the caretaker is a robot; full-body skeletal mesh for third person not seen in the menu dump |
| Anim BPs | `ABP_Manny_New` (+ `BP_Manny_AnimLayer_*`), anims under `/Game/Characters/Mannequins/Animations/Manny/MM_*` | UE5 "Manny" skeleton → body mesh is probably Manny-compatible |
| Held items (static meshes on character) | `Electric_Socket_In`, `crowbar_Lp_2` | from actor CSV |
| Mantle | `BP_CharacterMantleComponent` | |

## World
| What | Name | Notes |
|---|---|---|
| Menu map | `/Game/Maps/Empty` | |
| Game map | `__________` | from in-game dump: `World /Game/Maps/…` |
| World Partition | character has `UWorldPartitionStreamingSourceComponent` | ghost actors may need to be streaming sources too, or they'd stand in unloaded space — later |
| Coordinates | X≈609 000, Y≈−261 000, Z≈500 at the start boat | large but fine for float32 relative packets |

## Hooks of interest
- `/Script/Engine.PlayerController:ClientRestart` — fires on possess/respawn (the stock CheatManagerEnabler uses it).
- UE4SS built-ins: `RegisterLoadMapPreHook/PostHook`, `RegisterInitGameStatePostHook`, `RegisterBeginPlayPostHook`.

## Still needed from an in-game dump
- Live `BP_FirstPersonCharacter_New_C` instance path and its `Mesh` (ACharacter body) → skeletal mesh asset + anim class.
- Game map name(s).
- A spawnable, harmless actor class to use as ghost v1 (`StaticMeshActor` is engine-side, always available).
