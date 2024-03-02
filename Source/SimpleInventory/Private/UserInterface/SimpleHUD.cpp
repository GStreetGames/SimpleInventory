// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/SimpleHUD.h"
#include "UserInterface/IngameMenu.h"
#include "UserInterface/Interaction/InteractionWidget.h"

ASimpleHUD::ASimpleHUD()
{
}

void ASimpleHUD::BeginPlay()
{
	Super::BeginPlay();

	if (IngameMenuClass)
	{
		IngameMenuWidget = CreateWidget<UIngameMenu>(GetWorld(), IngameMenuClass);
		IngameMenuWidget->AddToViewport(5);
		IngameMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void ASimpleHUD::DisplayMenu()
{
	if (IngameMenuWidget)
	{
		bIsMenuVisible = true;
		IngameMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ASimpleHUD::HideMenu()
{
	if (IngameMenuWidget)
	{
		bIsMenuVisible = false;
		IngameMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ASimpleHUD::ToggleMenu()
{
	if (bIsMenuVisible)
	{
		HideMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayMenu();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void ASimpleHUD::ShowInteractionWidget()
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ASimpleHUD::HideInteractionWidget()
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ASimpleHUD::UpdateInteractionWidget(const FInteractableData* InteractableData)
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}
