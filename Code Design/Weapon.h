// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDOMINO_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

	AWeapon();

	/* Called when player wants to equip (pick up) a weapon */
	void Equip(USceneComponent* InParent, FName InSocketName);

	/* Attaches a mesh (ex. sword) to socket */
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

protected:

	/* Override of parent function */
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	/* Override of parent function */
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:

public:
	
};
