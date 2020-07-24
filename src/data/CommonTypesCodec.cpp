
namespace AutomationCurve {
namespace Codec {

    std::string ToString(const CycleType& ct)
    {
        std::string s;

        if (ct == CycleType::Hold)
            s = "Hold";
        if (ct == CycleType::Zero)
            s = "Zero";
        if (ct == CycleType::Mirror)
            s = "Mirror";
        if (ct == CycleType::Repeat)
            s = "Repeat";

        return s;
    }

    // ---

    bool Codec::FromString(CycleType& ct, const std::string& s)
    {
        if (s.compare("Hold") == 0) {
            ct = CycleType::Hold;
            return true;
        }
        if (s.compare("Zero") == 0) {
            ct = CycleType::Zero;
            return true;
        }
        if (s.compare("Mirror") == 0) {
            ct = CycleType::Mirror;
            return true;
        }
        if (s.compare("Repeat") == 0) {
            ct = CycleType::Repeat;
            return true;
        }

        return false;
    }
}
}
