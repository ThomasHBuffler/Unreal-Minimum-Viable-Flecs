// Copyright 2021 Red J

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InstancedStruct.h"
#include "NiagaraSystem.h"
#include "flecs.h"
#include "UObject/Class.h"
#include "UObject/ScriptMacros.h"
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

#pragma region Fragments
USTRUCT(Blueprintable)
struct FLECSTEST_API FFLECSGenericFragment
{
	GENERATED_BODY()

};

UCLASS(Abstract, Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew, meta = ( DisplayName = "FLECS Generic Fragment" ))
class FLECSTEST_API UFLECSGenericFragmentProxy : public UObject
{
	GENERATED_BODY()


public:

	virtual UScriptStruct* GetScriptStruct()
	{
		return nullptr;
	}

	virtual void* GetMemory()
	{
		return nullptr;
	}
protected:
};

// General Fragments
USTRUCT()
struct FLECSTEST_API FLocationFragment : public FFLECSGenericFragment
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		FVector Location = FVector::ZeroVector;

	FLocationFragment() {}

	FLocationFragment(FVector InLocation)
	{
		Location = InLocation;
	}
};

UCLASS()
class FLECSTEST_API ULocationFragmentProxy : public UFLECSGenericFragmentProxy
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FLocationFragment LocationValue;

	virtual void* GetMemory() override
	{
		return (void*)&LocationValue;
	}

protected:
	virtual UScriptStruct* GetScriptStruct() override
	{
		return FLocationFragment::StaticStruct();
	}
};

USTRUCT(Blueprintable)
struct FLECSTEST_API FOwnerEntity : public FFLECSGenericFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
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
struct FLECSTEST_API FOwnerActor : public FFLECSGenericFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	AActor* Owner;

	FOwnerActor() {}

	FOwnerActor(AActor* InOwner)
	{
		Owner = InOwner;
	}
};

USTRUCT(Blueprintable)
struct FLECSTEST_API FVisualEffect : public FFLECSGenericFragment
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		class UNiagaraComponent* VisualEffect;

	FVisualEffect() {}

	FVisualEffect(class UNiagaraComponent* InComponent)
	{
		VisualEffect = InComponent;
	}
};

USTRUCT(Blueprintable)
struct FLECSTEST_API FSoundEffect : public FFLECSGenericFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAudioComponent* SoundEffect;

	FSoundEffect() {}

	FSoundEffect(class UAudioComponent* InComponent)
	{
		SoundEffect = InComponent;
	}
};

/*
UCLASS()
class UVisualEffectProxy : public UFLECSGenericFragmentProxy
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UNiagaraSystem VisualEffect;

	virtual void* GetMemory() override
	{
		return nullptr;
	}
protected:
	virtual UScriptStruct* GetScriptStruct() override
	{
		return FVisualEffect::StaticStruct();
	}
};
*/

USTRUCT()
struct FLECSTEST_API FRotationFragment : public FFLECSGenericFragment
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		FRotator Rotation = FRotator::ZeroRotator;

	FRotationFragment() {}

	FRotationFragment(FRotator InRotation)
	{
		Rotation = InRotation;
	}
};

USTRUCT()
struct FLECSTEST_API FVelocityFragment : public FFLECSGenericFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector Velocity = FVector(100, 0, 0);

	FVelocityFragment() {}

	FVelocityFragment(FVector InVelocity)
	{
		Velocity = InVelocity;
	}
};

UCLASS()
class FLECSTEST_API UVelocityFragmentProxy : public UFLECSGenericFragmentProxy
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVelocityFragment VelocityValue;

	virtual void* GetMemory() override
	{
		return (void*)&VelocityValue;
	}

protected:
	virtual UScriptStruct* GetScriptStruct() override
	{
		return FVelocityFragment::StaticStruct();
	}
};

USTRUCT()
struct FLECSTEST_API FLocalAccelerationFragment : public FFLECSGenericFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector Acceleration = FVector(100, 0, 0);

	FLocalAccelerationFragment() {}

	FLocalAccelerationFragment(FVector InAcceleration)
	{
		Acceleration = InAcceleration;
	}
};

UCLASS()
class FLECSTEST_API ULocalAccelerationFragmentProxy : public UFLECSGenericFragmentProxy
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	FLocalAccelerationFragment AccelerationValue;
	virtual void* GetMemory() override
	{
		return (void*)&AccelerationValue;
	}

protected:

virtual UScriptStruct* GetScriptStruct() override
	{
		return FLocalAccelerationFragment::StaticStruct();
	}
};

USTRUCT()
struct FLECSTEST_API FGravityAccelerationFragment : public FFLECSGenericFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Gravity = -980;

	FGravityAccelerationFragment() {}

	FGravityAccelerationFragment(float InGravity)
	{
		Gravity = InGravity;
	}
};

UCLASS()
class FLECSTEST_API UGravityAccelerationFragmentProxy : public UFLECSGenericFragmentProxy
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	FGravityAccelerationFragment GravityValue;
	virtual void* GetMemory() override
	{
		return (void*)&GravityValue;
	}

protected:

	virtual UScriptStruct* GetScriptStruct() override
	{
		return FGravityAccelerationFragment::StaticStruct();
	}
};

USTRUCT()
struct FLECSTEST_API FMassFragment : public FFLECSGenericFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0"))
	float Mass = 1;

	FMassFragment() {}

	FMassFragment(float InMass)
	{
		Mass = InMass;
	}
};

UCLASS()
class FLECSTEST_API UMassFragmentProxy : public UFLECSGenericFragmentProxy
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	FMassFragment MassValue;
	virtual void* GetMemory() override
	{
		return (void*)&MassValue;
	}

protected:

	virtual UScriptStruct* GetScriptStruct() override
	{
		return FMassFragment::StaticStruct();
	}
};


USTRUCT()
struct FLECSTEST_API FDragFragment : public FFLECSGenericFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Drag = 0;

	FDragFragment() {}

	FDragFragment(float InDrag)
	{
		Drag = InDrag;
	}
};

UCLASS()
class FLECSTEST_API UDragFragmentProxy : public UFLECSGenericFragmentProxy
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FDragFragment DragValue;

	virtual void* GetMemory() override
	{
		return (void*)&DragValue;
	}

protected: 

virtual UScriptStruct* GetScriptStruct() override
	{
		return FDragFragment::StaticStruct();
	}
};

USTRUCT()
struct FLECSTEST_API FMaxSpeedFragment : public FFLECSGenericFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float MaxSpeed = 1000;

	FMaxSpeedFragment() {}

	FMaxSpeedFragment(float InMaxSpeed)
	{
		MaxSpeed = InMaxSpeed;
	}
};

UCLASS()
class FLECSTEST_API UMaxSpeedFragmentProxy : public UFLECSGenericFragmentProxy
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FMaxSpeedFragment MaxSpeedValue;
	virtual void* GetMemory() override
	{
		return (void*)&MaxSpeedValue;
	}

protected:
	virtual UScriptStruct* GetScriptStruct() override
	{
		return FMaxSpeedFragment::StaticStruct();
	}
};

/*
USTRUCT()
struct FLECSTEST_API FProjectileEvent : public FFLECSGenericFragment
{
	GENERATED_BODY()

	FProjectileEvent() {}
};

USTRUCT()
struct FLECSTEST_API FMarkForDelete : public FProjectileEvent
{
	GENERATED_BODY()

	FMarkForDelete() {}
};
*/

// Generic Events
USTRUCT()
struct FLECSTEST_API FGenericEvent : public FFLECSGenericFragment
{
	GENERATED_BODY()

	FGenericEvent() {}
};

USTRUCT()
struct FLECSTEST_API FTimerEventFragment : public FGenericEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0"))
	float TimeSeconds = 1;

	UPROPERTY(EditAnywhere)
	bool bLoop = false;

	// Instanced struct FGeneric Event
	UPROPERTY(EditDefaultsOnly, meta = (ExcludeBaseStruct, BaseStruct = "/Script/UnrealFlecs.GenericEvent"))
		TArray<FInstancedStruct> EventsToTrigger;

	FTimerEventFragment() {}

};

USTRUCT()
struct FLECSTEST_API FMarkAsGarbage : public FGenericEvent
{
	GENERATED_BODY()

	FMarkAsGarbage() {}
};

// Spawn Events
USTRUCT()
struct FLECSTEST_API FSpawnEvent : public FGenericEvent
{
	GENERATED_BODY()
};

USTRUCT()
struct FLECSTEST_API FEntitySpawnerFragment : public FSpawnEvent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UFlecsPrefabDefinition>> PrefabToSpawn;

	FEntitySpawnerFragment() {}

	FEntitySpawnerFragment(TArray<TObjectPtr<UFlecsPrefabDefinition>> InPrefabToSpawn)
	{
		PrefabToSpawn = InPrefabToSpawn;
	}
};

USTRUCT()
struct FLECSTEST_API FFXSpawnerFragment : public FSpawnEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> FXToSpawn;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> SoundToSpawn;

	FFXSpawnerFragment() {}

	FFXSpawnerFragment(TObjectPtr<UNiagaraSystem> InFXToSpawn, TObjectPtr<USoundBase> InSoundToSpawn)
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

USTRUCT(BlueprintType)
struct FLECSTEST_API FImpactEffectFragment
{
	GENERATED_BODY()

		FImpactEffectFragment() {}
};

USTRUCT()
struct FLECSTEST_API FDamageFragment : public FImpactEffectFragment
{
	GENERATED_BODY()

		UPROPERTY(EditDefaultsOnly)
		float Damage = 1;

	FDamageFragment() {}

	FDamageFragment(float InDamage)
	{
		Damage = InDamage;
	}
};

/*
USTRUCT()
struct FLECSTEST_API FProjectileCollisionResponseFragmentProxy : public FProjectileCollisionModule
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (ExcludeBaseStruct, BaseStruct = "/Script/UnrealFlecs.GenericEvent"))
	TArray<FInstancedStruct> EventsToTrigger;

	UPROPERTY(EditDefaultsOnly, meta = (ExcludeBaseStruct, BaseStruct = "/Script/UnrealFlecs.ImpactEffectFragment"))
	TArray<FInstancedStruct> ImpactEffects;

	// Max number of bounces, 0 = no bounce
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0")) 
	int Bounces = 0;

	// Bounciness, 0 = no bounce, 1 = full bounce
	UPROPERTY(EditAnywhere,  meta = (EditCondition = "Bounces > 0", EditConditionHides))
	float BounceFactor = 0.5;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "Bounces > 0", EditConditionHides), meta = (ExcludeBaseStruct, BaseStruct = "/Script/UnrealFlecs.GenericEvent"))
	TArray<FInstancedStruct> BounceEventsToTrigger;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "Bounces > 0", EditConditionHides), meta = (ExcludeBaseStruct, BaseStruct = "/Script/UnrealFlecs.ImpactEffectFragment"))
	TArray<FInstancedStruct> BounceImpactEffects;

	FProjectileCollisionResponseFragmentProxy() {}

	FProjectileCollisionResponseFragmentProxy(TArray<FInstancedStruct> InEventsToTrigger, TArray<FInstancedStruct> InImpactEffects, int InBounces, float InBounceFactor, TArray<FInstancedStruct> InBounceEventsToTrigger, TArray<FInstancedStruct> InBounceImpactEffects)
	{
		EventsToTrigger = InEventsToTrigger;
		ImpactEffects = InImpactEffects;
		Bounces = InBounces;
		BounceFactor = InBounceFactor;
		BounceEventsToTrigger = InBounceEventsToTrigger;
		BounceImpactEffects = InBounceImpactEffects;
	}
};
*/

USTRUCT()
struct FLECSTEST_API FProjectileCollisionResponseFragment : public FProjectileCollisionModule
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (ExcludeBaseStruct, BaseStruct = "/Script/UnrealFlecs.GenericEvent"))
	TArray<FInstancedStruct> EventsToTrigger;

	UPROPERTY(EditDefaultsOnly, meta = (ExcludeBaseStruct, BaseStruct = "/Script/UnrealFlecs.GenericEvent"))
	TArray<FInstancedStruct> TagsToAdd;

	UPROPERTY(EditDefaultsOnly, meta = (ExcludeBaseStruct, BaseStruct = "/Script/UnrealFlecs.ImpactEffectFragment"))
	TArray<FInstancedStruct> ImpactEffects;

	// Max number of bounces, 0 = no bounce
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	int Bounces = 0;

	// Bounciness, 0 = no bounce, 1 = full bounce
	UPROPERTY(EditAnywhere, meta = (EditCondition = "Bounces > 0", EditConditionHides))
	float BounceFactor = 0.5;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "Bounces > 0", EditConditionHides, ExcludeBaseStruct, BaseStruct = "/Script/UnrealFlecs.GenericEvent"))
	TArray<FInstancedStruct> BounceEventsToTrigger;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "Bounces > 0", EditConditionHides, ExcludeBaseStruct, BaseStruct = "/Script/UnrealFlecs.GenericEvent"))
	TArray<FInstancedStruct> BounceTagsToAdd;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "Bounces > 0", EditConditionHides, ExcludeBaseStruct, BaseStruct = "/Script/UnrealFlecs.ImpactEffectFragment"))
	TArray<FInstancedStruct> BounceImpactEffects;

	FProjectileCollisionResponseFragment() {}

	FProjectileCollisionResponseFragment(TArray<FInstancedStruct> InEventsToTrigger, TArray<FInstancedStruct> InImpactEffects, int InBounces, float InBounceFactor, TArray<FInstancedStruct> InBounceEventsToTrigger, TArray<FInstancedStruct> InBounceImpactEffects)
	{
		EventsToTrigger = InEventsToTrigger;
		ImpactEffects = InImpactEffects;
		Bounces = InBounces;
		BounceFactor = InBounceFactor;
		BounceEventsToTrigger = InBounceEventsToTrigger;
		BounceImpactEffects = InBounceImpactEffects;
	}
};

UCLASS()
class FLECSTEST_API UProjectileCollisionResponseFragmentProxy : public UFLECSGenericFragmentProxy
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FProjectileCollisionResponseFragment CollisionResponseValue;
	
	
	virtual void* GetMemory() override
	{
		return (void*)&CollisionResponseValue;
	}

protected:
	virtual UScriptStruct* GetScriptStruct() override
	{
		return FProjectileCollisionResponseFragment::StaticStruct();
	}
};

/*
USTRUCT()
struct FLECSTEST_API FProjectileCollisionFragmentProxy : public FProjectileCollisionModule
{
	GENERATED_BODY()

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(0.5);

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> TraceChannel;

	UPROPERTY(EditDefaultsOnly, meta = (BaseStruct = "/Script/UnrealFlecs.ProjectileCollisionResponseFragment"))
	TArray<FInstancedStruct> CollisionResponses;

	FProjectileCollisionFragmentProxy() {}

	FProjectileCollisionFragmentProxy(FCollisionShape InCollisionShape, TEnumAsByte<ECollisionChannel> InTraceChannel, TArray<FInstancedStruct> InCollisionResponses)
	{
		CollisionShape = InCollisionShape;
		TraceChannel = InTraceChannel;
		CollisionResponses = InCollisionResponses;
	}
};
*/

USTRUCT()
struct FLECSTEST_API FProjectileCollisionFragment : public FProjectileCollisionModule
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Radius = 0.5;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> TraceChannel;

	UPROPERTY(EditDefaultsOnly)
	TArray<FProjectileCollisionResponseFragment> CollisionResponses;

	FProjectileCollisionFragment() {}

	FProjectileCollisionFragment(float InRadius, TEnumAsByte<ECollisionChannel> InTraceChannel, TArray<FProjectileCollisionResponseFragment> InCollisionResponses)
	{
		Radius = InRadius;
		TraceChannel = InTraceChannel;
		CollisionResponses = InCollisionResponses;
	}
};

UCLASS()
class FLECSTEST_API UProjectileCollisionFragmentProxy : public UFLECSGenericFragmentProxy
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FProjectileCollisionFragment CollisionValue;

	virtual void* GetMemory() override
	{
		return (void*)&CollisionValue;
	}

protected:

	virtual UScriptStruct* GetScriptStruct() override
	{
		return FProjectileCollisionFragment::StaticStruct();
	}
};

USTRUCT()
struct FLECSTEST_API FProjectileCollisionManagerFragmentProxy : public FFLECSGenericFragment
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (BaseStruct = "/Script/UnrealFlecs.ProjectileCollisionFragment"))
	TArray<FInstancedStruct> Collisions;

	FProjectileCollisionManagerFragmentProxy() {}

	FProjectileCollisionManagerFragmentProxy(TArray<FInstancedStruct> InCollisions)
	{
		Collisions = InCollisions;
	}
};

USTRUCT()
struct FLECSTEST_API FProjectileCollisionManagerFragment : public FFLECSGenericFragment
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FProjectileCollisionFragment> Collisions;
	
	FProjectileCollisionManagerFragment() {}

	FProjectileCollisionManagerFragment(TArray<FProjectileCollisionFragment> InCollisions)
	{
		Collisions = InCollisions;
	}
};

UCLASS()
class FLECSTEST_API UProjectileCollisionManagerProxy : public UFLECSGenericFragmentProxy
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FProjectileCollisionManagerFragment ProjectileCollisionManagerValue;

	virtual void* GetMemory() override
	{
		return (void*)&ProjectileCollisionManagerValue;
	}

protected:
	virtual UScriptStruct* GetScriptStruct() override
	{
		return FProjectileCollisionManagerFragment::StaticStruct();
	}
};

USTRUCT()
struct FLECSTEST_API FAreaOfEffectDamageFragment : public FDamageFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float ExplosiveRadius = 500;

	UPROPERTY(EditAnywhere)
	float ExplosiveInnerRadius = 100;

	UPROPERTY(EditAnywhere)
	float DamageFallOff = 1;

	FAreaOfEffectDamageFragment() {}

	FAreaOfEffectDamageFragment(float InDamage, float InExplosiveRadius, float InExplosiveInnerRadius, float InDamageFallOff)
	{
		Damage = InDamage;
		ExplosiveRadius = InExplosiveRadius;
		ExplosiveInnerRadius = InExplosiveInnerRadius;
		DamageFallOff = InDamageFallOff;
	}
};

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
	UPROPERTY(EditDefaultsOnly, meta = (ExcludeBaseStruct, BaseStruct = "/Script/UnrealFlecs.FLECSGenericFragment"))
	TArray<FInstancedStruct> Fragments;

public: 
	FFLECSPrefab() {}	
};


USTRUCT()
struct FLECSTEST_API FFLECSPrefabClassType
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UFLECSGenericFragmentProxy*> Fragments;

public:
	FFLECSPrefabClassType() {}
};