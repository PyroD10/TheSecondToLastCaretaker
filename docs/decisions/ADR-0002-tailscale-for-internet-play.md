# ADR-0002 — Internet play via Tailscale, no NAT code in the mod

Date: 2026-10-03 · Status: accepted

## Context
Players will usually not be on the same LAN. Options: Tailscale/mesh VPN, port forwarding, relay server, STUN/hole punching.

## Decision
The mod only ever talks to plain IPv4:port. Players run **Tailscale** (Dan already uses it); the client
enters the host's Tailscale IP (100.x.y.z). Tailscale does direct peer-to-peer where possible and falls
back to its own relays (DERP) otherwise, so there is no NAT handling, no hole punching and no server of ours.

## Consequences
- Zero networking infrastructure to run or pay for. LAN and internet are the same code path.
- Friends must install Tailscale and be in the same tailnet (invite via Tailscale sharing).
- Keep the host port configurable (default UDP 27015) so port forwarding stays possible as a fallback.
- Tailscale MTU is 1280 bytes; keep packets well under 1200 bytes.
