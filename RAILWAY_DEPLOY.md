# 🚂 Railway Deployment - DudeGolf Server

## Current Status: Ready to Deploy! ✅

Your code is on GitHub: https://github.com/MolochDaGod/DudeGolf

---

## 🚀 Deploy to Railway (Follow These Steps):

### Step 1: Login to Railway
1. In the Railway page that just opened
2. Click **"Login"** or **"Sign up"**
3. Choose **"Login with GitHub"**
4. Authorize Railway to access your repositories

### Step 2: Create New Project
1. Click **"Start a New Project"** or **"New Project"**
2. Select **"Deploy from GitHub repo"**
3. You'll see your repositories - select **"MolochDaGod/DudeGolf"**

### Step 3: Configure Deployment
Railway will automatically:
- ✅ Detect the `Dockerfile` in `samples/golf/`
- ✅ Read `railway.json` configuration
- ✅ Set environment variables for server mode
- ✅ Start building your server

**Build time:** ~10-15 minutes (first deployment)

### Step 4: Generate Public Domain
Once deployed:
1. Click on your deployed service
2. Go to **"Settings"** tab
3. Scroll to **"Networking"** section
4. Click **"Generate Domain"**
5. Your server URL will be something like: `dudegolf-production.railway.app`

### Step 5: Note Your Server Address
Your multiplayer server will be at:
```
<your-railway-domain>:25565
```

Example: `dudegolf-production.railway.app:25565`

---

## 🎮 Connect to Your Server

### From Your Local Game:
1. Run `golf-nosteam.exe` (already built at `x64\Release\golf-nosteam.exe`)
2. Go to **Multiplayer** menu
3. Enter server address: `<your-railway-domain>:25565`
4. Click **Connect**

### Share With Friends:
Send them:
- Server address: `<your-railway-domain>:25565`
- Max players: 4
- Game mode: Golf with RPG progression

---

## 📊 Monitor Your Server

### View Logs:
1. In Railway dashboard, click your service
2. Go to **"Deployments"** tab
3. Click on the active deployment
4. View real-time logs

### Check Status:
- Green = Running
- Building = In progress
- Red = Error (check logs)

---

## 🔧 Server Configuration

Your server is configured with:
- **Port:** 25565 (TCP/UDP)
- **Max Players:** 4
- **Mode:** Dedicated server
- **Auto-restart:** Enabled
- **Environment:** Docker container

---

## 💰 Free Tier Details

Railway free tier includes:
- **500 hours/month** of uptime
- **$5 monthly credit**
- **100 GB bandwidth**
- **Persistent storage** (for progression database)

**This is MORE than enough for 4-player golf!**

---

## 🐛 Troubleshooting

### Build Failed?
1. Check Railway logs for errors
2. Verify Dockerfile is correct
3. Ensure all dependencies are available

### Can't Connect?
1. Verify server is running (check Railway dashboard)
2. Confirm you're using the correct domain
3. Check port 25565 is accessible
4. Try both TCP and UDP

### Server Sleeps?
- Free tier may sleep after inactivity
- Server auto-wakes when someone connects
- Takes ~30 seconds to wake up

---

## 🔄 Update Your Server

When you make changes:

```powershell
# Commit and push changes
git add .
git commit -m "Update game"
git push origin main

# Railway auto-deploys the changes!
```

---

## 📈 Next Steps After Deployment

1. **Test Connection:**
   - Connect from your local game
   - Verify RPG progression saves
   - Test 4-player matches

2. **Share Server:**
   - Give friends your server address
   - Test latency/performance
   - Monitor player count in Railway logs

3. **Customize:**
   - Adjust server settings if needed
   - Add more equipment items
   - Tune XP rewards

---

## 🎉 You're Almost Done!

Once Railway finishes building (watch the logs):
1. Generate your domain
2. Get your server address
3. Launch golf-nosteam.exe locally
4. Connect to your server
5. Play 4-player golf with RPG progression!

---

## 📝 Summary of What You Have:

✅ **Local Game:** `x64\Release\golf-nosteam.exe`
✅ **GitHub Repo:** https://github.com/MolochDaGod/DudeGolf
✅ **Server Deployment:** Railway (in progress)
✅ **RPG Features:** Experience, leveling, equipment, stats
✅ **Multiplayer:** Up to 4 players
✅ **Free Hosting:** $0/month

Enjoy DudeGolf! 🏌️‍♂️⛳
