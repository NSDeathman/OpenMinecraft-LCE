# OpenMinecraft Legacy Console Edition

[![Discord](https://img.shields.io/badge/Discord-Join%20Community-5865F2?logo=discord&logoColor=white)](https://discord.gg/ysTEzFVDCu)
[![Contributions Welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](CONTRIBUTING.md)
[![AI Friendly](https://img.shields.io/badge/AI-friendly-blueviolet)](CONTRIBUTING.md)

This project is a **community-driven fork** of [MinecraftConsoles](https://github.com/smartcmd/MinecraftConsoles), based on the leaked source code of Minecraft Legacy Console Edition v1.6.0560.0 (TU19). It inherits all the fixes and improvements from the original while embracing a completely **open contribution philosophy**.

## 🌟 Our Philosophy

OpenMinecraft Legacy Console Edition is built on the belief that creativity should have no boundaries. We welcome:

- ✨ **Any new features** – No arbitrary restrictions. If you can imagine it, we’re open to it.
- 🔄 **Porting from Java Edition or Bedrock Edition** – Bring over mechanics, items, or anything that fits.
- 🤖 **AI‑assisted contributions** – We recognize AI as a valuable tool. As long as the code is reviewed and meets our quality standards, it’s welcome.
- 🌍 **Contributions from everyone** – Whether you’re a seasoned developer or just starting, your ideas matter.

We want this to be a true community project where innovation thrives and everyone can help shape the future of Legacy Console Edition on modern platforms.

---

## 📥 Download

Pre-built binaries for Windows are available in our [Releases](https://github.com/yourusername/OpenMinecraft/releases) (coming soon).  
Nightly builds may also be provided – stay tuned.

If you’re looking for the **Dedicated Server**, check the releases section as well.

---

## 🖥️ Platform Support

- **Windows**: Fully supported for building and running.
- **macOS / Linux**: The Windows build runs via Wine/CrossOver (community reports), but official support is not guaranteed yet.
- **Android / iOS**: Community experiments are welcome!
- **Consoles**: The original console code remains, but we currently lack the hardware to test it. **We’d love contributions from anyone who can help revive console support!**

---

## ✨ Features (inherited + future)

All the great work from MinecraftConsoles is preserved, including:

- Fixed compilation with Visual Studio 2022 (Debug/Release)
- Keyboard & mouse input
- Fullscreen toggle (F11)
- Disabled V‑Sync (WIP) for smoother gameplay
- High‑resolution timer for high‑FPS timing
- Native screen resolution (instead of fixed 1080p)
- LAN multiplayer & discovery
- Dedicated server executable
- Persistent username via `username.txt`
- Decoupled UID from username
- Security fixes
- Splitscreen multiplayer
- In‑game server management

**On top of this, we encourage additions like:**

- Porting modern features from Java/Bedrock
- New gameplay mechanics
- Quality‑of‑life improvements
- Expanded platform support (Linux native, Android, etc.)

---

## 🎮 Controls (Keyboard & Mouse)

*Same as original MinecraftConsoles – feel free to suggest improvements!*

- **Movement**: `W` `A` `S` `D`
- **Jump / Fly (Up)**: `Space`
- **Sneak / Fly (Down)**: `Shift` (Hold)
- **Sprint**: `Ctrl` (Hold) or Double-tap `W`
- **Inventory**: `E`
- **Chat**: `T`
- **Drop Item**: `Q`
- **Crafting**: `C` (use `Q`/`E` to cycle tabs)
- **Toggle View**: `F5`
- **Fullscreen**: `F11`
- **Pause Menu**: `Esc`
- **Attack**: `Left Click`
- **Use / Place**: `Right Click`
- **Select Item**: `Mouse Wheel` or `1`–`9`
- **Accept/Decline Tutorial**: `Enter` / `B`
- **Player List / Host Options**: `TAB`
- **Toggle HUD**: `F1`
- **Toggle Debug Info**: `F3`
- **Open Debug Overlay**: `F4`
- **Toggle Debug Console**: `F6`

---

## 🚀 Client Launch Arguments

| Argument           | Description                         |
|--------------------|-------------------------------------|
| `-name <username>` | Override in‑game username           |
| `-fullscreen`      | Launch in fullscreen mode           |

Example:
Minecraft.Client.exe -name Steve -fullscreen

text

---

## 🌐 LAN Multiplayer

LAN multiplayer works on Windows builds:

- Hosted worlds are advertised automatically on the local network.
- Other LAN players can discover and join from the in‑game menu.
- TCP port `25565` (game), UDP port `25566` (discovery).
- Add servers manually with the in‑game **Add Server** button.
- Rename yourself without losing data – your `uid.dat` stays constant.
- Split‑screen players can join even in multiplayer.

*LAN discovery code based on [LCEMP](https://github.com/LCEMP/LCEMP) – thanks!*

---

## 🧩 Dedicated Server

The dedicated server (`Minecraft.Server.exe`) can be run directly or inside a Docker container (via Wine).

### Docker Quick Start (using prebuilt image)
```bash
./start-dedicated-server.sh   # pulls the latest image and starts the container
Local Build in Docker
bash
docker compose -f docker-compose.dedicated-server.yml up -d --build
Server Properties
server.properties controls server behaviour. See the original documentation for details.

Server Launch Arguments
Arguments override properties:

Argument	Description
-port <1-65535>	Override server-port
-ip <addr>	Override server-ip
-name <name>	Override server-name (max 16 chars)
-maxplayers <1-8>	Override max-players
-seed <int64>	Override level-seed
-loglevel <level>	Override log-level
-help	Show usage

---

## 🛠️ Build Instructions
Install Visual Studio 2019 - 2022 with “Desktop development with C++”.

Clone this repository.

Open MinecraftConsoles.sln.

Set Minecraft.Client as startup project.

Choose Debug (or Release) and Windows64 platform, then build & run.

Using CMake
powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug --target MinecraftClient
For more details, see COMPILE.md.

---

## 🤝 Contributing
We’d love your help! Please read our Contributor’s Guide before opening a pull request. Highlights:

No feature is off‑limits by default – propose and discuss!

AI‑generated code is accepted, but must be reviewed and tested.

Be respectful and collaborative.

Let’s build the ultimate Legacy Console Edition together!

This project is a fork of MinecraftConsoles by smartcmd and contributors. We thank them for their foundational work.
