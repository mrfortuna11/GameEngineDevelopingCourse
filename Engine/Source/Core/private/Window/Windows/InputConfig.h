#pragma once
#include <string>
#include <unordered_map>

namespace GameEngine::Core
{
    class Config
    {
    public:
        static void Load();
        static int GetKey(const std::string& action);

    private:
        static std::unordered_map<std::string, int> s_KeyBindings;
        static int KeyNameToVK(const std::string& keyName);
    };
}