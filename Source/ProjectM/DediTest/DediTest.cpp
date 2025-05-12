// Fill out your copyright notice in the Description page of Project Settings.


#include "DediTest/DediTest.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"


// Sets default values
ADediTest::ADediTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NetDormancy = ENetDormancy::DORM_DormantAll;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ADediTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADediTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	static float asd = 0.0f;
	if (HasAuthority() && bTestBool)
	{
		asd += DeltaTime;
		if (asd > 1)
		{
			asd = 0.0f;
			Test++;
// 			SetActorLocation(GetActorLocation() + FVector{ 1.0f, 0.0f, 0.0f });
		}
	}
}

void ADediTest::SetDorm(ENetDormancy Dorm)
{
	SetNetDormancy(Dorm);
}

void ADediTest::OnRep_Test()
{
	GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Red, FString::Printf(TEXT("test = %d"), Test));
}

void ADediTest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADediTest, Test);
}

