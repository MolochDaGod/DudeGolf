/*-----------------------------------------------------------------------

RPG Progression System Implementation
Custom addition - 2025

-----------------------------------------------------------------------*/

#include "Progression.hpp"

#include <crogine/core/FileSystem.hpp>
#include <crogine/core/Log.hpp>
#include <crogine/util/Constants.hpp>

#include <sqlite3.h>
#include <cmath>
#include <algorithm>

using namespace rpg;

namespace
{
    const std::string ProgressionDBName = "progression.db";
}

// PlayerProgression Implementation
PlayerStats PlayerProgression::calculateTotalStats(const std::vector<Equipment>& equipmentDatabase) const
{
    PlayerStats totalStats = baseStats;
    
    // Add equipment bonuses
    for (size_t i = 0; i < equippedItems.size(); ++i)
    {
        std::uint32_t itemID = equippedItems[i];
        if (itemID > 0)
        {
            auto it = std::find_if(equipmentDatabase.begin(), equipmentDatabase.end(),
                [itemID](const Equipment& eq) { return eq.id == itemID; });
            
            if (it != equipmentDatabase.end())
            {
                totalStats.power += it->powerBonus;
                totalStats.accuracy += it->accuracyBonus;
                totalStats.spin += it->spinBonus;
                totalStats.putting += it->puttingBonus;
                totalStats.recovery += it->recoveryBonus;
            }
        }
    }
    
    // Clamp stats to valid range
    totalStats.power = std::clamp(totalStats.power, PlayerStats::MinStat, PlayerStats::MaxStat);
    totalStats.accuracy = std::clamp(totalStats.accuracy, PlayerStats::MinStat, PlayerStats::MaxStat);
    totalStats.spin = std::clamp(totalStats.spin, PlayerStats::MinStat, PlayerStats::MaxStat);
    totalStats.putting = std::clamp(totalStats.putting, PlayerStats::MinStat, PlayerStats::MaxStat);
    totalStats.recovery = std::clamp(totalStats.recovery, PlayerStats::MinStat, PlayerStats::MaxStat);
    
    return totalStats;
}

std::uint32_t PlayerProgression::getExperienceForLevel(std::uint32_t level)
{
    // Exponential curve: 100 * (level^1.5)
    return static_cast<std::uint32_t>(100.0f * std::pow(static_cast<float>(level), 1.5f));
}

bool PlayerProgression::addExperience(std::uint32_t exp)
{
    experience += exp;
    
    std::uint32_t requiredExp = getExperienceForLevel(level + 1);
    if (experience >= requiredExp)
    {
        levelUp();
        return true; // Leveled up
    }
    
    return false;
}

void PlayerProgression::levelUp()
{
    level++;
    skillPoints += 3; // Grant 3 skill points per level
    
    // Small automatic stat boost on level up
    baseStats.power += 0.5f;
    baseStats.accuracy += 0.5f;
    baseStats.spin += 0.5f;
    baseStats.putting += 0.5f;
    baseStats.recovery += 0.5f;
    
    cro::Logger::log("Player leveled up to level " + std::to_string(level), cro::Logger::Type::Info);
}

bool PlayerProgression::applySkillPoint(PlayerStats& stat, float& statValue)
{
    if (skillPoints > 0 && statValue < PlayerStats::MaxStat)
    {
        statValue = std::min(statValue + 2.0f, PlayerStats::MaxStat);
        skillPoints--;
        return true;
    }
    return false;
}

// EquipmentDatabase Implementation
EquipmentDatabase::EquipmentDatabase()
{
    initializeEquipment();
}

void EquipmentDatabase::initializeEquipment()
{
    // Drivers
    m_equipment.push_back({1, "Starter Driver", "Basic driver for beginners", EquipmentSlot::Driver, 1, 0, 0, 0, 0, 0, 0, true});
    m_equipment.push_back({2, "Pro Driver", "Increased power and accuracy", EquipmentSlot::Driver, 5, 5, 2, 0, 0, 0, 0, false});
    m_equipment.push_back({3, "Elite Driver", "Maximum distance driver", EquipmentSlot::Driver, 10, 10, 3, 0, 0, 0, 0, false});
    m_equipment.push_back({4, "Legend Driver", "Perfect balance of power and control", EquipmentSlot::Driver, 20, 12, 8, 0, 0, 0, 0, false});
    
    // Irons
    m_equipment.push_back({11, "Starter Irons", "Basic iron set", EquipmentSlot::Iron, 1, 0, 0, 0, 0, 0, 0, true});
    m_equipment.push_back({12, "Forged Irons", "Better accuracy", EquipmentSlot::Iron, 5, 2, 5, 0, 0, 0, 0, false});
    m_equipment.push_back({13, "Tour Irons", "Professional grade accuracy", EquipmentSlot::Iron, 10, 3, 10, 2, 0, 0, 0, false});
    m_equipment.push_back({14, "Champion Irons", "Ultimate precision", EquipmentSlot::Iron, 20, 5, 15, 5, 0, 0, 0, false});
    
    // Wedges
    m_equipment.push_back({21, "Standard Wedge", "Basic short game club", EquipmentSlot::Wedge, 1, 0, 0, 0, 0, 0, 0, true});
    m_equipment.push_back({22, "Spin Wedge", "Enhanced spin control", EquipmentSlot::Wedge, 5, 0, 2, 8, 0, 5, 0, false});
    m_equipment.push_back({23, "Precision Wedge", "Maximum spin and accuracy", EquipmentSlot::Wedge, 15, 0, 5, 12, 0, 8, 0, false});
    
    // Putters
    m_equipment.push_back({31, "Basic Putter", "Standard putter", EquipmentSlot::Putter, 1, 0, 0, 0, 0, 0, 0, true});
    m_equipment.push_back({32, "Balanced Putter", "Improved putting feel", EquipmentSlot::Putter, 5, 0, 3, 0, 8, 0, 0, false});
    m_equipment.push_back({33, "Pro Putter", "Professional putting", EquipmentSlot::Putter, 12, 0, 5, 0, 12, 0, 0, false});
    m_equipment.push_back({34, "Master Putter", "Perfect putting control", EquipmentSlot::Putter, 25, 0, 8, 0, 18, 0, 0, false});
    
    // Gloves
    m_equipment.push_back({41, "Basic Gloves", "Standard gloves", EquipmentSlot::Gloves, 1, 0, 0, 0, 0, 0, 0, true});
    m_equipment.push_back({42, "Grip Gloves", "Better club control", EquipmentSlot::Gloves, 7, 2, 5, 3, 2, 0, 0, false});
    m_equipment.push_back({43, "Pro Gloves", "Enhanced feel and control", EquipmentSlot::Gloves, 15, 3, 8, 5, 4, 0, 0, false});
    
    // Shoes
    m_equipment.push_back({51, "Standard Shoes", "Basic golf shoes", EquipmentSlot::Shoes, 1, 0, 0, 0, 0, 0, 0, true});
    m_equipment.push_back({52, "Stable Shoes", "Improved stance stability", EquipmentSlot::Shoes, 8, 3, 3, 0, 3, 5, 0, false});
    m_equipment.push_back({53, "Tour Shoes", "Professional stability", EquipmentSlot::Shoes, 18, 5, 5, 0, 5, 8, 0, false});
}

const Equipment* EquipmentDatabase::getEquipment(std::uint32_t id) const
{
    auto it = std::find_if(m_equipment.begin(), m_equipment.end(),
        [id](const Equipment& eq) { return eq.id == id; });
    
    return (it != m_equipment.end()) ? &(*it) : nullptr;
}

std::vector<const Equipment*> EquipmentDatabase::getEquipmentBySlot(EquipmentSlot slot) const
{
    std::vector<const Equipment*> result;
    for (const auto& eq : m_equipment)
    {
        if (eq.slot == slot)
        {
            result.push_back(&eq);
        }
    }
    return result;
}

// ProgressionManager Implementation
ProgressionManager::ProgressionManager()
{
    m_databasePath = cro::FileSystem::getConfigDirectory(cro::App::getPreferencePath()) + ProgressionDBName;
    initializeDatabase();
}

bool ProgressionManager::initializeDatabase()
{
    sqlite3* db = nullptr;
    int result = sqlite3_open(m_databasePath.c_str(), &db);
    
    if (result != SQLITE_OK)
    {
        cro::Logger::log("Failed to open progression database: " + std::string(sqlite3_errmsg(db)), cro::Logger::Type::Error);
        if (db) sqlite3_close(db);
        return false;
    }
    
    // Create progression table
    const char* createTable = R"(
        CREATE TABLE IF NOT EXISTS player_progression (
            player_uid TEXT PRIMARY KEY,
            experience INTEGER DEFAULT 0,
            level INTEGER DEFAULT 1,
            skill_points INTEGER DEFAULT 0,
            power REAL DEFAULT 50.0,
            accuracy REAL DEFAULT 50.0,
            spin REAL DEFAULT 50.0,
            putting REAL DEFAULT 50.0,
            recovery REAL DEFAULT 50.0,
            equipped_driver INTEGER DEFAULT 1,
            equipped_iron INTEGER DEFAULT 11,
            equipped_wedge INTEGER DEFAULT 21,
            equipped_putter INTEGER DEFAULT 31,
            equipped_gloves INTEGER DEFAULT 41,
            equipped_shoes INTEGER DEFAULT 51,
            holes_played INTEGER DEFAULT 0,
            holes_in_one INTEGER DEFAULT 0,
            eagles INTEGER DEFAULT 0,
            birdies INTEGER DEFAULT 0,
            pars INTEGER DEFAULT 0,
            tournaments_won INTEGER DEFAULT 0,
            longest_drive REAL DEFAULT 0.0,
            longest_putt REAL DEFAULT 0.0
        );
    )";
    
    char* errorMsg = nullptr;
    result = sqlite3_exec(db, createTable, nullptr, nullptr, &errorMsg);
    
    if (result != SQLITE_OK)
    {
        cro::Logger::log("Failed to create progression table: " + std::string(errorMsg), cro::Logger::Type::Error);
        sqlite3_free(errorMsg);
        sqlite3_close(db);
        return false;
    }
    
    // Create unlocked equipment table
    const char* createUnlockedTable = R"(
        CREATE TABLE IF NOT EXISTS unlocked_equipment (
            player_uid TEXT,
            equipment_id INTEGER,
            unlocked_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            PRIMARY KEY (player_uid, equipment_id),
            FOREIGN KEY (player_uid) REFERENCES player_progression(player_uid)
        );
    )";
    
    result = sqlite3_exec(db, createUnlockedTable, nullptr, nullptr, &errorMsg);
    
    if (result != SQLITE_OK)
    {
        cro::Logger::log("Failed to create unlocked equipment table: " + std::string(errorMsg), cro::Logger::Type::Error);
        sqlite3_free(errorMsg);
    }
    
    sqlite3_close(db);
    return true;
}

bool ProgressionManager::loadProgression(const std::string& playerUID)
{
    sqlite3* db = nullptr;
    int result = sqlite3_open(m_databasePath.c_str(), &db);
    
    if (result != SQLITE_OK)
    {
        if (db) sqlite3_close(db);
        return false;
    }
    
    // Check if player exists
    const char* query = "SELECT * FROM player_progression WHERE player_uid = ?;";
    sqlite3_stmt* stmt = nullptr;
    
    result = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        sqlite3_close(db);
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, playerUID.c_str(), -1, SQLITE_TRANSIENT);
    
    result = sqlite3_step(stmt);
    
    if (result == SQLITE_ROW)
    {
        // Load existing progression
        m_currentProgression.playerUID = playerUID;
        m_currentProgression.experience = sqlite3_column_int(stmt, 1);
        m_currentProgression.level = sqlite3_column_int(stmt, 2);
        m_currentProgression.skillPoints = sqlite3_column_int(stmt, 3);
        m_currentProgression.baseStats.power = static_cast<float>(sqlite3_column_double(stmt, 4));
        m_currentProgression.baseStats.accuracy = static_cast<float>(sqlite3_column_double(stmt, 5));
        m_currentProgression.baseStats.spin = static_cast<float>(sqlite3_column_double(stmt, 6));
        m_currentProgression.baseStats.putting = static_cast<float>(sqlite3_column_double(stmt, 7));
        m_currentProgression.baseStats.recovery = static_cast<float>(sqlite3_column_double(stmt, 8));
        
        m_currentProgression.equippedItems[0] = sqlite3_column_int(stmt, 9);
        m_currentProgression.equippedItems[1] = sqlite3_column_int(stmt, 10);
        m_currentProgression.equippedItems[2] = sqlite3_column_int(stmt, 11);
        m_currentProgression.equippedItems[3] = sqlite3_column_int(stmt, 12);
        m_currentProgression.equippedItems[4] = sqlite3_column_int(stmt, 13);
        m_currentProgression.equippedItems[5] = sqlite3_column_int(stmt, 14);
        
        m_currentProgression.holesPlayed = sqlite3_column_int(stmt, 15);
        m_currentProgression.holesInOne = sqlite3_column_int(stmt, 16);
        m_currentProgression.eagles = sqlite3_column_int(stmt, 17);
        m_currentProgression.birdies = sqlite3_column_int(stmt, 18);
        m_currentProgression.pars = sqlite3_column_int(stmt, 19);
        m_currentProgression.tournamentsWon = sqlite3_column_int(stmt, 20);
        m_currentProgression.longestDrive = static_cast<float>(sqlite3_column_double(stmt, 21));
        m_currentProgression.longestPutt = static_cast<float>(sqlite3_column_double(stmt, 22));
        
        sqlite3_finalize(stmt);
        
        // Load unlocked equipment
        const char* unlockQuery = "SELECT equipment_id FROM unlocked_equipment WHERE player_uid = ?;";
        result = sqlite3_prepare_v2(db, unlockQuery, -1, &stmt, nullptr);
        if (result == SQLITE_OK)
        {
            sqlite3_bind_text(stmt, 1, playerUID.c_str(), -1, SQLITE_TRANSIENT);
            
            m_currentProgression.unlockedEquipment.clear();
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                m_currentProgression.unlockedEquipment.push_back(sqlite3_column_int(stmt, 0));
            }
            sqlite3_finalize(stmt);
        }
        
        sqlite3_close(db);
        return true;
    }
    else
    {
        // Create new progression entry
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return createProgressionEntry(playerUID);
    }
}

bool ProgressionManager::createProgressionEntry(const std::string& playerUID)
{
    sqlite3* db = nullptr;
    int result = sqlite3_open(m_databasePath.c_str(), &db);
    
    if (result != SQLITE_OK)
    {
        if (db) sqlite3_close(db);
        return false;
    }
    
    const char* insertQuery = "INSERT INTO player_progression (player_uid) VALUES (?);";
    sqlite3_stmt* stmt = nullptr;
    
    result = sqlite3_prepare_v2(db, insertQuery, -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        sqlite3_close(db);
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, playerUID.c_str(), -1, SQLITE_TRANSIENT);
    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    // Unlock starter equipment
    for (const auto& eq : m_equipmentDB.getAllEquipment())
    {
        if (eq.requiredLevel == 1)
        {
            const char* unlockQuery = "INSERT INTO unlocked_equipment (player_uid, equipment_id) VALUES (?, ?);";
            result = sqlite3_prepare_v2(db, unlockQuery, -1, &stmt, nullptr);
            if (result == SQLITE_OK)
            {
                sqlite3_bind_text(stmt, 1, playerUID.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(stmt, 2, eq.id);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
        }
    }
    
    sqlite3_close(db);
    
    // Load the newly created progression
    return loadProgression(playerUID);
}

bool ProgressionManager::saveProgression()
{
    sqlite3* db = nullptr;
    int result = sqlite3_open(m_databasePath.c_str(), &db);
    
    if (result != SQLITE_OK)
    {
        if (db) sqlite3_close(db);
        return false;
    }
    
    const char* updateQuery = R"(
        UPDATE player_progression SET
            experience = ?, level = ?, skill_points = ?,
            power = ?, accuracy = ?, spin = ?, putting = ?, recovery = ?,
            equipped_driver = ?, equipped_iron = ?, equipped_wedge = ?,
            equipped_putter = ?, equipped_gloves = ?, equipped_shoes = ?,
            holes_played = ?, holes_in_one = ?, eagles = ?, birdies = ?, pars = ?,
            tournaments_won = ?, longest_drive = ?, longest_putt = ?
        WHERE player_uid = ?;
    )";
    
    sqlite3_stmt* stmt = nullptr;
    result = sqlite3_prepare_v2(db, updateQuery, -1, &stmt, nullptr);
    
    if (result != SQLITE_OK)
    {
        sqlite3_close(db);
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, m_currentProgression.experience);
    sqlite3_bind_int(stmt, 2, m_currentProgression.level);
    sqlite3_bind_int(stmt, 3, m_currentProgression.skillPoints);
    sqlite3_bind_double(stmt, 4, m_currentProgression.baseStats.power);
    sqlite3_bind_double(stmt, 5, m_currentProgression.baseStats.accuracy);
    sqlite3_bind_double(stmt, 6, m_currentProgression.baseStats.spin);
    sqlite3_bind_double(stmt, 7, m_currentProgression.baseStats.putting);
    sqlite3_bind_double(stmt, 8, m_currentProgression.baseStats.recovery);
    sqlite3_bind_int(stmt, 9, m_currentProgression.equippedItems[0]);
    sqlite3_bind_int(stmt, 10, m_currentProgression.equippedItems[1]);
    sqlite3_bind_int(stmt, 11, m_currentProgression.equippedItems[2]);
    sqlite3_bind_int(stmt, 12, m_currentProgression.equippedItems[3]);
    sqlite3_bind_int(stmt, 13, m_currentProgression.equippedItems[4]);
    sqlite3_bind_int(stmt, 14, m_currentProgression.equippedItems[5]);
    sqlite3_bind_int(stmt, 15, m_currentProgression.holesPlayed);
    sqlite3_bind_int(stmt, 16, m_currentProgression.holesInOne);
    sqlite3_bind_int(stmt, 17, m_currentProgression.eagles);
    sqlite3_bind_int(stmt, 18, m_currentProgression.birdies);
    sqlite3_bind_int(stmt, 19, m_currentProgression.pars);
    sqlite3_bind_int(stmt, 20, m_currentProgression.tournamentsWon);
    sqlite3_bind_double(stmt, 21, m_currentProgression.longestDrive);
    sqlite3_bind_double(stmt, 22, m_currentProgression.longestPutt);
    sqlite3_bind_text(stmt, 23, m_currentProgression.playerUID.c_str(), -1, SQLITE_TRANSIENT);
    
    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return result == SQLITE_DONE;
}

void ProgressionManager::awardExperience(std::uint32_t amount, const std::string& reason)
{
    bool leveledUp = m_currentProgression.addExperience(amount);
    
    cro::Logger::log("Awarded " + std::to_string(amount) + " XP: " + reason, cro::Logger::Type::Info);
    
    if (leveledUp)
    {
        // Check for newly unlockable equipment
        for (const auto& eq : m_equipmentDB.getAllEquipment())
        {
            if (eq.requiredLevel == m_currentProgression.level && !eq.unlocked)
            {
                unlockEquipment(eq.id);
            }
        }
        
        saveProgression();
    }
}

void ProgressionManager::recordHoleScore(std::uint8_t strokes, std::uint8_t par)
{
    m_currentProgression.holesPlayed++;
    
    std::int32_t diff = static_cast<std::int32_t>(strokes) - static_cast<std::int32_t>(par);
    
    if (diff == -3)
    {
        m_currentProgression.holesInOne++; // Or albatross
        awardExperience(XPRewards::HoleInOne, "Hole in One!");
    }
    else if (diff == -2)
    {
        m_currentProgression.eagles++;
        awardExperience(XPRewards::Eagle, "Eagle!");
    }
    else if (diff == -1)
    {
        m_currentProgression.birdies++;
        awardExperience(XPRewards::Birdie, "Birdie!");
    }
    else if (diff == 0)
    {
        m_currentProgression.pars++;
        awardExperience(XPRewards::Par, "Par");
    }
    
    awardExperience(XPRewards::CompletedHole, "Completed hole");
}

void ProgressionManager::recordDriveDistance(float distance)
{
    if (distance > m_currentProgression.longestDrive)
    {
        m_currentProgression.longestDrive = distance;
        
        if (distance >= 300.0f)
        {
            awardExperience(XPRewards::LongDrive, "Long drive: " + std::to_string(static_cast<int>(distance)) + " yards");
        }
    }
}

void ProgressionManager::recordPuttDistance(float distance)
{
    if (distance > m_currentProgression.longestPutt)
    {
        m_currentProgression.longestPutt = distance;
        
        if (distance >= 50.0f)
        {
            awardExperience(XPRewards::LongPutt, "Long putt: " + std::to_string(static_cast<int>(distance)) + " feet");
        }
    }
}

bool ProgressionManager::unlockEquipment(std::uint32_t equipmentID)
{
    const Equipment* eq = m_equipmentDB.getEquipment(equipmentID);
    if (!eq || eq->requiredLevel > m_currentProgression.level)
    {
        return false;
    }
    
    // Check if already unlocked
    auto it = std::find(m_currentProgression.unlockedEquipment.begin(),
                        m_currentProgression.unlockedEquipment.end(),
                        equipmentID);
    
    if (it != m_currentProgression.unlockedEquipment.end())
    {
        return false; // Already unlocked
    }
    
    m_currentProgression.unlockedEquipment.push_back(equipmentID);
    
    // Save to database
    sqlite3* db = nullptr;
    int result = sqlite3_open(m_databasePath.c_str(), &db);
    
    if (result == SQLITE_OK)
    {
        const char* insertQuery = "INSERT INTO unlocked_equipment (player_uid, equipment_id) VALUES (?, ?);";
        sqlite3_stmt* stmt = nullptr;
        
        result = sqlite3_prepare_v2(db, insertQuery, -1, &stmt, nullptr);
        if (result == SQLITE_OK)
        {
            sqlite3_bind_text(stmt, 1, m_currentProgression.playerUID.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 2, equipmentID);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        
        sqlite3_close(db);
    }
    
    cro::Logger::log("Unlocked equipment: " + eq->name.toAnsiString(), cro::Logger::Type::Info);
    return true;
}

bool ProgressionManager::equipItem(EquipmentSlot slot, std::uint32_t equipmentID)
{
    // Check if equipment is unlocked
    auto it = std::find(m_currentProgression.unlockedEquipment.begin(),
                        m_currentProgression.unlockedEquipment.end(),
                        equipmentID);
    
    if (it == m_currentProgression.unlockedEquipment.end())
    {
        return false;
    }
    
    const Equipment* eq = m_equipmentDB.getEquipment(equipmentID);
    if (!eq || eq->slot != slot)
    {
        return false;
    }
    
    m_currentProgression.equippedItems[static_cast<size_t>(slot)] = equipmentID;
    saveProgression();
    
    return true;
}
