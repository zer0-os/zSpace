// Copyright 2020 Sabre Dart Studios


#include "Tests/SandBoxTests/SandBoxTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt, "ZSpace.Math.MaxInt", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);


bool FMathMaxInt::RunTest(const FString& Parameters)
{
	TestTrue("2 Different positive number",FMath::Max(10, 11) == 11);
	return true;	
}
