# 03 — Roadmap and status

Update the status column as things happen. Legend: ⬜ todo · 🟨 in progress · ✅ done · ⛔ blocked

| Phase | Step | Owner | Status | Notes |
|---|---|---|---|---|
| 0 | Install UE4SS from Nexus, game launches with UE4SS console | Dan | ✅ | 2026-10-03 |
| 0 | Record UE4SS version string + game version | Dan | 🟨 | UE4SS ✅ `v3.0.1 #527a483b`; game version still needed |
| 0 | Decide LAN vs internet | Dan | ✅ | Tailscale, ADR-0002 |
| 1 | Lua probe `PositionProbe` prints local position every 500 ms | Claude / Dan | ✅ | works first try; pawn classes identified |
| 1 | Object dump + CXX header dump reachable via `ue4ss/` | Dan | 🟨 | headers ✅; object dump was taken in menu → re-dump in-game |
| 1 | Identify player pawn class, character mesh, level-name source | Claude | 🟨 | see `06-game-reference.md`; body mesh + map name need in-game dump |
| 2 | C++ toolchain: VS 2022 + CMake, RE-UE4SS cloned at the Nexus build's commit | Dan | ⬜ | `04-dev-setup.md` §4 |
| 2 | Hello-world C++ mod `GhostNet` loads, logs once per second | Claude / Dan | ⬜ | |
| 2 | C++ reads local pawn transform (same output as Lua probe) | Claude / Dan | ⬜ | |
| 3 | UDP transport host/client, loopback test on one PC (two game instances not possible → use `tools/fake_peer.py`) | Claude / Dan | ⬜ | |
| 3 | Protocol doc `docs/protocol.md` | Claude | ⬜ | |
| 4 | Ghost actor spawn (StaticMeshActor), follows fake peer | Claude / Dan | ⬜ | |
| 4 | Interpolation, timeout, respawn after level load | Claude / Dan | ⬜ | |
| 4 | Two real PCs, two real players | Dan + friend | ⬜ | milestone 🎉 |
| 5 | Better ghost (skeletal mesh + anim), name tag, config file | Claude / Dan | ⬜ | |
| 5 | >2 players, release zip, Nexus page? | — | ⬜ | |

## Connection (decided — ADR-0002)
Players connect over **Tailscale**; the client enters the host's Tailscale IP (100.x.y.z). Same code path as LAN.
Fallbacks if a friend can't run Tailscale: port-forward UDP 27015 on the host, or a tiny relay on a VPS. Not planned for v1.

## Known risks
- Every Early Access patch may rename classes → `GameNames.h` table + Lua probe as a canary.
- UE4SS experimental ABI changes → pin the RE-UE4SS commit, rebuild on UE4SS updates.
- Official co-op announcement makes the project moot. Acceptable.
