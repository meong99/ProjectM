#pragma once

#include "Components/ControllerComponent.h"
#include "PMCharacterPartTypes.h"
#include "PMPawnComponent_CharacterParts.h"
#include "PMControllerComponent_CharacterParts.generated.h"

/*
* Controller가 보유하는 CharacterPart에 대한 데이터다.
*/
USTRUCT()
struct FPMControllerCharacterPartEntry 
{
	GENERATED_BODY()

	// Possess하고있는 Pawn에 대해 Instancing된 Character Part의 핸들.
	UPROPERTY(EditAnywhere)
	FPMCharacterPart Part;

	// Character Part에 대한 MetaData(가벼운 데이터)
	FPMCharacterPartHandle Handle;
};

/*
* Controller는 기본적으로 MetaData를 들고있고, Character에서 그 메타데이터를 이용해서 실제 보여지는 메쉬들을 생성 및 장착하게된다.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class PROJECTM_API UPMControllerComponent_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UPMControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
/*
* Member Functions
*/
public:

	UPMPawnComponent_CharacterParts* GetPawnCustomizer() const;
private:
	UFUNCTION(BlueprintCallable, Category = "Cosmetics")
	void AddCharacterPart(const FPMCharacterPart& NewPart);
	void AddCharacterPartInternal(const FPMCharacterPart& NewPart);

	void RemoveAllCharacterParts();

	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

/*
* Member Variables
*/
private:
	UPROPERTY(EditAnywhere, Category = "Cosmetics")
	TArray<FPMControllerCharacterPartEntry> CharacterParts;
};