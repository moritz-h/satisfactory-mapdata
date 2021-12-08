#include "MapDataSubsystem.h"

#include "FGDropPod.h"
#include "FGResourceNodeGeyser.h"
#include "Resources/FGResourceDeposit.h"
#include "Resources/FGResourceNode.h"
#include "Resources/FGResourceNodeFrackingCore.h"
#include "Resources/FGResourceNodeFrackingSatellite.h"

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

    ExportAllActors(MapDataDir + TEXT("/AllActors.json"));
    ExportResourceNodes(MapDataDir + TEXT("/ResourceNodes.json"));
    ExportDropPods(MapDataDir + TEXT("/DropPods.json"));
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
    for (AActor* Actor : FoundActors) {
        InfoArray.Emplace(FActorInfo(Actor));
    }
    JsonUtils::WriteStructArrayToJsonFile(InfoArray, TEXT("actors"), *FileName);
}

void AMapDataSubsystem::ExportResourceNodes(const FString& FileName)
{
    FResourceInfo ResourceInfo;
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFGResourceNodeBase::StaticClass(), FoundActors);
    for (AActor* Actor : FoundActors) {
        AFGResourceNodeBase* ResourceNodeBase = Cast<AFGResourceNodeBase>(Actor);
        if (ResourceNodeBase == nullptr) {
            continue;
        }
        // ignore deposits
        if (Cast<AFGResourceDeposit>(ResourceNodeBase)) {
            continue;
        }
        AFGResourceNodeFrackingSatellite* FrackingSatellite = Cast<AFGResourceNodeFrackingSatellite>(ResourceNodeBase);
        if (FrackingSatellite != nullptr) {
            ResourceInfo.FrackingSatellites.Emplace(FResourceFrackingSatelliteInfo(FrackingSatellite));
            continue;
        }
        AFGResourceNodeFrackingCore* FrackingCore = Cast<AFGResourceNodeFrackingCore>(ResourceNodeBase);
        if (FrackingCore != nullptr) {
            ResourceInfo.FrackingCores.Emplace(FResourceNodeBaseInfo(FrackingCore));
            continue;
        }
        AFGResourceNodeGeyser* Geyser = Cast<AFGResourceNodeGeyser>(ResourceNodeBase);
        if (Geyser != nullptr) {
            ResourceInfo.Geysers.Emplace(FResourceNodeInfo(Geyser));
            continue;
        }
        AFGResourceNode* ResourceNode = Cast<AFGResourceNode>(ResourceNodeBase);
        if (ResourceNode != nullptr) {
            ResourceInfo.Nodes.Emplace(FResourceNodeInfo(ResourceNode));
            continue;
        }
        UE_LOG(LogTemp, Warning, TEXT("MapData: Unknown ResourceNode!"));
    }
    JsonUtils::WriteStructToJsonFile(ResourceInfo, *FileName);
}

void AMapDataSubsystem::ExportDropPods(const FString& FileName)
{
    TArray<FDropPodInfo> InfoArray;
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFGDropPod::StaticClass(), FoundActors);
    for (AActor* TActor : FoundActors) {
        AFGDropPod* DropPod = Cast<AFGDropPod>(TActor);
        if (DropPod != nullptr) {
            InfoArray.Emplace(FDropPodInfo(DropPod));
        }
    }
    JsonUtils::WriteStructArrayToJsonFile(InfoArray, TEXT("dropPods"), *FileName);
}
