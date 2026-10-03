# ADR-0001 — Build on UE4SS (C++ mod) instead of raw DLL injection with memory offsets

Date: 2026-10-03 · Status: accepted

## Context
The abandoned Gentlemannn repo read the player position via hardcoded pointer chains in
`VoyageSteam-Win64-Shipping.exe` and injected a DLL by hand with Process Hacker. The game is in
Early Access and patches frequently; UE4SS exists for the game and is maintained.

## Options
1. **Raw DLL + memory offsets** (the old approach). Pro: no dependency. Con: breaks every patch,
   needs Cheat Engine work per version, can't spawn actors sanely, manual injection.
2. **UE4SS Lua mod.** Pro: no compiler, instant iteration, reflection-based. Con: no sockets → no
   networking without an external helper process and a hacky IPC.
3. **UE4SS C++ mod.** Pro: reflection-based (survives patches better), full Win32/Winsock access,
   `on_update()` on the game thread, proper actor spawning via UFunctions. Con: VS 2022 + CMake,
   must be rebuilt against the exact UE4SS build.
4. **Lua mod + external companion exe over named pipe/file.** Pro: no C++. Con: two processes,
   fragile IPC, still needs a compiled companion anyway.

## Decision
Option 3 for the real mod. Option 2 as a probe/canary for fast checks (`mods/PositionProbe`).

## Consequences
- Dan needs the C++ toolchain (docs/04 §4). One-time cost.
- We pin the RE-UE4SS commit to the Nexus build and rebuild when it moves.
- All game-specific names live in one table so patch breakage is a one-file fix.
