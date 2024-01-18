#include "FlecsSubsystem.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

flecs::world* UFlecsSubsystem::GetEcsWorld() const{return ECSWorld;}
void UFlecsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	OnTickDelegate = FTickerDelegate::CreateUObject(this, &UFlecsSubsystem::Tick);
	OnTickHandle = FTSTicker::GetCoreTicker().AddTicker(OnTickDelegate);
	
	//sets title in Flecs Explorer
	char* argv[] = {"Minimum Viable Flecs"};
	ECSWorld = new flecs::world(1, argv);
	
	//flecs explorer and monitor
	//comment this out if you not using it, it has some performance overhead
	//go to https://www.flecs.dev/explorer/ when the project is running to inspect active entities and values
	GetEcsWorld()->import<flecs::monitor>();
	GetEcsWorld()->set<flecs::Rest>({});

	/*
	//expose values with names to Flecs Explorer for easier inspection & debugging
	GetEcsWorld()->component<FlecsCorn>().member<float>("Current Growth");
	GetEcsWorld()->component<FlecsISMIndex>().member<int>("ISM Render index");	
	*/

	UE_LOG(LogTemp, Warning, TEXT("UUnrealFlecsSubsystem has started!"));
	Super::Initialize(Collection);
}

void UFlecsSubsystem::Deinitialize()
{
	FTSTicker::GetCoreTicker().RemoveTicker(OnTickHandle);
	
	if (ECSWorld)
	{
		delete ECSWorld;
		ECSWorld = nullptr;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("UUnrealFlecsSubsystem has shut down!"));
	Super::Deinitialize();
}

FFlecsEntityHandle UFlecsSubsystem::SpawnProjectileEntity(FVector location, FRotator rotation)
{
	auto entity = GetEcsWorld()->entity();
	FString bIsAlive = ecs_is_alive(ECSWorld->c_ptr(), entity.id()) ? "true" : "false";
	GEngine ->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SpawnProjectileEntity: %s"), *bIsAlive));
	return FFlecsEntityHandle{ int(entity.id()) };
	
}

bool UFlecsSubsystem::Tick(float DeltaTime)
{
	if(ECSWorld) ECSWorld->progress(DeltaTime);
	return true;
}
