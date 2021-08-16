#include "extern/codegen/include/GlobalNamespace/GameplayModifiers.hpp"
#include "extern/codegen/include/GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp"

#include "extern/beatsaber-hook/shared/utils/utils.h"
#include "extern/beatsaber-hook/shared/utils/logging.hpp"
#include "extern/modloader/shared/modloader.hpp"
#include "extern/beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "extern/beatsaber-hook/shared/utils/typedefs.h"

#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/Object.hpp"

#include "extern/bs-utils/shared/utils.hpp"

#include "include/main.hpp"
#include "include/config.hpp"
#include "include/SettingsViewController.hpp"

#include "extern/beatsaber-hook/shared/utils/hooking.hpp"
#include "extern/custom-types/shared/register.hpp"
#include "extern/questui/shared/QuestUI.hpp"
#include "extern/questui/shared/BeatSaberUI.hpp"


using namespace GlobalNamespace;
using namespace UnityEngine;

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

MAKE_HOOK_MATCH(StandardLevelScenesTransitionSetupDataSO_Init, &StandardLevelScenesTransitionSetupDataSO::Init, void, GlobalNamespace::StandardLevelScenesTransitionSetupDataSO* self, Il2CppString* gameMode, GlobalNamespace::IDifficultyBeatmap* difficultyBeatmap, GlobalNamespace::IPreviewBeatmapLevel* previewBeatmapLevel, GlobalNamespace::OverrideEnvironmentSettings* overrideEnvironmentSettings, GlobalNamespace::ColorScheme* overrideColorScheme, GlobalNamespace::GameplayModifiers* gameplayModifiers, GlobalNamespace::PlayerSpecificSettings* playerSpecificSettings, GlobalNamespace::PracticeSettings* practiceSettings, Il2CppString* backButtonText, bool useTestNoteCutSoundEffects)
{
    if (config.foscEnabled)
    {
        if(bs_utils::Submission::getEnabled())
        {
            getLogger().info("Disabling score submission because a modifier is enabled and submission was previously enabled");
            bs_utils::Submission::disable(modInfo);
        }
    } 
    else
    {
        if(!bs_utils::Submission::getEnabled())
        {
            getLogger().info("Enabling score submission because no modifiers are enabled and submission was previously disabled");
            bs_utils::Submission::enable(modInfo);
        }
        
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
        gameplayModifiers->get_strictAngles(),
        gameplayModifiers->get_disappearingArrows(),
        gameplayModifiers->get_songSpeed(),
        gameplayModifiers->get_noArrows(),
        gameplayModifiers->get_ghostNotes(),
        gameplayModifiers->get_proMode(),
        gameplayModifiers->get_zenMode(),
        gameplayModifiers->get_smallCubes());

    StandardLevelScenesTransitionSetupDataSO_Init(self, gameMode, difficultyBeatmap, previewBeatmapLevel, overrideEnvironmentSettings, overrideColorScheme, mods, playerSpecificSettings, practiceSettings, backButtonText, useTestNoteCutSoundEffects);
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

    INSTALL_HOOK(getLogger(), StandardLevelScenesTransitionSetupDataSO_Init);

    getLogger().info("Installed all hooks!");

      custom_types::Register::AutoRegister();
      QuestUI::Register::RegisterModSettingsViewController(modInfo, DidActivate);
}