/*-----------------------------------------------------------------------

LoginState.cpp - User Authentication Implementation

-----------------------------------------------------------------------*/

#include "LoginState.hpp"
#include "SharedStateData.hpp"
#include "MessageIDs.hpp"
#include "MenuConsts.hpp"

#include <crogine/core/App.hpp>
#include <crogine/core/Clock.hpp>
#include <crogine/graphics/RenderTarget.hpp>
#include <crogine/gui/Gui.hpp>
#include <crogine/util/String.hpp>

#include <iostream>
#include <regex>

LoginState::LoginState(cro::StateStack& stack, cro::State::Context context, SharedStateData& sharedData)
    : cro::State(stack, context),
      m_sharedData(sharedData)
{
    // Initial mode is main menu
    m_mode = LoginMode::Main;
}

bool LoginState::handleEvent(const cro::Event& evt)
{
    switch (evt.type)
    {
    default: break;
    case SDL_KEYUP:
        switch (evt.key.keysym.sym)
     {
        default: break;
        case SDLK_ESCAPE:
   if (m_mode != LoginMode::Main)
            {
    switchMode(LoginMode::Main);
            }
            break;
        case SDLK_RETURN:
      if (m_mode == LoginMode::Login)
            {
       validateLogin();
    }
       else if (m_mode == LoginMode::Register)
        {
       validateRegistration();
      }
            break;
        }
        break;
    }
    
    return true;
}

void LoginState::handleMessage(const cro::Message& msg)
{
    // Handle authentication responses from UserManager
}

void LoginState::simulate(float dt)
{
    if (m_isProcessing)
    {
  m_loadingTime += dt;
        
    // Simulate authentication delay (remove in production with real server)
  if (m_loadingTime > 1.0f)
    {
     m_isProcessing = false;
     m_loadingTime = 0.f;
            
     // TODO: Verify credentials with UserManager
 // For now, allow any login to proceed
   if (!m_username.empty())
      {
         // Successfully logged in - return to normal game flow
      requestStackPop();
            }
   else
    {
              m_errorMessage = "Invalid credentials";
            }
        }
    }
}

void LoginState::render()
{
    auto& rt = getContext().renderTarget;
    rt.clear(cro::Colour::Black);
    
  // Begin ImGui frame
    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize(glm::vec2(rt.getSize()));
 ImGui::Begin("##LoginWindow", nullptr, 
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
  
    const auto windowSize = ImGui::GetWindowSize();
ImGui::SetCursorPosX((windowSize.x - 300.f) * 0.5f);
    ImGui::SetCursorPosY((windowSize.y - 200.f) * 0.3f);
    
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 10.f, 8.f });
    
    // Title
    ImGui::PushFont(m_sharedData.helpFonts[0]);
ImGui::TextUnformatted("DudeGolf - Login");
    ImGui::PopFont();
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    switch (m_mode)
    {
    case LoginMode::Main:
  drawMainMenu();
 break;
    case LoginMode::Login:
  drawLoginForm();
        break;
    case LoginMode::Register:
        drawRegisterForm();
    break;
    case LoginMode::Loading:
     ImGui::TextUnformatted("Authenticating...");
        ImGui::ProgressBar(m_loadingTime / 1.0f);
        break;
    }
    
    ImGui::PopStyleVar(2);
    ImGui::End();
}

void LoginState::drawMainMenu()
{
    ImGui::Spacing();
  
    ImGui::PushItemWidth(300.f);
    if (ImGui::Button("Login", ImVec2(300.f, 40.f)))
    {
        switchMode(LoginMode::Login);
    }
    
    ImGui::Spacing();
    
    if (ImGui::Button("Create Account", ImVec2(300.f, 40.f)))
    {
  switchMode(LoginMode::Register);
    }
    
    ImGui::Spacing();
    ImGui::Spacing();
    
    if (ImGui::Button("Play Offline", ImVec2(300.f, 40.f)))
    {
        // Skip authentication, create default profile
        requestStackPop();
    }
    
    ImGui::PopItemWidth();
    
    ImGui::Spacing();
    ImGui::TextWrapped("Welcome to DudeGolf! Log in or create an account to get started.");
}

void LoginState::drawLoginForm()
{
    ImGui::PushItemWidth(300.f);
    
    ImGui::TextUnformatted("Username:");
    ImGui::InputText("##username", &m_username);
    
    ImGui::TextUnformatted("Password:");
    ImGui::InputText("##password", &m_password, ImGuiInputTextFlags_Password);
    
    ImGui::Spacing();
    
    if (ImGui::Button("Login", ImVec2(300.f, 40.f)))
    {
        validateLogin();
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(150.f, 40.f)))
    {
        switchMode(LoginMode::Main);
    }
    
    // Error message display
    if (!m_errorMessage.empty())
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 0.f, 0.f, 1.f));
   ImGui::TextWrapped("Error: %s", m_errorMessage.c_str());
        ImGui::PopStyleColor();
    }
    
    ImGui::PopItemWidth();
}

void LoginState::drawRegisterForm()
{
    ImGui::PushItemWidth(300.f);
    
    ImGui::TextUnformatted("Username:");
    ImGui::InputText("##reg_username", &m_username);
    
  ImGui::TextUnformatted("Email:");
  ImGui::InputText("##email", &m_email);
    
    ImGui::TextUnformatted("Password:");
    ImGui::InputText("##reg_password", &m_password, ImGuiInputTextFlags_Password);
    
    ImGui::TextUnformatted("Confirm Password:");
    ImGui::InputText("##confirm_password", &m_confirmPassword, ImGuiInputTextFlags_Password);
    
    ImGui::Spacing();
    
    if (ImGui::Button("Create Account", ImVec2(300.f, 40.f)))
    {
   validateRegistration();
 }
  
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(150.f, 40.f)))
    {
        switchMode(LoginMode::Main);
    }
 
    // Error message display
    if (!m_errorMessage.empty())
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 0.f, 0.f, 1.f));
        ImGui::TextWrapped("Error: %s", m_errorMessage.c_str());
        ImGui::PopStyleColor();
    }
    
    ImGui::PopItemWidth();
}

void LoginState::validateLogin()
{
    m_errorMessage.clear();
    
    // Basic validation
    if (m_username.empty())
    {
m_errorMessage = "Username is required";
     return;
    }
    
    if (m_password.empty())
    {
        m_errorMessage = "Password is required";
      return;
    }
    
    // TODO: Verify credentials with backend/database
    // For now, just accept non-empty credentials
    m_isProcessing = true;
    m_mode = LoginMode::Loading;
}

void LoginState::validateRegistration()
{
  m_errorMessage.clear();
    
    // Validation checks
    if (m_username.empty() || m_username.length() < 3)
    {
        m_errorMessage = "Username must be at least 3 characters";
  return;
    }
    
    if (m_email.empty() || !std::regex_match(m_email, std::regex("^[^@]+@[^@]+\\.[^@]+$")))
    {
   m_errorMessage = "Please enter a valid email";
  return;
    }
    
    if (m_password.empty() || m_password.length() < 6)
    {
 m_errorMessage = "Password must be at least 6 characters";
   return;
    }
    
    if (m_password != m_confirmPassword)
    {
        m_errorMessage = "Passwords do not match";
   return;
    }
    
    // TODO: Create account with backend/database
    m_isProcessing = true;
    m_mode = LoginMode::Loading;
}

void LoginState::switchMode(LoginMode newMode)
{
    m_mode = newMode;
    m_username.clear();
    m_password.clear();
    m_email.clear();
    m_confirmPassword.clear();
    m_errorMessage.clear();
    m_isProcessing = false;
}
