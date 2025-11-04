# GitHub Repository Creation Script
Write-Host "==================================" -ForegroundColor Cyan
Write-Host "   GitHub Repository Setup" -ForegroundColor Cyan
Write-Host "==================================" -ForegroundColor Cyan
Write-Host ""

# Open GitHub create page
Write-Host "Opening GitHub in your browser..." -ForegroundColor Yellow
Start-Process "https://github.com/new"

Write-Host ""
Write-Host "In your browser, please:" -ForegroundColor Green
Write-Host "  1. Repository name: crogine-golf-rpg" -ForegroundColor White
Write-Host "  2. Description: Golf game with RPG progression and 4-player PVP" -ForegroundColor White
Write-Host "  3. Keep it PUBLIC" -ForegroundColor White
Write-Host "  4. DO NOT add README, .gitignore, or license" -ForegroundColor White
Write-Host "  5. Click 'Create repository'" -ForegroundColor White
Write-Host ""

# Wait for user
Read-Host "Press ENTER after you've created the repository on GitHub"

# Get GitHub username
Write-Host ""
$username = Read-Host "Enter your GitHub username"

# Setup remote and push
Write-Host ""
Write-Host "Setting up remote repository..." -ForegroundColor Yellow

try {
    # Remove existing remote if any
    git remote remove origin 2>$null
    
    # Add new remote
    git remote add origin "https://github.com/$username/crogine-golf-rpg.git"
    
    # Rename branch to main
    git branch -M main
    
    Write-Host ""
    Write-Host "Pushing to GitHub..." -ForegroundColor Yellow
    Write-Host "You'll be prompted to login to GitHub." -ForegroundColor Cyan
    Write-Host ""
    
    # Push to GitHub
    git push -u origin main
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host ""
        Write-Host "✓ Successfully pushed to GitHub!" -ForegroundColor Green
        Write-Host ""
        Write-Host "Your repository: https://github.com/$username/crogine-golf-rpg" -ForegroundColor Cyan
        Write-Host ""
        Write-Host "Now let's deploy to Railway..." -ForegroundColor Yellow
        Write-Host ""
        Start-Sleep -Seconds 2
        
        # Open Railway
        Write-Host "Opening Railway.app in your browser..." -ForegroundColor Yellow
        Start-Process "https://railway.app/new"
        
        Write-Host ""
        Write-Host "In Railway:" -ForegroundColor Green
        Write-Host "  1. Click 'Start a New Project'" -ForegroundColor White
        Write-Host "  2. Login with GitHub" -ForegroundColor White
        Write-Host "  3. Click 'Deploy from GitHub repo'" -ForegroundColor White
        Write-Host "  4. Select 'crogine-golf-rpg'" -ForegroundColor White
        Write-Host "  5. Railway will automatically build and deploy!" -ForegroundColor White
        Write-Host ""
        Write-Host "Your server will be live in ~10-15 minutes!" -ForegroundColor Cyan
        
    } else {
        Write-Host ""
        Write-Host "✗ Failed to push to GitHub" -ForegroundColor Red
        Write-Host "You may need to authenticate with a Personal Access Token" -ForegroundColor Yellow
        Write-Host ""
        Write-Host "To create a token:" -ForegroundColor Cyan
        Write-Host "  1. Go to: https://github.com/settings/tokens" -ForegroundColor White
        Write-Host "  2. Click 'Generate new token (classic)'" -ForegroundColor White
        Write-Host "  3. Give it 'repo' access" -ForegroundColor White
        Write-Host "  4. Copy the token" -ForegroundColor White
        Write-Host "  5. Use it as your password when pushing" -ForegroundColor White
        Write-Host ""
        
        Start-Process "https://github.com/settings/tokens"
    }
    
} catch {
    Write-Host "Error: $_" -ForegroundColor Red
}

Write-Host ""
Write-Host "Press any key to exit..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
