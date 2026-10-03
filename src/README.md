# src — GhostNet C++ mod

```
src/
  CMakeLists.txt        root: adds RE-UE4SS/ and GhostNet/
  RE-UE4SS/             NOT in git — clone at the pinned commit (docs/04 §4)
  GhostNet/
    CMakeLists.txt      builds dlls/main.dll
    dllmain.cpp         mod class, hooks, tick
    GameNames.h         all game-specific Unreal names
    LocalStateReader.*  reads local pawn transform via reflection
    (Phase 3) NetTransport.*, Protocol.hpp
    (Phase 4) GhostRenderer.*
```
Build (CMD, from repo root):
```
cmake -S src -B src\build -G "Visual Studio 17 2022"
cmake --build src\build --config Game__Shipping__Win64
```
Output: `src\build\GhostNet\Game__Shipping__Win64\main.dll` (path may vary slightly — look for main.dll).
Install: copy to `ue4ss\Mods\GhostNet\dlls\main.dll`, create empty `ue4ss\Mods\GhostNet\enabled.txt`.
