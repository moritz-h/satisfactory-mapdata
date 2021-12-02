#pragma once

#include "CoreMinimal.h"

#include "MapDataStructs.generated.h"

USTRUCT()
struct FActorInfo
{
    GENERATED_BODY()

    UPROPERTY()
    FString Name;

    UPROPERTY()
    FString Class;
};

USTRUCT()
struct FResourceNodeInfo
{
    GENERATED_BODY()

    UPROPERTY()
    FString Name;

    UPROPERTY()
    FString ResourceName;

    UPROPERTY()
    FString ResoucePurity;
};
