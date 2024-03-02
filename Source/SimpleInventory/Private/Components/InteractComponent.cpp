// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/InteractComponent.h"
#include "DrawDebugHelpers.h"
#include "UserInterface/SimpleHUD.h"


// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// we set variables in the constructor section duhh...
	InteractionCheckFrequency = 0.1;
	InteractionCheckDistance = 300.0f;
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	HUD = Cast<ASimpleHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	
}


// Called every frame
void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}
}


void UInteractComponent::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{GetOwner()->GetActorLocation()};
	FVector TraceEnd{TraceStart + (GetOwner()->GetActorRotation().Vector() * InteractionCheckDistance)};

	// Visualize the line trace - Debug only, comment out to turn off!
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	FHitResult TraceHit;

	if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
		{
			if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
			{
				FoundInteractable(TraceHit.GetActor());
				return;
			}

			if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
			{
				return;
			}
		}
	}

	NoInteractableFound();
}

void UInteractComponent::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting())
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus();
}

void UInteractComponent::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		// hide interaction widget on the HUD
		HUD->HideInteractionWidget();

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void UInteractComponent::BeginInteract()
{
	// verify that nothing has changed with the interactable state since beginning interaction
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if(FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_Interaction,
					this,
					&UInteractComponent::Interact,
					TargetInteractable->InteractableData.InteractionDuration,
					false);
			}
		}
	}
}

void UInteractComponent::EndInteract()
{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void UInteractComponent::Interact()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		AActor* InteractingActor = GetOwner();
		UInventoryComponent* InteractingActorInventory = InteractingActor->FindComponentByClass<UInventoryComponent>();
		if (InteractingActorInventory)
		{
			// Call the Interact() function of the TargetInteractable object
			TargetInteractable->Interact(InteractingActor, InteractingActorInventory);
		}
		else
		{
			// Handle case where the interacting actor does not have an inventory component
		}
	}
}

