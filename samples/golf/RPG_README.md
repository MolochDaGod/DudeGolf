# Super Video Golf - RPG Edition

## 🏌️ Enhanced Golf Game with RPG Progression & 4-Player PVP

This is an enhanced version of Super Video Golf with full RPG character progression, equipment systems, and free server hosting for up to 4-player online PVP matches.

---

## ✨ New RPG Features

### Character Progression System
- **Experience Points (XP)** earned from gameplay
  - Hole-in-One: 500 XP
  - Eagle: 150 XP
  - Birdie: 75 XP
  - Par: 25 XP
  - Completed Hole: 10 XP
  - Long Drive (300+ yards): 50 XP
  - Long Putt (50+ feet): 50 XP

- **Leveling System** with exponential curve
  - Earn 3 skill points per level
  - Automatic small stat boost on level up
  - New equipment unlocks at specific levels

### Player Stats (0-100 Scale)
- **Power**: Increases drive distance
- **Accuracy**: Reduces hook/slice, improves aim
- **Spin**: Better ball spin control
- **Putting**: Improved putt accuracy and power
- **Recovery**: Better shots from rough/bunkers

### Equipment System
24 unlockable items across 6 slots:

#### Drivers (4 items)
- Starter Driver (Lvl 1)
- Pro Driver (Lvl 5) - +5 Power, +2 Accuracy
- Elite Driver (Lvl 10) - +10 Power, +3 Accuracy
- Legend Driver (Lvl 20) - +12 Power, +8 Accuracy

#### Irons (4 items)
- Starter Irons (Lvl 1)
- Forged Irons (Lvl 5) - +2 Power, +5 Accuracy
- Tour Irons (Lvl 10) - +3 Power, +10 Accuracy, +2 Spin
- Champion Irons (Lvl 20) - +5 Power, +15 Accuracy, +5 Spin

#### Wedges (3 items)
- Standard Wedge (Lvl 1)
- Spin Wedge (Lvl 5) - +2 Accuracy, +8 Spin, +5 Recovery
- Precision Wedge (Lvl 15) - +5 Accuracy, +12 Spin, +8 Recovery

#### Putters (4 items)
- Basic Putter (Lvl 1)
- Balanced Putter (Lvl 5) - +3 Accuracy, +8 Putting
- Pro Putter (Lvl 12) - +5 Accuracy, +12 Putting
- Master Putter (Lvl 25) - +8 Accuracy, +18 Putting

#### Gloves (3 items)
- Basic Gloves (Lvl 1)
- Grip Gloves (Lvl 7) - +2 Power, +5 Accuracy, +3 Spin, +2 Putting
- Pro Gloves (Lvl 15) - +3 Power, +8 Accuracy, +5 Spin, +4 Putting

#### Shoes (3 items)
- Standard Shoes (Lvl 1)
- Stable Shoes (Lvl 8) - +3 Power, +3 Accuracy, +3 Putting, +5 Recovery
- Tour Shoes (Lvl 18) - +5 Power, +5 Accuracy, +5 Putting, +8 Recovery

### Statistics Tracking
- Holes played
- Holes-in-one
- Eagles, Birdies, Pars
- Tournaments won
- Longest drive
- Longest putt

### Persistent Storage
- All progression data saved in SQLite database
- Survives game restarts
- Per-player profile support

---

## 🎮 4-Player PVP Multiplayer

### Features
- Up to 4 simultaneous players
- Real-time networked gameplay
- Turn-based competition
- Stats affect gameplay outcomes
- Persistent character progression

### Server Hosting
Multiple **FREE** hosting options:

1. **Railway.app** (Recommended)
   - 500 hours/month free
   - Easy GitHub integration
   - Persistent storage included

2. **Render.com**
   - 750 hours/month free
   - Auto-deploy from GitHub
   - 1GB disk space

3. **Fly.io**
   - 3 free VMs
   - Best performance
   - 160GB transfer/month

See `DEPLOYMENT.md` for detailed setup instructions.

---

## 🛠️ Building the Game

### Prerequisites

#### Windows
- Visual Studio 2022
- CMake 3.5.2+
- vcpkg (optional, for dependencies)

#### Linux
```bash
sudo apt install libfreetype-dev libsdl2-dev libopenal-dev \
                 libbullet-dev libopus-dev libunwind-dev \
                 libtbb-dev libsqlite3-dev libcurl4-openssl-dev
```

### Build Steps

#### Windows (Visual Studio)
```powershell
# Open Visual Studio 2022
# File → Open → CMake...
# Select CMakeLists.txt in root directory
# Build → Build All

# Or use CMake GUI
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

#### Linux
```bash
mkdir build
cd build
cmake -DBUILD_SAMPLES=ON -DUSE_STEAM=OFF ../
make -j$(nproc)
```

### Adding RPG Files to Build

The new RPG system files need to be added to the CMake configuration:

1. Edit `samples/golf/src/golf/CMakeLists.txt`
2. Add these lines:
```cmake
SET(GOLF_SRC ${GOLF_SRC}
    ${GOLF_DIR}/Progression.hpp
    ${GOLF_DIR}/Progression.cpp
)
```

---

## 🚀 Running the Game

### Client Mode (Play)
```bash
./golf
```

### Server Mode
```bash
./golf --server --port 25565 --max-players 4
```

### Docker (Local Testing)
```bash
docker-compose up --build
```

---

## 📊 Using the RPG System

### In-Game Integration

The progression system is integrated into the game through:

1. **GolfGame** - Initialize ProgressionManager
2. **GolfState** - Apply stat multipliers to shots
3. **UI** - Display character sheet, equipment menu

### Code Example: Awarding XP

```cpp
// In GolfState.cpp, after hole completion
auto& progression = m_sharedData.progressionManager;
progression.recordHoleScore(strokes, par);

// On level up, show notification
if (leveledUp)
{
    showLevelUpNotification(progression.getProgression().level);
}
```

### Code Example: Applying Stats

```cpp
// In shot calculation
auto& stats = progressionManager.getProgression()
    .calculateTotalStats(progressionManager.getEquipmentDB().getAllEquipment());

// Apply power multiplier to drive distance
float finalDistance = baseDistance * stats.getPowerMultiplier();

// Apply accuracy to reduce hook/slice
float finalAccuracy = baseAccuracy * stats.getAccuracyMultiplier();
```

---

## 🎯 Gameplay Tips

### Leveling Strategy
- Play consistently to earn XP from completed holes
- Aim for birdies/eagles for bonus XP
- Practice long drives and putts for extra rewards

### Stat Allocation
- **Beginners**: Focus on Accuracy and Putting
- **Distance Players**: Maximize Power
- **All-rounders**: Balance all stats evenly
- **Short Game**: Prioritize Spin and Recovery

### Equipment Progression
- Start with basic gear at Level 1
- Unlock Pro tier at Levels 5-8
- Elite tier unlocks Levels 10-15
- Master tier at Levels 18-25

---

## 🔧 Configuration Files

### Progression Database
Located at: `<AppData>/SuperVideoGolf/progression.db`

### Tables
- `player_progression` - Player stats and progress
- `unlocked_equipment` - Equipment ownership

### Backup
```bash
# Backup your progression
cp progression.db progression.backup.db
```

---

## 🌐 Hosting Your Server

See `DEPLOYMENT.md` for complete hosting guide.

### Quick Start (Railway)
```bash
# 1. Push to GitHub
git init
git add .
git commit -m "Add RPG features"
git push

# 2. Go to railway.app
# 3. New Project → Deploy from GitHub
# 4. Select repository
# 5. Server auto-deploys!
```

---

## 📈 Roadmap

### Planned Features
- [ ] Skill tree system
- [ ] Daily challenges
- [ ] Seasonal events
- [ ] Clan/Guild system
- [ ] Ranked matchmaking
- [ ] Cosmetic customization
- [ ] Achievement system
- [ ] Leaderboards

---

## 🐛 Troubleshooting

### Build Errors
```bash
# Missing dependencies on Linux
sudo apt install --reinstall <package-name>

# CMake cache issues
rm -rf build/
mkdir build && cd build
cmake ..
```

### Database Errors
```bash
# Reset progression (WARNING: Loses all data)
rm progression.db

# Check database integrity
sqlite3 progression.db "PRAGMA integrity_check;"
```

### Network Issues
```bash
# Check server is running
netstat -an | grep 25565

# Test connection
telnet <server-url> 25565
```

---

## 📝 License

Maintains original Super Video Golf zlib license.

RPG additions are provided as-is without warranty.

---

## 🤝 Contributing

Feel free to:
- Report bugs
- Suggest features
- Submit pull requests
- Share equipment ideas

---

## 📧 Support

For issues with:
- Original game: See main crogine repository
- RPG features: Check this README and DEPLOYMENT.md
- Hosting: Refer to platform-specific docs

---

## 🎉 Credits

- Original Game: Matt Marchant (Super Video Golf)
- RPG System: Custom addition (2025)
- Engine: Crogine framework

Enjoy your enhanced golf experience with RPG progression! 🏌️‍♂️⛳
