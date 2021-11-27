#include "MapDataSubsystem.h"

AMapDataSubsystem::AMapDataSubsystem()
{
    PrimaryActorTick.bCanEverTick = true;
    SetActorTickEnabled(true);
}

void AMapDataSubsystem::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("########## AMapDataSubsystem::BeginPlay"));
}

void AMapDataSubsystem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    UE_LOG(LogTemp, Warning, TEXT("########## AMapDataSubsystem::EndPlay"));
}

void AMapDataSubsystem::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UE_LOG(LogTemp, Warning, TEXT("########## AMapDataSubsystem::Tick"));
}
