// Dude Golf - RPG Golf Adventure Game
class DudeGolf {
    constructor() {
        this.canvas = document.getElementById('game-canvas');
        this.ctx = this.canvas.getContext('2d');
        
        // Game constants
        this.UPGRADE_COST = 50;
        this.UPGRADE_INCREMENT = 5;
        this.MAX_STAT_VALUE = 150;
        
        // Player RPG stats
        this.player = {
            level: 1,
            xp: 0,
            xpToNext: 100,
            power: 50,
            accuracy: 50
        };

        // Game state
        this.ball = { x: 100, y: 500, vx: 0, vy: 0, radius: 8, moving: false };
        this.hole = { x: 700, y: 100, radius: 20 };
        this.currentHole = 1;
        this.strokeCount = 0;
        this.holePar = 3;
        this.holes = this.generateHoles();
        
        // Career stats
        this.stats = {
            holesCompleted: 0,
            totalStrokes: 0,
            bestScore: null,
            holesInOne: 0,
            totalXP: 0
        };

        // Achievements system
        this.achievements = [
            { id: 'first_shot', title: '🏌️ First Swing', desc: 'Take your first shot', unlocked: false },
            { id: 'hole_in_one', title: '⭐ Hole in One!', desc: 'Complete a hole in one stroke', unlocked: false },
            { id: 'complete_9', title: '🏆 Nine Holes', desc: 'Complete all 9 holes', unlocked: false },
            { id: 'level_5', title: '📈 Level 5', desc: 'Reach level 5', unlocked: false },
            { id: 'level_10', title: '🌟 Level 10', desc: 'Reach level 10', unlocked: false },
            { id: 'eagle', title: '🦅 Eagle', desc: 'Score 2 under par', unlocked: false },
            { id: 'birdie_streak', title: '🐦 Birdie Master', desc: 'Get 3 birdies in a row', unlocked: false },
            { id: 'power_50', title: '💪 Strong Golfer', desc: 'Reach 50 power', unlocked: false },
            { id: 'power_100', title: '💥 Power Master', desc: 'Reach 100 power', unlocked: false },
            { id: 'accuracy_100', title: '🎯 Sharpshooter', desc: 'Reach 100 accuracy', unlocked: false }
        ];

        this.birdieStreak = 0;
        this.obstacles = [];
        this.wind = { x: 0, y: 0 };
        
        this.loadGame();
        this.init();
    }

    generateHoles() {
        return [
            { par: 3, start: { x: 100, y: 500 }, hole: { x: 700, y: 100 } },
            { par: 4, start: { x: 100, y: 300 }, hole: { x: 700, y: 500 } },
            { par: 3, start: { x: 100, y: 100 }, hole: { x: 700, y: 300 } },
            { par: 5, start: { x: 100, y: 500 }, hole: { x: 700, y: 50 } },
            { par: 3, start: { x: 100, y: 200 }, hole: { x: 700, y: 400 } },
            { par: 4, start: { x: 100, y: 450 }, hole: { x: 700, y: 150 } },
            { par: 3, start: { x: 100, y: 300 }, hole: { x: 700, y: 300 } },
            { par: 4, start: { x: 100, y: 100 }, hole: { x: 700, y: 500 } },
            { par: 5, start: { x: 100, y: 500 }, hole: { x: 700, y: 100 } }
        ];
    }

    init() {
        this.updateDisplay();
        this.renderAchievements();
        this.setupEventListeners();
        this.loadHole(this.currentHole);
        this.gameLoop();
    }

    setupEventListeners() {
        const powerSlider = document.getElementById('power-slider');
        const angleSlider = document.getElementById('angle-slider');
        const shootButton = document.getElementById('shoot-button');
        const resetButton = document.getElementById('reset-button');

        powerSlider.addEventListener('input', (e) => {
            document.getElementById('power-value').textContent = e.target.value;
        });

        angleSlider.addEventListener('input', (e) => {
            document.getElementById('angle-value').textContent = e.target.value;
        });

        shootButton.addEventListener('click', () => this.shoot());
        resetButton.addEventListener('click', () => this.resetHole());

        // Upgrade buttons
        document.querySelectorAll('.upgrade-btn').forEach(btn => {
            btn.addEventListener('click', (e) => {
                const upgrade = e.target.dataset.upgrade;
                this.purchaseUpgrade(upgrade);
            });
        });
    }

    loadHole(holeNumber) {
        const hole = this.holes[holeNumber - 1];
        this.ball = { 
            x: hole.start.x, 
            y: hole.start.y, 
            vx: 0, 
            vy: 0, 
            radius: 8, 
            moving: false 
        };
        this.hole = { x: hole.hole.x, y: hole.hole.y, radius: 20 };
        this.holePar = hole.par;
        this.strokeCount = 0;
        this.wind = {
            x: (Math.random() - 0.5) * 2,
            y: (Math.random() - 0.5) * 2
        };
        this.generateObstacles();
        this.updateDisplay();
    }

    generateObstacles() {
        this.obstacles = [];
        const numObstacles = Math.floor(Math.random() * 3) + 1;
        for (let i = 0; i < numObstacles; i++) {
            this.obstacles.push({
                x: Math.random() * 600 + 100,
                y: Math.random() * 400 + 100,
                width: Math.random() * 80 + 40,
                height: Math.random() * 80 + 40
            });
        }
    }

    shoot() {
        if (this.ball.moving) return;

        const power = parseInt(document.getElementById('power-slider').value);
        const angle = parseInt(document.getElementById('angle-slider').value);
        
        // Apply player stats to shot
        const powerMultiplier = 1 + (this.player.power / 100);
        const accuracyFactor = this.player.accuracy / 100;
        const spread = (1 - accuracyFactor) * 10; // Less accurate = more spread
        
        const angleRad = (angle + (Math.random() - 0.5) * spread) * Math.PI / 180;
        const velocity = (power / 100) * 15 * powerMultiplier;

        this.ball.vx = Math.cos(angleRad) * velocity;
        this.ball.vy = -Math.sin(angleRad) * velocity;
        this.ball.moving = true;
        this.strokeCount++;

        // Check first shot achievement
        if (!this.achievements.find(a => a.id === 'first_shot').unlocked) {
            this.unlockAchievement('first_shot');
        }

        this.updateDisplay();
    }

    resetHole() {
        this.loadHole(this.currentHole);
    }

    update() {
        if (!this.ball.moving) return;

        // Apply wind effect
        this.ball.vx += this.wind.x * 0.01;
        this.ball.vy += this.wind.y * 0.01;

        // Apply gravity and friction
        this.ball.vy += 0.3;
        this.ball.vx *= 0.99;
        this.ball.vy *= 0.99;

        // Update position
        this.ball.x += this.ball.vx;
        this.ball.y += this.ball.vy;

        // Collision with ground
        if (this.ball.y > this.canvas.height - this.ball.radius) {
            this.ball.y = this.canvas.height - this.ball.radius;
            this.ball.vy *= -0.6;
            this.ball.vx *= 0.8;
            if (Math.abs(this.ball.vy) < 0.5) {
                this.ball.vy = 0;
                this.ball.moving = false;
            }
        }

        // Collision with walls
        if (this.ball.x < this.ball.radius || this.ball.x > this.canvas.width - this.ball.radius) {
            this.ball.vx *= -0.8;
            this.ball.x = this.ball.x < this.ball.radius ? this.ball.radius : this.canvas.width - this.ball.radius;
        }

        // Check obstacles collision
        this.obstacles.forEach(obs => {
            if (this.ball.x > obs.x && this.ball.x < obs.x + obs.width &&
                this.ball.y > obs.y && this.ball.y < obs.y + obs.height) {
                this.ball.vx *= -0.7;
                this.ball.vy *= -0.7;
            }
        });

        // Check if ball is in hole
        const dist = Math.sqrt(
            Math.pow(this.ball.x - this.hole.x, 2) + 
            Math.pow(this.ball.y - this.hole.y, 2)
        );

        if (dist < this.hole.radius && Math.abs(this.ball.vx) < 2 && Math.abs(this.ball.vy) < 2) {
            this.ball.moving = false;
            this.holeCompleted();
        }

        // Stop if moving too slow
        if (Math.abs(this.ball.vx) < 0.1 && Math.abs(this.ball.vy) < 0.1 && this.ball.y >= this.canvas.height - this.ball.radius) {
            this.ball.moving = false;
            this.ball.vx = 0;
            this.ball.vy = 0;
        }
    }

    holeCompleted() {
        const score = this.strokeCount - this.holePar;
        let message = '';
        let xpGained = 20;

        if (this.strokeCount === 1) {
            message = '🌟 HOLE IN ONE! 🌟';
            xpGained = 100;
            this.stats.holesInOne++;
            this.unlockAchievement('hole_in_one');
            this.birdieStreak++;
        } else if (score <= -2) {
            message = '🦅 EAGLE! Amazing!';
            xpGained = 80;
            this.unlockAchievement('eagle');
            this.birdieStreak++;
        } else if (score === -1) {
            message = '🐦 BIRDIE! Great shot!';
            xpGained = 50;
            this.birdieStreak++;
        } else if (score === 0) {
            message = '✓ PAR! Well done!';
            xpGained = 30;
            this.birdieStreak = 0;
        } else {
            message = `Bogey! (${score > 0 ? '+' : ''}${score})`;
            xpGained = 10;
            this.birdieStreak = 0;
        }

        if (this.birdieStreak >= 3) {
            this.unlockAchievement('birdie_streak');
        }

        this.stats.holesCompleted++;
        this.stats.totalStrokes += this.strokeCount;
        
        if (this.stats.bestScore === null || this.strokeCount < this.stats.bestScore) {
            this.stats.bestScore = this.strokeCount;
        }

        this.gainXP(xpGained);

        // Show message and proceed
        this.showMessage(`${message}<br><br>Hole ${this.currentHole} completed in ${this.strokeCount} strokes!<br>+${xpGained} XP`, () => {
            if (this.currentHole < 9) {
                this.currentHole++;
                this.loadHole(this.currentHole);
            } else {
                this.unlockAchievement('complete_9');
                this.showMessage('🏆 Congratulations! You completed all 9 holes! 🏆<br><br>Starting a new round...', () => {
                    this.currentHole = 1;
                    this.loadHole(this.currentHole);
                });
            }
        });
    }

    gainXP(amount) {
        this.player.xp += amount;
        this.stats.totalXP += amount;

        while (this.player.xp >= this.player.xpToNext) {
            this.player.xp -= this.player.xpToNext;
            this.player.level++;
            this.player.xpToNext = Math.floor(this.player.xpToNext * 1.5);
            
            this.showMessage(`🎉 LEVEL UP! 🎉<br><br>You are now level ${this.player.level}!`, null, 2000);

            // Check level achievements
            if (this.player.level === 5) this.unlockAchievement('level_5');
            if (this.player.level === 10) this.unlockAchievement('level_10');
        }

        this.updateDisplay();
        this.saveGame();
    }

    purchaseUpgrade(type) {
        if (this.player.xp < this.UPGRADE_COST) {
            this.showMessage('Not enough XP!', null, 1500);
            return;
        }

        this.player.xp -= this.UPGRADE_COST;
        
        if (type === 'power') {
            this.player.power = Math.min(this.MAX_STAT_VALUE, this.player.power + this.UPGRADE_INCREMENT);
            if (this.player.power >= 50 && !this.achievements.find(a => a.id === 'power_50').unlocked) {
                this.unlockAchievement('power_50');
            }
            if (this.player.power >= 100 && !this.achievements.find(a => a.id === 'power_100').unlocked) {
                this.unlockAchievement('power_100');
            }
        } else if (type === 'accuracy') {
            this.player.accuracy = Math.min(this.MAX_STAT_VALUE, this.player.accuracy + this.UPGRADE_INCREMENT);
            if (this.player.accuracy >= 100 && !this.achievements.find(a => a.id === 'accuracy_100').unlocked) {
                this.unlockAchievement('accuracy_100');
            }
        }

        this.updateDisplay();
        this.saveGame();
    }

    unlockAchievement(id) {
        const achievement = this.achievements.find(a => a.id === id);
        if (achievement && !achievement.unlocked) {
            achievement.unlocked = true;
            this.renderAchievements();
            this.showMessage(`🏆 Achievement Unlocked!<br><br>${achievement.title}<br>${achievement.desc}`, null, 3000);
            this.saveGame();
        }
    }

    showMessage(html, callback = null, duration = null) {
        const overlay = document.getElementById('message-overlay');
        const content = document.getElementById('message-content');
        
        content.innerHTML = html;
        
        if (callback) {
            const btn = document.createElement('button');
            btn.className = 'btn-primary';
            btn.textContent = 'Continue';
            btn.onclick = () => {
                overlay.classList.add('hidden');
                callback();
            };
            content.appendChild(btn);
        }

        overlay.classList.remove('hidden');

        if (duration) {
            setTimeout(() => {
                overlay.classList.add('hidden');
                if (callback) callback();
            }, duration);
        }
    }

    render() {
        // Clear canvas
        this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);

        // Draw obstacles (sand traps)
        this.ctx.fillStyle = '#F4A460';
        this.obstacles.forEach(obs => {
            this.ctx.fillRect(obs.x, obs.y, obs.width, obs.height);
        });

        // Draw hole
        this.ctx.fillStyle = '#000';
        this.ctx.beginPath();
        this.ctx.arc(this.hole.x, this.hole.y, this.hole.radius, 0, Math.PI * 2);
        this.ctx.fill();

        // Draw flag
        this.ctx.strokeStyle = '#000';
        this.ctx.lineWidth = 2;
        this.ctx.beginPath();
        this.ctx.moveTo(this.hole.x, this.hole.y);
        this.ctx.lineTo(this.hole.x, this.hole.y - 40);
        this.ctx.stroke();

        this.ctx.fillStyle = '#FF0000';
        this.ctx.beginPath();
        this.ctx.moveTo(this.hole.x, this.hole.y - 40);
        this.ctx.lineTo(this.hole.x + 20, this.hole.y - 30);
        this.ctx.lineTo(this.hole.x, this.hole.y - 20);
        this.ctx.fill();

        // Draw ball
        this.ctx.fillStyle = '#FFF';
        this.ctx.strokeStyle = '#000';
        this.ctx.lineWidth = 2;
        this.ctx.beginPath();
        this.ctx.arc(this.ball.x, this.ball.y, this.ball.radius, 0, Math.PI * 2);
        this.ctx.fill();
        this.ctx.stroke();

        // Draw trajectory line when not moving
        if (!this.ball.moving) {
            const angle = parseInt(document.getElementById('angle-slider').value) * Math.PI / 180;
            const power = parseInt(document.getElementById('power-slider').value);
            const length = power * 2;

            this.ctx.strokeStyle = 'rgba(102, 126, 234, 0.5)';
            this.ctx.lineWidth = 3;
            this.ctx.setLineDash([5, 5]);
            this.ctx.beginPath();
            this.ctx.moveTo(this.ball.x, this.ball.y);
            this.ctx.lineTo(
                this.ball.x + Math.cos(angle) * length,
                this.ball.y - Math.sin(angle) * length
            );
            this.ctx.stroke();
            this.ctx.setLineDash([]);
        }
    }

    updateDisplay() {
        document.getElementById('player-level').textContent = this.player.level;
        document.getElementById('player-xp').textContent = this.player.xp;
        document.getElementById('player-xp-next').textContent = this.player.xpToNext;
        document.getElementById('player-power').textContent = this.player.power;
        document.getElementById('player-accuracy').textContent = this.player.accuracy;
        document.getElementById('current-hole').textContent = this.currentHole;
        document.getElementById('stroke-count').textContent = this.strokeCount;
        document.getElementById('hole-par').textContent = this.holePar;
        document.getElementById('holes-completed').textContent = this.stats.holesCompleted;
        document.getElementById('total-strokes').textContent = this.stats.totalStrokes;
        document.getElementById('best-score').textContent = this.stats.bestScore || '-';
        document.getElementById('holes-in-one').textContent = this.stats.holesInOne;

        // Update upgrade buttons
        document.querySelectorAll('.upgrade-btn').forEach(btn => {
            btn.disabled = this.player.xp < this.UPGRADE_COST;
        });

        // Update shoot button
        document.getElementById('shoot-button').disabled = this.ball.moving;
    }

    renderAchievements() {
        const container = document.getElementById('achievements-list');
        container.innerHTML = '';

        this.achievements.forEach(achievement => {
            const div = document.createElement('div');
            div.className = `achievement ${achievement.unlocked ? 'unlocked' : 'locked'}`;
            div.innerHTML = `
                <div class="achievement-title">${achievement.title}</div>
                <div class="achievement-desc">${achievement.desc}</div>
            `;
            container.appendChild(div);
        });
    }

    gameLoop() {
        this.update();
        this.render();
        requestAnimationFrame(() => this.gameLoop());
    }

    saveGame() {
        const saveData = {
            player: this.player,
            stats: this.stats,
            achievements: this.achievements,
            currentHole: this.currentHole
        };
        localStorage.setItem('dudeGolfSave', JSON.stringify(saveData));
    }

    loadGame() {
        const saveData = localStorage.getItem('dudeGolfSave');
        if (saveData) {
            try {
                const data = JSON.parse(saveData);
                this.player = data.player || this.player;
                this.stats = data.stats || this.stats;
                if (data.achievements) {
                    data.achievements.forEach((saved, index) => {
                        if (this.achievements[index]) {
                            this.achievements[index].unlocked = saved.unlocked;
                        }
                    });
                }
                this.currentHole = data.currentHole || 1;
            } catch (e) {
                console.log('Failed to load save data');
            }
        }
    }
}

// Initialize game when page loads
window.addEventListener('DOMContentLoaded', () => {
    new DudeGolf();
});
