# Super Video Golf - Deployment Guide

## Free Server Hosting Options for 4-Player PVP

This guide covers deploying the golf server on free hosting platforms.

---

## Option 1: Railway.app (Recommended)

Railway offers 500 hours/month free tier with persistent storage.

### Steps:

1. **Create Railway Account**
   - Visit [railway.app](https://railway.app)
   - Sign up with GitHub

2. **Deploy from GitHub**
   ```bash
   # Push your code to GitHub first
   git init
   git add .
   git commit -m "Initial commit"
   git remote add origin <your-repo-url>
   git push -u origin main
   ```

3. **Create New Project**
   - Click "New Project" in Railway dashboard
   - Select "Deploy from GitHub repo"
   - Choose your golf repository
   - Railway will automatically detect the Dockerfile

4. **Configure Environment**
   - Railway will use `railway.json` configuration
   - Server will be available at: `<your-app>.railway.app:25565`

5. **Get Server URL**
   - Railway provides a public URL automatically
   - Note the port: 25565

---

## Option 2: Render.com

Render offers 750 hours/month free tier.

### Steps:

1. **Create Render Account**
   - Visit [render.com](https://render.com)
   - Sign up with GitHub

2. **Create New Web Service**
   - Click "New +" → "Web Service"
   - Connect your GitHub repository
   - Render will detect `render.yaml`

3. **Configuration**
   - Name: `golf-server`
   - Environment: Docker
   - Plan: Free
   - Auto-deploy: Yes

4. **Access**
   - Render provides a `.onrender.com` URL
   - Port 25565 is exposed

---

## Option 3: Fly.io

Fly.io offers generous free tier with better performance.

### Steps:

1. **Install flyctl**
   ```bash
   # Windows (PowerShell)
   iwr https://fly.io/install.ps1 -useb | iex
   
   # macOS/Linux
   curl -L https://fly.io/install.sh | sh
   ```

2. **Login & Deploy**
   ```bash
   # Login
   flyctl auth login
   
   # Launch app
   cd samples/golf
   flyctl launch
   ```

3. **Configure fly.toml** (created automatically)
   ```toml
   app = "golf-server"
   
   [build]
     dockerfile = "Dockerfile"
   
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

4. **Deploy**
   ```bash
   flyctl deploy
   ```

---

## Local Testing

Test the server locally before deploying:

```bash
# Build and run with Docker Compose
docker-compose up --build

# Server will be available at localhost:25565
```

---

## Server Configuration

### Environment Variables

- `SERVER_MODE=1` - Run in server mode
- `MAX_PLAYERS=4` - Maximum players (1-4)
- `SERVER_PORT=25565` - Server port

### Port Requirements

- **TCP Port 25565**: Main game communication
- **UDP Port 25565**: Real-time gameplay data

---

## Connecting Clients

Players can connect using:

```
<your-server-url>:25565
```

Examples:
- Railway: `golf-server-production.railway.app:25565`
- Render: `golf-server.onrender.com:25565`
- Fly.io: `golf-server.fly.dev:25565`

---

## Monitoring & Logs

### Railway
```bash
# Install Railway CLI
npm i -g @railway/cli

# View logs
railway logs
```

### Render
- View logs in Render dashboard
- Or use Render CLI

### Fly.io
```bash
# View logs
flyctl logs

# Check status
flyctl status
```

---

## Persistence

Player progression data is stored in SQLite database:
- Railway: Uses volumes (persists across restarts)
- Render: Uses disk mount at `/data`
- Fly.io: Uses persistent volumes

---

## Free Tier Limitations

### Railway
- 500 hours/month
- $5 credit/month
- Sleeps after inactivity

### Render
- 750 hours/month
- Sleeps after 15min inactivity
- Slower cold starts

### Fly.io
- 3 shared-cpu VMs
- 160GB outbound transfer
- Best performance

---

## Upgrading

All platforms offer paid tiers for:
- More uptime
- Better performance
- No sleep mode
- More storage

---

## Troubleshooting

### Server Won't Start
```bash
# Check logs for errors
docker logs <container-id>
```

### Connection Timeout
- Verify firewall allows ports 25565 TCP/UDP
- Check server status in hosting dashboard
- Ensure server isn't sleeping (free tier)

### Database Errors
- Check volume/disk is properly mounted
- Verify write permissions

---

## Cost Estimate (Free Tier)

- **Railway**: $0 (500 hours free)
- **Render**: $0 (750 hours free)
- **Fly.io**: $0 (3 VMs free)

All options support 4 concurrent players on free tier!
