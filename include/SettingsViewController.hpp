#include "HMUI/ViewController.hpp"

#include "extern/custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(AddedModifiers, SettingsViewController, HMUI::ViewController,
    // Override DidActivate, which is called whenever you enter the menu
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    REGISTER_FUNCTION(SettingsViewController,
        REGISTER_METHOD(DidActivate);
    )
)