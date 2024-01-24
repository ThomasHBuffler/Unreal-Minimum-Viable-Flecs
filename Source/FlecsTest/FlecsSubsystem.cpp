#include "FlecsSubsystem.h"
#include "FlecsComponents.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FlecsPrefabDefinition.h"

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
	
	//expose values with names to Flecs Explorer for easier inspection & debugging
	GetEcsWorld()->component<FlecsCorn>().member<float>("Current Growth");
	GetEcsWorld()->component<FlecsISMIndex>().member<int>("ISM Render index");	
	
	UE_LOG(LogTemp, Warning, TEXT("UUnrealFlecsSubsystem has started!"));
	Super::Initialize(Collection);
}

void UFlecsSubsystem::ApplyToEntity(flecs::entity& InEntity, FFLECSPrefab Prefab)
{
	for (auto& Component : Prefab.Components)
	{
		if (flecs::entity ComponentEntity = GetComponentForScriptStruct(Component.GetScriptStruct()))
		{
			InEntity.set_ptr(ComponentEntity, Component.GetMemory());
		}
	}
}

void UFlecsSubsystem::RegisterComponents()
{
	//register and map all components
	flecs::component<FFLECSGenericComp>(*GetEcsWorld(), "FFLECSGenericComp");
	EntityToScriptStruct.Add(TBaseStructure<FFLECSGenericComp>::Get(), GetEcsWorld()->entity("FFLECSGenericComp"));
	
	flecs::component<FOwnerEntity>(*GetEcsWorld(), "FOwnerEntity");
	EntityToScriptStruct.Add(TBaseStructure<FOwnerEntity>::Get(), GetEcsWorld()->entity("FOwnerEntity"));

	flecs::component<FLocationComp>(*GetEcsWorld(), "FLocationComp");
	EntityToScriptStruct.Add(TBaseStructure<FLocationComp>::Get(), GetEcsWorld()->entity("FLocationComp"));

	flecs::component<FRotationComp>(*GetEcsWorld(), "FRotationComp");
	EntityToScriptStruct.Add(TBaseStructure<FRotationComp>::Get(), GetEcsWorld()->entity("FRotationComp"));

	flecs::component<FVelocityComp>(*GetEcsWorld(), "FVelocityComp");
	EntityToScriptStruct.Add(TBaseStructure<FVelocityComp>::Get() , GetEcsWorld()->entity("FVelocityComp"));

	flecs::component<FLocalAccelerationComp>(*GetEcsWorld(), "FLocalAccelerationComp");
	EntityToScriptStruct.Add(TBaseStructure<FLocalAccelerationComp>::Get(), GetEcsWorld()->entity("FLocalAccelerationComp"));

	flecs::component<FGravityAccelerationComp>(*GetEcsWorld(), "FGravityAccelerationComp");
	EntityToScriptStruct.Add(TBaseStructure<FGravityAccelerationComp>::Get(), GetEcsWorld()->entity("FGravityAccelerationComp"));

	flecs::component<FMassComp>(*GetEcsWorld(), "FMassComp");
	EntityToScriptStruct.Add(TBaseStructure<FMassComp>::Get(), GetEcsWorld()->entity("FMassComp"));

	flecs::component<FDragComp>(*GetEcsWorld(), "FDragComp");
	EntityToScriptStruct.Add(TBaseStructure<FDragComp>::Get(), GetEcsWorld()->entity("FDragComp"));

	flecs::component<FMaxSpeedComp>(*GetEcsWorld(), "FMaxSpeedComp");
	EntityToScriptStruct.Add(TBaseStructure<FMaxSpeedComp>::Get(), GetEcsWorld()->entity("FMaxSpeedComp"));

	flecs::component<FGenericEvent>(*GetEcsWorld(), "FGenericEvent");
	EntityToScriptStruct.Add(TBaseStructure<FGenericEvent>::Get(), GetEcsWorld()->entity("FGenericEvent"));

	//FTimerEventComp
	flecs::component<FTimerEventComp>(*GetEcsWorld(), "FTimerEventComp");
	EntityToScriptStruct.Add(TBaseStructure<FTimerEventComp>::Get(), GetEcsWorld()->entity("FTimerEventComp"));

	//FAliveStatus
	flecs:: component<FAliveStatus>(*GetEcsWorld(), "FAliveStatus");
	EntityToScriptStruct.Add(TBaseStructure<FAliveStatus>::Get(), GetEcsWorld()->entity("FAliveStatus"));

	//FSpawnEvent
	flecs::component<FSpawnEvent>(*GetEcsWorld(), "FSpawnEvent");
	EntityToScriptStruct.Add(TBaseStructure<FSpawnEvent>::Get(), GetEcsWorld()->entity("FSpawnEvent"));

	//FEntitySpawnerComp 
	flecs::component<FEntitySpawnerComp>(*GetEcsWorld(), "FEntitySpawnerComp");
	EntityToScriptStruct.Add(TBaseStructure<FEntitySpawnerComp>::Get(), GetEcsWorld()->entity("FEntitySpawnerComp"));

	//FFXSpawnerComp
	flecs::component<FFXSpawnerComp>(*GetEcsWorld(), "FFXSpawnerComp");
	EntityToScriptStruct.Add(TBaseStructure<FFXSpawnerComp>::Get(), GetEcsWorld()->entity("FFXSpawnerComp"));

	//FProjectileCollisionModule
	flecs::component<FProjectileCollisionModule>(*GetEcsWorld(), "FProjectileCollisionModule");
	EntityToScriptStruct.Add(TBaseStructure<FProjectileCollisionModule>::Get(), GetEcsWorld()->entity("FProjectileCollisionModule"));

	//FProjectileCollisionResponseComp
	flecs::component<FProjectileCollisionResponseComp>(*GetEcsWorld(), "FProjectileCollisionResponseComp");
	EntityToScriptStruct.Add(TBaseStructure<FProjectileCollisionResponseComp>::Get(), GetEcsWorld()->entity("FProjectileCollisionResponseComp"));

	//FProjectileCollisionComp
	flecs::component<FProjectileCollisionComp>(*GetEcsWorld(), "FProjectileCollisionComp");
	EntityToScriptStruct.Add(TBaseStructure<FProjectileCollisionComp>::Get(), GetEcsWorld()->entity("FProjectileCollisionComp"));

	//FProjectileCollisionManagerComp 
	flecs::component<FProjectileCollisionManagerComp>(*GetEcsWorld(), "FProjectileCollisionManagerComp");
	EntityToScriptStruct.Add(TBaseStructure<FProjectileCollisionManagerComp>::Get(), GetEcsWorld()->entity("FProjectileCollisionManagerComp"));

	//FImpactEffect
	flecs::component<FImpactEffect>(*GetEcsWorld(), "FImpactEffect");
	EntityToScriptStruct.Add(TBaseStructure<FImpactEffect>::Get(), GetEcsWorld()->entity("FImpactEffect"));

	// FDamageComp
	flecs::component<FDamageComp>(*GetEcsWorld(), "FDamageComp");
	EntityToScriptStruct.Add(TBaseStructure<FDamageComp>::Get(), GetEcsWorld()->entity("FDamageComp"));

	//FAreaOfEffectDamageComp
	flecs::component<FAreaOfEffectDamageComp>(*GetEcsWorld(), "FAreaOfEffectDamageComp");
	EntityToScriptStruct.Add(TBaseStructure<FAreaOfEffectDamageComp>::Get(), GetEcsWorld()->entity("FAreaOfEffectDamageComp"));

	//FProjectile
	flecs::component<FProjectile>(*GetEcsWorld(), "FProjectile");
	EntityToScriptStruct.Add(TBaseStructure<FProjectile>::Get(), GetEcsWorld()->entity("FProjectile"));


}

void UFlecsSubsystem::InitFlecs(UStaticMesh* InMesh)
{
	RegisterComponents();

	/*
	//Spawn an actor and add an Instanced Static Mesh component to it.
	//This will render our entities.
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CornRenderer = Cast<UInstancedStaticMeshComponent>((GetWorld()->SpawnActor(AActor::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnInfo))->AddComponentByClass(UInstancedStaticMeshComponent::StaticClass(), false, FTransform(FVector::ZeroVector), false));
	CornRenderer->SetStaticMesh(InMesh);
	CornRenderer->bUseDefaultCollision = false;
	CornRenderer->SetGenerateOverlapEvents(false);
	CornRenderer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CornRenderer->SetCanEverAffectNavigation(false);
	CornRenderer->NumCustomDataFloats = 2;

	//this system processes the growth of our entities
	auto system_grow = GetEcsWorld()->system<FlecsCorn>("Grow System")
		.iter([](flecs::iter it, FlecsCorn* fc) {
		float GrowthRate = 20 * it.delta_time();
		for (int i : it) {
			//if we haven't grown fully (100) then grow
			fc[i].Growth += (fc[i].Growth < 100) * GrowthRate;
		}
	});
	*/
	auto System_SweepMovement  = GetEcsWorld()->system<FOwnerEntity, FOwnerActor, FLocationComp, FVelocityComp, FProjectileCollisionManagerComp>("Sweep Movement System")
		.iter([this](flecs::iter it, FOwnerEntity* EntityOwner, FOwnerActor* OwnerActor, FLocationComp* LocComp, FVelocityComp* VelComp, FProjectileCollisionManagerComp* ProjCollManager) {
		for (int i : it) {
			// Print i in green for 0 seconds
			GEngine -> AddOnScreenDebugMessage(-1, 0, FColor::Green, FString::FromInt(i));

			FVector EndPoint = LocComp[i].Location + VelComp[i].Velocity * it.delta_time();
			FHitResult HitResult;
			bool Killed = false;
			DrawDebugPoint(GetWorld(), LocComp[i].Location, 25, FColor::Blue, false, 0);
			for (auto Collision : ProjCollManager[i].Collisions)
			{
				ECollisionChannel TraceChannel = Collision.Get<FProjectileCollisionComp>().TraceChannel;
				FCollisionShape CollisionShape = Collision.Get<FProjectileCollisionComp>().CollisionShape;
				if (GetWorld()->SweepSingleByChannel(HitResult, LocComp[i].Location, EndPoint, VelComp[i].Velocity.Rotation().Quaternion(), TraceChannel, CollisionShape, FCollisionQueryParams(), FCollisionResponseParams()))
				{
					
					if (HitResult.GetActor() != OwnerActor[i].Owner)
					{
						
						// Print the ID number of the entity owner 
						int ID = EntityOwner[i].OwnerEntity.is_alive();
						// Lets destroy the entity by id



						//GEngine ->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Entity Owner ID: %d"), ID));
						EntityOwner[i].OwnerEntity.destruct();
						//EntityOwner->OwnerEntity.al //.destruct();
						//GetEcsWorld()->delete_with(ID);
						//GetEcsWorld()->entity(it).set<FAliveStatus>({ FAliveStatus(false) });
						//GetEcsWorld()->entity(EntityOwner[i].Owner.FlecsEntityId).destruct();
						for (auto CollisionResponse : Collision.Get<FProjectileCollisionComp>().CollisionResponses)
						{
							// Print the 
							/*
							EntityOwner->OwnerEntity.destruct();
								
							for (auto Event : CollisionResponse.Get<FProjectileCollisionResponseComp>().EventsToTrigger)
							{//<FAliveStatus>({});
								
								if (flecs::entity ComponentEntity = GetComponentForScriptStruct(Event.GetScriptStruct()))
								{
									flecs::entity(EntityOwner[i].Owner.FlecsEntityId).set_ptr(ComponentEntity, Event.GetMemory());
								}
							}*/
						}
						Killed = true;
						break;
					}
				}
			}		
			if (!Killed)
			{
				LocComp[i].Location = EndPoint;
			}
		}
	});
	/*
	// Kill entities that have been marked as garbage and have entityowner component
	auto System_KillGarbage = GetEcsWorld()->system<FAliveStatus, FOwnerEntity>("Is Alive Status")
		.iter([this](flecs::iter it, FAliveStatus* AliveStatus, FOwnerEntity* EntityOwner) {
		for (int i : it) {
			if (!AliveStatus[i].bIsAlive)
			{
				GetEcsWorld()->entity(EntityOwner[i].Owner.FlecsEntityId).destruct();
			}
			GEngine->AddOnScreenDebugMessage( - 1, 0, FColor::Red, FString::Printf(TEXT("Is Alive Status %d"), AliveStatus[i].bIsAlive));
		}
	});
	*/
	
	/*
	auto System_SweepMovement2 = GetEcsWorld()->system<FLocationComp, FVelocityComp>("Sweep Movement System 2")
		.iter([this](flecs::iter it, FLocationComp* LocComp, FVelocityComp* VelComp) {
		for (int i : it) {
			FVector EndPoint = LocComp[i].Location + VelComp[i].Velocity * it.delta_time();
			FHitResult HitResult;
			if (GetWorld()->SweepSingleByChannel(HitResult, LocComp[i].Location, EndPoint, VelComp[i].Velocity.Rotation().Quaternion(), ECC_Visibility, FCollisionShape::MakeSphere(1), FCollisionQueryParams(), FCollisionResponseParams()))
			{
				DrawDebugPoint(GetWorld(), LocComp[i].Location, 3, FColor::Orange, false, 0);
			}
			else
			{
				LocComp[i].Location = EndPoint;
				DrawDebugPoint(GetWorld(), LocComp[i].Location, 1,FColor::Green, false, 0);
			}
		}
	});
	*/
	/*
	auto System_ApplyGravity = GetEcsWorld()->system<FLocationComp, FVelocityComp>("Apply Gravity System")
	.iter([this](flecs::iter it, FVelocityComp* VelComp, FGravityAccelerationComp* GravAccel) {
		for (int i : it) {
			VelComp[i].Velocity += FVector(0, 0, GravAccel[i].Gravity) * it.delta_time();
		}
	});
	
	auto System_ApplyLocalAcceleration = GetEcsWorld()->system<FLocationComp, FVelocityComp>("Apply Local Acceleration System")
	.iter([this](flecs::iter it, FVelocityComp* VelComp, FLocalAccelerationComp* LocalAccel) {
		for (int i : it) {
			VelComp[i].Velocity += LocalAccel[i].Acceleration * it.delta_time();
		}
	});
	
	// Limit speed to max speed @todo optimize
	auto System_LimitSpeed = GetEcsWorld()->system<FLocationComp, FVelocityComp>("Limit Speed System")
	.iter([this](flecs::iter it, FVelocityComp* VelComp, FMaxSpeedComp* MaxSpeed) {
		for (int i : it) {
			if (
				VelComp[i].Velocity.Size() > MaxSpeed[i].MaxSpeed) {
				VelComp[i].Velocity = VelComp[i].Velocity.GetSafeNormal() * MaxSpeed[i].MaxSpeed;
			}
		}
	});
		
	//this system sets the growth value of our entities in ISM so we can access it from materials.
	auto system_copy_growth = GetEcsWorld()->system<FlecsCorn, FlecsISMIndex, FlecsIsmRef>("Grow Renderer System")
	.iter([](flecs::iter it, FlecsCorn* fw, FlecsISMIndex* fi, FlecsIsmRef* fr) {
		for (int i : it) {
			auto index = fi[i].Value;
			fr[i].Value->SetCustomDataValue(index, 0, fw[i].Growth, true);
		}
	});
	*/
	UE_LOG(LogTemp, Warning, TEXT("Flecs Corn system initialized!"));
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

FFlecsEntityHandle UFlecsSubsystem::SpawnCornEntity(FVector location, FRotator rotation)
{
	auto IsmID = CornRenderer->AddInstance(FTransform(rotation, location));
	auto entity = GetEcsWorld()->entity()
		.set<FlecsIsmRef>({ CornRenderer })
		.set<FlecsISMIndex>({ IsmID })
		.set<FlecsCorn>({ 0 })
		.child_of<Corns>()
		.set_name(StringCast<ANSICHAR>(*FString::Printf(TEXT("Corn%d"), IsmID)).Get());
	return FFlecsEntityHandle{ int(entity.id()) };
}

FFlecsEntityHandle UFlecsSubsystem::SpawnProjectile(FVector location, FRotator rotation)
{	/*
	// GEneric events
	TArray<FGenericEvent> EventsToTrigger = { FAliveStatus() };
	TArray<FInstancedStruct> InEventsToTrigger;
	FInstancedStruct::Make<FGenericEvent>(InEventsToTrigger, EventsToTrigger);

	// Collision responses
	TArray<FProjectileCollisionResponseComp> CollisionResponses = { FProjectileCollisionResponseComp(InEventsToTrigger, TArray<FInstancedStruct>()) };
	TArray<FInstancedStruct> InCollisionResponses;
	FInstancedStruct::Make<FProjectileCollisionResponseComp>(InCollisionResponses, CollisionResponses);
	
	// CollisionComp
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(1);
	FProjectileCollisionComp InCollisions(CollisionShape, ECollisionChannel::ECC_Visibility, InCollisionResponses);
	TArray<FInstancedStruct> Collisions;
	FInstancedStruct::Make<FProjectileCollisionComp>(Collisions, InCollisions);
	FProjectileCollisionManagerComp CollisionManagerComp = FProjectileCollisionManagerComp(Collisions);
	*/
	auto entity = GetEcsWorld()->entity();
	entity
		.set<FOwnerEntity>({ FOwnerEntity(entity.id(),entity) })
		//.set<FProjectileCollisionManagerComp>({ CollisionManagerComp })
		.set<FLocationComp>({ FLocationComp(location) })
		.set<FVelocityComp>({ FVelocityComp(rotation.Vector() * 100) })
		.set<FGravityAccelerationComp>({ FGravityAccelerationComp() })
		.set<FLocalAccelerationComp>({ FLocalAccelerationComp(FVector(10, 0, 0)) });


	auto vel = entity.get<FVelocityComp>()->Velocity;

	return FFlecsEntityHandle{ int(entity.id()) };
}

void UFlecsSubsystem::SpawnEntityFromPrefab(UFlecsPrefabDefinition* FlecsPrefabDefinition, FVector Location, FRotator Rotation, AActor* Owner)
{
	flecs::entity entity = GetEcsWorld()->entity(); 
	entity.set<FOwnerEntity>({ FOwnerEntity(entity.id(), entity) });
	FFLECSPrefab Prefab;
	Prefab = FlecsPrefabDefinition->GetPrefab();
	ApplyToEntity(entity, Prefab);
	entity.set<FLocationComp>({ FLocationComp(Location) });
	entity.set<FVelocityComp>({ Rotation.Quaternion().RotateVector(entity.get<FVelocityComp>()->Velocity) });
	entity.set<FAliveStatus>({ FAliveStatus(true) });
	if (IsValid(Owner)) entity.set<FOwnerActor>({ FOwnerActor(Owner) });
}

void UFlecsSubsystem::SetEntityHighlight(FFlecsEntityHandle entityHandle, bool isHighlighted)
{
	int idx = GetEcsWorld()->entity(entityHandle.FlecsEntityId).get<FlecsISMIndex>()->Value;
	CornRenderer->SetCustomDataValue(idx, 1, (float)isHighlighted, true);
}

float UFlecsSubsystem::GetEntityGrowthData(FFlecsEntityHandle entityHandle)
{
	return GetEcsWorld()->entity(entityHandle.FlecsEntityId).get<FlecsCorn>()->Growth;
}

bool UFlecsSubsystem::Tick(float DeltaTime)
{
	if(ECSWorld) ECSWorld->progress(DeltaTime);
	return true;
}
