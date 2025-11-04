/*-----------------------------------------------------------------------

UserManager.cpp - User Database Management Implementation

This is a basic implementation using file-based JSON storage.
For production, replace with SQLite or a proper backend.

-----------------------------------------------------------------------*/

#include "UserManager.hpp"

#include <crogine/core/FileSystem.hpp>
#include <crogine/core/Log.hpp>
#include <crogine/util/String.hpp>

#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <functional>

UserManager::UserManager()
{
    // Set database path in user content directory
    m_usersDatabasePath = cro::App::getPreferencePath() + "users/";
    
    if (!cro::FileSystem::directoryExists(m_usersDatabasePath))
    {
     cro::FileSystem::createDirectory(m_usersDatabasePath);
    }
    
    m_usersDatabasePath += "accounts.db";
    
    // Load existing users
    loadUsers();
}

UserManager::~UserManager()
{
    logoutUser();
    saveUsers();
}

bool UserManager::registerUser(const std::string& username, const std::string& email, const std::string& password)
{
    // Validation
  if (username.empty() || username.length() < 3)
    {
        LOG("Username must be at least 3 characters", cro::Logger::Type::Warning);
        return false;
    }
    
    if (email.empty() || email.find('@') == std::string::npos)
    {
    LOG("Invalid email address", cro::Logger::Type::Warning);
   return false;
    }
    
    if (password.empty() || password.length() < 6)
    {
        LOG("Password must be at least 6 characters", cro::Logger::Type::Warning);
 return false;
    }
    
    // Check if username/email already taken
    if (isUsernameTaken(username))
    {
     LOG("Username already taken", cro::Logger::Type::Warning);
   return false;
    }
    
 if (isEmailTaken(email))
    {
        LOG("Email already registered", cro::Logger::Type::Warning);
        return false;
    }
 
    // Create new user account
    UserAccount newUser;
    newUser.userID = generateUserID();
    newUser.username = username;
  newUser.email = email;
    newUser.passwordHash = hashPassword(password);
    newUser.createdAt = std::time(nullptr);
    
    m_users.push_back(newUser);
    saveUsers();
    
    LOG("User registered: " + username, cro::Logger::Type::Info);
    return true;
}

bool UserManager::loginUser(const std::string& username, const std::string& password)
{
    auto it = std::find_if(m_users.begin(), m_users.end(),
    [&username](const UserAccount& user)
    {
        return user.username == username;
     });
    
    if (it == m_users.end())
{
   LOG("User not found: " + username, cro::Logger::Type::Warning);
        return false;
    }
    
    if (!verifyPassword(password, it->passwordHash))
    {
      LOG("Invalid password for user: " + username, cro::Logger::Type::Warning);
        return false;
    }
    
    // Successful login
    it->lastLogin = std::time(nullptr);
    m_currentUser = &(*it);
    saveUsers();
    
  LOG("User logged in: " + username, cro::Logger::Type::Info);
    return true;
}

bool UserManager::isUsernameTaken(const std::string& username) const
{
    return std::find_if(m_users.begin(), m_users.end(),
        [&username](const UserAccount& user)
        {
  return user.username == username;
        }) != m_users.end();
}

bool UserManager::isEmailTaken(const std::string& email) const
{
    return std::find_if(m_users.begin(), m_users.end(),
    [&email](const UserAccount& user)
        {
          return user.email == email;
        }) != m_users.end();
}

UserAccount* UserManager::getCurrentUser()
{
    return m_currentUser;
}

const UserAccount* UserManager::getCurrentUser() const
{
    return m_currentUser;
}

bool UserManager::logoutUser()
{
    if (m_currentUser)
    {
        LOG("User logged out: " + m_currentUser->username, cro::Logger::Type::Info);
        m_currentUser = nullptr;
      return true;
    }
    return false;
}

bool UserManager::loadUsers()
{
    // TODO: Implement proper file parsing (JSON or binary)
// For now, this is a stub
    if (cro::FileSystem::fileExists(m_usersDatabasePath))
    {
        LOG("Loading users from: " + m_usersDatabasePath, cro::Logger::Type::Info);
     // Parse file and populate m_users
    }
  return true;
}

bool UserManager::saveUsers()
{
    // TODO: Implement proper file serialization (JSON or binary)
  // For now, this is a stub
    LOG("Saving " + std::to_string(m_users.size()) + " users", cro::Logger::Type::Info);
    return true;
}

std::string UserManager::hashPassword(const std::string& password) const
{
    // TODO: Implement proper password hashing (bcrypt, argon2, etc.)
    // For now, use a simple hash (DO NOT USE IN PRODUCTION)
    std::hash<std::string> hasher;
 auto hash = hasher(password + "salt");
    return std::to_string(hash);
}

bool UserManager::verifyPassword(const std::string& password, const std::string& hash) const
{
    // TODO: Implement proper password verification
    return hashPassword(password) == hash;
}

std::uint32_t UserManager::generateUserID()
{
    if (m_users.empty())
    {
        return 1;
    }
    return m_users.back().userID + 1;
}
