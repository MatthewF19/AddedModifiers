#pragma once

#include "HMUI/ViewController.hpp"

#include "extern/custom-types/shared/macros.hpp"
#include "extern/beatsaber-hook/shared/utils/hooking.hpp"

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);