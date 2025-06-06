#include "MToastMessageWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

UMToastMessageWidget::UMToastMessageWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMToastMessageWidget::NativeOnInitialized()
{
	FWidgetAnimationDynamicEvent FinishEvent;
	FinishEvent.BindDynamic(this, &UMToastMessageWidget::OnFinished_ToastAnim);
	BindToAnimationFinished(ToastAnim, FinishEvent);
}

void UMToastMessageWidget::AddToastMessage(const FText& NewMessage)
{
	ToastMessageQue.Enqueue(NewMessage);
	FlushMessageQueue();
}

void UMToastMessageWidget::FlushMessageQueue()
{
	if (!bIsPlaying)
	{
		PlayTopMessage();
	}
}

void UMToastMessageWidget::PlayTopMessage()
{
	if (!ToastMessageQue.IsEmpty())
	{
		FText NewMessage;
		ToastMessageQue.Dequeue(NewMessage);
		ToastText->SetText(NewMessage);

		PlayAnimation(ToastAnim);
		bIsPlaying = true;
	}
	else
	{
		bIsPlaying = false;
	}
}

void UMToastMessageWidget::OnFinished_ToastAnim()
{
	bIsPlaying = false;
	FlushMessageQueue();
}
