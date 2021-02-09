//MASSIVE thank you to red and everyone in #quest-mod-dev for helping me through the ui

#include "config.hpp" 
#include "main.hpp"

extern Logger& getLogger();
extern ModInfo modInfo;
config_t config;

void SaveConfig()
{
    getConfig().config.RemoveAllMembers();
    getConfig().config.SetObject();
    rapidjson::Document::AllocatorType& allocator = getConfig().config.GetAllocator();
    getConfig().config.AddMember("Enable Fail On Saber Clash", config.foscEnabled, allocator);
    getConfig().config.AddMember("Enable Strict Angles", config.saEnabled, allocator);
    getConfig().Write();
}

bool LoadConfig()
{
    
    getLogger().info("Loading Configuration...");
    getConfig().Load();
    bool foundEverything = true;

    if(getConfig().config.HasMember("Enable Fail On Saber Clash") && getConfig().config["Enable Fail On Saber Clash"].IsBool())
    {
        config.foscEnabled = getConfig().config["Enable Fail On Saber Clash"].GetBool();    
    }
    else
    {
        foundEverything = false;
    }
    if(getConfig().config.HasMember("Enable Strict Angles") && getConfig().config["Enable Strict Angles"].IsBool())
    {
        config.saEnabled = getConfig().config["Enable Strict Angles"].GetBool();    
    }
    else
    {
        foundEverything = false;
    }

    if (foundEverything) 
    {
        getLogger().info("Configuration Loaded Succesfully!");
    }

    return foundEverything;
}