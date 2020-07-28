// optional: JSON serialization

#include "AutomationCurve.hpp"
#include "AutomationCurveJSON.hpp"
#include "json.hpp"
using json = nlohmann::json;

namespace AutomationCurve {
namespace JSONCodec{
    // ---

template <typename T>
json _ToJSON(const std::vector<T>& vec)
{
    json ret = json::array();

    for (const auto& e : vec) {
        ret.push_back(Codec::ToString(e));
    }

    return ret;
}

template <typename T>
bool _FromJSON(std::vector<T>& ret, const json& j)
{
    for (const auto& e : j.array()) {
        T v;
        auto b = Codec::FromString(v, e);
        if (!b)
            return false;
        ret.push_back(v);
    }
    return true;
}

void ToJSONString(const ACurve& ct, std::string& s)
{

    json j;

    j["point_positions"] = ct.RawPositions();//_pointPositions;
    j["point_values"] = ct.RawValues();//_pointValues;

    j["transition_types"] = ct.RawTransitionTypenames();//_typenameOfTransitionToPoint;

    j["point_lock"] = _ToJSON(ct.RawPointLocks());

    j["default_transition_type"] = ct.DefaultTransitionType();

    j["cycle_left"] = Codec::ToString(ct.CycleLeft());
    j["cycle_right"] = Codec::ToString(ct.CycleRight());
    
    j["value_range"] = Codec::ToString(ct.RawValueRange());

    s = j.dump(4);
}

bool FromJSONString(ACurve& ct, const std::string& s)
{

    try {
        auto j = json::parse(s);

        auto pos = j["point_positions"].get<std::vector<float> >();     // ct._pointPositions
        auto val = j["point_values"].get<std::vector<float> >();        // ct._pointValues
        
        std::vector<LockEdit> locks;                                   // ct._pointLock
        _FromJSON(locks, j["point_lock"]);
        
        ct.SetRawPoints(pos, val, locks);
        
        size_t idx = 0;
        auto transitionTypes = j["transition_types"];
        for (auto& e : transitionTypes) {
            ct.SetTransitionToPoint(idx, e);
            idx++;
        };

        //
        
        auto defType = j["default_transition_type"];
        ct.SetDefaultTransitionType(defType);

        CycleType ctl;
        CycleType ctr;
        CurveValueRange vr;
        
        Codec::FromString(ctl, j["cycle_left"]);
        Codec::FromString(ctr, j["cycle_right"]);
        
        Codec::FromString(vr, j["value_range"]);
        
        ct.SetCycleLeft(ctl);
        ct.SetCycleRight(ctr);
        ct.SetRawValueRange(vr);

    } catch (std::exception& e) {
        printf("%s\n", e.what());
        return false;
    }

    return true;
}

} // namespace JSONCodec
} // namespace AutomationCurve
