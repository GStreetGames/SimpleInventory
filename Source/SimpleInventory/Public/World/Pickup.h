// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Pickup.generated.h"

class UItemBase;

UCLASS()
class SIMPLEINVENTORY_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	//================================================================================
	// Properties & Variables
	//================================================================================


	//================================================================================
	// FUNCTIONS
	//================================================================================
	APickup();

	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity);

	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity);

	FORCEINLINE UItemBase* GetItemData() { return ItemReference; };

	virtual void BeginFocus() override;
	virtual void EndFocus() override;

	virtual void Interact(AActor* InteractingActor, UInventoryComponent* InteractingActorInventory) override;

protected:
	//================================================================================
	// Properties & Variables
	//================================================================================
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FDataTableRowHandle DataTableItem;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;

	//================================================================================
	// FUNCTIONS
	//================================================================================
	virtual void BeginPlay() override;

	void UpdateInteractableData();

	// taker should be a reference to the owner and it's inventory component, to be unique
	void TakePickup(AActor* InteractingActor);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};