// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "InstrumentSample.h"

#include "SamplerInstrument.generated.h"

UCLASS( ClassGroup=(MIDI), meta=(BlueprintSpawnableComponent) )
class MIDIUTILS_API USamplerInstrument : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditAnywhere, Category = "MIDI")
	bool bUserInput = false;

	UPROPERTY(EditAnywhere, Category = "MIDI", meta = (ClampMin = "1", ClampMax = "128"))
	int32 MaxVoices;

	UPROPERTY(EditAnywhere, Category = "MIDI")
	UInstrumentSample* InstrumentData;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "MIDI")
    void OnMIDINoteOn(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Note, int32 Velocity);

	UFUNCTION(BlueprintCallable, Category = "MIDI")
	void OnMIDINoteOff(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Note, int32 Velocity);
	
private:
	TQueue<UAudioComponent*> AvailableVoices;

	TMap<int32, UAudioComponent*> ActiveVoices;

	int32 GetClosestSampleIndex(int32 Note);

	TArray<USoundWave*> GetSamplesForNoteAndVelocity(int32 Note, int32 Velocity);
};


