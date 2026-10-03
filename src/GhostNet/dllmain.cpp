// GhostNet — UE4SS C++ mod for TheSecondToLastCaretaker.
// Phase 2: load, tick, read the local pawn through reflection and log it once per second.
// Phase 3 adds NetTransport, Phase 4 GhostRenderer. See docs/02-architecture.md.

#include <Mod/CppUserModBase.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/Hooks.hpp>

#include <chrono>
#include <format>

#include "LocalStateReader.hpp"

using namespace RC;
using namespace RC::Unreal;

class GhostNetMod : public CppUserModBase
{
public:
    GhostNetMod() : CppUserModBase()
    {
        ModName        = STR("GhostNet");
        ModVersion     = STR("0.2.0");
        ModDescription = STR("See-each-other multiplayer for The Last Caretaker (ghost players over UDP)");
        ModAuthors     = STR("Dan + Claude");
        Output::send<LogLevel::Verbose>(STR("[GhostNet] loaded v{}\n"), ModVersion);
    }

    ~GhostNetMod() override = default;

    auto on_unreal_init() -> void override
    {
        m_unrealReady = true;
        // Drop the cached pawn whenever a map loads — the old actor is gone.
        Hook::RegisterLoadMapPostCallback([this](UEngine*, FWorldContext&, FURL, UPendingNetGame*, FString&) {
            m_reader.invalidate();
            Output::send<LogLevel::Verbose>(STR("[GhostNet] map loaded, pawn cache cleared\n"));
        });
        Output::send<LogLevel::Verbose>(STR("[GhostNet] unreal init done\n"));
    }

    auto on_update() -> void override
    {
        if (!m_unrealReady) return;

        auto now = std::chrono::steady_clock::now();
        if (now - m_lastLog < std::chrono::seconds(1)) return;
        m_lastLog = now;

        auto s = m_reader.read();
        if (!s || !s->inWorld)
        {
            Output::send<LogLevel::Verbose>(STR("[GhostNet] not in world\n"));
            return;
        }
        Output::send<LogLevel::Verbose>(STR("[GhostNet] X={:.1f} Y={:.1f} Z={:.1f} Yaw={:.1f}\n"),
                                        s->x, s->y, s->z, s->yaw);
    }

private:
    bool m_unrealReady{};
    LocalStateReader m_reader;
    std::chrono::steady_clock::time_point m_lastLog{};
};

#define GHOSTNET_API __declspec(dllexport)
extern "C"
{
    GHOSTNET_API CppUserModBase* start_mod() { return new GhostNetMod(); }
    GHOSTNET_API void uninstall_mod(CppUserModBase* mod) { delete mod; }
}
