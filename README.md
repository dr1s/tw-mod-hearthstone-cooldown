# tw-mod-hearthstone-cooldown

Changes the Hearthstone cooldown to a configurable value.

## Configuration

Copy `conf/tw-mod-hearthstone-cooldown.conf.dist` to your module config directory as `tw-mod-hearthstone-cooldown.conf` and adjust:

```ini
[HearthstoneCooldown]
HearthstoneCooldown.Enable = 1
HearthstoneCooldown.CooldownMinutes = 5
```

- `Enable`: set to `0` to disable the module and leave the default cooldown.
- `CooldownMinutes`: cooldown in minutes. Set to `0` to remove the cooldown entirely.

## How it works

The module modifies both `SpellCooldown` and `SpellCategoryCooldown` on item `6948` (Hearthstone) for spell `8690`. Because the prototype is changed before any player uses the item, the new cooldown applies globally to all players.

The cooldown is applied when the world starts (`OnBeforeWorldInitialized`) and again whenever the config is reloaded (`OnAfterConfigLoad`).

## Build

```sh
cmake -S . -B build -DMODULES=static
cmake -S . -B build -DMODULES=dynamic
# or per-module:
cmake -S . -B build -DMODULE_TW_MOD_HEARTHSTONE_COOLDOWN=static
```
