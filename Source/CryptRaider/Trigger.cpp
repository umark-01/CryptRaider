// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger.h"

UTrigger::UTrigger()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UTrigger::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);
 	
	for(AActor* Actor : Actors)
	{
        if(Actor->ActorHasTag(AcceptableActorTag))
		{
        	UE_LOG(LogTemp, Display, TEXT("Unlocking"));
		}
	}

}
