#pragma once
#include "flecs.h"
#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FlecsSubsystem.generated.h"

struct FFlecsEntityHandle;

struct FlecsTransform
{
	FTransform Value;
};
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

UCLASS()
class FLECSTEST_API UFlecsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	TMap<UScriptStruct*, flecs::entity> EntityToScriptStruct;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	flecs::world* GetEcsWorld() const;
	
	UPROPERTY(EditAnywhere)
	UInstancedStaticMeshComponent* CornRenderer = nullptr;

	flecs::entity GetFragmentForScriptStruct(const UScriptStruct* ScriptStruct)
	{
		return *EntityToScriptStruct.Find(ScriptStruct);
	}

	void ApplyPrefabToEntity(flecs::entity& InEntity, struct FFLECSPrefab Prefab);
	void ApplyPrefabClassToEntity(flecs::entity& InEntity, struct FFLECSPrefabClassType Prefab);

	void RegisterFragments();

	UFUNCTION(BlueprintCallable, Category="FLECS")
	void InitFlecs(UStaticMesh* InMesh);
	UFUNCTION(BlueprintCallable, Category="FLECS")
	FFlecsEntityHandle SpawnCornEntity(FVector location, FRotator rotation);
	UFUNCTION(BlueprintCallable, Category = "FLECS")
	FFlecsEntityHandle SpawnProjectile(FVector location, FRotator rotation);

	UFUNCTION(BlueprintCallable, Category = "FLECS")
	void SpawnProjectileFromPrefab(UFlecsPrefabDefinition* FlecsPrefabDefinition, FVector Location, FVector MuzzleLocation, FRotator Rotation, AActor* Owner);
	UFUNCTION(BlueprintCallable, Category="FLECS")
	void SetEntityHighlight(FFlecsEntityHandle entityHandle, bool isHighlighted);
	UFUNCTION(BlueprintCallable, Category="FLECS")
	float GetEntityGrowthData(FFlecsEntityHandle entityHandle);

protected:
	FTickerDelegate OnTickDelegate;
	FTSTicker::FDelegateHandle OnTickHandle;
	flecs::world* ECSWorld = nullptr;
private:
	bool Tick(float DeltaTime);
	
};
