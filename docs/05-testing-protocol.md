# 05 — Testing protocol (how Dan reports back)

Claude cannot run the game. Every test report should contain:

1. **What was tested** — phase/step from `03-roadmap.md`.
2. **Versions** — game version (main menu), UE4SS version (`UE4SS.log` line 1-5), mod commit/hash.
3. **Steps done** — short.
4. **Result** — ✅ / ❌ / partial, one sentence.
5. **Evidence** — paste the relevant `UE4SS.log` lines (search for `[Probe]` or `[GhostNet]`),
   or drop the whole log into `docs\test-logs\<date>-<what>.log`. Screenshots → `docs\test-logs\`.
6. **Anything odd** — stutter, crash, console spam, FPS drop.

Template:
```
Test: Phase 1 / Lua probe
Game v0.8.5.3, UE4SS <version>, repo @ <hash>
Steps: installed mod, loaded save "Test", walked 20 m north, opened map, closed map
Result: ✅ prints positions; ❌ stops printing after map opened
Log:
<paste>
Odd: console spams "Pawn invalid" while in menu (expected?)
```
Keep the UE4SS console open during tests; red = Lua error, yellow = warning.
