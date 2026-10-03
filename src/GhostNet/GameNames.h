#pragma once
// Every game-specific Unreal name lives here. When a patch renames something, fix it in this
// file only. Source of truth: docs/06-game-reference.md and ue4ss/UE4SS_ObjectDump.txt.
// Engine-level names (K2_GetActorLocation, StaticMeshActor…) are engine API and may be used directly.

#define GN_STR(x) STR(x)

namespace GameNames
{
    // Player character class (short class name as used by UObjectGlobals::FindFirstOf / FindAllOf).
    inline constexpr auto PlayerCharacterClass   = GN_STR("BP_FirstPersonCharacter_New_C");
    // Menu pawn — if the local pawn is this class we are not "in world".
    inline constexpr auto MenuPawnClass          = GN_STR("BP_VoyageMenuPawn_C");
    // Full-body third-person mesh + its skeletal mesh component name on the character.
    inline constexpr auto BodyMeshComponentName  = GN_STR("CharacterMesh0");
    inline constexpr auto BodySkeletalMeshAsset  = GN_STR("/Game/Characters/Mechanical_Light_01_A/SKM_PlayerCharacter.SKM_PlayerCharacter");
    inline constexpr auto BodyAnimClass          = GN_STR("/Game/Characters/Mannequins/Animations/ABP_Manny_New.ABP_Manny_New_C");
    inline constexpr auto IdleAnimAsset          = GN_STR("/Game/Characters/Mannequins/Animations/Manny/MM_Idle.MM_Idle");
    // Main game map (World Partition, streamed).
    inline constexpr auto GameMapName            = GN_STR("VoyageWorld2");
}
