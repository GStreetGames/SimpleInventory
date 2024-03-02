// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SimpleHUD.generated.h"

struct FInteractableData;
class UInteractionWidget;
class UIngameMenu;

/**
 * 
 */
UCLASS()
class SIMPLEINVENTORY_API ASimpleHUD : public AHUD
{
	GENERATED_BODY()

public:
	//================================================================================
	// Properties & Variables
	//================================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UIngameMenu> IngameMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	bool bIsMenuVisible;

	//================================================================================
	// FUNCTIONS
	//================================================================================
	ASimpleHUD();

	void DisplayMenu();
	void HideMenu();

	UFUNCTION(BlueprintCallable)
	void ToggleMenu();

	void ShowInteractionWidget();
	void HideInteractionWidget();
	void UpdateInteractionWidget(const FInteractableData* InteractableData);

protected:
	//================================================================================
	// Properties & Variables
	//================================================================================
	UPROPERTY()
	UIngameMenu* IngameMenuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	//================================================================================
	// FUNCTIONS
	//================================================================================
	virtual void BeginPlay() override;

};