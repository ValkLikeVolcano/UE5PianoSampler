#include "SamplerInstrument.h"
#include "MIDIControllerSubsystem.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AudioParameterControllerInterface.h"


void USamplerInstrument::BeginPlay()
{
	Super::BeginPlay();

	if (bUserInput)
	{
		UGameInstance* GameInstance = GetWorld()->GetGameInstance();

		if (GameInstance)
		{
			UMIDIControllerSubsystem* MIDIController = GameInstance->GetSubsystem<UMIDIControllerSubsystem>();

			if (MIDIController)
			{
				MIDIController->NoteOnTrigger.AddDynamic(this, &ThisClass::OnMIDINoteOn);
				MIDIController->NoteOffTrigger.AddDynamic(this, &ThisClass::OnMIDINoteOff);
			}
		}
	}

	if (!InstrumentData) return;

	for (int32 i = 0; i < MaxVoices; i++)
	{
		UAudioComponent* voice = UGameplayStatics::SpawnSound2D(GetWorld(), InstrumentData->MetasoundInstrument);
		voice->RegisterComponent();
		AvailableVoices.Enqueue(voice);
	}
}


void USamplerInstrument::OnMIDINoteOn(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Note, int32 Velocity)
{

	if (!InstrumentData) return;

	UAudioComponent* Voice = nullptr;

	if (AvailableVoices.Dequeue(Voice))
	{
		ActiveVoices.Add(Note, Voice);

		int32 SampleIndex = GetClosestSampleIndex(Note);

		TArray<USoundWave*> Samples = GetSamplesForNoteAndVelocity(SampleIndex, Velocity);

		if(Samples.Num() == 0) return;

		float PitchShift = Note - SampleIndex;

		IAudioParameterControllerInterface* AudioParameterController = Cast<IAudioParameterControllerInterface>(Voice);
		if (AudioParameterController)
		{
			AudioParameterController->SetFloatParameter("PitchShift", PitchShift);
			AudioParameterController->SetObjectParameter("Sample", Samples[FMath::RandRange(0, Samples.Num() - 1)]);
			AudioParameterController->SetTriggerParameter("NoteOn");
		}
	}
}

void USamplerInstrument::OnMIDINoteOff(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Note, int32 Velocity)
{

	if (!InstrumentData) return;

	UAudioComponent* StopVoice = nullptr;
	
	if (ActiveVoices.RemoveAndCopyValue(Note, StopVoice) && StopVoice)
	{
		int32 SampleIndex = GetClosestSampleIndex(Note);
		if (InstrumentData->NoteSamplesMap.Contains(SampleIndex))
		{
			USoundWave* ReleaseSample = InstrumentData->NoteSamplesMap[SampleIndex].ReleaseSample;
			if (ReleaseSample)
			{
				IAudioParameterControllerInterface* AudioParameterController = Cast<IAudioParameterControllerInterface>(StopVoice);
				if (AudioParameterController)
				{
					AudioParameterController->SetObjectParameter("ReleaseSample", ReleaseSample);
					AudioParameterController->SetTriggerParameter("NoteOff");
				}
			}
		}

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, StopVoice]() 
		{
			AvailableVoices.Enqueue(StopVoice);
		}, 0.15f, false);
		
	}
}

int32 USamplerInstrument::GetClosestSampleIndex(int32 Note)
{
	int32 remainder = Note % InstrumentData->SampleIncrements;
	int32 closest = Note - remainder;

	if (remainder > InstrumentData->SampleIncrements / 2)
	{
		closest += InstrumentData->SampleIncrements;
	}

	return closest;
}

TArray<USoundWave*> USamplerInstrument::GetSamplesForNoteAndVelocity(int32 Note, int32 Velocity)
{
	TArray<USoundWave*> Samples;

	if (!InstrumentData || !InstrumentData->NoteSamplesMap.Contains(Note))
		return Samples;

	const FNoteSamples& NoteSamples = InstrumentData->NoteSamplesMap[Note];
	const FVelocityLayer* SelectedLayer = nullptr;

	for (const FVelocityLayer& VelocityLayer : NoteSamples.VelocityLayer)
	{
		if (Velocity >= VelocityLayer.MinVelocity)
		{
			if (!SelectedLayer || VelocityLayer.MinVelocity > SelectedLayer->MinVelocity)
			{
				SelectedLayer = &VelocityLayer;
			}
		}
	}

	if (SelectedLayer)
	{
		Samples = SelectedLayer->Samples;

		UE_LOG(LogTemp, Warning, TEXT("Selected Layer: %s"), *SelectedLayer->LayerName);
	}

	return Samples;
}
