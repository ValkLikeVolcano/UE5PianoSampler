// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Chord.generated.h"

UENUM(Blueprintable)
enum class EChordNote : uint8
{
	C,
	Db,
	D,
	Eb,
	E,
	F,
	Gb,
	G,
	Ab,
	A,
	Bb,
	B
};

UENUM(Blueprintable)
enum class EChordType : uint8
{
	None = 0,

	Maj, min, Aug, Sus2, Sus4, Dim, Quartal, M7, min7, Dom7, Dim7, HalfDim, Aug7
};

UENUM(Blueprintable)
enum class EChordInversion : uint8
{
	Root = 0,
	First,
	Second,
	Third
};

USTRUCT(BlueprintType)
struct MIDIUTILS_API FChord
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MIDI|Chord")
	EChordNote Note{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MIDI|Chord")
	EChordType Type{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MIDI|Chord")
	EChordInversion Inversion{};
};
