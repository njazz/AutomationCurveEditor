#include "imgui.h"
#include "FVector.hpp"
#include "AutomationCurve.hpp"

struct FVectorViewSettingsCommon {
    ImVec2 size = ImVec2(400,300);
    ImVec2 viewRange = ImVec2(0,1);

    bool replacePointTime; // for ACurve
};

struct BarsViewSettings : public FVectorViewSettingsCommon {
};

struct SlicesViewSettings : public FVectorViewSettingsCommon {
    enum class NamesPosition{
        None,
        Top,
        Bottom
    };
    
    NamesPosition namePosition;
};

// ---

namespace AutomationCurve {
// new api style
bool ImWidgetBars(const std::string& name, FVector& vec, const BarsViewSettings& settings = BarsViewSettings());
bool ImWidgetSlices(const std::string& name, FVector& vec, const SlicesViewSettings& settings = SlicesViewSettings());

// draw curve values. replace time or equal spaced

bool ImWidgetBars(const std::string& name, ACurve& vec, const BarsViewSettings& settings = BarsViewSettings());
bool ImWidgetSlices(const std::string& name, ACurve& vec, const SlicesViewSettings& settings = SlicesViewSettings());

};
