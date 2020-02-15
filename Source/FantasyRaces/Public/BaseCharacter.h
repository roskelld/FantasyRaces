// Fill out your copyright notice in the Description page of Project Settings.

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

	void StartFocus();

	void EndFocus();

	void StartCrouch();

	void EndCrouch();

	void ToggleCrouch();




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

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	FVector CameraOffset;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
