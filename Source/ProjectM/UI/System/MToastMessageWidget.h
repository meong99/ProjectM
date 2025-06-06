#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "MToastMessageWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;

UCLASS()
class PROJECTM_API UMToastMessageWidget : public UMWidgetBase
{
	GENERATED_BODY()

	/*
	* Overrided Functions
	*/
public:
	UMToastMessageWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeOnInitialized() override;

	/*
	* Member Functions
	*/
public:
	UFUNCTION(BlueprintCallable)
	void AddToastMessage(const FText& NewMessage);

protected:
	void FlushMessageQueue();
	void PlayTopMessage();

	UFUNCTION()
	void OnFinished_ToastAnim();

	/*
	* Member Variables
	*/
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UTextBlock> ToastText;
	
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim), Category = "ProjectM")
	TObjectPtr<UWidgetAnimation> ToastAnim;

	TQueue<FText> ToastMessageQue;

	bool bIsPlaying = false;
};
