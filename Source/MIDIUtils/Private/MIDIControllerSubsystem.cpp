// Fill out your copyright notice in the Description page of Project Settings.


#include "MIDIControllerSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UMIDIControllerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Warning, TEXT("MIDI Controller Subsystem Initialized"));

#if 0

	//Quarts

	//Major
	ChordBuffer.Add(FChordIntervals(EChordType::M7, { 4, 3, 4 }, EChordInversion::Root));
	ChordBuffer.Add(FChordIntervals(EChordType::M7, { 3, 4, 1 }, EChordInversion::First));
	ChordBuffer.Add(FChordIntervals(EChordType::M7, { 4, 1, 4 }, EChordInversion::Second));
	ChordBuffer.Add(FChordIntervals(EChordType::M7, { 1, 4, 3 }, EChordInversion::Third));

	//Minor
	ChordBuffer.Add(FChordIntervals(EChordType::min7, { 3, 4, 3 }, EChordInversion::Root));
	ChordBuffer.Add(FChordIntervals(EChordType::min7, { 4, 3, 2 }, EChordInversion::First));
	ChordBuffer.Add(FChordIntervals(EChordType::min7, { 3, 2, 3 }, EChordInversion::Second));
	ChordBuffer.Add(FChordIntervals(EChordType::min7, { 2, 3, 4 }, EChordInversion::Third));

	//Dominant
	ChordBuffer.Add(FChordIntervals(EChordType::Dom7, { 4, 3, 3 }, EChordInversion::Root));
	ChordBuffer.Add(FChordIntervals(EChordType::Dom7, { 3, 3, 2 }, EChordInversion::First));
	ChordBuffer.Add(FChordIntervals(EChordType::Dom7, { 3, 2, 4 }, EChordInversion::Second));
	ChordBuffer.Add(FChordIntervals(EChordType::Dom7, { 2, 4, 3 }, EChordInversion::Third));

	//Diminished
	ChordBuffer.Add(FChordIntervals(EChordType::Dim7, { 3, 3, 3 }, EChordInversion::Root));

	//Half Diminished
	ChordBuffer.Add(FChordIntervals(EChordType::HalfDim, { 3, 3, 4 }, EChordInversion::Root));
	ChordBuffer.Add(FChordIntervals(EChordType::HalfDim, { 3, 4, 2 }, EChordInversion::First));
	ChordBuffer.Add(FChordIntervals(EChordType::HalfDim, { 4, 2, 3 }, EChordInversion::Second));
	ChordBuffer.Add(FChordIntervals(EChordType::HalfDim, { 2, 3, 3 }, EChordInversion::Third));

	//Augmented
	ChordBuffer.Add(FChordIntervals(EChordType::Aug7, { 4, 4, 3 }, EChordInversion::Root));
	ChordBuffer.Add(FChordIntervals(EChordType::Aug7, { 4, 3, 1 }, EChordInversion::First));
	ChordBuffer.Add(FChordIntervals(EChordType::Aug7, { 3, 1, 4 }, EChordInversion::Second));
	ChordBuffer.Add(FChordIntervals(EChordType::Aug7, { 1, 4, 4 }, EChordInversion::Third));

	//Triads

	//Major
	ChordBuffer.Add(FChordIntervals(EChordType::Maj, { 4, 3 }, EChordInversion::Root));
	ChordBuffer.Add(FChordIntervals(EChordType::Maj, { 5, 4 }, EChordInversion::First));
	ChordBuffer.Add(FChordIntervals(EChordType::Maj, { 3, 5 }, EChordInversion::Second));

	//Minor
	ChordBuffer.Add(FChordIntervals(EChordType::min, { 3, 4 }, EChordInversion::Root));
	ChordBuffer.Add(FChordIntervals(EChordType::min, { 5, 3 }, EChordInversion::First));
	ChordBuffer.Add(FChordIntervals(EChordType::min, { 4, 5 }, EChordInversion::Second));

	//Diminished
	ChordBuffer.Add(FChordIntervals(EChordType::Dim, { 3, 3 }, EChordInversion::Root));
	ChordBuffer.Add(FChordIntervals(EChordType::Dim, { 6, 3 }, EChordInversion::First));
	ChordBuffer.Add(FChordIntervals(EChordType::Dim, { 3, 6 }, EChordInversion::Second));

	//Augmented
	ChordBuffer.Add(FChordIntervals(EChordType::Aug, { 4, 4 }, EChordInversion::Root));

	//Sus4
	ChordBuffer.Add(FChordIntervals(EChordType::Sus4, { 5, 2 }, EChordInversion::Root));

	//Sus2
	ChordBuffer.Add(FChordIntervals(EChordType::Sus2, { 2, 5 }, EChordInversion::Root));

	//Quartal
	ChordBuffer.Add(FChordIntervals(EChordType::Quartal, { 5, 5 }, EChordInversion::Root));

#endif

}

void UMIDIControllerSubsystem::SetMIDIControllerByID(int32 ControllerID, int32 BufferSize)
{
	UMIDIDeviceManager::ShutDownAllMIDIDevices();

	if (ControllerID == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("MIDIControllerSubsystem::SetMIDIController: No MIDI controller selected"));
		return;
	}

	ActiveMIDIController = UMIDIDeviceManager::CreateMIDIDeviceInputController(ControllerID, BufferSize);

	if (ActiveMIDIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("MIDIControllerSubsystem::SetMIDIController: %s"), *ActiveMIDIController->GetDeviceName());

		ActiveMIDIController->OnMIDINoteOn.AddDynamic(this, &ThisClass::HandleMIDIOnTrigger);
		ActiveMIDIController->OnMIDINoteOff.AddDynamic(this, &ThisClass::HandleMIDIOffTrigger);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MIDIControllerSubsystem::SetMIDIController: Failed to set MIDI controller"));
	}
}

TArray<FFoundMIDIDevice> UMIDIControllerSubsystem::GetMIDIInputDevices()
{
	TArray<FFoundMIDIDevice> FoundDevices{};

	UMIDIDeviceManager::FindMIDIDevices(FoundDevices);

	TArray<int32> indexes;

	for (int32 i = 0; i < FoundDevices.Num(); i++)
	{
		if (!FoundDevices[i].bCanReceiveFrom)
		{
			indexes.Add(i);
		}
	}

	for (int32 i = indexes.Num() - 1; i >= 0; i--)
	{
		FoundDevices.RemoveAt(indexes[i]);
	}

	FoundDevices.Shrink();

	return FoundDevices;
}

void UMIDIControllerSubsystem::HandleMIDIOnTrigger(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Note, int32 Velocity)
{
	//UE_LOG(LogTemp, Warning, TEXT("MIDIControllerSubsystem::HandleMIDIOnTrigger: %s"), *MIDIDeviceController->GetDeviceName());
	NoteOnTrigger.Broadcast(MIDIDeviceController, Timestamp, Channel, Note, Velocity); // Broadcast

	PlayedNotes.Add(Note);
	PlayedNotes.Sort();

	auto chord = DetectChords();
}

void UMIDIControllerSubsystem::HandleMIDIOffTrigger(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Note, int32 Velocity)
{
	//UE_LOG(LogTemp, Warning, TEXT("MIDIControllerSubsystem::HandleMIDIOffTrigger: %s"), *MIDIDeviceController->GetDeviceName());
	NoteOffTrigger.Broadcast(MIDIDeviceController, Timestamp, Channel, Note, Velocity); // Broadcast

	PlayedNotes.Remove(Note);

	if (bHasChord)
	{
		bHasChord = false;
		OnChordStopped.Broadcast(CurrentChord);
		CurrentChord = FChord();
	}

	if (PlayedNotes.Num() < 3) return;
	DetectChords();
}

// Chord Detection

#define PUTS(str) \
if(GEngine) { \
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, str);\
}	
#define PUTF(f) \
if(GEngine) { \
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::SanitizeFloat(f));\
}

#define MAX_CHORD_SIZE 4

constexpr int Downscale(const int amnt, int i)
{
	while (i >= amnt)
		i -= amnt;

	return i;
}

FChord UMIDIControllerSubsystem::DetectChords()
{
	TArray<FChordIntervals> ChordBuffer = GetChordPatterns();

	for (const auto& c : ChordBuffer)
	{
		if (c.Intervals.Num() + 1 != PlayedNotes.Num()) continue;

		int i = 0;
		int LastPitch = 0;
		int notes[MAX_CHORD_SIZE];

		for (auto note : PlayedNotes)
		{
			notes[i] = note;
			if (i == 0)
			{
				LastPitch = note;
				i++;
				continue;
			}

			if (note - c.Intervals[i - 1] == LastPitch)
			{
				LastPitch = note;
			}
			else break;

			if (i == c.Intervals.Num())
			{
				//  Found chord
				FChord ret = FChord();
				ret.Note = (EChordNote)Downscale(12, notes[(int)c.Inversion]);
				ret.Type = c.Type;
				ret.Inversion = c.Inversion;

				//PlayedNotes.Empty()
				bHasChord = true;
				CurrentChord = ret;
				OnChordDetected.Broadcast(ret);

				return ret;
			}

			i++;
		}
	}

	return FChord();
}
