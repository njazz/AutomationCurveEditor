#include "AutomationCurve.hpp"


namespace AutomationCurve {

std::string Codec::ToString(const CycleType& ct )
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

std::string Codec::ToString(const CurveValueRange& ct)
{
    std::string s;

    if (ct == CurveValueRange::Positive)
        s = "Positive";
    if (ct == CurveValueRange::Full)
        s = "Full";

    return s;
}

std::string Codec::ToString(const LockEdit& ct)
{
    std::string s;

    if (ct == LockEdit::None)
        s = "None";
    if (ct == LockEdit::LockX)
        s = "Lock X";
    if (ct == LockEdit::LockY)
        s = "Lock Y";
    if (ct == LockEdit::LockBoth)
        s = "Lock Both";

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

bool Codec::FromString(CurveValueRange& ct, const std::string& s)
{
    if (s.compare("Positive") == 0) {
        ct = CurveValueRange::Positive;
        return true;
    }
    if (s.compare("Full") == 0) {
        ct = CurveValueRange::Full;
        return true;
    }
    return false;
}

bool Codec::FromString(LockEdit& ct, const std::string& s)
{
    if (s.compare("None") == 0) {
        ct = LockEdit::None;
        return true;
    }
    if (s.compare("Lock X") == 0) {
        ct = LockEdit::LockX;
        return true;
    }
    if (s.compare("Lock Y") == 0) {
        ct = LockEdit::LockY;
        return true;
    }
    if (s.compare("Lock Both") == 0) {
        ct = LockEdit::LockBoth;
        return true;
    }
    return false;
}

}; // namespace
