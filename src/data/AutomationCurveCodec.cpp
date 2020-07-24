#include "AutomationCurve.hpp"


namespace AutomationCurve {
namespace Codec{


std::string ToString(const CurveValueRange& ct)
{
    std::string s;

    if (ct == CurveValueRange::Positive)
        s = "Positive";
    if (ct == CurveValueRange::Full)
        s = "Full";

    return s;
}

std::string ToString(const LockEdit& ct)
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



bool FromString(CurveValueRange& ct, const std::string& s)
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

bool FromString(LockEdit& ct, const std::string& s)
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

}
}; // namespace
