#include "MItemContextWidget.h"
#include "Components/Image.h"
#include "System/MDataTableManager.h"
#include "Inventory/PMInventoryItemDefinition.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "PMGameplayTags.h"

UMItemContextWidget::UMItemContextWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMItemContextWidget::UpdateContextWidget(const int32 RowId)
{
	ItemRowId = RowId;
	SetWidgetPosition();
	UPMInventoryItemDefinition* ItemDef = UMDataTableManager::GetDefinitionObject<UPMInventoryItemDefinition>(this, ItemRowId);
	if (ItemDef)
	{
		SetItemImage(ItemDef->ItemIcon);
		SetItemContextText(ItemDef->ItemContext);
		SetItemName(ItemDef->DisplayName);

		bool bExistStat = false;
		for (const FMApplyEffectDefinition& EffectDef : ItemDef->ApplyEffectToSelf)
		{
			for (const FMSetbyCallerFloat& SetbyCallerInfo : EffectDef.EffectValues)
			{
				UTextBlock* OutTextBlock;
				FText OutName;
				GetTextBlockByTag(SetbyCallerInfo.SetByCallerTag, OutTextBlock, OutName);
				if (OutTextBlock)
				{
					SetSpecText(OutTextBlock, OutName, SetbyCallerInfo.Value);
					bExistStat = true;
				}
			}
		}

		bExistStat ? StatOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible) : StatOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMItemContextWidget::SetWidgetPosition()
{
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(FloatingTarget->Slot);
	if (CanvasSlot)
	{
		FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
		CanvasSlot->SetPosition(MousePosition);
		DisableTexts();
	}
}

void UMItemContextWidget::DisableTexts()
{
	Health->SetVisibility(ESlateVisibility::Collapsed);
	MaxHealth->SetVisibility(ESlateVisibility::Collapsed);
	AttackPower->SetVisibility(ESlateVisibility::Collapsed);
	DefencePower->SetVisibility(ESlateVisibility::Collapsed);
}

void UMItemContextWidget::SetItemImage(UTexture2D* NewTexture)
{
	ItemImage->SetBrushFromTexture(NewTexture);
}

void UMItemContextWidget::SetItemContextText(const FText& NewContext)
{
	ItemContext->SetText(NewContext);
}

void UMItemContextWidget::SetItemName(const FText& NewName)
{
	ItemName->SetText(NewName);
}

void UMItemContextWidget::SetSpecText(UTextBlock* InTextBlock, const FText& InName, const float Value)
{
	FText Template = FText::FromString("{Name} : {Value}");
	FFormatNamedArguments Args;

	Args.Add("Value", FText::AsNumber(Value));
	Args.Add("Name", InName);

	if (InTextBlock)
	{
		InTextBlock->SetText(FText::Format(Template, Args));
		InTextBlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UMItemContextWidget::GetTextBlockByTag(const FGameplayTag& AttributeTag, UTextBlock*& OutTextBlock, FText& OutName)
{
	if (AttributeTag == FPMGameplayTags::Get().Ability_Effect_SetByCaller_Health)
	{
		OutTextBlock = Health;
		OutName = FText::FromString(TEXT("회복량"));
	}
	else if (AttributeTag == FPMGameplayTags::Get().Ability_Effect_SetByCaller_MaxHealth)
	{
		OutTextBlock = MaxHealth;
		OutName = FText::FromString(TEXT("최대 체력"));
	}
	else if (AttributeTag == FPMGameplayTags::Get().Ability_Effect_SetByCaller_AttackPower)
	{
		OutTextBlock = AttackPower;
		OutName = FText::FromString(TEXT("공격력"));
	}
	else if (AttributeTag == FPMGameplayTags::Get().Ability_Effect_SetByCaller_DefensePower)
	{
		OutTextBlock = DefencePower;
		OutName = FText::FromString(TEXT("방어력"));
	}
}
