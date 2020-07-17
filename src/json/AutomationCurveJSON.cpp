// optional: JSON serialization

#include "AutomationCurve.hpp"
#include "AutomationCurveJSON.hpp"
#include "json.hpp"
using json = nlohmann::json;

namespace AutomationCurve {
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

void JSONCodec::ToJSONString(const ACurve& ct, std::string& s)
{

    json j;

    j["point_positions"] = ct._pointPositions;
    j["point_values"] = ct._pointValues;

    j["transition_types"] = ct._typenameOfTransitionToPoint;

    j["point_lock"] = _ToJSON(ct._pointLock);

    j["default_transition_type"] = ct._defaultTransitionType;

    j["cycle_left"] = Codec::ToString(ct._cycleLeft);
    j["cycle_right"] = Codec::ToString(ct._cycleRight);
    
    j["value_range"] = Codec::ToString(ct._valueRange);

    s = j.dump(4);
}

bool JSONCodec::FromJSONString(ACurve& ct, const std::string& s)
{

    try {
        auto j = json::parse(s);

        ct._pointPositions = j["point_positions"].get<std::vector<float> >();
        ct._pointValues = j["point_values"].get<std::vector<float> >();

        size_t idx = 0;
        auto transitionTypes = j["transition_types"];
        for (auto& e : transitionTypes) {
            ct.SetTransitionToPoint(idx, e);
            idx++;
        };

        _FromJSON(ct._pointLock, j["point_lock"]);

        ct._defaultTransitionType = j["default_transition_type"];

        Codec::FromString(ct._cycleLeft, j["cycle_left"]);
        Codec::FromString(ct._cycleRight, j["cycle_right"]);
        
        Codec::FromString(ct._valueRange, j["value_range"]);

    } catch (std::exception& e) {
        printf("%s\n", e.what());
        return false;
    }

    return true;
}
};
