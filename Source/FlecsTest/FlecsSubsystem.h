#pragma once
#include "flecs.h"
#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FlecsSubsystem.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

struct FlecksCollisionComponentRef
{
	FCollisionShape* Shape;
};

struct FlecsVFXRef
{
	UNiagaraComponent* Value;
};

struct FlecsSFXRef
{
	UAudioComponent* Value;
};

struct FlecsLocation
{
	FVector Value;
};

struct FlecsRotation
{
	FRotator Value;
};

struct FlecsVelocity
{
	FVector Value;
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

USTRUCT(BlueprintType)
struct FFlecsEntityHandle
{
	GENERATED_USTRUCT_BODY()
	FFlecsEntityHandle()  {}
	FFlecsEntityHandle(int inId)
	{
		FlecsEntityId = inId;
	}
	UPROPERTY(BlueprintReadWrite)
	int FlecsEntityId;
};

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
	FFlecsEntityHandle SpawnProjectileEntity(FVector location, FRotator rotation);

protected:
	FTickerDelegate OnTickDelegate;
	FTSTicker::FDelegateHandle OnTickHandle;
	flecs::world* ECSWorld = nullptr;
private:
	bool Tick(float DeltaTime);
	
};
