// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "Engine/World.h"
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// FRotator MyRotation = GetComponentRotation();
	// FString RotationString = MyRotation.ToCompactString();
	// UE_LOG(LogTemp, Display, TEXT("Grabber Rotation: %s"), *RotationString);

	
	// float Time = GetWorld()->TimeSeconds;
	// UE_LOG(LogTemp, Display, TEXT("Time elapsed: %f"), Time);

	// FVector Start = GetComponentLocation();
	// FVector End = Start + GetForwardVector() * MaxGrabDistance;
	// DrawDebugLine(GetWorld(), Start, End, FColor::Red);

	// FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	// FHitResult HitResult;
	// bool HasHit = GetWorld()->SweepSingleByChannel(
	// 	HitResult,
	// 	Start, End, 
	// 	FQuat::Identity, 
	// 	ECC_GameTraceChannel2,
	// 	Sphere
	// 	);

	// if(HasHit){
	// 	AActor* HitActor = HitResult.GetActor();
	// 	UE_LOG(LogTemp, Display, TEXT("Actor hit %s"), *HitActor->GetActorNameOrLabel());
	// }
	// else{
	// 	UE_LOG(LogTemp, Display, TEXT("No Actor hit"));
	// }

	// float Damage = 0;
	// float& DamageRef = Damage;
	// DamageRef = 5;
	// UE_LOG(LogTemp, Display, TEXT("DamageRef: %f Damage: %f"), DamageRef, Damage);
	
	// float Damage;
	// if (HasDamage(Damage))
	// {
	// 	PrintDamage(Damage);
	// }
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

void UGrabber::Grab()
{	
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle == nullptr)
	{
		return;
	}

	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);
	if(HasHit){
		// DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green, false, 5);
		// DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, 5);
		// AActor* HitActor = HitResult.GetActor();
		// UE_LOG(LogTemp, Display, TEXT("Actor hit %s"), *HitActor->GetActorNameOrLabel());
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		AActor* HitActor = HitResult.GetActor();
		HitActor->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
			);
	}
	// else{
	// 	UE_LOG(LogTemp, Display, TEXT("No Actor hit"));
	// }
}

void UGrabber::Release()
{
    //UE_LOG(LogTemp, Display, TEXT("Released grabber"));
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");

		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		PhysicsHandle->ReleaseComponent();
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(Result == nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Grabber requires a PHC"));
	}
	return Result;
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(
		OutResult,
		Start, End, 
		FQuat::Identity, 
		ECC_GameTraceChannel2,
		Sphere
		);
}

/*
void UGrabber::PrintDamage(const float& Damage)
{
	//Damage = 2;
	UE_LOG(LogTemp, Display, TEXT("Damage: %f"), Damage);
}

bool UGrabber::HasDamage(float& OutDamage)
{
	OutDamage = 5;
	return true;
}
*/
