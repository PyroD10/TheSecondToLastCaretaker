# 01 — Research (as of 2026-10-03)

## Official multiplayer status
- Game FAQ: "The Last Caretaker is currently single-player only."
  https://thelastcaretaker.com/faq/
- Developer jack.pattillo on Steam, 14 Oct 2025: "The game is releasing as a single-player
  experience, but we've heard enough from the Community that we're looking in to ways of
  implementing some form of multiplayer in the future!"
  https://steamcommunity.com/app/1783560/discussions/0/591784958348197718/
- 1.0 is planned for **Spring 2027** (also Xbox Game Pass). Press asked about co-op in Sept 2026;
  the studio would not confirm either way.
  https://playday.one/2026/09/09/the-last-caretaker-hits-1-0-spring-2027-also-coming-to-xbox-game-pass/

Conclusion: no official co-op is announced. Risk: if it comes, this mod becomes redundant. Fine —
the point is to have something now and to learn.

## The abandoned repo (Gentlemannn/LastCaretaker-Multiplayer)
- 12 commits, all on 1 Jan 2026. ~320 lines of real code.
- DLL injected manually via Process Hacker; reads X/Y/Z through **hardcoded pointer chains**
  (`BASE_XZ = 0x0A7C4118`, `BASE_Y = 0x0A38D5A0`, …) from `VoyageSteam-Win64-Shipping.exe`.
- Sends position over UDP to `127.0.0.1:27015` every 300 ms. "Launcher" is a console that prints them.
- Client and server disagree on the packet layout (`type` vs `valid` byte). Never tested across machines.
- Renders nothing in-game. Offsets are dead after any game update.
- Verdict: proof of concept only. Nothing to reuse except the exe name and the idea.

## Modding ecosystem
- **UE4SS** is available for the game and actively maintained:
  https://www.nexusmods.com/thelastcaretaker/mods/4 (updated 6 Sep 2026, built from UE4SS
  experimental with UE 5.8 support, targets game v0.8.5.x).
- Several Nexus mods already build on it (Bow Thrusters, Better Cables, Player Flight, Better Map).
  → Reading the player pawn and spawning actors through reflection is proven feasible in this game.
- The `.usmap` mapping file and AES key change per game version; the Nexus page points to a
  **TLC modding Discord, #resources**. We need the usmap only if we ever read cooked assets; for
  live reflection via UE4SS we don't.
- Game: Unreal Engine 5.8, exe `VoyageSteam-Win64-Shipping.exe`, Steam AppID 1783560.

## UE4SS facts relevant to the design
- Lua mods: `Mods/<Name>/Scripts/main.lua`, enabled via `Mods/<Name>/enabled.txt` (or `mods.txt`).
  API: `FindFirstOf`, `StaticFindObject`, `RegisterHook`, `NotifyOnNewObject`, `LoopAsync`,
  `ExecuteInGameThread`, `StaticConstructObject`; helper lib `UEHelpers` (`GetPlayerController()`).
- Lua ships with the standard Lua libraries only — **no sockets**. Networking therefore needs a C++ mod.
- C++ mods: derive from `RC::CppUserModBase`, hooks `on_unreal_init()`, `on_update()` (per frame);
  export `start_mod()` / `uninstall_mod()`. Built with CMake against a cloned RE-UE4SS tree,
  config `Game__Shipping__Win64`, Visual Studio 2022 generator. C++ mods must be rebuilt for the
  exact UE4SS build they run under (ABI is not stable across UE4SS versions).
- UE4SS reference: https://docs.ue4ss.com/ (Lua API, C++ mod guide).

## Findings from the live game (Phase 1, 2026-10-03)
- Install path: `E:\SteamLibrary\steamapps\common\Voyage\Voyage\Binaries\Win64\` — the Steam
  folder is named **Voyage**, not "The Last Caretaker". UE4SS lives in `Win64\ue4ss\`, mods in `Win64\ue4ss\Mods\`.
- Nexus UE4SS package version string: `UE4SS_v3.0.1-1125-g527a483b` → RE-UE4SS commit **527a483b**
  (from the Nexus page; confirm against line 1 of `UE4SS.log`). Ships with the stock Lua mods
  (CheatManagerEnabler, ConsoleEnabler, BPModLoader, Keybinds …) enabled via `mods.txt`; our mod loads via `enabled.txt`.
- Player pawn classes (from `PositionProbe`):
  - Main menu: `BlueprintGeneratedClass /Game/Blueprints/Game/BP_VoyageMenuPawn.BP_VoyageMenuPawn_C` — position reads 0/0/0.
  - In game: `BlueprintGeneratedClass /Game/Blueprints/BP_FirstPersonCharacter_New.BP_FirstPersonCharacter_New_C`
  → "in world" = pawn class is the first-person character; menu pawn = hide ghost.
- `UEHelpers.GetPlayerController()`, `pc.Pawn`, `K2_GetActorLocation/Rotation` all work through reflection. No offsets needed.
- World coordinates are large (X≈608 000, Y≈−261 000); Z oscillates ±10 cm·10 while standing still — the player is on
  a boat on water. Position sync must send absolute world coords as float64 or relative-to-origin float32
  (float32 at 6×10⁵ has ~0.06 cm resolution — fine, but keep an eye on it).
- `CheatManagerEnabler` re-runs on each `ClientRestart` (level load/respawn) — a usable hook for "pawn changed".

## Open questions (need Dan)
1. ~~LAN or internet?~~ **Decided: Tailscale** (see ADR-0002).
2. Confirm UE4SS version from line 1 of `UE4SS.log` is `v3.0.1-1125-g527a483b`, and note the game version (main menu).
3. Does the Nexus package include the CXX header dump option / Live View GUI enabled?
