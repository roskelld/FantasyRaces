// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraAttachSocketName = TEXT("head");

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->bUsePawnControlRotation = true;	

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanFly = false; 
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;
	
	// FOV settings
	DefaultFOV = 90.0f;
	FocusFOV = 120.0f;
	FocusInterpSpeed = 20.f;


}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Update FOV
	CameraComp->SetFieldOfView(DefaultFOV);

	// Only run on the server
	//if (GetLocalRole() == ROLE_Authority)
	//{

	//}

	// Attach the Camera to the Mesh Head
	CameraComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, CameraAttachSocketName);

	// Set Camera offset to match face location
	CameraComp->SetRelativeLocation(CameraOffset);
}

// Character Interactions 

void ABaseCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}



void ABaseCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void ABaseCharacter::StartFocus()
{
	bRequestedFocus = true;
}

void ABaseCharacter::EndFocus()
{
	bRequestedFocus = false;
}

void ABaseCharacter::StartCrouch()
{
	Crouch();
}

void ABaseCharacter::EndCrouch()
{
	UnCrouch();
}

void ABaseCharacter::ToggleCrouch()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV = bRequestedFocus ? FocusFOV : DefaultFOV;

	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, FocusInterpSpeed);

	CameraComp->SetFieldOfView(NewFOV);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement 
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	// Rotation
	PlayerInputComponent->BindAxis("CameraPitch", this, &ABaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("CameraYaw", this, &ABaseCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Focus", IE_Pressed, this, &ABaseCharacter::StartFocus);
	PlayerInputComponent->BindAction("Focus", IE_Released, this, &ABaseCharacter::EndFocus);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABaseCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ABaseCharacter::EndCrouch);
	PlayerInputComponent->BindAction("CrouchToggle", IE_Pressed, this, &ABaseCharacter::ToggleCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::Jump);
	

}

