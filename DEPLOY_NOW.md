# 🚀 Deploy Your Golf Server NOW - Step by Step

Your repository is ready! Follow these steps to deploy your server.

---

## ✅ Already Completed:
- ✅ Git repository initialized
- ✅ All files committed
- ✅ RPG system added
- ✅ Docker configuration ready

---

## 📋 Deployment Steps

### Option A: Railway.app (Recommended - Easiest)

#### Step 1: Create GitHub Repository

1. Go to https://github.com/new
2. Repository name: `crogine-golf-rpg` (or any name you prefer)
3. Description: "Golf game with RPG progression and 4-player PVP"
4. **Keep it PUBLIC** (or private if you have a paid GitHub account)
5. **DO NOT** initialize with README, .gitignore, or license
6. Click "Create repository"

#### Step 2: Push Your Code to GitHub

Copy and paste these commands in PowerShell:

```powershell
# Set your GitHub username (replace YOUR_USERNAME)
$username = "YOUR_USERNAME"
$reponame = "crogine-golf-rpg"

# Add remote
git remote add origin "https://github.com/$username/$reponame.git"

# Push to GitHub
git branch -M main
git push -u origin main
```

**Note:** You'll be prompted to login to GitHub. Use your username and a Personal Access Token (not password).

To create a token:
1. Go to https://github.com/settings/tokens
2. Click "Generate new token (classic)"
3. Give it a name like "Golf Deployment"
4. Check "repo" scope
5. Generate and copy the token
6. Use this token as your password when pushing

#### Step 3: Deploy to Railway

1. Go to https://railway.app
2. Click "Start a New Project"
3. Sign up/Login with GitHub
4. Click "Deploy from GitHub repo"
5. Select your `crogine-golf-rpg` repository
6. Railway will automatically:
   - Detect the Dockerfile
   - Build your server
   - Deploy it

#### Step 4: Configure Railway

1. After deployment, click on your service
2. Go to "Settings" tab
3. Under "Networking", click "Generate Domain"
4. Your server URL will be: `something.railway.app`
5. Note the URL - you'll use it to connect

#### Step 5: Get Your Server Address

Your server will be available at:
```
<your-app-name>.railway.app:25565
```

Example: `golf-server-production.railway.app:25565`

**Share this address with players to connect!**

---

### Option B: Render.com (Alternative)

#### Step 1-2: Same as Railway (Create GitHub repo and push)

#### Step 3: Deploy to Render

1. Go to https://render.com
2. Click "Get Started for Free"
3. Sign up with GitHub
4. Click "New +" → "Web Service"
5. Connect your GitHub repository
6. Render will detect `render.yaml`
7. Click "Create Web Service"

#### Step 4: Wait for Deployment

- First deployment takes 10-15 minutes
- Watch the logs for "Server started"

#### Step 5: Get Server URL

Your server will be at:
```
<your-service>.onrender.com:25565
```

---

### Option C: Fly.io (Best Performance)

#### Step 1: Install Fly CLI

```powershell
# Install Fly CLI
iwr https://fly.io/install.ps1 -useb | iex
```

#### Step 2: Login to Fly

```powershell
flyctl auth login
```

#### Step 3: Create Fly.toml

Navigate to golf directory:
```powershell
cd samples\golf
```

Create `fly.toml`:
```toml
app = "golf-server"

[build]
  dockerfile = "Dockerfile"

[env]
  SERVER_MODE = "1"
  MAX_PLAYERS = "4"
  SERVER_PORT = "25565"

[[services]]
  internal_port = 25565
  protocol = "tcp"

  [[services.ports]]
    port = 25565

[[services]]
  internal_port = 25565
  protocol = "udp"

  [[services.ports]]
    port = 25565
```

#### Step 4: Deploy

```powershell
# From samples/golf directory
flyctl launch --no-deploy --copy-config

# Deploy
flyctl deploy
```

Your server will be at: `golf-server.fly.dev:25565`

---

## 🎮 Testing Your Server

### Test Connection:

```powershell
# Test if server is reachable (replace URL with yours)
Test-NetConnection -ComputerName your-server.railway.app -Port 25565
```

### Check Server Logs:

**Railway:**
```powershell
# Install Railway CLI
npm install -g @railway/cli

# View logs
railway logs
```

**Render:**
- View in dashboard under "Logs" tab

**Fly.io:**
```powershell
flyctl logs
```

---

## 🔧 Troubleshooting

### Server Won't Start
- Check logs for errors
- Verify Dockerfile is correct
- Ensure all dependencies are listed

### Can't Connect
- Verify port 25565 is open
- Check firewall settings
- Ensure server isn't sleeping (free tier)
- Try TCP instead of UDP

### Build Fails
- Check if base images are accessible
- Verify CMake configuration
- Review build logs for missing dependencies

---

## 💡 Quick Tips

### Free Tier Limitations:
- **Railway**: Server sleeps after inactivity, wakes on connection
- **Render**: Sleeps after 15 minutes of inactivity
- **Fly.io**: Always-on (best option)

### Wake Up Sleeping Server:
Just connect - it will auto-start (takes ~30 seconds)

### Updating Your Server:
```powershell
# Make changes, then:
git add .
git commit -m "Update server"
git push

# Railway/Render auto-deploys
# For Fly.io, run: flyctl deploy
```

---

## 📊 Monitoring

### Check Server Status:

**Railway:**
```powershell
railway status
```

**Render:**
- View in dashboard

**Fly.io:**
```powershell
flyctl status
flyctl logs --app golf-server
```

---

## 🎉 You're Done!

Once deployed, share your server URL with friends:

```
Server Address: <your-server-url>:25565
Max Players: 4
Game Mode: PVP Golf with RPG Progression
```

Players can now:
- Connect to your server
- Play 4-player golf matches
- Earn XP and level up
- Unlock equipment
- Track statistics

---

## 🆘 Need Help?

### Common Issues:

1. **"Port already in use"**
   - Server is already running
   - Or another app is using port 25565

2. **"Cannot connect to server"**
   - Check firewall
   - Verify server is running
   - Ensure correct URL and port

3. **"Build failed"**
   - Check Dockerfile syntax
   - Verify all dependencies available
   - Review build logs

### Get Support:
- Check Railway/Render/Fly.io documentation
- Review error logs carefully
- Test locally with docker-compose first

---

## 🚀 Next Steps After Deployment:

1. Test with 2-4 players
2. Monitor server performance
3. Check database persistence
4. Test RPG progression
5. Share with more players!

Good luck with your deployment! 🏌️‍♂️
