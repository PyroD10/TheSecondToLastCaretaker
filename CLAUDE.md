# TheSecondToLastCaretaker — "see each other" multiplayer mod for The Last Caretaker

## What this is
A fan-made mod for *The Last Caretaker* (Channel 37, Unreal Engine 5.8, Steam Early Access).
Scope is deliberately small: each player keeps their own world and save; the mod shows the
other player(s) as a stand-in character ("ghost") at their live position. No shared inventory,
no shared world state. See `docs/02-architecture.md` and `docs/03-roadmap.md`.

Built as a **UE4SS C++ mod** (networking) with a throwaway **UE4SS Lua mod** for early probing.
Fresh start — not based on the abandoned `Gentlemannn/LastCaretaker-Multiplayer` repo
(see `docs/decisions/ADR-0001-ue4ss-instead-of-dll-injection.md`).

## Division of labour (important)
- Claude writes code, docs and tooling. Claude **cannot run the game** and has no Windows
  toolchain on its side.
- Dan (owner) builds, installs, runs and tests on his PC and reports results following
  `docs/05-testing-protocol.md`. Treat every in-game behaviour as unverified until Dan confirms it.
- When a step needs a game artefact (object dump, header dump, log), ask Dan for it and tell him
  exactly where it lands on disk. Dumps go in `dumps/` (git-ignored except for `.gitkeep`).

## Hard rules
- **Never hardcode memory addresses or pointer chains.** Use UE4SS reflection
  (`FindFirstOf`, `StaticFindObject`, UFunction calls, hooks). Offsets break every patch.
- Never modify or redistribute game files. Everything ships as loose files under `Mods/`.
- Nothing that touches other players' saves or world state. Read local state, render remote state.
- Don't guess Unreal class/function names for this game. Look them up in the current object dump
  in `dumps/` or ask Dan to dump. Engine-level names (APawn, APlayerController, K2_GetActorLocation)
  are fine.
- Keep the Lua probe (`mods/PositionProbe`) working as a smoke test even after the C++ mod exists.

## Repo layout
```
CLAUDE.md                this file
README.md                short public-facing description
docs/                    numbered docs, read in order; decisions/ holds ADRs
mods/<ModName>/          UE4SS Lua mods, drop-in ready (Scripts/main.lua + enabled.txt)
src/                     C++ mod sources (CMake, builds against RE-UE4SS) — Phase 2+
tools/                   helper scripts (dump parsing etc.), Python 3
dumps/                   game object/header dumps from Dan's PC (git-ignored)
```

## Conventions
- Docs are in English; code comments in English.
- One ADR per non-trivial decision: `docs/decisions/ADR-NNNN-short-title.md`.
- Update `docs/03-roadmap.md` status column when a phase step is done or blocked.
- Windows side: Dan uses **CMD**, not Git Bash. Give him CMD commands or GUI steps.
- Game exe: `VoyageSteam-Win64-Shipping.exe` (project codename "Voyage"). Steam AppID 1783560.

## Current phase
See the status table at the top of `docs/03-roadmap.md`.
