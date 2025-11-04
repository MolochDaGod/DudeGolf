# Integration Guide - Adding Login to DudeGolf

## Quick Start: 5 Steps to Add Login

### Step 1: Register LoginState in GolfGame.hpp

Add to the StateID enum in `golf/StateIDs.hpp` (or wherever it's defined):

```cpp
enum class StateID
{
  // ...existing states...
    Login, // ? Add this
    SplashScreen,
    Menu,
    // ...rest of states...
};
```

### Step 2: Register State in GolfGame Constructor

In `GolfGame.cpp` constructor, add:

```cpp
#include "golf/LoginState.hpp"

GolfGame::GolfGame()
: m_stateStack({*this, getWindow()}),
      m_activeIndex(0)
{
    //...existing code...
 
    // Add LoginState registration BEFORE other states
    m_stateStack.registerState<LoginState>(StateID::Login, m_sharedData);
    
// Existing state registrations follow...
    m_stateStack.registerState<SplashState>(StateID::SplashScreen, m_sharedData);
    // ...rest of registrations...
}
```

### Step 3: Initialize State Stack with Login

In `GolfGame::initialise()`, change the state stack initialization:

**BEFORE:**
```cpp
#ifdef CRO_DEBUG_
    m_stateStack.pushState(StateID::Menu);
#else
    m_stateStack.pushState(StateID::SplashScreen);
#endif
```

**AFTER:**
```cpp
#ifdef CRO_DEBUG_
    m_stateStack.pushState(StateID::Login);  // ? Start with login
    m_stateStack.pushState(StateID::Menu);
#else
    m_stateStack.pushState(StateID::Login);  // ? Start with login
    m_stateStack.pushState(StateID::SplashScreen);
#endif
```

### Step 4: Add Files to Project

Add these new files to your Visual Studio project:

- `samples/golf/include/golf/LoginState.hpp`
- `samples/golf/src/LoginState.cpp`
- `samples/golf/include/golf/UserManager.hpp`
- `samples/golf/src/UserManager.cpp`

In Visual Studio:
1. Right-click `golf` project
2. Add ? Existing Item
3. Select the 4 new files above

### Step 5: Compile & Test

```bash
# Build the solution
Build ? Build Solution (Ctrl+Shift+B)

# If successful, run the game
# You should see the login screen!
```

---

## File Integration Checklist

### Include Paths to Add

In `GolfGame.hpp`, add:
```cpp
#include "golf/LoginState.hpp"
#include "golf/UserManager.hpp"
```

### Project File Changes

In `samples\golf\golf.vcxproj`, add to the ItemGroup:

```xml
<ClInclude Include="..\..\..\include\golf\LoginState.hpp" />
<ClInclude Include="..\..\..\include\golf\UserManager.hpp" />

<ClCompile Include="LoginState.cpp" />
<ClCompile Include="UserManager.cpp" />
```

---

## Workflow After Login

### User Logs In Successfully

1. **Login Validation:**
   ```
   LoginState.validateLogin()
     ? UserManager.loginUser()
     ? Verify credentials
      ? Set current user
    ? Return success
   ```

2. **State Transition:**
   ```cpp
   // In LoginState.cpp simulate() after successful auth
   if (userAuthSuccessful) {
    requestStackPop();  // Pop LoginState
 // Now SplashScreen/Menu shows with logged-in user
   }
   ```

3. **User Data Available:**
   ```cpp
   // In any state, access current user via UserManager
   UserManager& userMgr = UserManager::instance();
   if (auto user = userMgr.getCurrentUser()) {
       std::cout << "Logged in as: " << user->username << std::endl;
   }
   ```

---

## Testing the Implementation

### Test 1: Login Flow

1. Run game
2. See login screen
3. Click "Login"
4. Enter test credentials (any username/password)
5. Click "Login" button
6. Should proceed to menu

### Test 2: Registration Flow

1. Run game
2. See login screen
3. Click "Create Account"
4. Enter username, email, password
5. Click "Create Account"
6. Should return to main menu and allow login

### Test 3: Offline Mode

1. Run game
2. Click "Play Offline"
3. Should proceed directly to splash screen/menu

---

## Advanced: Making Login Optional

If you want to make login optional and only require it for certain features:

```cpp
// In LoginState.cpp
void LoginState::drawMainMenu()
{
    // ...existing code...
    
    // Only allow offline play if explicitly enabled
    if (m_sharedData.allowOfflineMode) {  // New setting
 if (ImGui::Button("Play Offline", ImVec2(300.f, 40.f))) {
            requestStackPop();
        }
  }
}
```

Then in `SharedStateData`:
```cpp
struct SharedStateData {
    // ...existing members...
    bool allowOfflineMode = true;  // ? Add this
};
```

---

## Extending LoginState

### Adding Remember Me
```cpp
// In LoginState.hpp
bool m_rememberMe = false;

// In LoginState.cpp render()
ImGui::Checkbox("Remember me", &m_rememberMe);

// In validateLogin()
if (m_rememberMe) {
    // Save credentials securely for next launch
    SaveCredentials(m_username, m_password);
}
```

### Adding Social Login
```cpp
// In LoginState::drawMainMenu()
if (ImGui::Button("Login with Steam", ImVec2(300.f, 40.f))) {
 if (Social::loginWithSteam()) {
        requestStackPop();
    }
}

// Or Discord
if (ImGui::Button("Login with Discord", ImVec2(300.f, 40.f))) {
    // Discord login flow
}
```

### Email Verification
```cpp
// After registration
if (validateRegistration()) {
    // Send verification email
    SendVerificationEmail(m_email);
    // Show "Check your email" screen
    m_mode = LoginMode::VerifyingEmail;
}
```

---

## Common Issues & Solutions

### Issue: State not found compile error

**Solution:** Make sure StateID::Login is defined in StateIDs.hpp

### Issue: LoginState reference fails

**Solution:** Check you've included `#include "golf/LoginState.hpp"`

### Issue: Game crashes on login

**Solution:** Ensure UserManager is properly initialized

### Issue: ImGui text input not working

**Solution:** Make sure ImGui is properly initialized in SharedStateData

---

## Next: Backend Integration

Once the basic UI is working, integrate with a real backend:

### Option A: SQLite (Local)
```cpp
// UserManager.cpp
#include <sqlite3.h>

bool UserManager::loadUsers() {
    sqlite3* db;
    sqlite3_open(m_usersDatabasePath.c_str(), &db);
    // Query SELECT * FROM users
    // Populate m_users vector
    sqlite3_close(db);
}
```

### Option B: HTTP API
```cpp
// AuthenticationService.hpp
class AuthenticationService {
public:
    bool loginRemote(const std::string& username, const std::string& password);
private:
    std::string m_apiUrl = "https://api.dudegolf.com";
};
```

### Option C: Steam/Social
```cpp
// Use existing Social framework
if (Social::isSteamdeck()) {
    // Auto-login with Steam account
    auto steamID = Social::getPlayerID();
    auto username = Social::getPlayerName();
}
```

---

##Implementation Sequence

```
Week 1: Basic UI
  ? LoginState renders
  ? Input handling
  ? Form validation

Week 2: Local Storage
  ? UserManager skeleton
  ? File-based persistence
  ? Session management

Week 3: Backend
  ? Choose backend system
  ? Implement authentication
  ? Cloud sync (optional)

Week 4: Polish
  ? Error handling
  ? Recovery flows
  ? Security hardening
```

---

## Files Modified Summary

### New Files Created
- ? `samples/golf/include/golf/LoginState.hpp`
- ? `samples/golf/src/LoginState.cpp`
- ? `samples/golf/include/golf/UserManager.hpp`
- ? `samples/golf/src/UserManager.cpp`
- ? `IMPLEMENTATION_PLAN.md`
- ? `DEPLOYMENT_GUIDE.md`

### Files to Modify
- [ ] `samples/golf/golf.vcxproj` - Add new source files
- [ ] `golf/StateIDs.hpp` - Add StateID::Login
- [ ] `GolfGame.cpp` - Register state and initialize login

### Files Already Compatible
- ? `GolfGame.hpp` - No changes needed
- ? `SharedStateData.hpp` - No changes needed
- ? `MenuState.hpp` - No changes needed

---

## Testing Checklist

Before considering login complete:

- [ ] Game compiles without errors
- [ ] Login screen displays correctly
- [ ] Can enter username and password
- [ ] Form validation works
- [ ] Can switch between login/register/main menu
- [ ] Offline mode works
- [ ] User can proceed to splash screen after login
- [ ] No console errors or warnings
- [ ] Game performance not affected
- [ ] All existing features still work

---

## Support & Documentation

For questions or issues:

1. Check `IMPLEMENTATION_PLAN.md` for overall architecture
2. Review `DEPLOYMENT_GUIDE.md` for distribution
3. See code comments in LoginState.cpp for usage
4. Refer to crogine documentation for State system

---

**You're ready to integrate!** Follow the 5 steps above and you'll have a working login system in your game!
