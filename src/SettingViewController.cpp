/*
#include "SettingsViewController.hpp"
#include "config.hpp"

#include "extern/beatsaber-hook/shared/utils/hooking.hpp"

#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/UI/Toggle_ToggleEvent.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/ModalView.hpp"
#include "HMUI/Touchable.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

*/

#include "SettingsViewController.hpp"
#include "config.hpp"

#include "HMUI/Touchable.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/UI/Toggle_ToggleEvent.hpp"

#include "questui/shared/BeatSaberUI.hpp"

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;
using namespace GlobalNamespace;

extern config_t config; 

void DidActivate(ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if(firstActivation) {
        self->get_gameObject()->AddComponent<Touchable*>();

        GameObject* container = BeatSaberUI::CreateScrollableSettingsContainer(self->get_transform());
        Transform* parent = container->get_transform();

        BeatSaberUI::CreateToggle(parent, "Enable Fail On Saber Clash", config.foscEnabled, [](bool value) -> void { 
                config.foscEnabled = value;
                SaveConfig();
            }); 

    }
}