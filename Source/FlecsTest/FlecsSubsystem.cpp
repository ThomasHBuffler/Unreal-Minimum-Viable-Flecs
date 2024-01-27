#include "FlecsSubsystem.h"
#include "FlecsComponents.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
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

void UFlecsSubsystem::ApplyPrefabToEntity(flecs::entity& InEntity, FFLECSPrefab Prefab)
{
	for (auto& Fragment : Prefab.Fragments)
	{
		if (flecs::entity ComponentEntity = GetFragmentForScriptStruct(Fragment.GetScriptStruct()))
		{
			InEntity.set_ptr(ComponentEntity, Fragment.GetMemory());
		}
	}
}

void UFlecsSubsystem::ApplyPrefabClassToEntity(flecs::entity& InEntity, FFLECSPrefabClassType Prefab)
{
	for (auto& Fragment : Prefab.Fragments)
	{
		if (flecs::entity ComponentEntity = GetFragmentForScriptStruct(Fragment->GetScriptStruct()))
		{
			InEntity.set_ptr(ComponentEntity, Fragment->GetMemory());
		}
	}
}

void UFlecsSubsystem::RegisterFragments()
{
	//register and map all Fragments
	flecs::component<FFLECSGenericFragment>(*GetEcsWorld(), "FFLECSGenericFragment");
	EntityToScriptStruct.Add(TBaseStructure<FFLECSGenericFragment>::Get(), GetEcsWorld()->entity("FFLECSGenericFragment"));
	
	//FOwnerEntity
	flecs::component<FOwnerEntity>(*GetEcsWorld(), "FOwnerEntity");
	EntityToScriptStruct.Add(TBaseStructure<FOwnerEntity>::Get(), GetEcsWorld()->entity("FOwnerEntity"));

	//FLocationComp
	flecs::component<FLocationFragment>(*GetEcsWorld(), "FLocationFragment");
	EntityToScriptStruct.Add(TBaseStructure<FLocationFragment>::Get(), GetEcsWorld()->entity("FLocationFragment"));

	//FRotationComp
	flecs::component<FRotationFragment>(*GetEcsWorld(), "FRotationFragment");
	EntityToScriptStruct.Add(TBaseStructure<FRotationFragment>::Get(), GetEcsWorld()->entity("FRotationFragment"));

	//FVelocityComp
	flecs::component<FVelocityFragment>(*GetEcsWorld(), "FVelocityFragment");
	EntityToScriptStruct.Add(TBaseStructure<FVelocityFragment>::Get() , GetEcsWorld()->entity("FVelocityFragment"));

	//FLocalAccelerationComp
	flecs::component<FLocalAccelerationFragment>(*GetEcsWorld(), "FLocalAccelerationFragment");
	EntityToScriptStruct.Add(TBaseStructure<FLocalAccelerationFragment>::Get(), GetEcsWorld()->entity("FLocalAccelerationFragment"));

	//FGravityAccelerationComp
	flecs::component<FGravityAccelerationFragment>(*GetEcsWorld(), "FGravityAccelerationFragment");
	EntityToScriptStruct.Add(TBaseStructure<FGravityAccelerationFragment>::Get(), GetEcsWorld()->entity("FGravityAccelerationFragment"));

	//FMassComp
	flecs::component<FMassFragment>(*GetEcsWorld(), "FMassFragment");
	EntityToScriptStruct.Add(TBaseStructure<FMassFragment>::Get(), GetEcsWorld()->entity("FMassFragment"));

	//FDragComp
	flecs::component<FDragFragment>(*GetEcsWorld(), "FDragFragment");
	EntityToScriptStruct.Add(TBaseStructure<FDragFragment>::Get(), GetEcsWorld()->entity("FDragFragment"));

	//FMaxSpeedComp
	flecs::component<FMaxSpeedFragment>(*GetEcsWorld(), "FMaxSpeedFragment");
	EntityToScriptStruct.Add(TBaseStructure<FMaxSpeedFragment>::Get(), GetEcsWorld()->entity("FMaxSpeedFragment"));
	
	//FGenericEvent
	flecs::component<FGenericEvent>(*GetEcsWorld(), "FGenericEvent");
	EntityToScriptStruct.Add(TBaseStructure<FGenericEvent>::Get(), GetEcsWorld()->entity("FGenericEvent"));
	
	//FTimerEventComp
	flecs::component<FTimerEventFragment>(*GetEcsWorld(), "FTimerEventFragment");
	EntityToScriptStruct.Add(TBaseStructure<FTimerEventFragment>::Get(), GetEcsWorld()->entity("FTimerEventFragment"));

	//FMarkAsGarbage
	flecs::component<FMarkAsGarbage>(*GetEcsWorld(), "FMarkAsGarbage");
	EntityToScriptStruct.Add(TBaseStructure<FMarkAsGarbage>::Get(), GetEcsWorld()->entity("FMarkAsGarbage"));

	//FSpawnEvent
	flecs::component<FSpawnEvent>(*GetEcsWorld(), "FSpawnEvent");
	EntityToScriptStruct.Add(TBaseStructure<FSpawnEvent>::Get(), GetEcsWorld()->entity("FSpawnEvent"));

	//FEntitySpawnerComp 
	flecs::component<FEntitySpawnerFragment>(*GetEcsWorld(), "FEntitySpawnerFragment");
	EntityToScriptStruct.Add(TBaseStructure<FEntitySpawnerFragment>::Get(), GetEcsWorld()->entity("FEntitySpawnerFragment"));

	//FFXSpawnerComp
	flecs::component<FFXSpawnerFragment>(*GetEcsWorld(), "FFXSpawnerFragment");
	EntityToScriptStruct.Add(TBaseStructure<FFXSpawnerFragment>::Get(), GetEcsWorld()->entity("FFXSpawnerFragment"));

	//FProjectileCollisionModule
	flecs::component<FProjectileCollisionModule>(*GetEcsWorld(), "FProjectileCollisionModule");
	EntityToScriptStruct.Add(TBaseStructure<FProjectileCollisionModule>::Get(), GetEcsWorld()->entity("FProjectileCollisionModule"));
	
	//FProjectileCollisionResponseComp
	flecs::component<FProjectileCollisionResponseFragment>(*GetEcsWorld(), "FProjectileCollisionResponseFragment");
	EntityToScriptStruct.Add(TBaseStructure<FProjectileCollisionResponseFragment>::Get(), GetEcsWorld()->entity("FProjectileCollisionResponseFragment"));
	
	//FProjectileCollisionComp
	flecs::component<FProjectileCollisionFragment>(*GetEcsWorld(), "FProjectileCollisionFragment");
	EntityToScriptStruct.Add(TBaseStructure<FProjectileCollisionFragment>::Get(), GetEcsWorld()->entity("FProjectileCollisionFragment"));
	
	//FProjectileCollisionManagerComp 
	flecs::component<FProjectileCollisionManagerFragment>(*GetEcsWorld(), "FProjectileCollisionManagerFragment");
	EntityToScriptStruct.Add(TBaseStructure<FProjectileCollisionManagerFragment>::Get(), GetEcsWorld()->entity("FProjectileCollisionManagerFragment"));

	//FImpactEffect
	flecs::component<FImpactEffectFragment>(*GetEcsWorld(), "FImpactEffect");
	EntityToScriptStruct.Add(TBaseStructure<FImpactEffectFragment>::Get(), GetEcsWorld()->entity("FImpactEffect"));

	// FDamageComp
	flecs::component<FDamageFragment>(*GetEcsWorld(), "FDamageFragment");
	EntityToScriptStruct.Add(TBaseStructure<FDamageFragment>::Get(), GetEcsWorld()->entity("FDamageFragment"));

	//FAreaOfEffectDamageComp
	flecs::component<FAreaOfEffectDamageFragment>(*GetEcsWorld(), "FAreaOfEffectDamageFragment");
	EntityToScriptStruct.Add(TBaseStructure<FAreaOfEffectDamageFragment>::Get(), GetEcsWorld()->entity("FAreaOfEffectDamageFragment"));

	//FProjectile
	flecs::component<FProjectile>(*GetEcsWorld(), "FProjectile");
	EntityToScriptStruct.Add(TBaseStructure<FProjectile>::Get(), GetEcsWorld()->entity("FProjectile"));
}

void UFlecsSubsystem::InitFlecs(UStaticMesh* InMesh)
{
	RegisterFragments();

	auto System_SweepMovement  = GetEcsWorld()->system<FOwnerEntity, FOwnerActor, FLocationFragment, FVelocityFragment, FProjectileCollisionManagerFragment>("Sweep Movement System")
		.iter([this](flecs::iter it, FOwnerEntity* EntityOwner, FOwnerActor* OwnerActor, FLocationFragment* LocFragment, FVelocityFragment* VelFragment, FProjectileCollisionManagerFragment* ProjCollManager) {
		for (int i : it) 
		{
			GEngine->AddOnScreenDebugMessage (1, 2, FColor::Red, FString::Printf(TEXT("Projectiles : %d"), i));
			FVector EndPoint = LocFragment[i].Location + VelFragment[i].Velocity * it.delta_time();
			FHitResult HitResult;
			bool Killed = false;

			for (auto& Collision : ProjCollManager[i].Collisions)
			{
				ECollisionChannel TraceChannel = Collision.TraceChannel;
				if (GetWorld()->SweepSingleByChannel(HitResult, LocFragment[i].Location, EndPoint, VelFragment[i].Velocity.Rotation().Quaternion(), TraceChannel, FCollisionShape::MakeSphere(Collision.Radius), FCollisionQueryParams(), FCollisionResponseParams()))
				{
					if (HitResult.GetActor() != OwnerActor[i].Owner)
					{
						for (auto& CollisionResponse : Collision.CollisionResponses)
						{
							if (CollisionResponse.Bounces-- >= 1)
							{
								for (auto& Event : CollisionResponse.BounceEventsToTrigger)
								{
									
									if (flecs::entity FragmentonentEntity = GetFragmentForScriptStruct(Event.GetScriptStruct()))
									{
										EntityOwner[i].OwnerEntity.set_ptr(FragmentonentEntity, Event.GetMemory());
									}
								}
								for (auto& Tag : CollisionResponse.BounceTagsToAdd)
								{
									if (flecs::entity FragmentonentEntity = GetFragmentForScriptStruct(Tag.GetScriptStruct()))
									{
										EntityOwner[i].OwnerEntity.add(FragmentonentEntity);
									}
								}
								for (auto& Effect : CollisionResponse.ImpactEffects)
								{
									if (flecs::entity FragmentonentEntity = GetFragmentForScriptStruct(Effect.GetScriptStruct()))
									{
										EntityOwner[i].OwnerEntity.set_ptr(FragmentonentEntity, Effect.GetMemory());
									}
								}

								VelFragment[i].Velocity = UKismetMathLibrary::GetReflectionVector(VelFragment[i].Velocity, HitResult.ImpactNormal) * CollisionResponse.BounceFactor;
								LocFragment[i].Location = HitResult.Location + HitResult.Normal * 0.01f;
							}
							else
							{
								
								for (auto& Event : CollisionResponse.EventsToTrigger)
								{
									if (flecs::entity FragmentonentEntity = GetFragmentForScriptStruct(Event.GetScriptStruct()))
									{
										EntityOwner[i].OwnerEntity.set_ptr(FragmentonentEntity, Event.GetMemory());
									}
								}
								for (auto& Tag : CollisionResponse.TagsToAdd)
								{
									if (flecs::entity FragmentonentEntity = GetFragmentForScriptStruct(Tag.GetScriptStruct()))
									{
										EntityOwner[i].OwnerEntity.add(FragmentonentEntity);
									}
								}
								for (auto& Effect : CollisionResponse.ImpactEffects)
								{
									if (flecs::entity FragmentonentEntity = GetFragmentForScriptStruct(Effect.GetScriptStruct()))
									{
										EntityOwner[i].OwnerEntity.set_ptr(FragmentonentEntity, Effect.GetMemory());
									}
								}
							}
						}
						Killed = true;
						break;
					}
				}
			}		
			if (!Killed)
			{
				LocFragment[i].Location = EndPoint;
			}
		}
	});



	// Update the location of the visual component
	auto System_UpdateVisualLocation = GetEcsWorld()->system<FOwnerEntity, FLocationFragment, FVelocityFragment, FVisualEffect>("Update Visual Location System")
		.iter([this](flecs::iter it, FOwnerEntity* EntityOwner, FLocationFragment* LocFragment, FVelocityFragment* VelFragment, FVisualEffect* VisualComponent) {
		for (int i : it) {
			if (VisualComponent[i].VisualEffect)
			{
				VisualComponent[i].VisualEffect->SetWorldLocationAndRotation(FMath::VInterpTo(VisualComponent[i].VisualEffect->GetComponentLocation(), LocFragment[i].Location, it.delta_time(), 35), VelFragment[i].Velocity.Rotation());
			}
		}
	});

	// Update the location of the sound component
	auto System_UpdateSoundLocation = GetEcsWorld()->system<FOwnerEntity, FLocationFragment, FVelocityFragment, FSoundEffect>("Update Sound Location System")
		.iter([this](flecs::iter it, FOwnerEntity* EntityOwner, FLocationFragment* LocFragment, FVelocityFragment* VelFragment, FSoundEffect* SoundComponent) {
		for (int i : it) {
			if (SoundComponent[i].SoundEffect)
			{
				// Sound component might need to interpolate from its location to the LocationFragment location
				SoundComponent[i].SoundEffect->SetWorldLocationAndRotation(FMath::VInterpTo(SoundComponent[i].SoundEffect->GetComponentLocation(), LocFragment[i].Location, it.delta_time(), 35), VelFragment[i].Velocity.Rotation());
			}
		}
	});

	// System to handle timers 
	auto System_Timer = GetEcsWorld()->system<FOwnerEntity, FTimerEventFragment>("Timer System")
		.iter([this](flecs::iter it, FOwnerEntity* EntityOwner, FTimerEventFragment* TimerEvent) {
		for (int i : it) {
			if (TimerEvent[i].TimeSeconds > 0)
			{
				TimerEvent[i].TimeSeconds -= it.delta_time();
			}
			else
			{
				for (auto& Event : TimerEvent[i].EventsToTrigger)
				{
					if (flecs::entity FragmentonentEntity = GetFragmentForScriptStruct(Event.GetScriptStruct()))
					{
						EntityOwner[i].OwnerEntity.add(FragmentonentEntity);
					}
				}
				EntityOwner[i].OwnerEntity.remove<FTimerEventFragment>();
			}
		}
	});

	// Destroy visual components that have been set as dead (So just markasgarbage and visualcomponent)
	auto System_DestroyVisualComponent = GetEcsWorld()->system<FMarkAsGarbage, FVisualEffect>("Destroy Visual Component System")
		.iter([this](flecs::iter it, FMarkAsGarbage* MarkedAsGarbage, FVisualEffect* VisualComponent) {
		for (int i : it) {
			if (VisualComponent[i].VisualEffect)
			{
				VisualComponent[i].VisualEffect->DestroyComponent();
			}
		}
	});

	// Destroy sound components that have been set as dead (So just markasgarbage and soundcomponent)
	auto System_DestroySoundComponent = GetEcsWorld()->system<FMarkAsGarbage, FSoundEffect>("Destroy Sound Component System")
		.iter([this](flecs::iter it, FMarkAsGarbage* MarkedAsGarbage, FSoundEffect* SoundComponent) {
		for (int i : it) {
			if (SoundComponent[i].SoundEffect)
			{
				SoundComponent[i].SoundEffect->DestroyComponent();
			}
		}
	});

	// Kill entities that have been set as dead and have entityowner Fragmentonent
	auto System_KillGarbage = GetEcsWorld()->system<FOwnerEntity, FMarkAsGarbage >("Is Alive Status")
		.iter([this](flecs::iter it, FOwnerEntity* EntityOwner, FMarkAsGarbage* MarkedAsGarbage) {
		for (int i : it) {
			EntityOwner[i].OwnerEntity.destruct();
		}
	});

	// Apply local acceleration to entities that have LocalAccelerationFragment
	auto System_ApplyLocalAcceleration = GetEcsWorld()->system<FLocalAccelerationFragment, FVelocityFragment>("Apply Local Acceleration System")
		.iter([this](flecs::iter it, FLocalAccelerationFragment* LocalAccFragment, FVelocityFragment* VelFragment) {
		for (int i : it) {
			VelFragment[i].Velocity += VelFragment[i].Velocity.GetSafeNormal() * LocalAccFragment[i].Acceleration * it.delta_time();
		}
	});

	// Apply gravity force to entities that have gravity Fragmentonent
	auto System_ApplyGravity = GetEcsWorld()->system<FGravityAccelerationFragment, FVelocityFragment>("Apply Gravity System")
		.iter([this](flecs::iter it, FGravityAccelerationFragment* GravityFragment, FVelocityFragment* VelFragment) {
		for (int i : it) {
			VelFragment[i].Velocity += FVector(0, 0, GravityFragment[i].Gravity) * it.delta_time();
		}
	});

	// Apply drag force to entities that have drag Fragmentonent
	auto System_ApplyDrag = GetEcsWorld()->system<FDragFragment, FVelocityFragment>("Apply Drag System")
		.iter([this](flecs::iter it, FDragFragment* DragFragment, FVelocityFragment* VelFragment) {
		for (int i : it) {
			VelFragment[i].Velocity *= FMath::Pow(1 - DragFragment[i].Drag, it.delta_time());
		}
	});

	// Limit max velocity of entities that have max speed Fragmentonent
	auto System_LimitMaxSpeed = GetEcsWorld()->system<FMaxSpeedFragment, FVelocityFragment>("Limit Max Speed System")
		.iter([this](flecs::iter it, FMaxSpeedFragment* MaxSpeedFragment, FVelocityFragment* VelFragment) {
		for (int i : it) {
			if (VelFragment[i].Velocity.Size() > MaxSpeedFragment[i].MaxSpeed)
			{
				VelFragment[i].Velocity = VelFragment[i].Velocity.GetSafeNormal() * MaxSpeedFragment[i].MaxSpeed;
			}
		}
	});
	
	/*
	auto System_SweepMovement2 = GetEcsWorld()->system<FLocationFragment, FVelocityFragment>("Sweep Movement System 2")
		.iter([this](flecs::iter it, FLocationFragment* LocFragment, FVelocityFragment* VelFragment) {
		for (int i : it) {
			FVector EndPoint = LocFragment[i].Location + VelFragment[i].Velocity * it.delta_time();
			FHitResult HitResult;
			if (GetWorld()->SweepSingleByChannel(HitResult, LocFragment[i].Location, EndPoint, VelFragment[i].Velocity.Rotation().Quaternion(), ECC_Visibility, FCollisionShape::MakeSphere(1), FCollisionQueryParams(), FCollisionResponseParams()))
			{
				DrawDebugPoint(GetWorld(), LocFragment[i].Location, 3, FColor::Orange, false, 0);
			}
			else
			{
				LocFragment[i].Location = EndPoint;
				DrawDebugPoint(GetWorld(), LocFragment[i].Location, 1,FColor::Green, false, 0);
			}
		}
	});
	*/
	/*
	auto System_ApplyGravity = GetEcsWorld()->system<FLocationFragment, FVelocityFragment>("Apply Gravity System")
	.iter([this](flecs::iter it, FVelocityFragment* VelFragment, FGravityAccelerationFragment* GravAccel) {
		for (int i : it) {
			VelFragment[i].Velocity += FVector(0, 0, GravAccel[i].Gravity) * it.delta_time();
		}
	});
	
	auto System_ApplyLocalAcceleration = GetEcsWorld()->system<FLocationFragment, FVelocityFragment>("Apply Local Acceleration System")
	.iter([this](flecs::iter it, FVelocityFragment* VelFragment, FLocalAccelerationFragment* LocalAccel) {
		for (int i : it) {
			VelFragment[i].Velocity += LocalAccel[i].Acceleration * it.delta_time();
		}
	});
	
	// Limit speed to max speed @todo optimize
	auto System_LimitSpeed = GetEcsWorld()->system<FLocationFragment, FVelocityFragment>("Limit Speed System")
	.iter([this](flecs::iter it, FVelocityFragment* VelFragment, FMaxSpeedFragment* MaxSpeed) {
		for (int i : it) {
			if (
				VelFragment[i].Velocity.Size() > MaxSpeed[i].MaxSpeed) {
				VelFragment[i].Velocity = VelFragment[i].Velocity.GetSafeNormal() * MaxSpeed[i].MaxSpeed;
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
{
	return FFlecsEntityHandle{ -1 };
}

void UFlecsSubsystem::SpawnProjectileFromPrefab(UFlecsPrefabDefinition* FlecsPrefabDefinition, FVector Location, FVector MuzzleLocation, FRotator Rotation, AActor* Owner)
{
	flecs::entity entity = GetEcsWorld()->entity();
	entity.set<FOwnerEntity>({ FOwnerEntity(entity.id(), entity) });
	if (IsValid(Owner)) entity.set<FOwnerActor>({ FOwnerActor(Owner) });

	if (FlecsPrefabDefinition->ProjectileFX.VFXSpawnChance >= FMath::RandRange(0, 1))
	{
		entity.set<FVisualEffect>({
			FVisualEffect(UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FlecsPrefabDefinition->ProjectileFX.ProjectileFXClass, MuzzleLocation, Rotation))
			});
	}
	if (FlecsPrefabDefinition->ProjectileFX.SFXSpawnChance >= FMath::RandRange(0, 1))
	{
		entity.set<FSoundEffect>({
			FSoundEffect(UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FlecsPrefabDefinition->ProjectileFX.ProjectileSXClass, MuzzleLocation, Rotation))
			});
	}

	FFLECSPrefabClassType PrefabClass = FlecsPrefabDefinition->GetPrefabClass();
	ApplyPrefabClassToEntity(entity, PrefabClass);

	entity.set<FLocationFragment>({ Location });
	entity.set<FVelocityFragment>({ Rotation.Quaternion().RotateVector(entity.get<FVelocityFragment>()->Velocity) });
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
