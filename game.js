// Game constants
const GRAVITY = 0.5;
const FRICTION = 0.98;
const MIN_VELOCITY = 0.1;

// Game state
let gameState = {
    currentHole: 1,
    totalHoles: 9,
    strokes: 0,
    totalScore: 0,
    ball: null,
    hole: null,
    obstacles: [],
    isMoving: false,
    holes: []
};

// Canvas setup
const canvas = document.getElementById('game-canvas');
const ctx = canvas.getContext('2d');

// UI elements
const powerSlider = document.getElementById('power-slider');
const angleSlider = document.getElementById('angle-slider');
const powerValue = document.getElementById('power-value');
const angleValue = document.getElementById('angle-value');
const shootButton = document.getElementById('shoot-button');
const messageBox = document.getElementById('message-box');
const restartButton = document.getElementById('restart-button');

// Update slider displays
powerSlider.addEventListener('input', () => {
    powerValue.textContent = powerSlider.value;
});

angleSlider.addEventListener('input', () => {
    angleValue.textContent = angleSlider.value;
});

// Ball class
class Ball {
    constructor(x, y) {
        this.x = x;
        this.y = y;
        this.vx = 0;
        this.vy = 0;
        this.radius = 8;
    }

    update() {
        if (Math.abs(this.vx) < MIN_VELOCITY && Math.abs(this.vy) < MIN_VELOCITY) {
            this.vx = 0;
            this.vy = 0;
            return false;
        }

        this.x += this.vx;
        this.y += this.vy;
        this.vy += GRAVITY;
        this.vx *= FRICTION;
        this.vy *= FRICTION;

        // Bounce off walls
        if (this.x - this.radius < 0 || this.x + this.radius > canvas.width) {
            this.vx *= -0.8;
            this.x = Math.max(this.radius, Math.min(canvas.width - this.radius, this.x));
        }
        if (this.y - this.radius < 0 || this.y + this.radius > canvas.height) {
            this.vy *= -0.8;
            this.y = Math.max(this.radius, Math.min(canvas.height - this.radius, this.y));
        }

        // Check obstacles
        gameState.obstacles.forEach(obstacle => {
            if (this.checkCollision(obstacle)) {
                this.handleObstacleCollision(obstacle);
            }
        });

        return true;
    }

    checkCollision(obstacle) {
        return this.x + this.radius > obstacle.x &&
               this.x - this.radius < obstacle.x + obstacle.width &&
               this.y + this.radius > obstacle.y &&
               this.y - this.radius < obstacle.y + obstacle.height;
    }

    handleObstacleCollision(obstacle) {
        const centerX = this.x;
        const centerY = this.y;
        const obstacleLeft = obstacle.x;
        const obstacleRight = obstacle.x + obstacle.width;
        const obstacleTop = obstacle.y;
        const obstacleBottom = obstacle.y + obstacle.height;

        if (centerX < obstacleLeft) {
            this.x = obstacleLeft - this.radius;
            this.vx *= -0.8;
        } else if (centerX > obstacleRight) {
            this.x = obstacleRight + this.radius;
            this.vx *= -0.8;
        }

        if (centerY < obstacleTop) {
            this.y = obstacleTop - this.radius;
            this.vy *= -0.8;
        } else if (centerY > obstacleBottom) {
            this.y = obstacleBottom + this.radius;
            this.vy *= -0.8;
        }
    }

    draw() {
        ctx.fillStyle = 'white';
        ctx.beginPath();
        ctx.arc(this.x, this.y, this.radius, 0, Math.PI * 2);
        ctx.fill();
        ctx.strokeStyle = '#333';
        ctx.lineWidth = 2;
        ctx.stroke();
    }
}

// Hole class
class Hole {
    constructor(x, y) {
        this.x = x;
        this.y = y;
        this.radius = 15;
    }

    draw() {
        ctx.fillStyle = '#000';
        ctx.beginPath();
        ctx.arc(this.x, this.y, this.radius, 0, Math.PI * 2);
        ctx.fill();
        
        // Flag
        ctx.strokeStyle = '#fff';
        ctx.lineWidth = 2;
        ctx.beginPath();
        ctx.moveTo(this.x, this.y);
        ctx.lineTo(this.x, this.y - 40);
        ctx.stroke();
        
        ctx.fillStyle = '#ff0000';
        ctx.beginPath();
        ctx.moveTo(this.x, this.y - 40);
        ctx.lineTo(this.x + 20, this.y - 30);
        ctx.lineTo(this.x, this.y - 20);
        ctx.fill();
    }

    checkWin(ball) {
        const dx = ball.x - this.x;
        const dy = ball.y - this.y;
        const distance = Math.sqrt(dx * dx + dy * dy);
        return distance < this.radius;
    }
}

// Obstacle class
class Obstacle {
    constructor(x, y, width, height, type = 'sand') {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
        this.type = type;
    }

    draw() {
        if (this.type === 'sand') {
            ctx.fillStyle = '#DEB887';
        } else if (this.type === 'water') {
            ctx.fillStyle = '#4682B4';
        } else {
            ctx.fillStyle = '#8B4513';
        }
        ctx.fillRect(this.x, this.y, this.width, this.height);
        ctx.strokeStyle = '#333';
        ctx.lineWidth = 2;
        ctx.strokeRect(this.x, this.y, this.width, this.height);
    }
}

// Initialize holes
function initializeHoles() {
    gameState.holes = [
        {
            par: 3,
            ballStart: { x: 100, y: 300 },
            holePos: { x: 700, y: 300 },
            obstacles: []
        },
        {
            par: 4,
            ballStart: { x: 100, y: 500 },
            holePos: { x: 700, y: 100 },
            obstacles: [
                new Obstacle(300, 200, 200, 100, 'sand')
            ]
        },
        {
            par: 3,
            ballStart: { x: 400, y: 500 },
            holePos: { x: 400, y: 100 },
            obstacles: [
                new Obstacle(350, 300, 100, 50, 'water')
            ]
        },
        {
            par: 5,
            ballStart: { x: 100, y: 100 },
            holePos: { x: 700, y: 500 },
            obstacles: [
                new Obstacle(200, 200, 150, 80, 'sand'),
                new Obstacle(450, 350, 150, 80, 'sand')
            ]
        },
        {
            par: 4,
            ballStart: { x: 100, y: 300 },
            holePos: { x: 700, y: 300 },
            obstacles: [
                new Obstacle(350, 150, 100, 300, 'water')
            ]
        },
        {
            par: 3,
            ballStart: { x: 700, y: 500 },
            holePos: { x: 100, y: 100 },
            obstacles: [
                new Obstacle(300, 250, 100, 100, 'sand')
            ]
        },
        {
            par: 5,
            ballStart: { x: 400, y: 500 },
            holePos: { x: 400, y: 50 },
            obstacles: [
                new Obstacle(300, 250, 200, 80, 'sand'),
                new Obstacle(350, 400, 100, 50, 'water')
            ]
        },
        {
            par: 4,
            ballStart: { x: 100, y: 200 },
            holePos: { x: 700, y: 400 },
            obstacles: [
                new Obstacle(250, 100, 100, 400, 'water'),
                new Obstacle(450, 100, 100, 400, 'sand')
            ]
        },
        {
            par: 6,
            ballStart: { x: 100, y: 550 },
            holePos: { x: 700, y: 50 },
            obstacles: [
                new Obstacle(200, 400, 150, 80, 'sand'),
                new Obstacle(400, 250, 100, 100, 'water'),
                new Obstacle(550, 150, 100, 80, 'sand')
            ]
        }
    ];
}

// Load hole
function loadHole(holeNumber) {
    const holeData = gameState.holes[holeNumber - 1];
    gameState.ball = new Ball(holeData.ballStart.x, holeData.ballStart.y);
    gameState.hole = new Hole(holeData.holePos.x, holeData.holePos.y);
    gameState.obstacles = holeData.obstacles;
    gameState.strokes = 0;
    
    updateUI();
    showMessage(`Hole ${holeNumber} - Par ${holeData.par}`, 'info');
}

// Update UI
function updateUI() {
    document.getElementById('current-hole').textContent = gameState.currentHole;
    document.getElementById('total-holes').textContent = gameState.totalHoles;
    document.getElementById('par').textContent = gameState.holes[gameState.currentHole - 1].par;
    document.getElementById('strokes').textContent = gameState.strokes;
    document.getElementById('score').textContent = gameState.totalScore;
}

// Show message
function showMessage(text, type = 'info') {
    messageBox.textContent = text;
    messageBox.className = type;
}

// Shoot button handler
shootButton.addEventListener('click', () => {
    if (gameState.isMoving) return;
    
    const power = parseInt(powerSlider.value) / 10;
    const angle = parseInt(angleSlider.value) * Math.PI / 180;
    
    gameState.ball.vx = Math.cos(angle) * power;
    gameState.ball.vy = Math.sin(angle) * power;
    
    gameState.strokes++;
    gameState.isMoving = true;
    shootButton.disabled = true;
    
    updateUI();
    showMessage(`Stroke ${gameState.strokes}`, 'info');
});

// Game loop
function gameLoop() {
    // Clear canvas
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    
    // Draw background
    ctx.fillStyle = '#90EE90';
    ctx.fillRect(0, 0, canvas.width, canvas.height);
    
    // Draw obstacles
    gameState.obstacles.forEach(obstacle => obstacle.draw());
    
    // Draw hole
    gameState.hole.draw();
    
    // Update and draw ball
    if (gameState.isMoving) {
        const stillMoving = gameState.ball.update();
        if (!stillMoving) {
            gameState.isMoving = false;
            shootButton.disabled = false;
            checkWin();
        }
    }
    gameState.ball.draw();
    
    // Draw power/angle indicator
    if (!gameState.isMoving) {
        drawAimLine();
    }
    
    requestAnimationFrame(gameLoop);
}

// Draw aim line
function drawAimLine() {
    const power = parseInt(powerSlider.value) / 10;
    const angle = parseInt(angleSlider.value) * Math.PI / 180;
    
    const endX = gameState.ball.x + Math.cos(angle) * power * 10;
    const endY = gameState.ball.y + Math.sin(angle) * power * 10;
    
    ctx.strokeStyle = 'rgba(255, 0, 0, 0.5)';
    ctx.lineWidth = 3;
    ctx.beginPath();
    ctx.moveTo(gameState.ball.x, gameState.ball.y);
    ctx.lineTo(endX, endY);
    ctx.stroke();
    
    // Arrow head
    ctx.fillStyle = 'rgba(255, 0, 0, 0.5)';
    ctx.beginPath();
    ctx.moveTo(endX, endY);
    ctx.lineTo(endX - 10 * Math.cos(angle - Math.PI / 6), endY - 10 * Math.sin(angle - Math.PI / 6));
    ctx.lineTo(endX - 10 * Math.cos(angle + Math.PI / 6), endY - 10 * Math.sin(angle + Math.PI / 6));
    ctx.fill();
}

// Check win condition
function checkWin() {
    if (gameState.hole.checkWin(gameState.ball)) {
        const par = gameState.holes[gameState.currentHole - 1].par;
        const scoreDiff = gameState.strokes - par;
        let scoreText = '';
        
        if (scoreDiff <= -2) scoreText = 'Eagle! 🦅';
        else if (scoreDiff === -1) scoreText = 'Birdie! 🐦';
        else if (scoreDiff === 0) scoreText = 'Par! ⭐';
        else if (scoreDiff === 1) scoreText = 'Bogey 😐';
        else scoreText = 'Over Par 😞';
        
        gameState.totalScore += scoreDiff;
        showMessage(`Hole Complete! ${scoreText}`, 'success');
        
        setTimeout(() => {
            if (gameState.currentHole < gameState.totalHoles) {
                gameState.currentHole++;
                loadHole(gameState.currentHole);
            } else {
                endGame();
            }
        }, 2000);
    }
}

// End game
function endGame() {
    const gameOver = document.getElementById('game-over');
    const finalScore = document.getElementById('final-score');
    
    let rating = '';
    if (gameState.totalScore <= -5) rating = 'Incredible! 🌟';
    else if (gameState.totalScore <= 0) rating = 'Great job! 👏';
    else if (gameState.totalScore <= 10) rating = 'Good effort! 👍';
    else rating = 'Keep practicing! 💪';
    
    finalScore.textContent = `Final Score: ${gameState.totalScore > 0 ? '+' : ''}${gameState.totalScore} - ${rating}`;
    gameOver.style.display = 'block';
}

// Restart game
restartButton.addEventListener('click', () => {
    gameState.currentHole = 1;
    gameState.totalScore = 0;
    document.getElementById('game-over').style.display = 'none';
    loadHole(1);
});

// Initialize game
initializeHoles();
loadHole(1);
gameLoop();
