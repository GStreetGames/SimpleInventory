// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IngameMenu.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEINVENTORY_API UIngameMenu : public UUserWidget
{
	GENERATED_BODY()

//public:


protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};