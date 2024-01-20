#pragma once
#include "flecs.h"
#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FlecsSubsystem.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class APlayerController;

USTRUCT(BlueprintType)
struct FFlecsEntityHandle
{
	GENERATED_USTRUCT_BODY()
		FFlecsEntityHandle() {}
	FFlecsEntityHandle(int inId)
	{
		FFlecsEntityId = inId;
	}

	UPROPERTY(BlueprintReadWrite)
	int FFlecsEntityId;
};

struct FFlecsPlayerControllerRef
{
	APlayerController* Value;
};

struct FFlecsTeamID
{
	int Value;
};

struct FFlecsProjectile {};

struct FFlecksBaseStruct {};
/*
#pragma region Generic Collision Response
UClASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class FLECSTEST_API UFlecsGenericCollisionResponse : public UObject
{
	GENERATED_BODY()

public:
	void Activate() { ; };
};

// Explode on Death
UCLASS()
class FLECSTEST_API UFlecsGenericCollisionResponseDeath : public UFlecsGenericCollisionResponse
{
	GENERATED_BODY()

};

// Spawn VFX generic 
UCLASS()
class FLECSTEST_API UFlecsGenericCollisionResponseVFX : public UFlecsGenericCollisionResponse
{
	GENERATED_BODY()

public:
	void SpawnVFX(USceneComponent* HitComponent, FVector Location, FRotator Rotation)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* VFX = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* SFX = nullptr;
};
*/

#pragma region Collision
UCLASS(Abstract, Blueprintable, BlueprintType)
class FLECSTEST_API UFlecsCollisionResponse : public UObject
{
	GENERATED_BODY()
};

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class FLECSTEST_API UFlecsCollisionResponseBounce : public UObject
{
	GENERATED_BODY()

public:
	//Bounce properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Bounciness = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinBounceSpeed = 50.0f;

};
#pragma endregion

UCLASS(Abstract, Blueprintable, BlueprintType)
class FLECSTEST_API UFlecsCollisionDetection : public UObject
{
	GENERATED_BODY()

public:
    // TArray of collision responses 

	UFUNCTION(BlueprintNativeEvent)
	void OnCollisionDetected(FFlecsEntityHandle HitEntity, FHitResult CollisionResult);
	void OnCollisionDetected_Implementation(FFlecsEntityHandle HitEntity, FHitResult CollisionResult)
	{

	};
};

struct FFlecsCollisionComponentAndResponse
{
	FCollisionShape Shape;
	ECollisionChannel Channel;

	FFlecsCollisionComponentAndResponse() : Shape(FCollisionShape::MakeSphere(0)), Channel(ECC_Visibility) {}

	FFlecsCollisionComponentAndResponse(FCollisionShape inShape, ECollisionChannel inChannel) : Shape(inShape), Channel(inChannel) {}
};

struct FFlecsCollisionClass
{
	FFlecsEntityHandle Owner;
	TArray<FFlecsCollisionComponentAndResponse> CollisionComponents;

	FFlecsCollisionClass(TArray<FFlecsCollisionComponentAndResponse> inCollisionComponents, FFlecsEntityHandle inOwner) : CollisionComponents(inCollisionComponents), Owner(inOwner) {}

	FFlecsCollisionClass() { ; }
};

struct FFlecsVFXRef
{
	UNiagaraComponent* Value;

	FFlecsVFXRef() : Value(nullptr) {}

	FFlecsVFXRef(UNiagaraComponent* inValue) : Value(inValue) {}
};

struct FFlecsSFXRef
{
	UAudioComponent* Value;
};

struct FFlecsLocation
{
	FVector Value;

	FFlecsLocation() : Value(FVector::ZeroVector) {}

	FFlecsLocation(FVector inValue) : Value(inValue) {}
};

struct FFlecsRotation
{
	FRotator Value;
};

struct FFlecsVelocity
{
	FVector Value;

	FFlecsVelocity() : Value(FVector::ZeroVector) {}

	FFlecsVelocity(FVector inValue) : Value(inValue) {}
};

struct FFlecsWorldAcceleration
{
	FVector Value;
};

struct FFlecsLocalAcceleration
{
	FVector Value;
};

/*
struct FFlecsISMIndex
{
	int Value;
};

struct FFlecsIsmRef
{
	UInstancedStaticMeshComponent* Value;
};

struct FFlecsCorn
{
	float Growth;
};
struct Corns {};
*/

UCLASS()
class FLECSTEST_API UFlecsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	flecs::world* GetEcsWorld() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* ProjectileVFX = nullptr;

	UFUNCTION(BlueprintCallable, Category = "FLECS")
	FFlecsEntityHandle SpawnProjectileEntity(APlayerController* PlayerController, FVector Location, FRotator Rotation, FVector MuzzleLocation);

	UFUNCTION(BlueprintCallable, Category = "FLECS")
	FFlecsEntityHandle SpawnProjectileEntitySpecific(APlayerController* PlayerController, FVector Location, FRotator Rotation, FVector MuzzleLocation, UNiagaraSystem* ProjectileSystemVFX, float Radius, float Speed);

protected:
	FTickerDelegate OnTickDelegate;
	FTSTicker::FDelegateHandle OnTickHandle;
	flecs::world* ECSWorld = nullptr;
private:
	bool Tick(float DeltaTime);

	void SweepMovement(FFlecsLocation& FFLoc, FFlecsCollisionClass& FFCol, FFlecsVelocity& FFVel);
	void UpdateVFX(FFlecsLocation& FFLoc, FFlecsVelocity& FFVel, FFlecsVFXRef& FFVFXRef);

	flecs::query<FFlecsLocation, FFlecsCollisionClass, FFlecsVelocity> FFlecsQuerySweepMovement;
	flecs::query<FFlecsVelocity, FFlecsLocalAcceleration> FFlecsQueryLocalAccelerationForce;
	flecs::query<FFlecsVelocity, FFlecsWorldAcceleration> FFlecsQueryWorldAccelerationForce;
	flecs::query<FFlecsLocation, FFlecsVelocity, FFlecsVFXRef> FFlecsQueryVFX;
	flecs::query<FFlecsLocation, FFlecsVelocity, FFlecsSFXRef> FFlecsQuerySFX;
};
