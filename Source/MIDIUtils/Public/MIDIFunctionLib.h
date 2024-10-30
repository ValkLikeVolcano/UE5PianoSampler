// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Chord.h"

#include "MIDIFunctionLib.generated.h"

UENUM(BlueprintType)
enum class EDiatonicScale : uint8
{
	Major,
	NaturalMinor,
	Dorian,
	Phrygian,
	Lydian,
	Mixolydian,
	Locrian
};

UENUM(BlueprintType)
enum class EInterval : uint8
{
	None = 0,
	PerfectUnison,
	MinorSecond,
	MajorSecond,
	MinorThird,
	MajorThird,
	PerfectFourth,
	Tritone,
	PerfectFifth,
	MinorSixth,
	MajorSixth,
	MinorSeventh,
	MajorSeventh,
	PerfectOctave
};

UCLASS()
class MIDIUTILS_API UMIDIFunctionLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Chord")
	static FString GetChordName(const FChord& Chord, bool bUseSharps, FString& OutInversion);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Chord")
	static FChord MakeValidChord(EChordNote Note, EChordType Type, EChordInversion Inversion);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Note")
	static FString GetNoteFromPitch(int32 Pitch, bool bUseSharps, EChordNote& OutNote);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Note")
	static int32 GetPitchFromNote(EChordNote Note, int32 Octave);

	UFUNCTION(BlueprintCallable, Category = "Chord")
	static TArray<FChord> GetDiatonicScaleFromRoot(const EChordNote& Root, const EDiatonicScale& Scale);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Chord")
	static TArray<int32> GetChordPitches(const FChord& Chord, int32 Octave = 4, EChordInversion BassInversion = EChordInversion::Root);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interval")
	static EInterval GetInterval(const int32 Note1, const int32 Note2, int32& OutInterval);
};
