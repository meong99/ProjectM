#include "MLoadingWidget.h"
#include "Animation/WidgetAnimation.h"

UMLoadingWidget::UMLoadingWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMLoadingWidget::PlayFadeIn(FWidgetAnimationDynamicEvent&& Callback)
{
	BindToAnimationFinished(FadeIn, MoveTemp(Callback));
	PlayAnimation(FadeIn);
}

void UMLoadingWidget::PlayFadeOut(FWidgetAnimationDynamicEvent&& Callback)
{
	BindToAnimationFinished(FadeOut, MoveTemp(Callback));
	PlayAnimation(FadeOut);
}

void UMLoadingWidget::UnbindAnimationBind()
{
	UnbindAllFromAnimationFinished(FadeIn);
	UnbindAllFromAnimationFinished(FadeOut);
}
