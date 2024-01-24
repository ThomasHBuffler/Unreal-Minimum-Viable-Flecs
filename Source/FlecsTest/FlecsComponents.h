// Copyright 2021 Red J

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InstancedStruct.h"
#include "NiagaraSystem.h"
#include "flecs.h"
#include "UObject/Class.h"
#include "FlecsComponents.generated.h"

class UFlecsPrefabDefinition;

USTRUCT(BlueprintType)
struct FFlecsEntityHandle
{
	GENERATED_USTRUCT_BODY()

	FFlecsEntityHandle() {}

	FFlecsEntityHandle(int inId)
	{
		FlecsEntityId = inId;
	}
	UPROPERTY(BlueprintReadWrite)
	int FlecsEntityId;
};

#pragma region Components
USTRUCT(Blueprintable)
struct FLECSTEST_API FFLECSGenericComp
{
	GENERATED_BODY()
};

USTRUCT(Blueprintable)
struct FLECSTEST_API FOwnerEntity : public FFLECSGenericComp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFlecsEntityHandle Owner;

	flecs::entity OwnerEntity;

	FOwnerEntity() {}

	FOwnerEntity(FFlecsEntityHandle InOwner, flecs::entity InOwnerEntity)
	{
		Owner = InOwner;
		OwnerEntity = InOwnerEntity;
	}

};

USTRUCT(Blueprintable)
struct FLECSTEST_API FOwnerActor : public FFLECSGenericComp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Owner;

	FOwnerActor() {}

	FOwnerActor(AActor* InOwner)
	{
		Owner = InOwner;
	}
};

// General components
USTRUCT()
struct FLECSTEST_API FLocationComp : public FFLECSGenericComp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;

	FLocationComp() {}

	FLocationComp(FVector InLocation)
	{
		Location = InLocation;
	}
};

USTRUCT()
struct FLECSTEST_API FRotationComp : public FFLECSGenericComp
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator Rotation = FRotator::ZeroRotator;

	FRotationComp() {}

	FRotationComp(FRotator InRotation)
	{
		Rotation = InRotation;
	}
};

USTRUCT()
struct FLECSTEST_API FVelocityComp : public FFLECSGenericComp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Velocity = FVector(100, 0, 0);

	FVelocityComp() {}

	FVelocityComp(FVector InVelocity)
	{
		Velocity = InVelocity;
	}
};

USTRUCT()
struct FLECSTEST_API FLocalAccelerationComp : public FFLECSGenericComp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Acceleration = FVector(100, 0, 0);

	FLocalAccelerationComp() {}

	FLocalAccelerationComp(FVector InAcceleration)
	{
		Acceleration = InAcceleration;
	}
};

USTRUCT()
struct FLECSTEST_API FGravityAccelerationComp : public FFLECSGenericComp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Gravity = -980;

	FGravityAccelerationComp() {}

	FGravityAccelerationComp(float InGravity)
	{
		Gravity = InGravity;
	}
};

USTRUCT()
struct FLECSTEST_API FMassComp : public FFLECSGenericComp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float Mass = 1;

	FMassComp() {}

	FMassComp(float InMass)
	{
		Mass = InMass;
	}
};

USTRUCT()
struct FLECSTEST_API FDragComp : public FFLECSGenericComp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Drag = 0;

	FDragComp() {}

	FDragComp(float InDrag)
	{
		Drag = InDrag;
	}
};

USTRUCT()
struct FLECSTEST_API FMaxSpeedComp : public FFLECSGenericComp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpeed = 1000;

	FMaxSpeedComp() {}

	FMaxSpeedComp(float InMaxSpeed)
	{
		MaxSpeed = InMaxSpeed;
	}
};

// Generic Events
USTRUCT()
struct FLECSTEST_API FGenericEvent : public FFLECSGenericComp
{
	GENERATED_BODY()

	FGenericEvent() {}
};

USTRUCT()
struct FLECSTEST_API FTimerEventComp : public FGenericEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float TimeSeconds = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLoop = false;

	// Instanced struct FGeneric Event
	UPROPERTY(EditDefaultsOnly, meta = (ExcludeBaseStruct, BaseStruct = "/Script/UnrealFlecs.GenericEvent"))
	TArray<FInstancedStruct> EventsToTrigger;

	FTimerEventComp() {}
	/*
	FTimerEventComp(float InTimeSeconds, bool InbLoop, TArray<FInstancedStruct> InEventsToTrigger)
	{
		TimeSeconds = InTimeSeconds;
		bLoop = InbLoop;
		EventsToTrigger = InEventsToTrigger;
	}*/
};

USTRUCT()
struct FLECSTEST_API FAliveStatus : public FGenericEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAlive = true;

	FAliveStatus() {}

	FAliveStatus(bool InbIsAlive)
	{
		bIsAlive = InbIsAlive;
	}
};

// Spawn Events
USTRUCT()
struct FLECSTEST_API FSpawnEvent : public FGenericEvent
{
	GENERATED_BODY()
};

USTRUCT()
struct FLECSTEST_API FEntitySpawnerComp : public FSpawnEvent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UFlecsPrefabDefinition>> PrefabToSpawn;

	FEntitySpawnerComp() {}

	FEntitySpawnerComp(TArray<TObjectPtr<UFlecsPrefabDefinition>> InPrefabToSpawn)
	{
		PrefabToSpawn = InPrefabToSpawn;
	}
};

USTRUCT()
struct FLECSTEST_API FFXSpawnerComp : public FSpawnEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> FXToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> SoundToSpawn;

	FFXSpawnerComp() {}

	FFXSpawnerComp(TObjectPtr<UNiagaraSystem> InFXToSpawn, TObjectPtr<USoundBase> InSoundToSpawn)
	{
		FXToSpawn = InFXToSpawn;
		SoundToSpawn = InSoundToSpawn;
	}
};

// Collision 
USTRUCT(BlueprintType)
struct FLECSTEST_API FProjectileCollisionModule
{
	GENERATED_BODY();
};

USTRUCT()
struct FLECSTEST_API FProjectileCollisionResponseComp : public FProjectileCollisionModule
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (ExcludeBaseStruct, BaseStruct = "/Script/UnrealFlecs.GenericEvent"))
	TArray<FInstancedStruct> EventsToTrigger;

	UPROPERTY(EditDefaultsOnly, meta = (ExcludeBaseStruct, BaseStruct = "/Script/UnrealFlecs.ImpactEffectComp"))
	TArray<FInstancedStruct> ImpactEffects;

	FProjectileCollisionResponseComp() {}

	FProjectileCollisionResponseComp(TArray<FInstancedStruct> InEventsToTrigger, TArray<FInstancedStruct> InImpactEffects)
	{
		EventsToTrigger = InEventsToTrigger;
		ImpactEffects = InImpactEffects;
	}
};

USTRUCT()
struct FLECSTEST_API FProjectileCollisionComp : public FProjectileCollisionModule
{
	GENERATED_BODY()

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(0.5);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ECollisionChannel> TraceChannel;

	UPROPERTY(EditDefaultsOnly, meta = (BaseStruct = "/Script/UnrealFlecs.ProjectileCollisionResponseComp"))
	TArray<FInstancedStruct> CollisionResponses;

	FProjectileCollisionComp() {}

	FProjectileCollisionComp(FCollisionShape InCollisionShape, TEnumAsByte<ECollisionChannel> InTraceChannel, TArray<FInstancedStruct> InCollisionResponses)
	{
		CollisionShape = InCollisionShape;
		TraceChannel = InTraceChannel;
		CollisionResponses = InCollisionResponses;
	}
};

USTRUCT()
struct FLECSTEST_API FProjectileCollisionManagerComp : public FFLECSGenericComp
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (BaseStruct = "/Script/UnrealFlecs.ProjectileCollisionComp"))
	TArray<FInstancedStruct> Collisions;

	FProjectileCollisionManagerComp() {}

	FProjectileCollisionManagerComp(TArray<FInstancedStruct> InCollisions)
	{
		Collisions = InCollisions;
	}
	/*
	FProjectileCollisionManagerComp(FProjectileCollisionComp InCollisions)
	{
		FInstancedStruct::Make<FProjectileCollisionComp>(Collisions, InCollisions);
	}*/
};

//FImpactEffectComp
USTRUCT(BlueprintType)
struct FLECSTEST_API FImpactEffect
{
	GENERATED_BODY()

	FImpactEffect() {}
};

USTRUCT()
struct FLECSTEST_API FDamageComp : public FImpactEffect
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float Damage = 1;

	FDamageComp() {}

	FDamageComp(float InDamage)
	{
		Damage = InDamage;
	}
};

USTRUCT()
struct FLECSTEST_API FAreaOfEffectDamageComp : public FDamageComp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplosiveRadius = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplosiveInnerRadius = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageFallOff = 1;

	FAreaOfEffectDamageComp() {}

	FAreaOfEffectDamageComp(float InDamage, float InExplosiveRadius, float InExplosiveInnerRadius, float InDamageFallOff)
	{
		Damage = InDamage;
		ExplosiveRadius = InExplosiveRadius;
		ExplosiveInnerRadius = InExplosiveInnerRadius;
		DamageFallOff = InDamageFallOff;
	}
};

// Tags
USTRUCT(BlueprintType)
struct FLECSTEST_API FProjectile
{
	GENERATED_BODY()
};

#pragma endregion

USTRUCT()
struct FLECSTEST_API FFLECSPrefab
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, meta = (ExcludeBaseStruct, BaseStruct = "/Script/UnrealFlecs.FLECSGenericComp"))
	TArray<FInstancedStruct> Components;

public: 
	FFLECSPrefab() {}	
};