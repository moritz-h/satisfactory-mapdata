#include "MapDataSubsystem.h"

#include "FGDropPod.h"
#include "Resources/FGResourceNodeFrackingCore.h"
#include "Resources/FGResourceNode.h"
#include "Resources/FGResourceDeposit.h"

#include "JsonUtils.h"
#include "MapDataStructs.h"

AMapDataSubsystem::AMapDataSubsystem()
{
    PrimaryActorTick.bCanEverTick = true;
    SetActorTickEnabled(true);
}

void AMapDataSubsystem::BeginPlay()
{
    Super::BeginPlay();

    FString MapDataDir = FPaths::ProjectUserDir();
    MapDataDir.Append(TEXT("MapData-Export"));

    IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

    if (!FileManager.DirectoryExists(*MapDataDir)) {
        if (!FileManager.CreateDirectory(*MapDataDir)) {
            UE_LOG(LogTemp, Error, TEXT("MapData: Cannot create export dir!"));
            return;
        }
    }

    ExportAllActors(MapDataDir + TEXT("/all-actors.json"));
    ExportResourceNodes(MapDataDir + TEXT("/resource-nodes.json"));
    ExportDropPods(MapDataDir + TEXT("/drop-pods.json"));
}

void AMapDataSubsystem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

void AMapDataSubsystem::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AMapDataSubsystem::ExportAllActors(const FString& FileName)
{
    TArray<FActorInfo> InfoArray;
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);
    for (AActor* TActor : FoundActors) {
        FActorInfo ActorInfo;
        ActorInfo.Name = TActor->GetName();
        ActorInfo.Class = TActor->GetClass()->GetName();
        InfoArray.Emplace(ActorInfo);
    }
    JsonUtils::WriteStructArrayToJsonFile(InfoArray, TEXT("actors"), *FileName);
}

void AMapDataSubsystem::ExportResourceNodes(const FString& FileName)
{
    TArray<FResourceNodeInfo> InfoArray;
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFGResourceNodeBase::StaticClass(), FoundActors);
    for (AActor* TActor : FoundActors) {
        AFGResourceNodeBase* resource = Cast<AFGResourceNodeBase>(TActor);
        if (Cast<AFGResourceDeposit>(resource)) {
            continue;
        }
        if (resource != nullptr) {
            FResourceNodeInfo ResourceNodeInfo;
            ResourceNodeInfo.Name = resource->GetName();
            ResourceNodeInfo.ResourceName = resource->GetResourceName().ToString();
            // (int32)resource->GetResourceNodeType();
            if (Cast<AFGResourceNode>(resource)) {
                AFGResourceNode* obj = Cast<AFGResourceNode>(resource);
                ResourceNodeInfo.ResoucePurity = obj->GetResoucePurityText().ToString();
                // (int32)obj->GetResoucePurity();
            } else {
                ResourceNodeInfo.ResoucePurity = TEXT("");
            }
            InfoArray.Emplace(ResourceNodeInfo);
        }
    }
    JsonUtils::WriteStructArrayToJsonFile(InfoArray, TEXT("resource-nodes"), *FileName);
}

void AMapDataSubsystem::ExportDropPods(const FString& FileName)
{
    TArray<FDropPodInfo> InfoArray;
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFGDropPod::StaticClass(), FoundActors);
    for (AActor* TActor : FoundActors) {
        AFGDropPod* DropPod = Cast<AFGDropPod>(TActor);
        if (DropPod != nullptr) {
            FDropPodInfo DropPodInfo;
            DropPodInfo.Name = DropPod->GetName();

            // Power
            FFloatProperty* PowerConsumption = Cast<FFloatProperty>(DropPod->GetClass()->FindPropertyByName(FName(TEXT("mPowerConsumption"))));
            DropPodInfo.PowerConsumption = PowerConsumption->GetPropertyValue_InContainer(DropPod);

            // Items
            FStructProperty* RepairAmount = Cast<FStructProperty>(DropPod->GetClass()->FindPropertyByName(FName(TEXT("mRepairAmount"))));
            FItemAmount* ItemAmount = RepairAmount->ContainerPtrToValuePtr<FItemAmount>(DropPod);
            auto ItemClass = ItemAmount->ItemClass;
            if (ItemClass) {
                DropPodInfo.RepairItemClass = ItemClass->GetName();
                DropPodInfo.RepairItemName = UFGItemDescriptor::GetItemName(ItemClass).ToString();
            }
            DropPodInfo.RepairItemAmount = ItemAmount->Amount;

            InfoArray.Emplace(DropPodInfo);
        }
    }
    JsonUtils::WriteStructArrayToJsonFile(InfoArray, TEXT("drop-pods"), *FileName);
}
