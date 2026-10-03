# 02 — Architecture

## Goal, restated
Two (later: up to 4) players run their own single-player sessions. Each sees the others as
**ghost characters** moving in real time. No shared gameplay state.

## Components
```
 Player A PC                                  Player B PC
 ┌───────────────────────────────┐            ┌───────────────────────────────┐
 │ The Last Caretaker (UE 5.8)   │            │ The Last Caretaker            │
 │  └ UE4SS                      │            │  └ UE4SS                      │
 │     └ GhostNet.dll (C++ mod)  │◄──UDP────► │     └ GhostNet.dll            │
 │        ├ LocalStateReader     │            │                               │
 │        ├ NetTransport (host)  │            │        NetTransport (client)  │
 │        └ GhostRenderer        │            │                               │
 └───────────────────────────────┘            └───────────────────────────────┘
```
- **LocalStateReader** — every tick: find local `APlayerController` → `Pawn`, read world location,
  rotation, velocity, plus a coarse "context" (level name, is-in-menu, is-dead). Via reflection only.
- **NetTransport** — UDP, one player is **host** (listens), others connect to host IP:port.
  Host rebroadcasts every client's state to all others (star topology). 20 Hz send rate.
  Protocol in `docs/protocol.md` once Phase 3 starts. Versioned packet header; mismatched
  versions refuse to connect with a clear log line.
- **GhostRenderer** — per remote player: spawn one ghost actor on first packet, then each tick
  set its transform, interpolating between the last two received states (render ~100 ms behind
  to hide jitter). Hide/destroy on timeout (3 s) or disconnect. Re-spawn after level transitions.
- **Config** — `Mods/GhostNet/config.ini`: `mode=host|client`, `host=ip`, `port=27015`,
  `name=Dan`. No in-game UI in v1; later maybe a UE4SS Lua overlay for status text.

## What the ghost is
Phase 4 decision, after the object dump. Candidates, from simplest to best:
1. A `StaticMeshActor` with an engine mesh (capsule/cube) — guaranteed to work, ugly.
2. An actor of the game's own player character class spawned without a controller — looks right,
   may fight the game's systems (AI, save, interaction) and needs disabling of its components.
3. A `SkeletalMeshActor` reusing the player skeletal mesh + idle anim — good middle ground.
Start with 1 to prove the pipeline, move to 3.

## Why UE4SS C++ (not Lua, not raw DLL injection)
See `decisions/ADR-0001-ue4ss-instead-of-dll-injection.md`.

## Threading
UE4SS calls `on_update()` on the game thread. Sockets are polled non-blocking there
(cheap at 20 Hz); no second thread, no locking. If receive work ever grows, move the socket to a
worker thread and hand over a double-buffered snapshot.

## Failure handling
- Pawn not found (menu, loading): send `context=NOT_IN_WORLD`; remote side hides the ghost.
- Game patch changes class/function names: mod logs the missing name and disables itself instead
  of crashing the game. Names live in one lookup table (`GameNames.h`) for quick fixing.
- UE4SS version mismatch: UE4SS refuses to load the DLL; documented in `04-dev-setup.md`.

## Non-goals (v1)
Shared inventory/crafting/machines, enemies, boat sync, voice chat, matchmaking, in-game UI,
anti-cheat concerns, Steam networking APIs.
