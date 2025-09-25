#include "InputConfig.h"
#include <INIReader.h>
#include <Windows.h>
#include <cctype>

using namespace GameEngine::Core;

std::unordered_map<std::string, int> Config::s_KeyBindings;

int Config::KeyNameToVK(const std::string& keyName)
{
    if (keyName.size() == 1 && std::isalpha(keyName[0]))
    {
       SHORT vk = VkKeyScanA(static_cast<char>(toupper(keyName[0])));
       return static_cast<unsigned char>(vk & 0xFF);
    }
    else if (keyName == "Space") return VK_SPACE;
    else if (keyName == "Shift") return VK_SHIFT;
    return 0;
}

void Config::Load()
{
    s_KeyBindings.clear();

    INIReader reader("config.ini");

    if (reader.ParseError() != 0)
    {

        return;
    }

    auto section = "KeyBindings";
    for (auto const& kv : reader.GetFields(section))
    {
        std::string action = kv;
        std::string keyName = reader.Get(section, action, "");
        s_KeyBindings[action] = KeyNameToVK(keyName);
    }
}

int Config::GetKey(const std::string& action)
{
    auto it = s_KeyBindings.find(action);
    if (it != s_KeyBindings.end())
        return it->second;
    return 0;
}
