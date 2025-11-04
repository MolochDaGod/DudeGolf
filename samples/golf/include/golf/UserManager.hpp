/*-----------------------------------------------------------------------

UserManager.hpp - User Database Management

Simple local user management system using file-based storage.
Can be extended to use SQLite or connect to a remote server.

-----------------------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <cstdint>

struct UserAccount
{
    std::uint32_t userID = 0;
    std::string username;
    std::string email;
    std::string passwordHash;  // TODO: Implement proper hashing
    std::int64_t createdAt = 0;
    std::int64_t lastLogin = 0;
    
    bool operator==(const UserAccount& other) const
    {
   return userID == other.userID;
    }
};

class UserManager
{
public:
    UserManager();
    ~UserManager();
    
    // Authentication
    bool registerUser(const std::string& username, const std::string& email, const std::string& password);
 bool loginUser(const std::string& username, const std::string& password);
    bool isUsernameTaken(const std::string& username) const;
 bool isEmailTaken(const std::string& email) const;
    
    // User management
    UserAccount* getCurrentUser();
    const UserAccount* getCurrentUser() const;
    bool logoutUser();
    
    // File I/O
    bool loadUsers();
    bool saveUsers();
    
private:
std::vector<UserAccount> m_users;
    UserAccount* m_currentUser = nullptr;
    std::string m_usersDatabasePath;
    
    std::string hashPassword(const std::string& password) const;
    bool verifyPassword(const std::string& password, const std::string& hash) const;
 std::uint32_t generateUserID();
};
