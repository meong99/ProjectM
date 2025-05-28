#include "MDamageTextWidget.h"
#include "MSlideToastText.h"
#include "Components/Overlay.h"

UMDamageTextWidget::UMDamageTextWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMDamageTextWidget::OnDamaged(const float Damage)
{
	UMSlideToastText* Widget = GetDeactivatedWidget();
	if (Widget)
	{
		// 데미지 표기만 버림 후 표시.
		int32 DamageInt = FMath::FloorToInt32(Damage);
		Widget->SetText(FText::AsNumber(DamageInt));
	}
}

UMSlideToastText* UMDamageTextWidget::GetDeactivatedWidget() const
{
	TArray<UWidget*> Children = TextOverlay->GetAllChildren();
	for (int32 i = 0; i < Children.Num(); i++)
	{
		UMSlideToastText* Widget = Cast<UMSlideToastText>(Children[i]);
		if (Widget && !Widget->IsActivatedText())
		{
			return Widget;
		}
	}

	return Cast<UMSlideToastText>(Children[0]);
}
