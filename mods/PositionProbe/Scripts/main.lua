-- PositionProbe — Phase 1 smoke test for TheSecondToLastCaretaker.
-- Prints the local player's position/rotation to the UE4SS console every 500 ms.
-- Purpose: prove we can read the pawn through reflection (no memory offsets) and learn
-- which pawn class the game uses. Keep this mod working; it is our canary after game patches.

local UEHelpers = require("UEHelpers")

local INTERVAL_MS = 500
local lastClassName = nil
local lastLevelName = nil

local function fmt(v) return string.format("%.1f", v) end

local function probe()
    local pc = UEHelpers.GetPlayerController()
    if not pc or not pc:IsValid() then
        print("[Probe] no PlayerController (menu/loading?)\n")
        return
    end

    local pawn = pc.Pawn
    if not pawn or not pawn:IsValid() then
        print("[Probe] PlayerController ok, no Pawn (dead/loading?)\n")
        return
    end

    -- Log the pawn class once; this tells us the game's player character class name.
    local className = pawn:GetClass():GetFullName()
    if className ~= lastClassName then
        print(string.format("[Probe] Pawn class: %s\n", className))
        lastClassName = className
    end

    -- Level/world name, logged on change — candidate for the "context" field in the net protocol.
    -- Isolated in its own pcall so a wrong call here never kills the position print.
    pcall(function()
        local world = UEHelpers.GetWorld()
        if world and world:IsValid() then
            local levelName = world:GetFName():ToString()
            if levelName ~= lastLevelName then
                print(string.format("[Probe] World: %s\n", levelName))
                lastLevelName = levelName
            end
        end
    end)

    local loc = pawn:K2_GetActorLocation()
    local rot = pawn:K2_GetActorRotation()
    print(string.format("[Probe] X=%s Y=%s Z=%s Yaw=%s\n",
        fmt(loc.X), fmt(loc.Y), fmt(loc.Z), fmt(rot.Yaw)))
end

print("[Probe] PositionProbe loaded\n")

LoopAsync(INTERVAL_MS, function()
    -- UObject access must happen on the game thread.
    ExecuteInGameThread(function()
        local ok, err = pcall(probe)
        if not ok then print("[Probe] error: " .. tostring(err) .. "\n") end
    end)
    return false -- false = keep looping
end)
