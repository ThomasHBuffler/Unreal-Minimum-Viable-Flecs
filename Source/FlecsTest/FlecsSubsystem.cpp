#include "FlecsSubsystem.h"
#include "Flecs.h"
#include "GameFramework/PlayerController.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Lets define world as the function ECSWorld->c_ptr()
#define cworld ECSWorld->c_ptr()

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

	FlecsQuerySweepMovement = GetEcsWorld()->query<FlecsLocation, FlecsCollisionComponent, FlecsVelocity>();
	FlecsQueryVFX = GetEcsWorld()->query<FlecsLocation, FlecsVelocity, FlecsVFXRef>();
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

/*
FFlecsEntityHandle UFlecsSubsystem::SpawnCornEntity(FVector location, FRotator rotation)
{
	auto IsmID = CornRenderer->AddInstance(FTransform(rotation, location));
	auto entity = GetEcsWorld()->entity()
	.set<FlecsIsmRef>({CornRenderer})
	.set<FlecsISMIndex>({IsmID})
	.set<FlecsCorn>({0})
	.child_of<Corns>()
	.set_name(StringCast<ANSICHAR>(*FString::Printf(TEXT("Corn%d"), IsmID)).Get());
	return FFlecsEntityHandle{int(entity.id())};
}
*/

FFlecsEntityHandle UFlecsSubsystem::SpawnProjectileEntity(APlayerController* PlayerController, FVector Location, FRotator Rotation, FVector MuzzleLocation)
{
	FCollisionShape Sphere = FCollisionShape::MakeSphere(10);
	auto entity = GetEcsWorld()->entity();
	entity
		.set<FlecsPlayerControllerRef>({ PlayerController })
		.set<FlecsVFXRef>({ UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ProjectileVFX, MuzzleLocation, Rotation, FVector(1,1,1), true, true, ENCPoolMethod::AutoRelease, false) })
		.set<FlecsTeamID>({ 0 })
		.set<FlecsLocation>({ FlecsLocation(Location) })
		.set<FlecsCollisionComponent>({FlecsCollisionComponent(Sphere, int(entity.id()))})
		.set<FlecsVelocity>({ FlecsVelocity(Rotation.Vector() * 100) })
		.add<FlecsProjectile>()
		.set_name(StringCast<ANSICHAR>("Projectile").Get());
	return int(entity.id());
}

FFlecsEntityHandle UFlecsSubsystem::SpawnProjectileEntitySpecific(APlayerController* PlayerController, FVector Location, FRotator Rotation, FVector MuzzleLocation, UNiagaraSystem* ProjectileSystemVFX, float Radius, float Speed)
{
	auto entity = GetEcsWorld()->entity();
	entity 
		.set<FlecsPlayerControllerRef>({ PlayerController })
		.set<FlecsVFXRef>({ UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ProjectileSystemVFX, MuzzleLocation, Rotation, FVector(1,1,1), true, true, ENCPoolMethod::AutoRelease, false) })
		.set<FlecsTeamID>({ 0 })
		.set<FlecsLocation>({ FlecsLocation(Location) })
		.set<FlecsCollisionComponent>({ FCollisionShape::MakeSphere(Radius), int(entity.id()) })
		.set<FlecsVelocity>({ FlecsVelocity(Rotation.Vector() * Speed) })
		.add<FlecsProjectile>();
	return FFlecsEntityHandle{ int(entity.id()) };
}

/*
bool UFlecsSubsystem::Tick(float DeltaTime)
{
	if(ECSWorld) ECSWorld->progress(DeltaTime);

	auto ECSQuery = ECSWorld->query_builder<FlecsLocation, FlecsCollisionComponent, FlecsVelocity>().build();
	ECSQuery.each(&SweepMovement);
	/*
	auto ECSQuery = ECSWorld->query_builder<FlecsLocation, FlecsCollisionComponent, FlecsVelocity>().build();
	ECSQuery.each([](FlecsLocation& FLoc, FlecsCollisionComponent& FCol, FlecsVelocity& FVel, UFlecsSubsystem* inThis = this)
	{
		FHitResult HitResult;
		FVector Target = FLoc.Value + FVel.Value * inThis->GetWorld()->DeltaTimeSeconds;
		if (inThis->GetWorld()->SweepSingleByChannel(HitResult, FLoc.Value, Target, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(FCol.Shape.GetSphereRadius())))
		{

		}
		else
		{
			FLoc.Value = Target;
		}
	});*/
	/*
	ECSWorld->query<FlecsProjectile, FlecsVelocity, FlecsLocation, FlecsCollisionComponent>().each([&](flecs::entity e, FlecsProjectile& projectile, FlecsVelocity& velocity)
	{
		
	});
	ECSWorld->query<FlecsProjectile, FlecsLocation, FlecsVFXRef>().each([&](flecs::entity e, FlecsProjectile& projectile, FlecsVelocity& velocity)
	{
			UNiagaraComponent* NiagaraComponent = e.get<FlecsVFXRef>()->Value;
			if (NiagaraComponent)
			{
			NiagaraComponent->SetWorldLocation(e.get<FlecsLocation>()->Value);
		}
	});
	*//*
	return true;
}

void UFlecsSubsystem::SweepMovement(FlecsLocation& FLoc, FlecsCollisionComponent& FCol, FlecsVelocity& FVel)
{
	FHitResult HitResult;
	FVector Target = FLoc.Value + FVel.Value * GetWorld()->DeltaTimeSeconds;

	if (GetWorld()->SweepSingleByChannel(HitResult, FLoc.Value, Target, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(FCol.Shape.GetSphereRadius())))
	{
		// Handle collision
	}
	else
	{
		FLoc.Value = Target;
	}
}
*/
/*
bool UFlecsSubsystem::Tick(float DeltaTime)
{
	if (ECSWorld) ECSWorld->progress(DeltaTime);

	// Capture 'this' in a lambda function
	auto SweepMovementLambda = [this](FlecsLocation& FLoc, FlecsCollisionComponent& FCol, FlecsVelocity& FVel)
	{
		this->SweepMovement(FLoc, FCol, FVel);
	};

	FlecsQuerySweepMovement.each(SweepMovementLambda);


	//auto FlecsQueryVFX = ECSWorld->query_builder<FlecsLocation, FlecsVelocity, FlecsVFXRef>().build();

	auto UpdateVFXLambda = [](FlecsLocation& FLoc, FlecsVelocity& FVel, FlecsVFXRef& FVFXRef)
	{
		UNiagaraComponent* NiagaraComponent = FVFXRef.Value;
		if (NiagaraComponent)
		{
			NiagaraComponent->SetWorldLocation(FLoc.Value);
		}
	};

	FlecsQueryVFX.each(UpdateVFXLambda);

	return true;
}
*/

bool UFlecsSubsystem::Tick(float DeltaTime)
{
	if (ECSWorld) ECSWorld->progress(DeltaTime);

	FlecsQuerySweepMovement.each([this](FlecsLocation& FLoc, FlecsCollisionComponent& FCol, FlecsVelocity& FVel)
		{
			SweepMovement(FLoc, FCol, FVel);
		});

	FlecsQueryVFX.each([this](FlecsLocation& FLoc, FlecsVelocity& FVel, FlecsVFXRef& FVFXRef)
		{
			UpdateVFX(FLoc, FVel, FVFXRef);
		});

	return true;
}


void UFlecsSubsystem::SweepMovement(FlecsLocation& FLoc, FlecsCollisionComponent& FCol, FlecsVelocity& FVel)
{
	FHitResult HitResult;
	FVector Target = FLoc.Value + FVel.Value * GetWorld()->DeltaTimeSeconds;
	//GEngine->AddOnScreenDebugMessage (-1, 0, FColor::Red, FString::Printf(TEXT("SweepMovement: %s"), *Target.ToString()));
	if (GetWorld()->SweepSingleByChannel(HitResult, FLoc.Value, Target, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(FCol.Shape.GetSphereRadius())))
	{
		auto FlecsEntity = ECSWorld->get_alive(FCol.Owner.FlecsEntityId);
		FlecsEntity.get<FlecsVFXRef>()->Value->DestroyComponent();
		FlecsEntity.destruct();
	}
	else
	{
		FLoc = FlecsLocation(Target);	
	}
}

void UFlecsSubsystem::UpdateVFX(FlecsLocation& FLoc, FlecsVelocity& FVel, FlecsVFXRef& FVFXRef)
{
	if (IsValid(FVFXRef.Value)) FVFXRef.Value->SetWorldLocation(FLoc.Value);
}
