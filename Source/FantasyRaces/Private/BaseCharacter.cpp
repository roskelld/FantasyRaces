/** 
* Copyright PrimeVR 2020 
* @author       roskelld https://github.com/roskelld 
* @description   
* @version      0.1 
*/


#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/Character.h"

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
	FocusFOV = 55.0f;
	FocusInterpSpeed = 2.0f;

	// Character Size Defaults
	DefaultCharacterHeight = 183.0f;
	CharacterHeight = 183.0f;
	CharacterRadius = 34.0f;

	// @TODO: 
	// Edit Jump height
	// Edit weight / mass

	// Vision
	bRequestedFreelook = false;

	// Movement
	CharacterMaxProneSpeed = 150.0f;
	CharacterMaxCrouchSpeed = 250.0f;
	CharacterMaxCreepSpeed = 250.0f;
	CharacterMaxWalkSpeed = 400.0f;
	CharacterMaxJogSpeed = 600.0f;
	CharacterMaxSprintSpeed = 800.0f;

	bRequestedCreep = false;
	bRequestedJog = false;
	bRequestedSprint = false;
	bRequestedProne = false;

	// DEBUG
	bShowDebug = true;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->SetCapsuleSize(CharacterRadius, CharacterHeight / 2);
	GetCharacterMovement()->CrouchedHalfHeight = CharacterHeight / 4;
	GetMesh()->SetRelativeScale3D(FVector(CharacterHeight / DefaultCharacterHeight));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, 1 - (CharacterHeight / 2)));


	// Update FOV
	CameraComp->SetFieldOfView(DefaultFOV);

	// Vision
	LastForwardVector = GetActorForwardVector();
	LastRightVector = GetActorRightVector();

	// Only run on the server
	//if (GetLocalRole() == ROLE_Authority)
	//{

	//}

	// Attach the Camera to the Mesh Head
	CameraComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, CameraAttachSocketName);

	// Set Camera offset to match face location
	CameraComp->SetRelativeLocation(CameraOffset);

	// Movement
	GetCharacterMovement()->MaxWalkSpeedCrouched = CharacterMaxCrouchSpeed;
	GetCharacterMovement()->MaxWalkSpeed = CharacterMaxWalkSpeed;
}

// Character Movement 
void ABaseCharacter::MoveForward(float Value)
{
	if (!bRequestedFreelook) 
	{
		LastForwardVector = GetActorForwardVector();
	}

	AddMovementInput(LastForwardVector * Value);
}

void ABaseCharacter::MoveRight(float Value)
{
	if (!bRequestedFreelook)
	{
		LastRightVector = GetActorRightVector();
	}
	AddMovementInput(LastRightVector * Value);
}

void ABaseCharacter::StartCreep()
{
	if (bShowDebug)	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("START CREEP"), true, FVector2D(1.0f));
	bRequestedCreep = true;
	GetCharacterMovement()->MaxWalkSpeed = CharacterMaxCreepSpeed;

}

void ABaseCharacter::EndCreep()
{
	if (bShowDebug)	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("END CREEP"), true, FVector2D(1.0f));
	bRequestedCreep = false;
}

void ABaseCharacter::ToggleCreep()
{
	bRequestedCreep = !bRequestedCreep;
	if (bRequestedCreep) GetCharacterMovement()->MaxWalkSpeed = CharacterMaxCreepSpeed;
}

void ABaseCharacter::StartJog()
{
	if (bShowDebug)	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("START JOG"), true, FVector2D(1.0f));
	bRequestedJog = true;
	GetCharacterMovement()->MaxWalkSpeed = CharacterMaxJogSpeed;
}

void ABaseCharacter::EndJog()
{
	if (bShowDebug)	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("END JOG"), true, FVector2D(1.0f));
	bRequestedJog = false;
}

void ABaseCharacter::ToggleJog()
{
	if (bShowDebug)	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("TOGGLE JOG"), true, FVector2D(1.0f));
	bRequestedJog = !bRequestedJog;
	if (bRequestedJog) GetCharacterMovement()->MaxWalkSpeed = CharacterMaxJogSpeed;
}

void ABaseCharacter::StartSprint()
{
	if (bShowDebug)	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("START SPRINT"), true, FVector2D(1.0f));
	bRequestedSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = CharacterMaxSprintSpeed;
}

void ABaseCharacter::EndSprint()
{
	if (bShowDebug)	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("END SPRINT"), true, FVector2D(1.0f));
	bRequestedSprint = false;
}

void ABaseCharacter::ToggleSprint()
{
	if (bShowDebug)	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("TOGGLE SPRINT"), true, FVector2D(1.0f));
	bRequestedSprint = !bRequestedSprint;
	if (bRequestedSprint) GetCharacterMovement()->MaxWalkSpeed = CharacterMaxSprintSpeed;
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

void ABaseCharacter::StartProne()
{
	if (bShowDebug)	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("START PRONE"), true, FVector2D(1.0f));
	bRequestedProne = true;
	GetCharacterMovement()->MaxWalkSpeed = CharacterMaxProneSpeed;
}

void ABaseCharacter::EndProne()
{
	if (bShowDebug)	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("END PRONE"), true, FVector2D(1.0f));
	bRequestedProne = false;
}

void ABaseCharacter::ToggleProne()
{
	bRequestedProne = !bRequestedProne;
	if (bRequestedProne) GetCharacterMovement()->MaxWalkSpeed = CharacterMaxProneSpeed;
}

// Vision
void ABaseCharacter::StartFocus()
{
	bRequestedFocus = true;
}

void ABaseCharacter::EndFocus()
{
	bRequestedFocus = false;
}

void ABaseCharacter::StartFreelook()
{
	if (bShowDebug)	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("START FREELOOK"), true, FVector2D(1.0f));
	bRequestedFreelook = true;
}

void ABaseCharacter::EndFreelook()
{
	if (bShowDebug)	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("END FREELOOK"), true, FVector2D(1.0f));
	bRequestedFreelook = false;
}

// Interaction
void ABaseCharacter::Interact()
{
	if (bShowDebug)	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("INTERACT"), true, FVector2D(1.0f));
}

// Action
void ABaseCharacter::StartGestureWave()
{
	if (bShowDebug)	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("START GESTURE WAVE"), true, FVector2D(1.0f));
}

void ABaseCharacter::EndGestureWave()
{
	if (bShowDebug)	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("END GESTURE WAVE"), true, FVector2D(1.0f));
}

// Generate Character from Data

void ABaseCharacter::SetCharacterSize()
{

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV = bRequestedFocus ? FocusFOV : DefaultFOV;

	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, FocusInterpSpeed);

	CameraComp->SetFieldOfView(NewFOV);

	// Set Movement Speed
	if (!bRequestedCreep && !bRequestedJog && !bRequestedSprint && !bRequestedProne)
	{
		GetCharacterMovement()->MaxWalkSpeed = CharacterMaxWalkSpeed;
	}

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement 
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	PlayerInputComponent->BindAction("Creep", IE_Pressed, this, &ABaseCharacter::StartCreep);
	PlayerInputComponent->BindAction("Creep", IE_Released, this, &ABaseCharacter::EndCreep);
	PlayerInputComponent->BindAction("CreepToggle", IE_Pressed, this, &ABaseCharacter::ToggleCreep);

	PlayerInputComponent->BindAction("Jog", IE_Pressed, this, &ABaseCharacter::StartJog);
	PlayerInputComponent->BindAction("Jog", IE_Released, this, &ABaseCharacter::EndJog);
	PlayerInputComponent->BindAction("JogToggle", IE_Pressed, this, &ABaseCharacter::ToggleJog);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABaseCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABaseCharacter::EndSprint);
	PlayerInputComponent->BindAction("SprintToggle", IE_Pressed, this, &ABaseCharacter::ToggleSprint);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::Jump);

	// Rotation
	PlayerInputComponent->BindAxis("CameraPitch", this, &ABaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("CameraYaw", this, &ABaseCharacter::AddControllerYawInput);

	// Pose
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABaseCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ABaseCharacter::EndCrouch);
	PlayerInputComponent->BindAction("CrouchToggle", IE_Pressed, this, &ABaseCharacter::ToggleCrouch);

	PlayerInputComponent->BindAction("Prone", IE_Pressed, this, &ABaseCharacter::StartProne);
	PlayerInputComponent->BindAction("Prone", IE_Released, this, &ABaseCharacter::EndProne);
	PlayerInputComponent->BindAction("ProneToggle", IE_Pressed, this, &ABaseCharacter::ToggleProne);
	
	// Vision
	PlayerInputComponent->BindAction("Focus", IE_Pressed, this, &ABaseCharacter::StartFocus);
	PlayerInputComponent->BindAction("Focus", IE_Released, this, &ABaseCharacter::EndFocus);

	PlayerInputComponent->BindAction("FreeLook", IE_Pressed, this, &ABaseCharacter::StartFreelook);
	PlayerInputComponent->BindAction("FreeLook", IE_Released, this, &ABaseCharacter::EndFreelook);

	// Hearing

	// Interaction
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ABaseCharacter::Interact);

	// Action
	PlayerInputComponent->BindAction("GestureWave", IE_Pressed, this, &ABaseCharacter::StartGestureWave);
	PlayerInputComponent->BindAction("GestureWave", IE_Released, this, &ABaseCharacter::EndGestureWave);

}



