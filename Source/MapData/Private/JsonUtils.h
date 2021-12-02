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
    static TArray<TSharedPtr<FJsonValue>> StructArrayToJsonValueArray(const TArray<Struct_T>& StructArray)
    {
        TArray<TSharedPtr<FJsonValue>> JsonArray;

        for (const Struct_T& item : StructArray) {
            TSharedRef<FJsonObject> JsonObj = MakeShareable(new FJsonObject);
            FJsonObjectConverter::UStructToJsonObject(Struct_T::StaticStruct(), &item, JsonObj, 0, 0);
            TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(JsonObj));
            JsonArray.Add(JsonValue);
        }
        return JsonArray;
    }

    template<typename Struct_T>
    static void WriteStructArrayToJsonFile(const TArray<Struct_T>& StructArray, const FString& ObjName, const FString& FileName)
    {
        TArray<TSharedPtr<FJsonValue>> ObjArray = JsonUtils::StructArrayToJsonValueArray(StructArray);
        TSharedRef<FJsonObject> JsonRoot = MakeShareable(new FJsonObject);
        JsonRoot->SetArrayField(ObjName, ObjArray);
        JsonUtils::WriteJsonToFile(JsonRoot, *FileName);
    }
};
