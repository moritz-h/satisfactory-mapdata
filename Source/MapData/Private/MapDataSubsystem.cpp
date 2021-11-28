#include "MapDataSubsystem.h"

#include "Resources/FGResourceNodeFrackingCore.h"
#include "Resources/FGResourceNode.h"
#include "Resources/FGResourceDeposit.h"

AMapDataSubsystem::AMapDataSubsystem()
{
    PrimaryActorTick.bCanEverTick = true;
    SetActorTickEnabled(true);
}

void AMapDataSubsystem::BeginPlay()
{
    Super::BeginPlay();

    // FPaths::RootDir()         | C:/Program Files (x86)/Steam/steamapps/common/Satisfactory/
    // FPaths::ProjectUserDir()  | %LOCALAPPDATA%/FactoryGame/
    // FPaths::ProjectSavedDir() | %LOCALAPPDATA%/FactoryGame/Saved/

    FString MapDataDir = FPaths::ProjectUserDir();
    MapDataDir.Append(TEXT("MapData-Export"));

    IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

    if (!FileManager.DirectoryExists(*MapDataDir))
    {
        if (!FileManager.CreateDirectory(*MapDataDir))
        {
            UE_LOG(LogTemp, Error, TEXT("MapData: Cannot create export dir!"));
            return;
        }
    }

    TArray<FString> Lines;
    TArray<AActor*> FoundActors;
    FString FileName;

    // All Actor names
    Lines.Empty();
    FoundActors.Empty();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);
    for (AActor* TActor : FoundActors) {
        Lines.Add(TActor->GetName());
    }
    FileName = MapDataDir;
    FileName.Append(TEXT("/all-actor-names.txt"));
    FFileHelper::SaveStringArrayToFile(Lines, *FileName);

    // Resource Nodes
    Lines.Empty();
    FoundActors.Empty();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFGResourceNodeBase::StaticClass(), FoundActors);
    for (AActor* TActor : FoundActors) {
        AFGResourceNodeBase* resource = Cast<AFGResourceNodeBase>(TActor);
        if (Cast<AFGResourceDeposit>(resource)) {
            continue;
        }
        if (resource != nullptr) {
            FString name = resource->GetName();
            FString resName = resource->GetResourceName().ToString();
            // (int32)resource->GetResourceNodeType();
            if (Cast<AFGResourceNode>(resource)) {
                AFGResourceNode* obj = Cast<AFGResourceNode>(resource);
                FString p = obj->GetResoucePurityText().ToString();
                // (int32)obj->GetResoucePurity();
                Lines.Add(FString::Printf(TEXT("%s,%s,%s"), *name, *resName, *p));
            }
            else {
                Lines.Add(FString::Printf(TEXT("%s,%s"), *name, *resName));
            }
        }
    }
    FileName = MapDataDir;
    FileName.Append(TEXT("/resource-nodes.txt"));
    FFileHelper::SaveStringArrayToFile(Lines, *FileName);
}

void AMapDataSubsystem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

void AMapDataSubsystem::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}
