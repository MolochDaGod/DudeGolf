# DudeGolf - Deployment & Distribution Guide

## Pre-Deployment Checklist

- [ ] All source code compiles without errors
- [ ] Game tests successfully in all game modes
- [ ] Assets are properly packaged
- [ ] Configuration files are set correctly
- [ ] No debug logging enabled (or configurable)
- [ ] Version number updated
- [ ] Release notes prepared

## Windows Deployment

### Step 1: Build Release Executable

1. Open `crogine.sln` in Visual Studio 2017 or later
2. Select **Release** configuration
3. Select **x64** platform (recommended) or **x86** (for compatibility)
4. Right-click **golf** project ? **Build**
5. Wait for compilation to complete

**Output Location:**
```
build/Release/golf.exe  (or your build output directory)
```

### Step 2: Gather Required Files

Create a distribution folder structure:
```
DudeGolf/
??? DudeGolf.exe          ? Compiled executable
??? assets/       ? All game assets
?   ??? golf/
?   ??? images/
?   ??? ...
??? README.txt? Installation instructions
??? LICENSE.txt           ? License information
??? config.ini       ? Configuration file
```

### Step 3: Create Installer (NSIS)

Install NSIS (Nullsoft Scriptable Install System) from https://nsis.sourceforge.io/

Create file: `installer.nsi`

```nsis
; DudeGolf Installer Script

!include "MUI2.nsh"

; Name and file
Name "DudeGolf"
OutFile "DudeGolf_Installer_v1.0.exe"

; Default installation folder
InstallDir "$PROGRAMFILES\DudeGolf"

; Pages
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

; Language
!insertmacro MUI_LANGUAGE "English"

; Installer section
Section "Install"
    SetOutPath "$INSTDIR"
    
    ; Copy main executable
    File "DudeGolf.exe"
    
    ; Copy all assets
    SetOutPath "$INSTDIR\assets"
    File /r "assets\*.*"
    
    ; Copy configuration
    SetOutPath "$INSTDIR"
    File "config.ini"
    File "README.txt"
    File "LICENSE.txt"
    
    ; Create Start Menu shortcuts
    CreateDirectory "$SMPROGRAMS\DudeGolf"
    CreateShortcut "$SMPROGRAMS\DudeGolf\DudeGolf.lnk" "$INSTDIR\DudeGolf.exe"
    CreateShortcut "$SMPROGRAMS\DudeGolf\Uninstall.lnk" "$INSTDIR\uninstall.exe"
    CreateShortcut "$DESKTOP\DudeGolf.lnk" "$INSTDIR\DudeGolf.exe"
    
    ; Create uninstaller
    WriteUninstaller "$INSTDIR\uninstall.exe"
SectionEnd

; Uninstaller section
Section "Uninstall"
    Delete "$INSTDIR\DudeGolf.exe"
    Delete "$INSTDIR\config.ini"
    Delete "$INSTDIR\README.txt"
    Delete "$INSTDIR\LICENSE.txt"
    RMDir /r "$INSTDIR\assets"
    Delete "$INSTDIR\uninstall.exe"
    RMDir "$INSTDIR"
    
 Delete "$SMPROGRAMS\DudeGolf\DudeGolf.lnk"
    Delete "$SMPROGRAMS\DudeGolf\Uninstall.lnk"
    RMDir "$SMPROGRAMS\DudeGolf"
    Delete "$DESKTOP\DudeGolf.lnk"
SectionEnd
```

**Build installer:**
```bash
# From command line
"C:\Program Files (x86)\NSIS\makensis.exe" installer.nsi
```

### Step 4: Create Portable Version (Optional)

For a portable/no-install version, simply create a ZIP file:

```
DudeGolf_v1.0_Portable.zip
??? DudeGolf.exe
??? assets/
??? README.txt
??? LICENSE.txt
```

## Distribution Platforms

### Option 1: Direct Download Website

1. Create a simple website (GitHub Pages, Itch.io, custom site)
2. Host installer/portable ZIP
3. Include:
   - Download links
   - System requirements
   - Installation instructions
   - Patch notes
   - Screenshots/videos

### Option 2: Itch.io

1. Go to https://itch.io/
2. Create account
3. Upload game
4. Set up:
   - Game title
   - Description
   - Screenshots
   - Game files (windows executable)
5. Publish

### Option 3: Steam

1. Register as Steamworks partner
2. Create app on Steamworks dashboard
3. Set up:
   - App metadata
   - Screenshots/trailers
 - Build configurations
4. Upload build through Steam SDK
5. Prepare store page

### Option 4: Custom Launcher

Create a launcher application that:
- Auto-updates game from server
- Manages multiple versions
- Shows patch notes
- Handles mod installation (if applicable)

## System Requirements Documentation

```
Minimum Requirements:
- OS: Windows 7 or later (64-bit or 32-bit)
- Processor: Intel Core 2 Duo @ 2.0 GHz
- Memory: 2 GB RAM
- Graphics: DirectX 9 compatible with 128 MB VRAM
- Storage: 2 GB available space
- DirectX: Version 9.0 or later

Recommended:
- OS: Windows 10 or later (64-bit)
- Processor: Intel Core i5 @ 2.5 GHz or better
- Memory: 4 GB RAM
- Graphics: DirectX 11 compatible with 512 MB VRAM
- Storage: SSD with 2 GB available space
- DirectX: Version 11 or later
```

## Post-Installation Configuration

Users should be able to configure:
- Resolution
- Graphics quality
- Audio levels
- Control scheme
- Login credentials

Store configuration in:
```
%APPDATA%\Trederia\golf\user_prefs.cfg
```

## Update/Patch Distribution

### Versioning Scheme
```
v[MAJOR].[MINOR].[PATCH][-BETA]

Examples:
v1.0.0      - Initial release
v1.0.1- Bug fixes
v1.1.0      - New features
v2.0.0      - Major overhaul
v1.1.0-BETA - Beta test release
```

### Patch Distribution
1. Create patch file containing only changed files
2. Include patch notes/changelog
3. Implement in-game or launcher update checker
4. Provide manual download option

## Troubleshooting & Support

### Common Issues & Solutions

**Game won't start:**
- Verify DirectX 9+ installed
- Check graphics drivers updated
- Try administrator mode
- Check system requirements

**Graphics errors:**
- Update GPU drivers
- Check resolution settings
- Try lower graphics quality
- Disable post-processing

**Performance issues:**
- Lower resolution
- Reduce graphics quality
- Close background programs
- Update drivers

### Support Channels
- Email support address
- Discord server
- GitHub issues
- Community forums

## Security Considerations

- [ ] Sign executable (Windows code signing certificate)
- [ ] Scan installer for malware
- [ ] Use HTTPS for downloads
- [ ] Verify checksums (SHA-256)
- [ ] Privacy policy for user data
- [ ] GDPR compliance (if applicable)

## Continuous Deployment Workflow

```
1. Developer commits to main branch
   ?
2. Automated build system triggers
   ?
3. Compile Release build
   ?
4. Run automated tests
   ?
5. Generate installer
   ?
6. Create changelog
   ?
7. Tag release version
   ?
8. Upload to distribution platforms
   ?
9. Notify users of new version
```

## File Checklist for Distribution

### Essential Files
- [x] Compiled executable (golf.exe)
- [x] All game assets (assets/ folder)
- [x] Configuration files
- [x] License file
- [x] README/Manual

### Optional Files
- [ ] Installer (NSIS/MSI)
- [ ] Portable ZIP
- [ ] Source code (if open source)
- [ ] Developer documentation
- [ ] Mod SDK (if applicable)

## Next Steps

1. **Immediate (This Week):**
   - [ ] Compile Release build
   - [ ] Test on clean Windows machine
   - [ ] Create installer with NSIS

2. **Short Term (This Month):**
   - [ ] Set up Itch.io account
   - [ ] Publish first public release
   - [ ] Set up feedback mechanisms

3. **Medium Term (Next 3 Months):**
   - [ ] Implement auto-updater
   - [ ] Collect player feedback
   - [ ] Plan first patch/update

4. **Long Term (6+ Months):**
   - [ ] Expand to other platforms (Mac, Linux)
   - [ ] Consider Steam store launch
   - [ ] Implement multiplayer features

---

## Quick Start Commands

### Build Release
```bash
cd C:\Users\nugye\Documents\GitHub\DudeGolf
# In Visual Studio: Select Release|x64, Build Solution
```

### Create Installer
```bash
"C:\Program Files (x86)\NSIS\makensis.exe" installer.nsi
```

### Package Portable
```bash
# Create ZIP with: DudeGolf.exe + assets/ + docs
```

### Test Installation
```bash
# Run on clean virtual machine
# Test:
# - Game launches correctly
# - All features work
# - Audio/video play
# - User can create profile
# - User can start game
```

---

**Remember:** Always test on a clean system before releasing to the public!
