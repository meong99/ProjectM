#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "MDialogueBoxWidget.generated.h"

class UTextBlock;
class UButton;
class UHorizontalBox;

UCLASS()
class PROJECTM_API UMDialogueBoxWidget : public UMWidgetBase
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMDialogueBoxWidget(const FObjectInitializer& ObjectInitializer);

/*
* Member Functions
*/
public:
	void SetCallback1(TFunction<void ()> Callback);
	void SetCallback2(TFunction<void ()> Callback);

protected:
	UFUNCTION()
	void OnClick_Button1();
	UFUNCTION()
	void OnClick_Button2();

	void SetButton1_Text(const FText& InText);
	void SetButton2_Text(const FText& InText);
/*
* Member Variables
*/
protected:
	TFunction<void()> Callback1;
	TFunction<void()> Callback2;

	FText Button1_Text;
	FText Button2_Text;
	
	UPROPERTY()
	TArray<FText> DialogTextArray;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UHorizontalBox> ButtonBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UTextBlock> Button1Text;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UTextBlock> Button2Text;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UButton> CallbackButton1;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UButton> CallbackButton2;
};
