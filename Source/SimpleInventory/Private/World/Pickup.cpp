// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Pickup.h"
#include "Components/InventoryComponent.h"
#include "Items/ItemBase.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);
}

void APickup::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(UItemBase::StaticClass(), ItemQuantity);
}

void APickup::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
	const FName DesiredItemID = DataTableItem.RowName;
	const UDataTable* ItemDataTable = DataTableItem.DataTable;
	
	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		ItemReference = NewObject<UItemBase>(this, UItemBase::StaticClass());

		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemQuality = ItemData->ItemQuality;
		ItemReference->NumericData = ItemData->NumericData;
		ItemReference->TextData = ItemData->TextData;
		ItemReference->AssetData = ItemData->AssetData;

		ItemReference->NumericData.bIsStackable = ItemData->NumericData.MaxStackSize > 1;
		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractableData();
	}
}

void APickup::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight();
	ItemReference->OwningInventory = nullptr;
	PickupMesh->SetStaticMesh(ItemToDrop->AssetData.Mesh);

	UpdateInteractableData();
}

void APickup::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->TextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->TextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InteractableData = InstanceInteractableData;
}


// we need to check for the owning actors inventory component before we can take the pickup and put it there
void APickup::Interact(AActor* InteractingActor, UInventoryComponent* InteractingActorInventory)
{
	if (InteractingActorInventory)
	{
		FString InventoryName = InteractingActorInventory->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Player inventory PICKUP INTERACT CALLED. InteractingActorInventory Name: %s"), *InventoryName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player inventory PICKUP INTERACT CALLED. InteractingActorInventory is nullptr"));
	}
}


// begin and end focus need to be altered to show an actual highlight, as they are now they depend upon a post process material
void APickup::BeginFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
}

void APickup::EndFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}

// TODO we're calling "PlayerInventory" here, but we haven't established that yet, this is needed ASAP!
void APickup::TakePickup(AActor* InteractingActor)
{
    if (!IsPendingKillPending() && InteractingActor)
    {
        UInventoryComponent* TakerInventory = Cast<UInventoryComponent>(InteractingActor->GetComponentByClass(UInventoryComponent::StaticClass()));

        if (TakerInventory)
        {
            if (ItemReference)
            {
                const FItemAddResult AddResult = TakerInventory->HandleAddItem(ItemReference);

                switch (AddResult.OperationResult)
                {
                case EItemAddResult::IAR_NoItemAdded:
                    break;
                case EItemAddResult::IAR_PartialAmountItemAdded:
                    UpdateInteractableData();
                    TakerInventory->UpdateInteractionWidget();
                    break;
                case EItemAddResult::IAR_AllItemAdded:
                    Destroy();
                    break;
                }

                UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Pickup internal item reference was somehow null!"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Taker inventory component is null!"));
        }
    }
}

// TODO fix this to update the mesh in editor, we mixed the old system that typed it out with the new system that is a dropdown!
void APickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	const FName DesiredItemID = DataTableItem.RowName;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickup, DataTableItem.RowName))
	{
		if (DataTableItem.DataTable)
		{
			if (const FItemData* ItemData = DataTableItem.DataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString()))
			{
				PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
			}
		}
	}
}
