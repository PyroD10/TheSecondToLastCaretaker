#include "LocalStateReader.hpp"
#include "GameNames.h"

#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/FVector.hpp>
#include <Unreal/FRotator.hpp>

using namespace RC;
using namespace RC::Unreal;

// NOTE (Phase 2 spike): API surface used here — FindAllOf, GetFunctionByNameInChain, ProcessEvent,
// IsUnreachable, GetName — exists in RE-UE4SS main around 527a483b. If a name doesn't compile,
// check src/RE-UE4SS/deps/first/Unreal/include/Unreal/*.hpp before changing the approach.

bool LocalStateReader::findPawn()
{
    std::vector<UObject*> found;
    UObjectGlobals::FindAllOf(GameNames::PlayerCharacterClass, found);
    for (UObject* obj : found)
    {
        if (!obj) continue;
        // Skip the class default object (Default__BP_…) — it has no world position.
        if (obj->GetName().starts_with(STR("Default__"))) continue;
        if (obj->IsUnreachable()) continue;
        m_pawn = obj;
        m_fnGetLocation = obj->GetFunctionByNameInChain(STR("K2_GetActorLocation"));
        m_fnGetRotation = obj->GetFunctionByNameInChain(STR("K2_GetActorRotation"));
        if (!m_fnGetLocation || !m_fnGetRotation)
        {
            Output::send<LogLevel::Warning>(STR("[GhostNet] pawn found but K2_GetActor* missing — engine API changed?\n"));
            m_pawn = nullptr;
            return false;
        }
        Output::send<LogLevel::Verbose>(STR("[GhostNet] local pawn: {}\n"), obj->GetFullName());
        return true;
    }
    return false;
}

std::optional<LocalState> LocalStateReader::read()
{
    auto* pawn = static_cast<UObject*>(m_pawn);
    if (pawn && pawn->IsUnreachable()) { m_pawn = nullptr; pawn = nullptr; }
    if (!pawn)
    {
        if (!findPawn()) return LocalState{ .inWorld = false };
        pawn = static_cast<UObject*>(m_pawn);
    }

    // K2_GetActorLocation / K2_GetActorRotation are BlueprintPure UFunctions whose only parameter
    // is the return value, so the params struct is just that value.
    struct { FVector  ReturnValue{}; } locParams{};
    struct { FRotator ReturnValue{}; } rotParams{};
    pawn->ProcessEvent(static_cast<UFunction*>(m_fnGetLocation), &locParams);
    pawn->ProcessEvent(static_cast<UFunction*>(m_fnGetRotation), &rotParams);

    LocalState s{};
    s.x = locParams.ReturnValue.X();  s.y = locParams.ReturnValue.Y();  s.z = locParams.ReturnValue.Z();
    s.pitch = static_cast<float>(rotParams.ReturnValue.GetPitch());
    s.yaw   = static_cast<float>(rotParams.ReturnValue.GetYaw());
    s.roll  = static_cast<float>(rotParams.ReturnValue.GetRoll());
    s.inWorld = true;
    return s;
}
