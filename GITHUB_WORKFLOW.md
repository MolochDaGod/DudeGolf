# GitHub Workflow Guide for DudeGolf Development

## ?? Setup (First Time Only)

### 1. Initialize Git (Already Done ?)
```bash
cd "C:\Users\nugye\Documents\GitHub\DudeGolf"
git status  # Should show your repo
```

### 2. Verify Remote
```bash
git remote -v
# Should show: origin https://github.com/fallahn/crogine.git
```

### 3. Create .gitignore (If Not Present)
```bash
# Create file: .gitignore in root directory
# Add these lines:

# Build directories
/build/
/bin/
/obj/

# Visual Studio
*.sln.user
*.vcxproj.user
*.vcxproj.filters.user
.vs/

# Generated files
*.o
*.obj
*.lib
*.exe

# User data (don't commit)
/AppData/
user_prefs.cfg
prefs.cfg

# Temp files
*.tmp
*.bak

# IDE
.vscode/
.idea/
```

---

## ?? Daily Workflow

### Before Starting Work
```bash
# Update your local copy
git pull origin master

# Check status
git status
```

### During Development

#### Making Changes:
```bash
# 1. Make your code changes
# 2. Test thoroughly
# 3. View what changed
git status

# 4. Stage files
git add .
# Or stage specific files:
git add samples/golf/src/LoginState.cpp
git add samples/golf/include/golf/LoginState.hpp

# 5. Review staged changes
git diff --staged

# 6. Commit with descriptive message
git commit -m "feat: Add LoginState for user authentication

- Implement login/registration UI with ImGui
- Add form validation (username, email, password)
- Integrate with SharedStateData
- Support offline mode bypass"

# 7. Push to GitHub
git push origin master
```

### Commit Message Best Practices

```
Format: <type>: <subject>

Types:
- feat: New feature (login system, user manager)
- fix: Bug fix (validation error, crash)
- docs: Documentation only
- refactor: Code restructuring without feature change
- perf: Performance improvement
- test: Adding/updating tests
- chore: Build process, dependencies

Examples:
? git commit -m "feat: Implement UserManager with file persistence"
? git commit -m "fix: Correct email validation regex"
? git commit -m "docs: Add integration guide"
? git commit -m "refactor: Simplify LoginState form handling"
? git commit -m "fixed stuff"  <- Too vague!
? git commit -m "WIP"  <- Not descriptive!
```

---

## ?? Branch Workflow (Recommended)

### Create Feature Branch
```bash
# Create and switch to new branch
git checkout -b feature/login-system

# Make changes and commit
git add .
git commit -m "feat: Add LoginState and UserManager"
git commit -m "feat: Implement form validation"
git commit -m "feat: Add user persistence layer"

# Push branch to GitHub
git push origin feature/login-system
```

### Merge Back to Master
```bash
# Switch to master
git checkout master

# Update master
git pull origin master

# Merge feature branch
git merge feature/login-system

# Delete feature branch (cleanup)
git branch -d feature/login-system
git push origin --delete feature/login-system

# Push merged changes
git push origin master
```

### Branch Naming Convention
```
feature/login-system        ? New features
feature/password-hashing
feature/cloud-backend

bugfix/validation-error     ? Bug fixes
bugfix/crash-on-startup

docs/integration-guide      ? Documentation

release/v1.0.0          ? Release branches
```

---

## ?? Git Best Practices

### 1. Commit Often, Push Regularly
```bash
# Good: Multiple small commits
git commit -m "feat: Add LoginState class"
git commit -m "feat: Implement form rendering"
git commit -m "feat: Add validation logic"
git push origin master

# Bad: One giant commit
git commit -m "Added everything" -A
```

### 2. Pull Before Push
```bash
# Always pull latest changes first
git pull origin master

# Then make your changes and push
git push origin master
```

### 3. Review Before Committing
```bash
# Check what's about to be committed
git diff

# Stage carefully
git add LoginState.cpp
# NOT: git add .  (adds everything!)

# Double-check
git diff --staged
```

---

## ?? Useful Commands

### View History
```bash
# See recent commits
git log --oneline -10

# See commits with details
git log --oneline --graph --all

# See what changed in a specific file
git log -p samples/golf/src/LoginState.cpp
```

### Undo Changes
```bash
# Discard local changes (WARNING: irreversible)
git checkout -- <filename>

# Or for all files:
git reset --hard HEAD

# Undo last commit (keep changes)
git reset --soft HEAD~1

# Undo last commit (discard changes) WARNING!
git reset --hard HEAD~1

# Undo after push (create new commit that reverses changes)
git revert HEAD
git push origin master
```

### Stash Changes (Temporary Save)
```bash
# Save work in progress without committing
git stash

# Do something else (pull, switch branch)
git pull origin master

# Bring back your work
git stash pop
```

### Check Differences
```bash
# Between local and remote
git diff origin/master

# Between two branches
git diff feature/login-system master

# Between two commits
git diff abc123 def456
```

---

## ?? Release Workflow

### Before Release
```bash
# 1. Create release branch
git checkout -b release/v1.0.0

# 2. Update version numbers
# Edit: VERSION file, code comments, etc.
git add VERSION
git commit -m "chore: Bump version to 1.0.0"

# 3. Test Release build
# Build in Visual Studio Release configuration
# Test thoroughly

# 4. Push release branch
git push origin release/v1.0.0
```

### After Release
```bash
# 1. Merge back to master
git checkout master
git pull origin master
git merge release/v1.0.0
git push origin master

# 2. Create GitHub Release (on GitHub.com)
# Go to Releases ? Draft new release
# Tag: v1.0.0
# Release notes: What's new, known issues, credits

# 3. Delete release branch
git branch -d release/v1.0.0
git push origin --delete release/v1.0.0

# 4. Merge back to develop if you have one
git checkout develop
git merge master
git push origin develop
```

---

## ?? Debugging with Git

### Find When Bug Was Introduced
```bash
# Binary search through commits
git bisect start
git bisect bad HEAD
git bisect good v0.9.0

# Test current version
# Then tell git if it's bad or good
git bisect bad  # or git bisect good
# Git will narrow down the problem commit
```

### Blame a Line
```bash
# See who changed each line
git blame samples/golf/src/LoginState.cpp

# See history of a specific line
git log -L 10,20:samples/golf/src/LoginState.cpp
```

---

## ?? Collaboration Workflow

### When Working with Others

```bash
# Before starting work
git pull origin master

# Create feature branch for your work
git checkout -b feature/your-feature

# Make commits regularly
git commit -m "..."

# Push to GitHub when ready for review
git push origin feature/your-feature

# On GitHub: Create Pull Request (PR)
# - Describe what you changed
# - Reference any issues
# - Ask for review

# After review & approval:
# Merge PR on GitHub

# Clean up locally:
git checkout master
git pull origin master
git branch -d feature/your-feature
```

---

## ?? Inspecting Your Repository

### Current Status
```bash
# Clean and up-to-date?
git status
# Should show: "On branch master... nothing to commit"

# Behind on updates?
git fetch origin
git status
# Will show: "Your branch is X commits behind"

# Untracked files cluttering things?
git status
# Lists all uncommitted changes
```

### File History
```bash
# All commits for a file
git log samples/golf/src/LoginState.cpp

# Changes to a file
git log -p samples/golf/src/LoginState.cpp

# Who changed what
git blame samples/golf/src/LoginState.cpp
```

---

## ?? Tagging Releases

### Create a Tag
```bash
# Lightweight tag (just a marker)
git tag v1.0.0
git push origin v1.0.0

# Annotated tag (with message)
git tag -a v1.0.0 -m "Release version 1.0.0"
git push origin v1.0.0

# List tags
git tag -l

# View tag details
git show v1.0.0
```

---

## ?? Pro Tips

### 1. Alias for Common Commands
```bash
git config --global alias.st status
git config --global alias.co checkout
git config --global alias.br branch
git config --global alias.ci commit
git config --global alias.unstage 'reset HEAD --'
git config --global alias.last 'log -1 HEAD'
git config --global alias.visual 'log --oneline --graph --all'

# Now you can use:
git st (instead of git status)
git co          (instead of git checkout)
git visual      (pretty branch visualization)
```

### 2. Configure Git Globally
```bash
# Set name and email (first time setup)
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"

# Set default editor
git config --global core.editor "notepad"

# View your config
git config --list
```

### 3. Useful .gitconfig

Create or edit `~/.gitconfig`:
```ini
[user]
    name = Your Name
    email = your.email@example.com

[alias]
    st = status
    co = checkout
    br = branch
    ci = commit
    unstage = reset HEAD --
    last = log -1 HEAD
  visual = log --oneline --graph --all

[core]
    editor = notepad
    autocrlf = true

[color]
    ui = true
```

---

## ?? Your First GitHub Workflow

### Step 1: Prepare Your Code
```bash
cd "C:\Users\nugye\Documents\GitHub\DudeGolf"

# Check status
git status

# Add all new files
git add LoginState.hpp LoginState.cpp UserManager.hpp UserManager.cpp
git add INTEGRATION_GUIDE.md IMPLEMENTATION_PLAN.md
```

### Step 2: Commit
```bash
git commit -m "feat: Add complete login system with authentication

- Implement LoginState with ImGui-based UI
- Add UserManager for account management
- Provide comprehensive integration guides
- Include deployment documentation
- Support offline and online modes"
```

### Step 3: Push
```bash
git push origin master
```

### Step 4: Verify on GitHub
```
Go to: https://github.com/fallahn/crogine
Check your new commits are visible
```

---

## ?? Emergency Fixes

### Undo Last Push (DANGEROUS!)
```bash
# Only if push was wrong!
git reset --hard HEAD~1
git push -f origin master

# ?? ONLY USE IF NO ONE ELSE IS USING REPO
```

### Recover Deleted Commits
```bash
# See all commits ever
git reflog

# Restore a deleted branch
git checkout -b recovered-branch abc123
```

---

## ?? Learning Resources

### GitHub/Git Tutorials
- **Official Git Book:** https://git-scm.com/book
- **GitHub Guides:** https://guides.github.com
- **Crogine Examples:** Check fallahn/crogine commits

### Visual Tools
- **GitHub Desktop:** https://desktop.github.com (GUI)
- **Gitextensions:** http://gitextensions.github.io/ (Windows)
- **Tortoisegit:** https://tortoisegit.org/ (Windows explorer integration)

---

## ? GitHub Workflow Checklist

### Before Each Commit
- [ ] Changes tested locally
- [ ] Code compiles without errors
- [ ] No compiler warnings added
- [ ] Comments added for complex logic
- [ ] No debug logging left in

### Before Each Push
- [ ] Pull latest changes first
- [ ] All commits have descriptive messages
- [ ] Related commits grouped logically
- [ ] Ready to be seen by others

### Before Each Release
- [ ] Version number updated
- [ ] Changelog/release notes prepared
- [ ] Tag created
- [ ] Binary built and tested
- [ ] All documentation updated

---

**You're now ready to use GitHub professionally for DudeGolf development!** ??

Use these workflows to keep your code organized, documented, and collaborative.
