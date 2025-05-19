// Fill out your copyright notice in the Description page of Project Settings.


#include "DediTest/DediTest.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"


// Sets default values
ADediTest::ADediTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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

}

void ADediTest::SetDorm(ENetDormancy Dorm)
{
	SetNetDormancy(Dorm);
}

void ADediTest::DoReplication()
{
	if (HasAuthority())
	{
		//ReplicationTestVar = ++ReplicationTestVar;
	}
}

void ADediTest::Client_TestRPC_Implementation(const int32& n)
{
	GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Red, FString::Printf(TEXT("test = %d"), n));
}

//
//void ADediTest::OnRep_Test()
//{
//	GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Red, FString::Printf(TEXT("test = %d"), Test));
//}
//
//void ADediTest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(ADediTest, ReplicationTestVar);
//}

void ADediTest::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);
}

void ADediTest::BeginReplication()
{
	Super::BeginReplication();
}

