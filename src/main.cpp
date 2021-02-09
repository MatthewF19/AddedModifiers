#include "extern/codegen/include/GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp"

#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/typedefs.h"

#include "UnityEngine/SceneManagement/Scene.hpp"

#include "bs-utils/shared/utils.hpp"

#include "main.hpp"
#include "config.hpp"
#include "SettingsViewController.hpp"

#include "custom-types/shared/register.hpp"
#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"

using namespace GlobalNamespace;

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

std::string sceneLoadedName = "";
std::string gameCore = "GameCore";

extern config_t config;

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

MAKE_HOOK_OFFSETLESS(StandardLevelScenesTransitionSetupDataSO_Init, void, GlobalNamespace::StandardLevelScenesTransitionSetupDataSO* self, Il2CppString* gameMode, Il2CppObject* difficultyBeatmap, Il2CppObject* overrideEnvironmentSettings, Il2CppObject* overrideColorScheme, GlobalNamespace::GameplayModifiers* gameplayModifiers, GlobalNamespace::PlayerSpecificSettings* playerSpecificSettings, Il2CppObject* practiceSettings, Il2CppString* backButtonText, bool useTestNoteCutSoundEffects)
{
    if (config.foscEnabled || config.saEnabled)
    {
        getLogger().info("Disabling score submission because either modifier is enabled");
        bs_utils::Submission::disable(modInfo);
    } 
    else 
    {
        getLogger().info("Enabling score submission because both modifiers are disabled");
        bs_utils::Submission::enable(modInfo);
    }

    GameplayModifiers* mods = GameplayModifiers::New_ctor(
        gameplayModifiers->get_demoNoFail(),
        gameplayModifiers->get_demoNoObstacles(),
        gameplayModifiers->get_energyType(),
        gameplayModifiers->get_noFailOn0Energy(),
        gameplayModifiers->get_instaFail(),
        config.foscEnabled,
        gameplayModifiers->get_enabledObstacleType(),
        gameplayModifiers->get_noBombs(),
        gameplayModifiers->get_fastNotes(),
        config.saEnabled,
        gameplayModifiers->get_disappearingArrows(),
        gameplayModifiers->get_songSpeed(),
        gameplayModifiers->get_noArrows(),
        gameplayModifiers->get_ghostNotes());

    StandardLevelScenesTransitionSetupDataSO_Init(self, gameMode, difficultyBeatmap, overrideEnvironmentSettings, overrideColorScheme, mods, playerSpecificSettings, practiceSettings, backButtonText, useTestNoteCutSoundEffects);
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load(); // Load the config file
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {

    if (!LoadConfig()) 
            SaveConfig();

    il2cpp_functions::Init();
    QuestUI::Init();
    getLogger().info("Installing hooks...");

    INSTALL_HOOK_OFFSETLESS(getLogger(), StandardLevelScenesTransitionSetupDataSO_Init, il2cpp_utils::FindMethodUnsafe("", "StandardLevelScenesTransitionSetupDataSO", "Init", 9));
    
    getLogger().info("Installed all hooks!");

    custom_types::Register::RegisterType<AddedModifiers::SettingsViewController>();

    QuestUI::Register::RegisterModSettingsViewController<AddedModifiers::SettingsViewController*>((ModInfo){"Added Modifiers", VERSION}, "Added Modifiers");
}