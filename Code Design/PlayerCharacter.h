// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AItem;
class UAnimMontage;
class AWeapon;


UCLASS()
class PROJECTDOMINO_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Moves player forward or backward */
	void MoveForward(float Value);

	/* Moves player right or left */
	void MoveRight(float Value);

	/* Called when player presses the pickup key. Houses the functions that are called when pickup key is pressed */
	void PickupKeyPressed();

	/* If Conditions met, player can disarm weapon */
	bool CanDisarm();

	/* If conditions met, player can arm weapon */
	bool CanArm();

	/* Plays disarm or arm animations */
	void PlayEquipMontage(const FName& SectionName);

	/* Called when player presses the attack button. When called, the player will attack */
	void Attack();

	/* If conditions are met, the player can attack */
	bool CanAttack();

	/* Called to play attack montage */
	void PlayAttackMontage();

	/* Called at the end of an attack animation and sets action state back to unoccupied */
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	/* Called when player wants to disarm their weapon */
	UFUNCTION(BlueprintCallable)
	void Disarm();

	/* Called when player wants to arm their weapon */
	UFUNCTION(BlueprintCallable)
	void Arm();

	/* Called when player finishes equipping */
	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

private:

	/* Character's character state */
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	/* Character's action state */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	/* Camera Boom that positions camera behind the player character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/* Camera that follows the player character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/* Input value for moving forward or backward */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float MoveForwardValue;

	/* Input value for moving right or left */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float MoveRightValue;

	/* How fast the player turns when the player wants to walk in the opposite direction they are facing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float CharacterTurnSpeed;

	/* Reference to the item that the player is overlapping */
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	/* Animation montage for attacking */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	/* Animation montage for Equipping */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	/* Reference to the player's equipped weapon */
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;

public:

	/* Getters */
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE float GetMoveForwardValue() const { return MoveForwardValue; }
	FORCEINLINE float GetMoveRightValue() const { return MoveRightValue; }
	FORCEINLINE float GetCharacterTurnSpeed() const { return CharacterTurnSpeed; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	/* Setters */
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
};
