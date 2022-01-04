#pragma once

#include "EngineMinimal.h"
#include "EngineUtils.h"
#include "JsonObjectConverter.h"

class JsonUtils {
public:
    static void WriteJsonToFile(TSharedRef<FJsonObject> JsonObject, const TCHAR* FileName)
    {
        FString OutputString;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
        FJsonSerializer::Serialize(JsonObject, Writer);
        FFileHelper::SaveStringToFile(OutputString, FileName);
    }

    template<typename Struct_T>
    static void WriteStructToJsonFile(const Struct_T& Struct, const FString& FileName)
    {
        TSharedRef<FJsonObject> JsonRoot = MakeShared<FJsonObject>();
        FJsonObjectConverter::UStructToJsonObject(Struct_T::StaticStruct(), &Struct, JsonRoot, 0, 0);
        JsonUtils::WriteJsonToFile(JsonRoot, *FileName);
    }

    template<typename Struct_T>
    static TArray<TSharedPtr<FJsonValue>> StructArrayToJsonValueArray(const TArray<Struct_T>& StructArray)
    {
        TArray<TSharedPtr<FJsonValue>> JsonArray;

        for (const Struct_T& item : StructArray) {
            TSharedRef<FJsonObject> JsonObj = MakeShared<FJsonObject>();
            FJsonObjectConverter::UStructToJsonObject(Struct_T::StaticStruct(), &item, JsonObj, 0, 0);
            JsonArray.Add(MakeShared<FJsonValueObject>(JsonObj));
        }
        return JsonArray;
    }

    template<typename Struct_T>
    static void WriteStructArrayToJsonFile(const TArray<Struct_T>& StructArray, const FString& ObjName, const FString& FileName)
    {
        TArray<TSharedPtr<FJsonValue>> ObjArray = JsonUtils::StructArrayToJsonValueArray(StructArray);
        TSharedRef<FJsonObject> JsonRoot = MakeShared<FJsonObject>();
        JsonRoot->SetArrayField(ObjName, ObjArray);
        JsonUtils::WriteJsonToFile(JsonRoot, *FileName);
    }
};
