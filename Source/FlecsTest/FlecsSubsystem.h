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
		FlecsEntityId = inId;
	}

	UPROPERTY(BlueprintReadWrite)
	int FlecsEntityId;
};

struct FlecsPlayerControllerRef
{
	APlayerController* Value;
};

struct FlecsTeamID
{
	int Value;
};

struct FlecsProjectile {};

struct FlecksBaseStruct {};

struct FlecsCollisionComponent
{
	FCollisionShape Shape;
	FFlecsEntityHandle Owner;

	FlecsCollisionComponent() : Shape(FCollisionShape::MakeSphere(0)), Owner(0) {}

	FlecsCollisionComponent(FCollisionShape inShape, FFlecsEntityHandle inOwner) : Shape(inShape), Owner(inOwner) {}
};

struct FlecsVFXRef
{
	UNiagaraComponent* Value;

	FlecsVFXRef() : Value(nullptr) {}

	FlecsVFXRef(UNiagaraComponent* inValue) : Value(inValue) {}
};

struct FlecsSFXRef
{
	UAudioComponent* Value;
};

struct FlecsLocation
{
	FVector Value;

	FlecsLocation() : Value(FVector::ZeroVector) {}

	FlecsLocation(FVector inValue) : Value(inValue) {}
};

struct FlecsRotation
{
	FRotator Value;
};

struct FlecsVelocity
{
	FVector Value;

	FlecsVelocity() : Value(FVector::ZeroVector) {}

	FlecsVelocity(FVector inValue) : Value(inValue) {}
};

struct FlecsWorldAcceleration
{
	FVector Value;
};

struct FlecsLocalAcceleration
{
	FVector Value;
};

/*
struct FlecsISMIndex
{
	int Value;
};

struct FlecsIsmRef
{
	UInstancedStaticMeshComponent* Value;
};

struct FlecsCorn
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

	void SweepMovement(FlecsLocation& FLoc, FlecsCollisionComponent& FCol, FlecsVelocity& FVel);
	void UpdateVFX(FlecsLocation& FLoc, FlecsVelocity& FVel, FlecsVFXRef& FVFXRef);

	flecs::query<FlecsLocation, FlecsCollisionComponent, FlecsVelocity> FlecsQuerySweepMovement;
	flecs::query<FlecsLocation, FlecsVelocity, FlecsVFXRef> FlecsQueryVFX;
};
