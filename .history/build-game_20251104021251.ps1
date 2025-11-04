Write-Host "==================================" -ForegroundColor Cyan
Write-Host "  Building DudeGolf Game" -ForegroundColor Cyan
Write-Host "==================================" -ForegroundColor Cyan
Write-Host ""

# Find Visual Studio installation
$vsWhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"

if (Test-Path $vsWhere) {
    Write-Host "Finding Visual Studio..." -ForegroundColor Yellow
    $vsPath = & $vsWhere -latest -property installationPath
    
    if ($vsPath) {
        Write-Host "Found Visual Studio at: $vsPath" -ForegroundColor Green
        
        # Import Visual Studio environment
        $devShell = Join-Path $vsPath "Common7\Tools\Launch-VsDevShell.ps1"
        
Connection URL

hide
open
postgresql://postgres:UyfuHOzPtLRhTLrLcDftlteVRuLMiDDU@interchange.proxy.rlwy.net:55024/railway

Raw psql command

hide
PGPASSWORD=UyfuHOzPtLRhTLrLcDftlteVRuLMiDDU psql -h interchange.proxy.rlwy.net -U postgres -p 55024 -d railway

Railway CLI connect command

railway connect Postgres        if (Test-Path $devShell) {
            Write-Host "Loading Visual Studio environment..." -ForegroundColor Yellow
            & $devShell -SkipAutomaticLocation -Arch amd64
            
            Write-Host ""
            Write-Host "Building the golf game (this may take 5-10 minutes)..." -ForegroundColor Yellow
            Write-Host ""
            
            # Build with MSBuild
            msbuild crogine.sln -t:golf -p:Configuration=Release -p:Platform=x64 -m -v:minimal
            
            if ($LASTEXITCODE -eq 0) {
                Write-Host ""
                Write-Host "✓ Build successful!" -ForegroundColor Green
                Write-Host ""
                
                # Find the built executable
                $exePath = Get-ChildItem -Path "samples\golf" -Recurse -Filter "golf.exe" -ErrorAction SilentlyContinue | Select-Object -First 1
                
                if ($exePath) {
                    Write-Host "Game executable created at:" -ForegroundColor Cyan
                    Write-Host "  $($exePath.FullName)" -ForegroundColor White
                    Write-Host ""
                    
                    $runNow = Read-Host "Do you want to run the game now? (y/n)"
                    if ($runNow -eq 'y' -or $runNow -eq 'Y') {
                        Write-Host "Starting DudeGolf..." -ForegroundColor Green
                        Start-Process $exePath.FullName
                    }
                } else {
                    Write-Host "Built successfully but couldn't find golf.exe" -ForegroundColor Yellow
                }
            } else {
                Write-Host ""
                Write-Host "✗ Build failed!" -ForegroundColor Red
                Write-Host ""
                Write-Host "You may need to install Visual Studio C++ Desktop Development workload" -ForegroundColor Yellow
                Write-Host ""
                Write-Host "To install:" -ForegroundColor Cyan
                Write-Host "  1. Open Visual Studio Installer" -ForegroundColor White
                Write-Host "  2. Click 'Modify' on Visual Studio 2022" -ForegroundColor White
                Write-Host "  3. Check 'Desktop development with C++'" -ForegroundColor White
                Write-Host "  4. Click 'Modify' to install" -ForegroundColor White
                Write-Host "  5. Run this script again after installation" -ForegroundColor White
            }
        } else {
            Write-Host "Could not find Developer Shell script" -ForegroundColor Red
        }
    } else {
        Write-Host "Visual Studio not found!" -ForegroundColor Red
    }
} else {
    Write-Host "Visual Studio not found!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Please install Visual Studio 2022 with C++ Desktop Development" -ForegroundColor Yellow
    Write-Host "Download from: https://visualstudio.microsoft.com/downloads/" -ForegroundColor Cyan
}

Write-Host ""
Write-Host "Press any key to exit..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
