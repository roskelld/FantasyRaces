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

	// Only run on the server
	//if (GetLocalRole() == ROLE_Authority)
	//{

	//}

	// Attach the Camera to the Mesh Head
	CameraComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, CameraAttachSocketName);

	// Set Camera offset to match face location
	CameraComp->SetRelativeLocation(CameraOffset);
}

// Character Movement 
void ABaseCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void ABaseCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void ABaseCharacter::StartCreep()
{

}

void ABaseCharacter::EndCreep()
{
}

void ABaseCharacter::ToggleCreep()
{
}

void ABaseCharacter::StartJog()
{
}

void ABaseCharacter::EndJog()
{
}

void ABaseCharacter::ToggleJog()
{
}

void ABaseCharacter::StartSprint()
{
}

void ABaseCharacter::EndSprint()
{
}

void ABaseCharacter::ToggleSprint()
{
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
}

void ABaseCharacter::EndProne()
{
}

void ABaseCharacter::ToggleProne()
{
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
}

void ABaseCharacter::EndFreelook()
{
}

// Interaction
void ABaseCharacter::Interact()
{
}

// Action
void ABaseCharacter::StartGestureWave()
{
}

void ABaseCharacter::EndGestureWave()
{
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
	PlayerInputComponent->BindAction("JogToggle", IE_Pressed, this, &ABaseCharacter::ToggleSprint);

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

