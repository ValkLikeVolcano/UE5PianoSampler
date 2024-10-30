// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Chord.h"

#include "ChordIntervals.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct MIDIUTILS_API FChordIntervals
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MIDI|Chord")
	EChordType Type{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MIDI|Chord")
	TArray<int32> Intervals{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MIDI|Chord")
	EChordInversion Inversion{};
	
};

	static TArray<FChordIntervals> GetChordPatterns()
{
	static TArray<FChordIntervals> ChordBuffer{};

	if(ChordBuffer.Num() > 0)
	{
		return ChordBuffer;
	}
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

	return ChordBuffer;
}


