// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "MIDIDeviceManager.h"
#include "Chord.h"
#include "ChordIntervals.h"

#include "MIDIControllerSubsystem.generated.h"

//Note Event Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FMIDIControllerNoteOnDelegate, UMIDIDeviceInputController*, MIDIDeviceController, int32, Timestamp, int32, Channel, int32, Note, int32, Velocity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FMIDIControllerNoteOffDelegate, UMIDIDeviceInputController*, MIDIDeviceController, int32, Timestamp, int32, Channel, int32, Note, int32, Velocity);

//Chord Event Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChordCallback, FChord, Chord);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVoidCallback, FChord, Chord);


UCLASS()
class MIDIUTILS_API UMIDIControllerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "MIDI")
	void SetMIDIControllerByID(int32 ControllerID, int32 BufferSize);

	UFUNCTION(BlueprintCallable, Category = "MIDI")
	TArray<FFoundMIDIDevice> GetMIDIInputDevices();

	UPROPERTY(BlueprintReadOnly, Category = "MIDI")
	UMIDIDeviceInputController* ActiveMIDIController;

private:
	UFUNCTION()
	void HandleMIDIOnTrigger(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Note, int32 Velocity);
	UFUNCTION()
	void HandleMIDIOffTrigger(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Note, int32 Velocity);

public:
	UPROPERTY(BlueprintAssignable, Category = "MIDI")
	FMIDIControllerNoteOnDelegate NoteOnTrigger;
	UPROPERTY(BlueprintAssignable, Category = "MIDI")
	FMIDIControllerNoteOffDelegate NoteOffTrigger;
	
	//Chord Detection
	//TArray<FChordIntervals> ChordBuffer{};

	TArray<int32> PlayedNotes{};

	UPROPERTY(BlueprintReadOnly, Category = "MIDI")
	FChord CurrentChord{};

private:
	bool bHasChord{ false };

	FChord DetectChords();

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "MIDI")
	FChordCallback OnChordDetected{};
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "MIDI")
	FVoidCallback OnChordStopped{};
};
