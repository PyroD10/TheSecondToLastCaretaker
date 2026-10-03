# 04 — Dev setup (Dan's PC, Windows, CMD)

## 1. Install UE4SS
1. Nexus: https://www.nexusmods.com/thelastcaretaker/mods/4 — download the latest file.
2. Game folder: Steam → right-click *The Last Caretaker* → Manage → Browse local files.
   On Dan's PC: `E:\SteamLibrary\steamapps\common\Voyage\Voyage\Binaries\Win64\`
   (Steam folder is "Voyage"). UE4SS: `Win64\ue4ss\`, mods: `Win64\ue4ss\Mods\`, log: `Win64\ue4ss\UE4SS.log`.
3. Follow the Nexus page's install instructions (normally: unzip next to the exe so you get
   `Win64\ue4ss\` with `UE4SS.dll`, `UE4SS-settings.ini`, `Mods\`, plus `dwmapi.dll` as loader).
4. Start the game once. Success = a console window opens alongside the game and
   `Win64\ue4ss\UE4SS.log` exists.
5. Copy the first lines of `UE4SS.log` (version string, UE version detected) into
   `docs/01-research.md` → open Q2, and note the game version shown in the main menu.

## 2. Install the Lua probe
1. Link the folder `mods\PositionProbe` from this repo into `Win64\ue4ss\Mods\` (Dan uses a symlink —
   good, edits in the repo are live). CMD as admin:
   `mklink /D "E:\SteamLibrary\steamapps\common\Voyage\Voyage\Binaries\Win64\ue4ss\Mods\PositionProbe" "C:\Users\Ich\Desktop\Projects\TheSecondToLastCaretaker\mods\PositionProbe"`
   Result: `Win64\ue4ss\Mods\PositionProbe\Scripts\main.lua` and `...\PositionProbe\enabled.txt`.
2. Start the game, load a save, walk around. The UE4SS console should print lines like
   `[Probe] X=... Y=... Z=... Yaw=...` twice a second.
3. Report per `05-testing-protocol.md`. If it errors, paste the red lines — most likely the
   pawn property name differs and we fix it in minutes.

## 3. Dumps (needed for Phase 1/4)
In `UE4SS-settings.ini` set `GuiConsoleEnabled = 1` and `GuiConsoleVisible = 1` if not already.
In the GUI console (Live View tab / Dumpers tab):
- **Dump Objects** → writes `UE4SS_ObjectDump.txt` next to UE4SS. Do this *in-game with a save loaded*.
- **Generate CXX Headers** → folder `CXXHeaderDump\`.
Link the ue4ss folder into the repo once (CMD as admin) so the dumps are always current:
`mklink /D "C:\Users\Ich\Desktop\Projects\TheSecondToLastCaretaker\ue4ss" "E:\SteamLibrary\steamapps\common\Voyage\Voyage\Binaries\Win64\ue4ss"` (done 2026-10-03)
Then tell Claude "dumps are in". **Dump while a save is loaded**, not in the menu — the menu world is `/Game/Maps/Empty` and has no character.

## 4. C++ toolchain (Phase 2)
Requirements from the RE-UE4SS README (verified 2026-10-03):
- **Visual Studio 2022 ≥ 17.13** with "Desktop development with C++" (needs MSVC ≥ 19.43, C++23). Check: Help → About.
  Also tick "C++ CMake tools for Windows" in the installer — gives CMake ≥ 3.22 and Ninja.
- **Rust toolchain ≥ 1.73** — https://rustup.rs (default install, then reopen CMD so `cargo --version` works).
- **GitHub account linked to an Epic Games account.** RE-UE4SS has a submodule with Unreal-derived headers
  that only members of the EpicGames GitHub org can clone. Link at https://www.epicgames.com/account/connections
  → GitHub, accept the invitation e-mail from GitHub. Without this `git submodule update` fails with "repository not found".
- Git configured so HTTPS clones of private repos work (Git Credential Manager prompts once).

Clone RE-UE4SS **at the exact commit** of the installed UE4SS (`UE4SS.log` line 2 → `Git SHA #527a483b`).
In CMD, from the repo root:
```
cd src
git clone https://github.com/UE4SS-RE/RE-UE4SS.git
cd RE-UE4SS
git checkout 527a483b
git submodule update --init --recursive
```
(Don't add `--remote` to the submodule command — the README warns it breaks dependencies.)
Configure and build (CMD, repo root). The first build compiles all of UE4SS — expect 10–30 min and several GB:
```
cmake -S src -B src\build -G "Visual Studio 17 2022"
cmake --build src\build --config Game__Shipping__Win64 --target GhostNet
```
GUI alternative: open `src\build\TheSecondToLastCaretaker.sln` in Visual Studio, pick configuration
`Game__Shipping__Win64`, build the `GhostNet` project.

Install: find `main.dll` under `src\build\GhostNet\` → copy to `ue4ss\Mods\GhostNet\dlls\main.dll`,
create an empty `ue4ss\Mods\GhostNet\enabled.txt`. (A symlink `ue4ss\Mods\GhostNet` → a folder in the repo
works too, like PositionProbe.) Start the game: `UE4SS.log` should show `[GhostNet] loaded v0.2.0`, then once
per second `[GhostNet] X=… Y=… Z=…` matching the Lua probe.

## 5. Things that commonly go wrong
- Game updated → Nexus UE4SS build may lag a few days; our C++ DLL must be rebuilt if UE4SS changed.
- Antivirus flags `dwmapi.dll` loader → add exclusion for the Win64 folder.
- Two game instances on one PC don't work (Steam) → use `tools\fake_peer.py` to simulate the other player.
