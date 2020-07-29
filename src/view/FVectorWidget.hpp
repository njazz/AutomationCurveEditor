#include "imgui.h"
#include "FVector.hpp"
#include "ACurve.hpp"

struct FVectorViewSettingsCommon{
    ImVec2 size;
    ImVec2 viewRange;
};

struct BarsViewSettings : public FVectorViewSettingsCommon{

};

struct SlicesViewSettings : public FVectorViewSettingsCommon{

};

// ---

namespace AutomationCurve {
    // new api style
  bool ImWidgetBars(const std::string& name, const ImVec2& size, FVector& vec, const ImVec2& viewRange);
  bool ImWidgetSlices(const std::string& name, const ImVec2& size, FVector& vec, const ImVec2& viewRange);
  
  bool ImWidgetBars(const std::string& name, FVector& vec, const BarsViewSettings& settings = BarsViewSettings());
  bool ImWidgetSlices(const std::string& name, FVector& vec, const SlicesViewSettings& settings = SlicesViewSettings());
  
  // draw curve values. replace time or equal
  bool ImWidgetBars(const std::string& name, const ImVec2& size, ACurve& vec, const ImVec2& viewRange);
  bool ImWidgetSlices(const std::string& name, const ImVec2& size, ACurve& vec, const ImVec2& viewRange);
  
  bool ImWidgetBars(const std::string& name, ACurve& vec, const BarsViewSettings& settings = BarsViewSettings());
  bool ImWidgetSlices(const std::string& name, ACurve& vec, const SlicesViewSettings& settings = SlicesViewSettings());
};
