/**
* Copyright PrimeVR 2020
* @author       roskelld https://github.com/roskelld
* @description
* @version      0.1
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UCameraComponent;

UCLASS()
class FANTASYRACES_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName CameraAttachSocketName;

	// Movement Functions
	void MoveForward(float Value);

	void MoveRight(float Value);

	void StartCreep();

	void EndCreep();

	void ToggleCreep();

	void StartJog();

	void EndJog();

	void ToggleJog();

	void StartSprint();

	void EndSprint();

	void ToggleSprint();

	// Pose Functions
	void StartCrouch();

	void EndCrouch();

	void ToggleCrouch();

	void StartProne();

	void EndProne();

	void ToggleProne();

	// Vision Functions
	void StartFocus();

	void EndFocus();

	void StartFreelook();

	void EndFreelook();

	// Interaction
	void Interact();

	// Action

	void StartGestureWave();

	void EndGestureWave();

	bool bRequestedFocus; 

	/* Character's default FOV */
	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 10, ClampMax = 150))
	float DefaultFOV;

	/* FOV used for focus vision */
	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 10, ClampMax = 150))
	float FocusFOV;

	/* Speed to reach maximum focus */
	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
	float FocusInterpSpeed;

	/* Offset position of camera. Currently used mainly to avoid clipping with character mesh */
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	FVector CameraOffset;

	/* Height of character in cm. Default Character is 183cm */
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float CharacterHeight;

	/* Radius of character in cm */
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float CharacterRadius;

	void SetCharacterSize();

	/* This is the height of the default mesh used to scale */
	float DefaultCharacterHeight;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
