#pragma once

#include "Components/PawnComponent.h"
#include "PMPawnExtensionComponent.generated.h"

class UPMPawnData;

UCLASS()
class PROJECTM_API UPMPawnExtensionComponent : public UPawnComponent
{
	GENERATED_BODY()

	/*
	* Engine Functions
	*/
public:
	UPMPawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	static UPMPawnExtensionComponent* FindPawnExtensionComponent(const APawn* Actor)
	{
		return IsValid(Actor) ? Actor->FindComponentByClass<UPMPawnExtensionComponent>() : nullptr;
	}

/*
* Member Functions
*/
public:
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const UPMPawnData* InPawnData) { PawnData = InPawnData; }

/*
* Member Variables
*/
private:
	UPROPERTY(EditInstanceOnly, Category = "ProjectM | Pawn")
	TObjectPtr<const UPMPawnData> PawnData;
};