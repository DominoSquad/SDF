// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
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
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f)) // Don't Execute if player controller is null
	{
		// Determine what direction player controller is facing
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };

		AddMovementInput(Direction, Value); // Value is 1 if player is moving forward and -1 if backward
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };

		AddMovementInput(Direction, Value);
	}
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
}

