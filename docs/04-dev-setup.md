# 04 — Dev setup (Dan's PC, Windows, CMD)

## 1. Install UE4SS
1. Nexus: https://www.nexusmods.com/thelastcaretaker/mods/4 — download the latest file.
2. Game folder: Steam → right-click *The Last Caretaker* → Manage → Browse local files.
   The exe is at `...\The Last Caretaker\VoyageSteam\Binaries\Win64\VoyageSteam-Win64-Shipping.exe`
   (path to verify — note the real one here: `__________`).
3. Follow the Nexus page's install instructions (normally: unzip next to the exe so you get
   `Win64\ue4ss\` with `UE4SS.dll`, `UE4SS-settings.ini`, `Mods\`, plus `dwmapi.dll` as loader).
4. Start the game once. Success = a console window opens alongside the game and
   `Win64\ue4ss\UE4SS.log` exists.
5. Copy the first lines of `UE4SS.log` (version string, UE version detected) into
   `docs/01-research.md` → open Q2, and note the game version shown in the main menu.

## 2. Install the Lua probe
1. Copy the folder `mods\PositionProbe` from this repo into `Win64\ue4ss\Mods\`.
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
Copy both into this repo's `dumps\` folder (git-ignored). Then tell Claude "dumps are in".

## 4. C++ toolchain (Phase 2)
- Visual Studio 2022 with "Desktop development with C++" workload (MSVC v143, Windows SDK).
- CMake ≥ 3.22 (ships with VS or https://cmake.org). Git.
- Rust toolchain (`rustup`) — RE-UE4SS builds a Rust dependency (patternsleuth). Verify this is
  still required for the commit we pin; if `cmake` complains about cargo, install it.
- Clone RE-UE4SS **at the exact commit** matching the Nexus build (from the version string in
  `UE4SS.log`): in CMD inside `src\`:
  ```
  git clone --recursive https://github.com/UE4SS-RE/RE-UE4SS.git
  cd RE-UE4SS
  git checkout <commit>
  ```
- Configure/build (Claude will provide a `src\CMakeLists.txt` in Phase 2):
  ```
  cmake -S src -B src\build -G "Visual Studio 17 2022"
  cmake --build src\build --config Game__Shipping__Win64
  ```
  Expect the first build to take a while (UE4SS itself compiles).
- Install: `src\build\...\GhostNet.dll` → `Win64\ue4ss\Mods\GhostNet\dlls\main.dll`
  plus `Mods\GhostNet\enabled.txt`.

## 5. Things that commonly go wrong
- Game updated → Nexus UE4SS build may lag a few days; our C++ DLL must be rebuilt if UE4SS changed.
- Antivirus flags `dwmapi.dll` loader → add exclusion for the Win64 folder.
- Two game instances on one PC don't work (Steam) → use `tools\fake_peer.py` to simulate the other player.
