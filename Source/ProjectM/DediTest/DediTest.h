// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DediTest.generated.h"

UCLASS()
class PROJECTM_API ADediTest : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADediTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetDorm(ENetDormancy Dorm);

	UFUNCTION(BlueprintCallable)
	void DoReplication();

	UFUNCTION(Client, Reliable)
	void Client_TestRPC(const int32& n);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTestBool = true;

	//UFUNCTION()
	//void OnRep_Test();

	//UPROPERTY(Replicated/*Using=OnRep_Test*/)
	//int32 ReplicationTestVar = 0;
public:
	void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;


	void BeginReplication() override;
};
