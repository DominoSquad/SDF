// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerCharacterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
		
	}
	if (PlayerCharacter)
	{
		// Get the lateral speed of the character from velocity
		FVector Velocity{ PlayerCharacter->GetVelocity() };
		Velocity.Z = 0;
		Speed = Velocity.Size();

		// Is the character in the air?
		bIsInAir = PlayerCharacter->GetCharacterMovement()->IsFalling();

		// Is the character accelerating?
		if (PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
		}

		FRotator AimRotation = PlayerCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(PlayerCharacter->GetVelocity());
		
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

		if (PlayerCharacter->GetVelocity().Size() > 0.f)
		{
			LastMovementOffsetYaw = MovementOffsetYaw;
		}

		UseControllerYawOrNah(bIsAccelerating);

		InputForward = PlayerCharacter->GetMoveForwardValue();
		InputRight = PlayerCharacter->GetMoveRightValue();
	}
}

void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerCharacterAnimInstance::UseControllerYawOrNah(bool IsAccelerating)
{
	if (IsAccelerating)
	{
		PlayerCharacter->bUseControllerRotationYaw = true;
	}
	else
	{
		PlayerCharacter->bUseControllerRotationYaw = false;
	}
}
