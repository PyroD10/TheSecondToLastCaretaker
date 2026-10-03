#pragma once
#include <optional>
#include <cstdint>

// What we know about the local player each tick. Plain data, no Unreal types, so the net layer
// can serialize it without including Unreal headers.
struct LocalState
{
    double x{}, y{}, z{};       // world cm (UE5 uses double-precision vectors)
    float  pitch{}, yaw{}, roll{};
    bool   inWorld{};           // false in menu / loading / dead
};

class LocalStateReader
{
public:
    // Call from on_update() (game thread). Cheap: caches the pawn and re-finds it only when invalid.
    // Returns nullopt when the Unreal side isn't ready at all.
    std::optional<LocalState> read();

    // Drop the cached pawn (call on map load / ClientRestart).
    void invalidate() { m_pawn = nullptr; }

private:
    void* m_pawn{};             // RC::Unreal::UObject* kept opaque here
    void* m_fnGetLocation{};    // UFunction* K2_GetActorLocation
    void* m_fnGetRotation{};    // UFunction* K2_GetActorRotation
    bool  findPawn();
};
