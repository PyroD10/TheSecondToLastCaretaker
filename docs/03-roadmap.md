# 03 — Roadmap and status

Update the status column as things happen. Legend: ⬜ todo · 🟨 in progress · ✅ done · ⛔ blocked

| Phase | Step | Owner | Status | Notes |
|---|---|---|---|---|
| 0 | Install UE4SS from Nexus, game launches with UE4SS console | Dan | ⬜ | `04-dev-setup.md` §1 |
| 0 | Record UE4SS version string + game version | Dan | ⬜ | → `01-research.md` open Q2 |
| 0 | Decide LAN vs internet | Dan | ⬜ | open Q1 |
| 1 | Lua probe `PositionProbe` prints local position every 500 ms | Claude ✅ / Dan test ⬜ | 🟨 | `mods/PositionProbe` written, untested |
| 1 | Object dump + CXX header dump in `dumps/` | Dan | ⬜ | `04-dev-setup.md` §3 |
| 1 | Identify player pawn class, character mesh, level-name source | Claude | ⬜ | needs dumps |
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

## Connection options (open Q1)
- **Same LAN**: client enters host's LAN IP. Done.
- **Internet, simplest**: both install a mesh VPN (Tailscale / ZeroTier / Radmin VPN) → behaves like LAN.
  Recommended for v1; zero code.
- **Internet, port forward**: host forwards UDP 27015. Works, fiddly for the host.
- **Internet, relay server**: tiny Python relay on a VPS. Only if the above fails for friends.

## Known risks
- Every Early Access patch may rename classes → `GameNames.h` table + Lua probe as a canary.
- UE4SS experimental ABI changes → pin the RE-UE4SS commit, rebuild on UE4SS updates.
- Official co-op announcement makes the project moot. Acceptable.
