#pragma once

#include "CoreMinimal.h"
#include "FGDropPod.h"
#include "Resources/FGResourceNode.h"
#include "Resources/FGResourceNodeFrackingCore.h"
#include "Resources/FGResourceNodeFrackingSatellite.h"

#include "MapDataStructs.generated.h"

USTRUCT()
struct FItemInfo
{
    GENERATED_BODY()

    FItemInfo() = default;
    FItemInfo(TSubclassOf<UFGItemDescriptor> Item) {
        Class = Item->GetName();
        Name = UFGItemDescriptor::GetItemName(Item).ToString();
    }

    UPROPERTY()
    FString Class = TEXT("");

    UPROPERTY()
    FString Name = TEXT("");
};


USTRUCT()
struct FActorInfo
{
    GENERATED_BODY()

    FActorInfo() = default;
    FActorInfo(const AActor* Actor) {
        Name = Actor->GetName();
        Path = Actor->GetFullGroupName(false);
        Class = Actor->GetClass()->GetPathName();
        Location = Actor->GetActorLocation();
        Quat = Actor->GetActorQuat();
        Scale = Actor->GetActorScale();
    }

    UPROPERTY()
    FString Name = TEXT("");

    UPROPERTY()
    FString Path = TEXT("");

    UPROPERTY()
    FString Class = TEXT("");

    UPROPERTY()
    FVector Location = FVector::ZeroVector;

    UPROPERTY()
    FQuat Quat = FQuat::Identity;

    UPROPERTY()
    FVector Scale = FVector::ZeroVector;
};

USTRUCT()
struct FResourceNodeBaseInfo : public FActorInfo
{
    GENERATED_BODY()

    FResourceNodeBaseInfo() = default;
    FResourceNodeBaseInfo(const AFGResourceNodeBase* ResourceNode) : FActorInfo(ResourceNode) {
        Item = FItemInfo(ResourceNode->GetResourceClass());
    }

    UPROPERTY()
    FItemInfo Item;
};

USTRUCT()
struct FResourceNodeInfo : public FResourceNodeBaseInfo
{
    GENERATED_BODY()

    FResourceNodeInfo() = default;
    FResourceNodeInfo(AFGResourceNode* ResourceNode) : FResourceNodeBaseInfo(ResourceNode) {
        Purity = ResourceNode->GetResoucePurity();
    }

    UPROPERTY()
    TEnumAsByte<EResourcePurity> Purity;
};

USTRUCT()
struct FResourceFrackingSatelliteInfo : public FResourceNodeInfo
{
    GENERATED_BODY()

    FResourceFrackingSatelliteInfo() = default;
    FResourceFrackingSatelliteInfo(AFGResourceNodeFrackingSatellite* FrackingSatellite) : FResourceNodeInfo(FrackingSatellite) {
        if (FrackingSatellite->GetCore().IsValid()) {
            FrackingCore = FrackingSatellite->GetCore().Get()->GetFullGroupName(false);
        }
    }

    UPROPERTY()
    FString FrackingCore = TEXT("");
};

USTRUCT()
struct FResourceInfo
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FResourceNodeInfo> Nodes;

    UPROPERTY()
    TArray<FResourceNodeInfo> Geysers;

    UPROPERTY()
    TArray<FResourceNodeBaseInfo> FrackingCores;

    UPROPERTY()
    TArray<FResourceFrackingSatelliteInfo> FrackingSatellites;
};

USTRUCT()
struct FDropPodInfo : public FActorInfo
{
    GENERATED_BODY()

    FDropPodInfo() = default;
    FDropPodInfo(AFGDropPod* DropPod) : FActorInfo(DropPod) {
        FFloatProperty* PowerConsumptionProperty = Cast<FFloatProperty>(DropPod->GetClass()->FindPropertyByName(FName(TEXT("mPowerConsumption"))));
        PowerConsumption = PowerConsumptionProperty->GetPropertyValue_InContainer(DropPod);

        FStructProperty* RepairAmountProperty = Cast<FStructProperty>(DropPod->GetClass()->FindPropertyByName(FName(TEXT("mRepairAmount"))));
        FItemAmount* ItemAmount = RepairAmountProperty->ContainerPtrToValuePtr<FItemAmount>(DropPod);
        if (ItemAmount->ItemClass) {
            RepairItem = FItemInfo(ItemAmount->ItemClass);
        }
        RepairItemAmount = ItemAmount->Amount;
    }

    UPROPERTY()
    float PowerConsumption;

    UPROPERTY()
    FItemInfo RepairItem;

    UPROPERTY()
    int RepairItemAmount;
};
