#include "FlecsSubsystem.h"
#include "Flecs.h"
#include "GameFramework/PlayerController.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Lets define world as the FFunction ECSWorld->c_ptr()
#define cworld ECSWorld->c_ptr()

flecs::world* UFlecsSubsystem::GetEcsWorld() const{return ECSWorld;}
void UFlecsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	OnTickDelegate = FTickerDelegate::CreateUObject(this, &UFlecsSubsystem::Tick);
	OnTickHandle = FTSTicker::GetCoreTicker().AddTicker(OnTickDelegate);
	
	//sets title in FFlecs Explorer
	char* argv[] = {"Minimum Viable FFlecs"};
	ECSWorld = new flecs::world(1, argv);
	
	//flecs explorer and monitor
	//comment this out if you not using it, it has some performance overhead
	//go to https://www.flecs.dev/explorer/ when the project is running to inspect active entities and values
	GetEcsWorld()->import<flecs::monitor>();
	GetEcsWorld()->set<flecs::Rest>({});

	/*
	//expose values with names to FFlecs Explorer FFor easier inspection & debugging
	GetEcsWorld()->component<FlecsCorn>().member<float>("Current Growth");
	GetEcsWorld()->component<FlecsISMIndex>().member<int>("ISM Render index");	
	*/

	UE_LOG(LogTemp, Warning, TEXT("UUnrealFlecsSubsystem has started!"));
	Super::Initialize(Collection);

	FFlecsQuerySweepMovement = GetEcsWorld()->query<FFlecsLocation, FFlecsCollisionClass, FFlecsVelocity>();
	FFlecsQueryVFX = GetEcsWorld()->query<FFlecsLocation, FFlecsVelocity, FFlecsVFXRef>();
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
		.set<FFlecsPlayerControllerRef>({ PlayerController })
		.set<FFlecsVFXRef>({ UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ProjectileVFX, MuzzleLocation, Rotation, FVector(1,1,1), true, true, ENCPoolMethod::AutoRelease, false) })
		.set<FFlecsTeamID>({ 0 })
		.set<FFlecsLocation>({ FFlecsLocation(Location) })
		.set<FFlecsCollisionClass>({FFlecsCollisionClass()})
		.set<FFlecsVelocity>({ FFlecsVelocity(Rotation.Vector() * 100) })
		.add<FFlecsProjectile>()
		.set_name(StringCast<ANSICHAR>("Projectile").Get());
	return int(entity.id());
}

FFlecsEntityHandle UFlecsSubsystem::SpawnProjectileEntitySpecific(APlayerController* PlayerController, FVector Location, FRotator Rotation, FVector MuzzleLocation, UNiagaraSystem* ProjectileSystemVFX, float Radius, float Speed)
{
	auto entity = GetEcsWorld()->entity();
	entity 
		.set<FFlecsPlayerControllerRef>({ PlayerController })
		.set<FFlecsVFXRef>({ UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ProjectileSystemVFX, MuzzleLocation, Rotation, FVector(1,1,1), true, true, ENCPoolMethod::AutoRelease, false) })
		.set<FFlecsTeamID>({ 0 })
		.set<FFlecsLocation>({ FFlecsLocation(Location) })
		//.set<FFlecsCollisionClass>({ FCollisionShape::MakeSphere(Radius), int(entity.id()) })
		.set<FFlecsVelocity>({ FFlecsVelocity(Rotation.Vector() * Speed) })
		.add<FFlecsProjectile>();
	return FFlecsEntityHandle{ int(entity.id()) };
}

/*
bool UFlecsSubsystem::Tick(float DeltaTime)
{
	if(ECSWorld) ECSWorld->progress(DeltaTime);

	auto ECSQuery = ECSWorld->query_builder<FlecsLocation, FFlecsCollisionClass, FFlecsVelocity>().build();
	ECSQuery.each(&SweepMovement);
	/*
	auto ECSQuery = ECSWorld->query_builder<FlecsLocation, FFlecsCollisionClass, FFlecsVelocity>().build();
	ECSQuery.each([](FlecsLocation& FFLoc, FFlecsCollisionClass& FFCol, FFlecsVelocity& FFVel, UFlecsSubsystem* inThis = this)
	{
		FHitResult HitResult;
		FVector Target = FFLoc.Value + FFVel.Value * inThis->GetWorld()->DeltaTimeSeconds;
		if (inThis->GetWorld()->SweepSingleByChannel(HitResult, FFLoc.Value, Target, FFQuat::Identity, ECC_Visibility, FFCollisionShape::MakeSphere(FCol.Shape.GetSphereRadius())))
		{

		}
		else
		{
			FLoc.Value = Target;
		}
	});*/
	/*
	ECSWorld->query<FlecsProjectile, FFlecsVelocity, FFlecsLocation, FFlecsCollisionClass>().each([&](flecs::entity e, FFlecsProjectile& projectile, FFlecsVelocity& velocity)
	{
		
	});
	ECSWorld->query<FlecsProjectile, FFlecsLocation, FFlecsVFXRef>().each([&](flecs::entity e, FFlecsProjectile& projectile, FFlecsVelocity& velocity)
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

void UFlecsSubsystem::SweepMovement(FlecsLocation& FFLoc, FFlecsCollisionClass& FFCol, FFlecsVelocity& FFVel)
{
	FHitResult HitResult;
	FVector Target = FFLoc.Value + FFVel.Value * GetWorld()->DeltaTimeSeconds;

	if (GetWorld()->SweepSingleByChannel(HitResult, FFLoc.Value, Target, FFQuat::Identity, ECC_Visibility, FFCollisionShape::MakeSphere(FCol.Shape.GetSphereRadius())))
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

	// Capture 'this' in a lambda FFunction
	auto SweepMovementLambda = [this](FlecsLocation& FFLoc, FFlecsCollisionClass& FFCol, FFlecsVelocity& FFVel)
	{
		this->SweepMovement(FLoc, FFCol, FFVel);
	};

	FlecsQuerySweepMovement.each(SweepMovementLambda);


	//auto FFlecsQueryVFX = ECSWorld->query_builder<FlecsLocation, FFlecsVelocity, FFlecsVFXRef>().build();

	auto UpdateVFXLambda = [](FlecsLocation& FFLoc, FFlecsVelocity& FFVel, FFlecsVFXRef& FFVFXRef)
	{
		UNiagaraComponent* NiagaraComponent = FFVFXRef.Value;
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

	FFlecsQuerySweepMovement.each([this](FFlecsLocation& FFLoc, FFlecsCollisionClass& FFCol, FFlecsVelocity& FFVel)
		{
			SweepMovement(FFLoc, FFCol, FFVel);
		});

	FFlecsQueryVFX.each([this](FFlecsLocation& FFLoc, FFlecsVelocity& FFVel, FFlecsVFXRef& FFVFXRef)
		{
			UpdateVFX(FFLoc, FFVel, FFVFXRef);
		});

	return true;
}


void UFlecsSubsystem::SweepMovement(FFlecsLocation& FFLoc, FFlecsCollisionClass& FFCol, FFlecsVelocity& FFVel)
{
	FHitResult HitResult;
	FVector Target = FFLoc.Value + FFVel.Value * GetWorld()->DeltaTimeSeconds;
	//GEngine->AddOnScreenDebugMessage (-1, 0, FFColor::Red, FFString::Printf(TEXT("SweepMovement: %s"), *Target.ToString())); //FCollisionShape::MakeSphere(FFCol.Shape.GetSphereRadius()
	if (GetWorld()->SweepSingleByChannel(HitResult, FFLoc.Value, Target, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(0)))
	{
		auto FFlecsEntity = ECSWorld->get_alive(FFCol.Owner.FFlecsEntityId);
		FFlecsEntity.get<FFlecsVFXRef>()->Value->DestroyComponent();
		FFlecsEntity.destruct();
	}
	else
	{
		FFLoc = FFlecsLocation(Target);	
	}
}

void UFlecsSubsystem::UpdateVFX(FFlecsLocation& FFLoc, FFlecsVelocity& FFVel, FFlecsVFXRef& FFVFXRef)
{
	if (IsValid(FFVFXRef.Value)) FFVFXRef.Value->SetWorldLocation(FFLoc.Value);
}
