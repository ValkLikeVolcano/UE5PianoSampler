// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "InstrumentSample.generated.h"

//Line 10
USTRUCT(BlueprintType)
struct FVelocityLayer
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MIDI")
    FString LayerName; // Easy identification

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MIDI")
    int32 MinVelocity; // The minimum velocity that triggers this layer

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MIDI")
    TArray<USoundWave*> Samples; // The samples for this layer
};


USTRUCT(BlueprintType)
struct FNoteSamples
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MIDI")
    TArray<FVelocityLayer> VelocityLayer;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MIDI")
    USoundWave* ReleaseSample;
};

UCLASS()
class MIDIUTILS_API UInstrumentSample : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MIDI")
    TMap<int32, FNoteSamples> NoteSamplesMap;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MIDI")
    USoundBase* MetasoundInstrument;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MIDI")
    int32 SampleIncrements = 3;
};
