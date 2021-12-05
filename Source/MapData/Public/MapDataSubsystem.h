#pragma once

#include "CoreMinimal.h"
#include "Subsystem/ModSubsystem.h"

#include "MapDataSubsystem.generated.h"

/**
 *
 */
UCLASS()
class MAPDATA_API AMapDataSubsystem : public AModSubsystem
{
    GENERATED_BODY()

    AMapDataSubsystem();

    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void Tick(float DeltaSeconds) override;

protected:
    void ExportAllActors(const FString& FileName);

    void ExportResourceNodes(const FString& FileName);

    void ExportDropPods(const FString& FileName);
};
