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

## Open questions (need Dan)
1. LAN or internet between the players? Decides NAT handling (see roadmap).
2. Exact UE4SS build the Nexus package ships (version string in `UE4SS.log`). Our C++ mod must
   build against that same commit.
3. Does the Nexus package include the CXX header dump option / Live View GUI enabled?
