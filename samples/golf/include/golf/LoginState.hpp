/*-----------------------------------------------------------------------

LoginState.hpp - User Authentication State

This state handles user login and registration before accessing the game.
It integrates with the existing state system and player profile system.

-----------------------------------------------------------------------*/

#pragma once

#include <crogine/core/State.hpp>
#include <crogine/graphics/Drawable.hpp>
#include <crogine/graphics/Font.hpp>
#include <crogine/gui/Gui.hpp>

#include <string>

class SharedStateData;

enum class LoginMode
{
    Main,      // Login or Register choice
    Login,     // Login form
    Register,  // Registration form
    Loading    // Validating credentials
};

class LoginState final : public cro::State
{
public:
    explicit LoginState(cro::StateStack& stack, cro::State::Context context, SharedStateData& sharedData);
    
  cro::StateID getStateID() const override { return StateID::Login; }

    bool handleEvent(const cro::Event& evt) override;
    void handleMessage(const cro::Message& msg) override;
    void simulate(float dt) override;
    void render() override;

private:
    SharedStateData& m_sharedData;
    LoginMode m_mode = LoginMode::Main;
    
    std::string m_username;
    std::string m_password;
    std::string m_email;
    std::string m_confirmPassword;
    std::string m_errorMessage;
    
    float m_loadingTime = 0.f;
bool m_isProcessing = false;
    
    // UI State
    bool m_usernameActive = false;
    bool m_passwordActive = false;
 bool m_emailActive = false;
    bool m_confirmPasswordActive = false;
    
    void drawLoginForm();
  void drawRegisterForm();
    void drawMainMenu();
    void validateLogin();
    void validateRegistration();
    void switchMode(LoginMode newMode);
};
