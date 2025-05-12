// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DediTest.generated.h"

UCLASS()
class PROJECTM_API ADediTest : public AActor
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTestBool = true;

	UFUNCTION()
	void OnRep_Test();
	UPROPERTY(ReplicatedUsing=OnRep_Test)
	int32 Test = 0;
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
