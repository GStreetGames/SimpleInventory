// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/InteractionInterface.h"
#include "InteractComponent.generated.h"

class ASimpleHUD;

USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{
		
	};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEINVENTORY_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	//================================================================================
	// Properties & Variables
	//================================================================================
	


	
	//================================================================================
	// FUNCTIONS
	//================================================================================

	// Sets default values for this component's properties
	UInteractComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE bool IsInteracting() const { return GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_Interaction); };

	
protected:

	//================================================================================
	// Properties & Variables
	//================================================================================

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	UPROPERTY()
	ASimpleHUD* HUD;

	float InteractionCheckFrequency;

	float InteractionCheckDistance;

	FTimerHandle TimerHandle_Interaction;

	FInteractionData InteractionData;
	

	//================================================================================
	// FUNCTIONS
	//================================================================================

	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact();
	
		
};
