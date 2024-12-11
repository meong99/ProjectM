// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UIExtensionSystem.h"
#include "Components/DynamicEntryBoxBase.h"
#include "UIExtensionPointWidget.generated.h"

/** forward declarations */
class UCommonLocalPlayer;

/**
 * 
 */
UCLASS()
class UIEXTENSION_API UUIExtensionPointWidget : public UDynamicEntryBoxBase
{
	GENERATED_BODY()
public:
	UUIExtensionPointWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void ResetExtensionPoint();
	void RegisterExtensionPoint();
	void RegisterExtensionPointForPlayerState(UCommonLocalPlayer* LocalPlayer, APlayerState* PlayerState);
	void OnAddOrRemoveExtension(EUIExtensionAction Action, const FUIExtensionRequest& Request);

	/**
	* UWidget's interfaces
	*/
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
	FGameplayTag ExtensionPointTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
	EUIExtensionPointMatch ExtensionPointTagMatch = EUIExtensionPointMatch::ExactMatch;

	/** UIExtensionPointWidget�� ������ ���� UWidget Classes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
	TArray<TObjectPtr<UClass>> DataClasses;

	/** UIExtension --- Widget ���� ���� */
	UPROPERTY(Transient)
	TMap<FUIExtensionHandle, TObjectPtr<UUserWidget>> ExtensionMapping;

	/** UIExtensionPoint ��ü ���� */
	TArray<FUIExtensionPointHandle> ExtensionPointHandles;
};
