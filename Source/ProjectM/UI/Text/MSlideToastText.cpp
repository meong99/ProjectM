#include "MSlideToastText.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "Materials/MaterialInstanceDynamic.h"

UMSlideToastText::UMSlideToastText(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMSlideToastText::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	FontMaterial = ToastText->GetDynamicOutlineMaterial();
}

void UMSlideToastText::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsActivatedText && AppearancedTime < MaxAppearanceTime)
	{
		float Alpha = FMath::Clamp(1 - AppearancedTime / MaxAppearanceTime, 0, 1);
		if (FontMaterial)
		{
			FontMaterial->SetScalarParameterValue(TEXT("Alpha"), Alpha);
		}
		ToastText->SetOpacity(Alpha);
		AppearancedTime += InDeltaTime;
	}
	else
	{
		DisableSlideWidget();
	}
}

void UMSlideToastText::SetText(const FText& InText)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ToastText->SetText(InText);
	bIsActivatedText = true;
	PlayAnimation(SlideWidgetAnim);
}

void UMSlideToastText::DisableSlideWidget()
{
	bIsActivatedText = false;
	AppearancedTime = 0.f;
	if (GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}
