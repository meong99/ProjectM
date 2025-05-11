#include "MDialogueBoxWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"

UMDialogueBoxWidget::UMDialogueBoxWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMDialogueBoxWidget::SetCallback1(TFunction<void()> Callback)
{
	Callback1 = Callback;
	CallbackButton1->OnClicked.AddDynamic(this, &UMDialogueBoxWidget::OnClick_Button1);
}

void UMDialogueBoxWidget::SetCallback2(TFunction<void()> Callback)
{
	Callback2 = Callback;
	CallbackButton2->OnClicked.AddDynamic(this, &UMDialogueBoxWidget::OnClick_Button2);
}

void UMDialogueBoxWidget::OnClick_Button1()
{
	if (Callback1)
	{
		Callback1();
	}
}

void UMDialogueBoxWidget::OnClick_Button2()
{
	if (Callback2)
	{
		Callback2();
	}
}

void UMDialogueBoxWidget::SetButton1_Text(const FText& InText)
{
	Button1Text->SetText(InText);
}

void UMDialogueBoxWidget::SetButton2_Text(const FText& InText)
{
	Button2Text->SetText(InText);
}
