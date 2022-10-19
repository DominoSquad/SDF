// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"

// Sets default values
APlayerCharacter::APlayerCharacter() :
	CharacterTurnSpeed(3.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* Create subobject for Camera Boom */
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f; // Camera Boom distance followed behind player character
	CameraBoom->bUsePawnControlRotation = true; // Camera Boom rotates relative to player controller rotation

	/* Create subobject for Follow Camera */
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Camera attaches to the socket at the end of the Camera Boom
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to player controller

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called when player wants to move forward or backward
void APlayerCharacter::MoveForward(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if ((Controller != nullptr) && (Value != 0.0f)) // Don't Execute if player controller is null
	{
		// Determine what direction player controller is facing
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };

		AddMovementInput(Direction, Value); // Value is 1 if player is moving forward and -1 if backward
	}

	MoveForwardValue = Value;
}

// Called when player wants to move right or left
void APlayerCharacter::MoveRight(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if ((Controller != nullptr) && (Value != 0.0f)) // Don't Execute if player controller is null
	{
		// Determine what direction player controller is facing
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };

		AddMovementInput(Direction, Value); // Value is 1 if player is moving forward and -1 if backward
	}

	MoveRightValue = Value;
}

// Called when player presses the pickup key
void APlayerCharacter::PickupKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);

	if (OverlappingWeapon) // If player picks up weapon...
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket")); // Attaches weapon to right hand
		CharacterState = ECharacterState::ECS_EquippedOnHandedWeapon; // Sets the character's state to equipped one-handed weapon
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}
	else
	{
		if (CanDisarm())
		{
			PlayEquipMontage(FName("Unequip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
		else if (CanArm())
		{
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOnHandedWeapon;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
	}
}

// Checks if player can disarm weapon
bool APlayerCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

bool APlayerCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Unequipped && EquippedWeapon;
}

// Plays arm or disarm animations
void APlayerCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

// Called when the player wants to attack
void APlayerCharacter::Attack()
{
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

// Checks if player can attack
bool APlayerCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

// Called to play attack animation montage
void APlayerCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage); // Plays the montage

		const int32 Selection = FMath::RandRange(0, 3); // Picks a random number (0, 1, 2 or 3)
		FName SectionName = FName();

		// If selection is 1, Attack1 anim is played. If 2, Attack2 anim is played, and so on
		switch (Selection)
		{
		case 0:

			SectionName = FName("Attack1");
			break;

		case 1:

			SectionName = FName("Attack2");
			break;

		case 2:

			SectionName = FName("Attack3");
			break;

		case 3:

			SectionName = FName("Attack4");
			break;

		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

/* Sets action state back to unoccupied at the end of an attack animation */
void APlayerCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

// Detaches sword from RightHandSocket and attaches it to SpineSocket
void APlayerCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

// Detaches sword from SpineSocket and attaches it to RightHandSocket
void APlayerCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

// Sets action state back to unoccupied after player has finished equipping
void APlayerCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	/* Input for player moving forward and backward */
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	/* Input for player moving left and right */
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	/* Input for player moving camera left and right */
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	/* Input for player moving camera up and down */
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	/* Input for player picking up items */
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &APlayerCharacter::PickupKeyPressed);
	/* Input for player attacking */
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);
}

