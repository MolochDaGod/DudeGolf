/*-----------------------------------------------------------------------

RPG Progression System for Super Video Golf
Custom addition - 2025

This software is provided 'as-is', without any express or
implied warranty.In no event will the authors be held
liable for any damages arising from the use of this software.

-----------------------------------------------------------------------*/

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <crogine/core/String.hpp>

namespace rpg
{
    // Core player stats that affect golf gameplay
    struct PlayerStats final
    {
        // Base attributes (0-100 scale)
        float power = 50.0f;        // Affects max drive distance
        float accuracy = 50.0f;     // Reduces hook/slice and improves aim
        float spin = 50.0f;         // Affects ball spin control
        float putting = 50.0f;      // Improves putt accuracy and power control
        float recovery = 50.0f;     // Better performance from rough/bunkers
        
        // Calculated from level and equipment
        float getPowerMultiplier() const { return 1.0f + (power - 50.0f) * 0.01f; }
        float getAccuracyMultiplier() const { return 1.0f + (accuracy - 50.0f) * 0.01f; }
        float getSpinMultiplier() const { return 1.0f + (spin - 50.0f) * 0.01f; }
        float getPuttingMultiplier() const { return 1.0f + (putting - 50.0f) * 0.01f; }
        float getRecoveryMultiplier() const { return 1.0f + (recovery - 50.0f) * 0.01f; }
        
        // Maximum stat value
        static constexpr float MaxStat = 100.0f;
        static constexpr float MinStat = 10.0f;
    };

    // Equipment slot types
    enum class EquipmentSlot : std::uint8_t
    {
        Driver,
        Iron,
        Wedge,
        Putter,
        Gloves,
        Shoes,
        Count
    };

    // Equipment item definition
    struct Equipment final
    {
        std::uint32_t id = 0;
        cro::String name;
        cro::String description;
        EquipmentSlot slot = EquipmentSlot::Driver;
        std::uint32_t requiredLevel = 1;
        
        // Stat bonuses
        float powerBonus = 0.0f;
        float accuracyBonus = 0.0f;
        float spinBonus = 0.0f;
        float puttingBonus = 0.0f;
        float recoveryBonus = 0.0f;
        
        // Visual/cosmetic
        std::uint32_t modelID = 0;
        bool unlocked = false;
    };

    // Player progression data
    struct PlayerProgression final
    {
        std::string playerUID;  // Links to PlayerData::profileID
        
        // Experience and leveling
        std::uint32_t experience = 0;
        std::uint32_t level = 1;
        std::uint32_t skillPoints = 0;
        
        // Base stats (improved through leveling)
        PlayerStats baseStats;
        
        // Equipment slots
        std::array<std::uint32_t, static_cast<size_t>(EquipmentSlot::Count)> equippedItems = {0};
        std::vector<std::uint32_t> unlockedEquipment;
        
        // Achievements and milestones
        std::uint32_t holesPlayed = 0;
        std::uint32_t holesInOne = 0;
        std::uint32_t eagles = 0;
        std::uint32_t birdies = 0;
        std::uint32_t pars = 0;
        std::uint32_t tournamentsWon = 0;
        float longestDrive = 0.0f;
        float longestPutt = 0.0f;
        
        // Calculate total stats (base + equipment bonuses)
        PlayerStats calculateTotalStats(const std::vector<Equipment>& equipmentDatabase) const;
        
        // Experience required for next level (exponential curve)
        static std::uint32_t getExperienceForLevel(std::uint32_t level);
        
        // Add experience and handle leveling
        bool addExperience(std::uint32_t exp);
        
        // Level up - grants skill points
        void levelUp();
        
        // Apply skill point to a stat
        bool applySkillPoint(PlayerStats& stat, float& statValue);
    };

    // Experience rewards for various actions
    namespace XPRewards
    {
        constexpr std::uint32_t HoleInOne = 500;
        constexpr std::uint32_t Eagle = 150;
        constexpr std::uint32_t Birdie = 75;
        constexpr std::uint32_t Par = 25;
        constexpr std::uint32_t CompletedHole = 10;
        constexpr std::uint32_t CompletedRound = 100;
        constexpr std::uint32_t WonTournament = 500;
        constexpr std::uint32_t LongDrive = 50;  // 300+ yards
        constexpr std::uint32_t LongPutt = 50;   // 50+ feet
    }

    // Equipment database - predefined items
    class EquipmentDatabase final
    {
    public:
        EquipmentDatabase();
        
        const Equipment* getEquipment(std::uint32_t id) const;
        const std::vector<Equipment>& getAllEquipment() const { return m_equipment; }
        std::vector<const Equipment*> getEquipmentBySlot(EquipmentSlot slot) const;
        
    private:
        std::vector<Equipment> m_equipment;
        void initializeEquipment();
    };

    // Progression manager - handles loading/saving progression data
    class ProgressionManager final
    {
    public:
        ProgressionManager();
        
        // Load progression for a player profile
        bool loadProgression(const std::string& playerUID);
        
        // Save progression for current player
        bool saveProgression();
        
        // Get current player's progression
        PlayerProgression& getProgression() { return m_currentProgression; }
        const PlayerProgression& getProgression() const { return m_currentProgression; }
        
        // Award experience for gameplay actions
        void awardExperience(std::uint32_t amount, const std::string& reason);
        
        // Track statistics
        void recordHoleScore(std::uint8_t strokes, std::uint8_t par);
        void recordDriveDistance(float distance);
        void recordPuttDistance(float distance);
        
        // Equipment management
        bool unlockEquipment(std::uint32_t equipmentID);
        bool equipItem(EquipmentSlot slot, std::uint32_t equipmentID);
        
        // Get equipment database
        const EquipmentDatabase& getEquipmentDB() const { return m_equipmentDB; }
        
    private:
        PlayerProgression m_currentProgression;
        EquipmentDatabase m_equipmentDB;
        std::string m_databasePath;
        
        // Database operations
        bool initializeDatabase();
        bool createProgressionEntry(const std::string& playerUID);
    };
}
