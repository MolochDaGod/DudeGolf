# DudeGolf Game - Implementation Roadmap

## Current Status
Your golf game is built on the **crogine** 2D game engine and already has:
- ? Comprehensive game states (Menu, Golf, Shop, Career, etc.)
- ? Player profile system (PlayerData, profiles.tar storage)
- ? Gameplay stats and systems (achievements, leaderboards, etc.)
- ? Multi-platform support (Windows, possibly others)
- ? Networking infrastructure (websocket server, multiplayer support)

## Implementation Phases

### Phase 1: User Authentication System
**Goal:** Create a login/registration system before accessing game menus

#### Components Needed:
1. **Authentication Server** (choose one):
   - Option A: Local SQLite database (simplest, single-player/LAN)
   - Option B: Remote HTTP API (scalable, online multiplayer)
   - Option C: Steam/OAuth integration (existing in codebase as USE_GNS)

2. **Login UI State** - New game state that appears before menu
   - Username/password input
   - Registration form
   - Session token management

3. **User Database Schema**:
   ```sql
   CREATE TABLE users (
       user_id INTEGER PRIMARY KEY AUTOINCREMENT,
       username TEXT UNIQUE NOT NULL,
       email TEXT UNIQUE NOT NULL,
       password_hash TEXT NOT NULL,
   created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
       last_login TIMESTAMP
   );
   ```

### Phase 2: Player Profile Management
**Goal:** Allow users to create and manage golfer profiles

#### Integration Points:
- **PlayerManagementState** already exists - enhance with server sync
- **ProfileState** for viewing/editing golfer stats
- **Player Creation**: Name, appearance (avatar, hair, skin), clubs, etc.

#### Database Schema:
```sql
CREATE TABLE player_profiles (
    profile_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER FOREIGN KEY,
    player_name TEXT NOT NULL,
    creation_date TIMESTAMP,
 level INTEGER DEFAULT 1,
    experience INTEGER DEFAULT 0,
    credits INTEGER DEFAULT 0,
    avatar_flags BLOB,
    stats BLOB -- Serialized golf stats
);
```

### Phase 3: Game Integration
**Goal:** Connect login system to existing game flow

#### Changes to GolfGame.cpp:
1. Before `m_stateStack.pushState(StateID::SplashScreen)`, push new `StateID::Login`
2. Login state handles:
   - User authentication
   - Session validation
   - Load player profile into `m_profileData`
3. After successful login, continue to menu as normal

### Phase 4: Deployment
**Goal:** Create distributable game package

#### Windows Build:
1. **Visual Studio Project**: Compile in Release mode
2. **Asset Packaging**: Bundle all assets/
3. **Installer**: Create .msi or NSIS installer
4. **Configuration**: Document install requirements

#### Distribution Options:
- Steam (already using USE_GNS flag)
- itch.io
- Direct download (.zip or installer)

---

## Recommended Implementation Order

### Week 1: Local Authentication
1. Create `LoginState` (basic mock with hardcoded user)
2. Integrate into game startup
3. Test profile loading

### Week 2: SQLite Database
1. Add SQLite support
2. Implement user registration/login
3. Session token management

### Week 3: Advanced Features
1. Password hashing (bcrypt/argon2)
2. Email verification
3. Account recovery

### Week 4: Deployment
1. Create installer
2. Test on clean system
3. Documentation

---

## File Structure to Create

```
samples/golf/include/
  ??? LoginState.hpp    ? New authentication UI
  ??? UserManager.hpp          ? User database operations
  ??? SessionManager.hpp          ? Session/auth tokens
  ??? AuthenticationService.hpp   ? Core auth logic

samples/golf/src/
  ??? LoginState.cpp
  ??? UserManager.cpp
  ??? SessionManager.cpp
??? AuthenticationService.cpp
```

---

## Quick Start: What to Do Next

### Option A: Quick Demo (No Backend)
- [ ] Create `LoginState` with mock authentication
- [ ] Hardcode test user credentials
- [ ] Integrate into game startup
- **Time: 2-4 hours**

### Option B: Complete Solution (SQLite)
- [ ] Set up SQLite3 library
- [ ] Create user database schema
- [ ] Implement login/registration with hashed passwords
- [ ] Session management
- **Time: 2-3 days**

### Option C: Cloud Solution
- [ ] Design REST API
- [ ] Set up backend server (Node.js/Python/C#)
- [ ] Implement API client in game
- [ ] Deployment and hosting
- **Time: 1-2 weeks**

---

## Immediate Next Steps

1. **Clarify Requirements:**
   - Single-player or multiplayer?
   - Online or local play?
   - Need account persistence across devices?

2. **Choose Technology Stack:**
   - Authentication method (local/cloud)
   - Database backend
   - Deployment platform

3. **Start Implementation:**
- Begin with Phase 1 (Authentication)
   - Follow with Phase 2-4

---

## Architecture Notes

### Existing Systems You'll Integrate With:
- **GolfGame::m_profileData** - PlayerProfile storage
- **SharedStateData** - Global game state
- **StateStack** - UI/menu system
- **PlayerData::saveProfile()** / **loadProfile()** - Profile persistence

### Key Integration Points:
```cpp
// In GolfGame.cpp initialise()
m_stateStack.pushState(StateID::Login);  // Before SplashScreen

// Login state would do:
if (authenticateUser(username, password)) {
    loadPlayerProfile(userID);
    m_stateStack.popState();  // Return to normal flow
  m_stateStack.pushState(StateID::SplashScreen);
}
```

---

## Questions for You

1. **Scope**: Are you building for:
   - Single-player (local profiles)?
   - Multiplayer (online synced)?

2. **Hosting**: Will this game:
   - Run offline only?
   - Use a local network?
   - Connect to cloud servers?

3. **Timeline**: When do you need:
   - Basic login working?
   - Full release ready?

4. **Infrastructure**: Do you have:
   - Server/hosting available?
   - Domain name?
   - Database experience?

---

**Next Action:** Reply with answers to the above questions, and I'll provide specific code implementations tailored to your needs!
