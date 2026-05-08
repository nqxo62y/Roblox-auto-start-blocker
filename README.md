# RobloxBlocker

Prevents RobloxPlayer from auto-starting in the background without your consent.

## What it does
- Removes Roblox autostart entries from the registry
- Kills any running Roblox background processes on startup
- Watches every 2 seconds and terminates them if they reappear
- Runs completely silent (no window)

## Build

```bash
g++ -o RobloxBlocker.exe main.cpp -ladvapi32 -std=c++17 -mwindows
```

## Autostart (optional)
Place the compiled `.exe` in your Windows startup folder:
```
Win+R → shell:startup
```

## Notes
- No admin required (HKLM registry cleanup is skipped silently if not available)
- No memory manipulation, no injection — safe to use
