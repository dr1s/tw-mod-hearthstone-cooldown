#include "ScriptObjects.h"
#include "Config/Config.h"
#include "ItemPrototype.h"
#include "Log.h"
#include "ObjectMgr.h"

namespace
{
    constexpr uint32 HEARTHSTONE_ITEM_ID = 6948;
    constexpr uint32 HEARTHSTONE_SPELL_ID = 8690;
    constexpr uint32 MINUTE_IN_MILLISECONDS = 60 * 1000;

    class TwModHearthstoneCooldownWorldScript : public WorldScript
    {
    public:
        TwModHearthstoneCooldownWorldScript()
            : WorldScript("tw-mod-hearthstone-cooldown_world", { WORLDHOOK_ON_BEFORE_WORLD_INITIALIZED, WORLDHOOK_ON_AFTER_CONFIG_LOAD })
        {
        }

        void OnBeforeWorldInitialized() override
        {
            ApplyHearthstoneCooldown();
        }

        void OnAfterConfigLoad(bool /*reload*/) override
        {
            ApplyHearthstoneCooldown();
        }

    private:
        static void ApplyHearthstoneCooldown()
        {
            if (!sConfig.GetBoolDefault("HearthstoneCooldown.Enable", false))
            {
                sLog.outString("[tw-mod-hearthstone-cooldown] disabled; hearthstone cooldown unchanged.");
                return;
            }

            int32 const configuredMinutes = sConfig.GetIntDefault("HearthstoneCooldown.CooldownMinutes", 5);
            uint32 const cooldownMinutes = configuredMinutes > 0 ? static_cast<uint32>(configuredMinutes) : 0;
            uint32 const cooldownMs = cooldownMinutes * MINUTE_IN_MILLISECONDS;

            ItemPrototype const* constProto = sObjectMgr.GetItemPrototype(HEARTHSTONE_ITEM_ID);
            if (!constProto)
            {
                sLog.outError("[tw-mod-hearthstone-cooldown] could not find item prototype %u; cooldown unchanged.", HEARTHSTONE_ITEM_ID);
                return;
            }

            ItemPrototype* proto = const_cast<ItemPrototype*>(constProto);
            bool found = false;
            for (uint32 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
            {
                if (proto->Spells[i].SpellId == HEARTHSTONE_SPELL_ID)
                {
                    proto->Spells[i].SpellCooldown = static_cast<int32>(cooldownMs);
                    proto->Spells[i].SpellCategoryCooldown = static_cast<int32>(cooldownMs);
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                sLog.outError("[tw-mod-hearthstone-cooldown] could not find spell %u on item %u; cooldown unchanged.", HEARTHSTONE_SPELL_ID, HEARTHSTONE_ITEM_ID);
                return;
            }

            if (cooldownMinutes == 0)
                sLog.outString("[tw-mod-hearthstone-cooldown] enabled; hearthstone cooldown removed.");
            else
                sLog.outString("[tw-mod-hearthstone-cooldown] enabled; hearthstone cooldown set to %u minute(s) (%u ms) for item %u, spell %u.", cooldownMinutes, cooldownMs, HEARTHSTONE_ITEM_ID, HEARTHSTONE_SPELL_ID);
        }
    };
}

void Addtw_mod_hearthstone_cooldownScripts()
{
    new TwModHearthstoneCooldownWorldScript();
}
