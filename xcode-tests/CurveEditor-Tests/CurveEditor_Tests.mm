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
}

-(void) testMultiCurve {
    AutomationCurve::MultiCurve mc;
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
