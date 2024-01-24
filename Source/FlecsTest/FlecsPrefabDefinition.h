// Copyright 2021 Red J

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InstancedStruct.h"
#include "FlecsComponents.h"
#include "../../FlecsLibrary/Public/flecs.h"
#include "FlecsPrefabDefinition.generated.h"

/*
#pragma region Components
USTRUCT(Blueprintable)
struct UNREALFLECS_API FFLECSGenericComp
{
	GENERATED_BODY()
};

// General components
USTRUCT()
struct UNREALFLECS_API FLocationComp : public FFLECSGenericComp
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;
};

USTRUCT()
struct UNREALFLECS_API FVelocityComp : public FFLECSGenericComp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Velocity = FVector(100, 0, 0);
};

USTRUCT()
struct UNREALFLECS_API FLocalAccelerationComp : public FFLECSGenericComp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Acceleration = FVector(100, 0, 0);
};

USTRUCT()
struct UNREALFLECS_API FGravityAccelerationComp : public FFLECSGenericComp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Gravity = -980;
};

USTRUCT()
struct UNREALFLECS_API FMassComp : public FFLECSGenericComp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float Mass = 1;
};

USTRUCT()
struct UNREALFLECS_API FRotationComp : public FFLECSGenericComp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation = FRotator::ZeroRotator;
};

USTRUCT()
struct UNREALFLECS_API FDragComp : public FFLECSGenericComp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Drag = 0;
};

USTRUCT()
struct UNREALFLECS_API FMaxSpeedComp : public FFLECSGenericComp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpeed = 1000;
};

// Generic Events
USTRUCT()
struct UNREALFLECS_API FGenericEvent : public FFLECSGenericComp
{
	GENERATED_BODY()
};

USTRUCT()
struct UNREALFLECS_API FTimerEventComp : public FGenericEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float TimeSeconds = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLoop = false;


	// Instanced struct FGeneric Event
	UPROPERTY(EditDefaultsOnly, meta = (ExcludeBaseStruct, BaseStruct = "/Script/UnrealFlecs.GenericEvent"))
	TArray<FInstancedStruct> EventsToTrigger;
};

USTRUCT()
struct UNREALFLECS_API FMarkAsGarbageComp : public FGenericEvent
{
	GENERATED_BODY()
};

// Spawn Events
USTRUCT()
struct UNREALFLECS_API FSpawnEvent : public FGenericEvent
{
	GENERATED_BODY()
};

USTRUCT()
struct UNREALFLECS_API FEntitySpawnerComp : public FSpawnEvent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UFlecsPrefabDefinition>> PrefabToSpawn;
};

USTRUCT()
struct UNREALFLECS_API FFXSpawnerComp : public FSpawnEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> FXToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> SoundToSpawn;
};

// Collision 

USTRUCT(BlueprintType)
struct UNREALFLECS_API FProjectileCollisionModule
{
	GENERATED_BODY();
};

USTRUCT()
struct UNREALFLECS_API FProjectileCollisionResponseComp : public FProjectileCollisionModule
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (ExcludeBaseStruct, BaseStruct = "/Script/UnrealFlecs.GenericEvent"))
	TArray<FInstancedStruct> EventsToTrigger;

	UPROPERTY(EditDefaultsOnly, meta = (ExcludeBaseStruct, BaseStruct = "/Script/UnrealFlecs.ImpactEffectComp"))
	TArray<FInstancedStruct> ImpactEffects;
};

USTRUCT()
struct UNREALFLECS_API FProjectileCollisionComp : public FProjectileCollisionModule
{
	GENERATED_BODY()

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(0.5);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ECollisionChannel> TraceChannel;

	UPROPERTY(EditDefaultsOnly, meta = (BaseStruct = "/Script/UnrealFlecs.ProjectileCollisionResponseComp"))
	TArray<FInstancedStruct> CollisionResponses;
};

USTRUCT()
struct UNREALFLECS_API FProjectileCollisionManagerComp : public FFLECSGenericComp
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (BaseStruct = "/Script/UnrealFlecs.ProjectileCollisionComp"))
	TArray<FInstancedStruct> Collisions;
};

//FImpactEffectComp
USTRUCT(BlueprintType)
struct UNREALFLECS_API FImpactEffect
{
	GENERATED_BODY()
};

USTRUCT()
struct UNREALFLECS_API FDamageComp : public FImpactEffect
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float Damage = 1;
};

USTRUCT()
struct UNREALFLECS_API FAreaOfEffectDamageComp : public FDamageComp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplosiveRadius = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplosiveInnerRadius = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageFallOff = 1;
};

#pragma endregion
*/
UCLASS()
class FLECSTEST_API UFlecsPrefabDefinition : public UDataAsset
{
    GENERATED_BODY()
       
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UFlecsPrefabDefinition> ParentPrefab = nullptr;
    
    UPROPERTY(EditDefaultsOnly)
    FFLECSPrefab Prefab;

public: 
	FFLECSPrefab GetPrefab() const { return Prefab; }

};
