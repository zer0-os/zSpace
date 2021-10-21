// Fill out your copyright notice in the Description page of Project Settings.


#include "CopySetupInfo.h"

#include "DesktopPlatformModule.h"
#include "JsonObjectConverter.h"
#include "Components/Widget.h"
#include "IDesktopPlatform.h"
#include "Misc/FileHelper.h"
#include "AssetSelection.h"
#include "EditorAssetLibrary.h"
#include "PluginTypes.h"


UCopySetupInfo::UCopySetupInfo()
{
	IPlatformFile& PlatFile = FPlatformFileManager::Get().GetPlatformFile();

	if (!PlatFile.DirectoryExists(*GetJsonDir().ToString()))
		PlatFile.CreateDirectory(*GetJsonDir().ToString());
}

void UCopySetupInfo::CreateFileMaterialInfo(const FString& CarName)
{
	TArray<FAssetData> OutSelectedAssets;
	AssetSelectionUtils::GetSelectedAssets(OutSelectedAssets);

	FMaterialInfoArray MaterialInfoArray;
	
	for(FAssetData& AssetData : OutSelectedAssets)
	{
		const UStaticMesh* StaticMesh = Cast<UStaticMesh>(AssetData.GetAsset());
		if (IsValid(StaticMesh))
		{
			FMaterialInfo MaterialInfo;
			MaterialInfo.MeshName = AssetData.PackageName;;
			
			TArray<FStaticMaterial> StaticMaterials = StaticMesh->GetStaticMaterials();
			for(const FStaticMaterial& StaticMaterial : StaticMaterials)
			{
				FAssetData Material(StaticMaterial.MaterialInterface);
				MaterialInfo.MaterialsPath.Add(Material.PackageName);
			}

			MaterialInfoArray.MaterialInfos.Add(MaterialInfo);
		}
	}

	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString<FMaterialInfoArray>(MaterialInfoArray, JsonString);

	FString FileName = GetJsonDir().ToString() + "/" + CarName + ".json";
	
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		FString OutputString;
		TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
		FJsonSerializer::Serialize(JsonObject->GetArrayField("materialInfos"), Writer);
		FFileHelper::SaveStringToFile(OutputString, *FileName, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_None);
	}
}

void UCopySetupInfo::SetMaterialsByFile()
{
	TArray<FAssetData> OutSelectedAssets;
	AssetSelectionUtils::GetSelectedAssets(OutSelectedAssets);
	
	TArray<FString> OutFileNames;
	
	if (GEditor)
	{
		const void* ParentWindowHandle = GEditor->GetActiveViewport()->GetWindow();
		IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
		if (DesktopPlatform)
		{
			constexpr uint32 SelectionFlag = 0; //A value of 0 represents single file selection while a value of 1 represents multiple file selection
			const FString DefaultPath = GetJsonDir().ToString();
			DesktopPlatform->OpenFileDialog(ParentWindowHandle, "Car Json", DefaultPath, FString(""), ".json", SelectionFlag, OutFileNames);
		}
	}
	
	if (OutFileNames.Num() == 0) return;

	// Read File
	FString OutString;
	FFileHelper::LoadFileToString(OutString, *OutFileNames[0]);

	// Convert Struct
	TArray<FMaterialInfo> Data;
	FJsonObjectConverter::JsonArrayStringToUStruct<FMaterialInfo>(OutString, &Data);

	for (FAssetData& AssetData : OutSelectedAssets)
	{
		UStaticMesh* StaticMesh = Cast<UStaticMesh>(AssetData.GetAsset());
		if (!IsValid(StaticMesh)) continue;
			
		for (const FMaterialInfo& MaterialInfo : Data)
		{
			if (AssetData.PackageName == MaterialInfo.MeshName)
			{
				int32 It(0);
				for (const FName& MaterialName : MaterialInfo.MaterialsPath)
				{
					FSoftObjectPath Material(MaterialName);
					UMaterialInterface* MaterialInterface = Cast<UMaterialInterface>(Material.TryLoad());
					if (IsValid(MaterialInterface))
					{
						StaticMesh->Modify();
						StaticMesh->SetMaterial(It, MaterialInterface);
					}
					It++;
				}
			}
		}

		UEditorAssetLibrary::SaveLoadedAsset(StaticMesh, true);
	}
}

FName UCopySetupInfo::GetJsonDir()
{
	const FString Path = FPaths::ProjectDir() + "/Plugins/SetupCopy/JsonFiles/";
	return *Path;
}
