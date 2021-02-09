#include "../extern/beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "../extern/beatsaber-hook/shared/config/config-utils.hpp"
#include "modloader/shared/modloader.hpp"

struct config_t {
    bool foscEnabled = false;
    bool saEnabled = false;
};

Configuration& getConfig();
void SaveConfig(); 
bool LoadConfig();