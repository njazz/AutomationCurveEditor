//
//  CurveEditor_Tests.m
//  CurveEditor-Tests
//
//  Created by Alex on 29/07/2020.
//

#import <XCTest/XCTest.h>

#include "AutomationCurve.hpp"
#include "CommonTypes.hpp"
#include "FVector.hpp"
#include "Sequence.hpp"

//#include "AutomationCurveWidget.hpp"

@interface CurveEditor_Tests : XCTestCase

@end

@implementation CurveEditor_Tests

-(void) testAutomationCurve {
    AutomationCurve::ACurve ac;
    
    // add
    ac.AddPoint(.3, 1);
    XCTAssertEqual(ac.Size() , 3);
    XCTAssertEqual(ac.TimeAt(1),.3f);
    XCTAssertEqual(ac.ValueAt(1),1.0f);
    
    // remove
    ac.RemovePointAt(1);
    XCTAssertEqual(ac.Size(),2);
    
    // cycle settings
    ac.SetCycleLeft(AutomationCurve::CycleType::Repeat);
    ac.SetCycleRight(AutomationCurve::CycleType::Repeat);
    XCTAssertEqual(ac.CycleLeft(), AutomationCurve::CycleType::Repeat);
    XCTAssertEqual(ac.CycleRight(), AutomationCurve::CycleType::Repeat);
    
    // locks
    ac.AddPoint(.3, 1);
    ac.SetPointLock(1, AutomationCurve::LockEdit::LockY);
    XCTAssertEqual(ac.LockAt(1), AutomationCurve::LockEdit::LockY);
    ac.SetPointLock(1,AutomationCurve::LockEdit::None);
    
    // scale
    ac.SetTimeOffset(1);
    ac.SetTimeScale(10);
    XCTAssertEqual(ac.TimeOffset(), 1.0f);
    XCTAssertEqual(ac.TimeScale(), 10.0f);
    
    ac.SetValueConverters([](const float& f){ return f/50.0f;}, [](const float& f){ return f*50.0f;});
    
    ac.SetScaledTime(1, 5);
    ac.SetScaledValue(1, 15);
    
    XCTAssertEqual(ac.ScaledTimeAt(1), 5.0f);
    XCTAssertEqualWithAccuracy(ac.ScaledValueAt(1), 15.0f,0.0001f);
    
    // raw points
    ac.SetRawPoints({.3,.5,.7}, {.1,.2,.3}, {AutomationCurve::LockEdit::LockX,AutomationCurve::LockEdit::LockY, AutomationCurve::LockEdit::None});
    XCTAssertEqual(ac.RawPoints()[0].second,.1f);
    XCTAssertEqual(ac.RawPoints()[1].second,.2f);
    XCTAssertEqual(ac.RawPoints()[2].second,.3f);
    
    XCTAssertEqual(ac.RawPoints()[0].first,.3f);
    XCTAssertEqual(ac.RawPoints()[1].first,.5f);
    XCTAssertEqual(ac.RawPoints()[2].first,.7f);
    
    XCTAssertEqual(ac.RawPointLocks()[0],AutomationCurve::LockEdit::LockX);
    XCTAssertEqual(ac.RawPointLocks()[1],AutomationCurve::LockEdit::LockY);
    XCTAssertEqual(ac.RawPointLocks()[2],AutomationCurve::LockEdit::None);
    
    XCTAssertEqual(ac.RawValues()[0],.1f);
    XCTAssertEqual(ac.RawValues()[1],.2f);
    XCTAssertEqual(ac.RawValues()[2],.3f);
    
    XCTAssertEqual(ac.RawPositions()[0],.3f);
    XCTAssertEqual(ac.RawPositions()[1],.5f);
    XCTAssertEqual(ac.RawPositions()[2],.7f);
}

-(void) testPixelConverters{
    AutomationCurve::WidgetCoordinateConverter conv;
    
    conv.widgetWidth = 500;
    
    // 1..4
    conv.timeOffset = 1;
    conv.timeScale = 3;
    
    // 1.5..3.5
    conv.curveTimeOffset = 1.5;
    conv.curveTimeScale = 2;
    
    float temp = 0;
    temp = conv.FractionCurveToGlobal(0.5f);
    XCTAssertEqual(.5f,conv.FractionGlobalToCurve(temp));
    
    temp = conv.PixelToCurveFraction(100.0f);
    XCTAssertEqualWithAccuracy(100.0f,conv.CurveFractionToPixel(temp),0.001);
    
    temp = conv.SecondsToPixel(1.0f);
    XCTAssertEqual(1.0f,conv.PixelToSeconds(temp));
    
    temp = conv.CurveFractionToSeconds(.25f);
    XCTAssertEqual(.25f,conv.SecondsToCurveFraction(temp));
    
}

-(void) testMultiCurve {
    AutomationCurve::MultiCurve mc;
    
    mc.curves["A"] = AutomationCurve::ACurve::CreatePtr();
    mc.curves["B"] = AutomationCurve::ACurve::CreatePtr();
    
    // time scaling
    mc.curves["A"]->SetTimeOffset(10);
    mc.curves["B"]->SetTimeOffset(4);
    mc.curves["A"]->SetTimeScale(3);
    mc.curves["B"]->SetTimeScale(2);
    
    XCTAssertEqual(mc.GetMinTimeOffset(), 4.0f);
    XCTAssertEqual(mc.GetMaxTimeScale(),9.0f);
    
}

-(void) testFVector {
    AutomationCurve::FVector fv;
    
    // add
    fv.AddValue(.2);
    XCTAssertEqual(fv.Size(), 1);
    
    // cycle settings
    fv.SetCycleLeft(AutomationCurve::CycleType::Repeat);
    fv.SetCycleRight(AutomationCurve::CycleType::Repeat);
    XCTAssertEqual(fv.CycleLeft(), AutomationCurve::CycleType::Repeat);
    XCTAssertEqual(fv.CycleRight(), AutomationCurve::CycleType::Repeat);
    
    // locks
    fv.SetPointLock(0, true);
    XCTAssertEqual(fv.LockAt(0), true);
    fv.SetPointLock(0,false);
    
    // sort
    
    fv.Clear();
    fv.InsertValueAt(0, .9);
    fv.InsertValueAt(0, .5);
    fv.InsertValueAt(0, .1);
    fv.Sort();
    XCTAssertEqual(fv.ValueAt(0), .1f);
    XCTAssertEqual(fv.ValueAt(1), .5f);
    XCTAssertEqual(fv.ValueAt(2), .9f);
    XCTAssertEqual(fv.Size(), 3);
    
}

-(void) testSequence {
    AutomationCurve::Sequence seq;
    
    AutomationCurve::SequenceTrack trk;
    
    trk.items.push_back(AutomationCurve::SequenceItem());
    trk.items.push_back(AutomationCurve::SequenceItem());
    
    trk.items[0].time = .5;
    trk.items[1].time = 1.;
    trk.items[0].length = .75;
    trk.items[1].length = 1.5;
    
    // at time
    auto l1 = trk.GetItemsAt(1);
    auto l2 = trk.GetItemsAt(2);
    
    XCTAssertEqual(l1.size(), 2);
    XCTAssertEqual(l2.size(), 1);
    
    // at range
    auto l3 = trk.GetItemsAtRange(1, 2);
    XCTAssertEqual(l3.size(), 2);
}

@end
