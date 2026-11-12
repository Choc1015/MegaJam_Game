// Copyright Epic Games, Inc. All Rights Reserved.

#include "WhatWasCutCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WhatWasCut.h"
#include "Cut/CutInteractable.h"
#include "Cut/CutPickup.h"
#include "Cut/CutWorldSubsystem.h"

AWhatWasCutCharacter::AWhatWasCutCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	// 눈높이 135cm: 캡슐(중심이 바닥+75)에 붙이고 상대 Z 60 (ROOM_PLAN §1)
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 60.f), FRotator::ZeroRotator);
	FirstPersonCameraComponent->SetFieldOfView(95.f);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(30.0f, 75.0f);
	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	PrimaryActorTick.bCanEverTick = true;
}

void AWhatWasCutCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FocusTimer -= DeltaSeconds;
	if (FocusTimer > 0.f || !IsLocallyControlled())
	{
		return;
	}
	FocusTimer = 0.05f;

	FocusActor = nullptr;
	const FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	const FVector End = Start + FirstPersonCameraComponent->GetForwardVector() * InteractDistance;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(CutFocus), false, this);
	if (HeldItem)
	{
		Params.AddIgnoredActor(HeldItem);
	}
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		FocusActor = Cast<ACutInteractable>(Hit.GetActor());
	}
}

FString AWhatWasCutCharacter::GetFocusPrompt() const
{
	return FocusActor ? FocusActor->GetPrompt(this) : FString();
}

void AWhatWasCutCharacter::TryInteract()
{
	if (UCutWorldSubsystem* Sub = UCutWorldSubsystem::Get(this))
	{
		if (Sub->IsTransitioning())
		{
			return;
		}
	}
	if (FocusActor)
	{
		FocusActor->Interact(this);
	}
}

void AWhatWasCutCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// E: 상호작용 (Input Action 에셋 없이 키로 직접 바인딩)
	PlayerInputComponent->BindKey(EKeys::E, IE_Pressed, this, &AWhatWasCutCharacter::TryInteract);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AWhatWasCutCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AWhatWasCutCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWhatWasCutCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWhatWasCutCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AWhatWasCutCharacter::LookInput);
	}
	else
	{
		UE_LOG(LogWhatWasCut, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AWhatWasCutCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void AWhatWasCutCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void AWhatWasCutCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AWhatWasCutCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AWhatWasCutCharacter::DoJumpStart()
{
	// 이 게임엔 점프가 없다

}

void AWhatWasCutCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}
