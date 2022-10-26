// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;

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

	TArray<AActor*> IgnoreActors;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Override of parent function */
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	/* Override of parent function */
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	/* Handles what happens when box collision overlaps something. When overlap occurs, blood or spark particles will appear and damage will be delt */
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	/* Box Collision for the item. Enabled when attacking and disabled when not attacking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

public:

	/* Getters */
	FORCEINLINE UBoxComponent* GetCollisionBox() const { return CollisionBox; }
	
};
