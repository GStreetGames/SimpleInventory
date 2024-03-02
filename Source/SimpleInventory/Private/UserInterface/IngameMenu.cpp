// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/IngameMenu.h"

void UIngameMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UIngameMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UIngameMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	// cast operation to item drag drop, ensure player is valid, call drop item on player
}
