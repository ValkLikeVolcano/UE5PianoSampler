// Fill out your copyright notice in the Description page of Project Settings.


#include "MIDIFunctionLib.h"
#include "ChordIntervals.h"

#define NOTE_DEF(flat, sharp) sharps ? sharp : flat
static FString GetChordNote(EChordNote c, bool sharps)
{
	TArray<FString> NoteNames{};

#pragma region Names
	NoteNames.Add("C");
	NoteNames.Add(NOTE_DEF("Db", "C#"));
	NoteNames.Add("D");
	NoteNames.Add(NOTE_DEF("Eb", "D#"));
	NoteNames.Add("E");
	NoteNames.Add("F");
	NoteNames.Add(NOTE_DEF("Gb", "F#"));
	NoteNames.Add("G");
	NoteNames.Add(NOTE_DEF("Ab", "G#"));
	NoteNames.Add("A");
	NoteNames.Add(NOTE_DEF("Bb", "A#"));
	NoteNames.Add("B");
#pragma endregion

	return NoteNames[(int)c];
}

#define NAMECASE(chordType) case EChordType::chordType: return #chordType;
FString GetChordType(EChordType ty)
{

	switch (ty)
	{
		NAMECASE(Maj);
		NAMECASE(min);
		NAMECASE(Aug);
		NAMECASE(Sus2);
		NAMECASE(Sus4);
		NAMECASE(Dim);
		NAMECASE(Quartal);
		NAMECASE(M7);
		NAMECASE(min7);
		NAMECASE(Dim7);
		NAMECASE(HalfDim);
		NAMECASE(Aug7);

		// Custom cases
	case EChordType::Dom7: return L"7";
	}

	return "Invalid";
}

#define INVERSIONCASE(chordInversion) case EChordInversion::chordInversion: return #chordInversion;
FString GetChordInversion(EChordInversion Inversion)
{
	switch (Inversion)
	{
		INVERSIONCASE(Root);
		INVERSIONCASE(First);
		INVERSIONCASE(Second);
		INVERSIONCASE(Third);
	}

	return L"Unknown";
}

FString UMIDIFunctionLib::GetChordName(const FChord& Chord, bool bUseSharps, FString& OutInversion)
{
	OutInversion = GetChordInversion(Chord.Inversion);
	return GetChordNote(Chord.Note, bUseSharps) + L" " + GetChordType(Chord.Type);
}

FChord UMIDIFunctionLib::MakeValidChord(EChordNote Note, EChordType Type, EChordInversion Inversion)
{

#if 1
	EChordInversion ValidInversion = Inversion;

	switch (Type)
	{
	case EChordType::Aug:
	case EChordType::Sus2:
	case EChordType::Sus4:
	case EChordType::Quartal:
	case EChordType::Dim7:
		ValidInversion = EChordInversion::Root;
		break;

	case EChordType::Maj:
	case EChordType::min:
	case EChordType::Dim:
		if (Inversion == EChordInversion::Third)
			ValidInversion = EChordInversion::Root;
		break;

	default:
		break;
	}

#endif

	// Construct the chord struct with the validated inversion
	FChord Chord;
	Chord.Note = Note;
	Chord.Type = Type;
	Chord.Inversion = ValidInversion;

	return Chord;
}

FString UMIDIFunctionLib::GetNoteFromPitch(int32 Pitch, bool bUseSharps, EChordNote& OutNote)
{
	EChordNote Note = static_cast<EChordNote>((Pitch % 12 + 12) % 12);
	OutNote = Note;
	int32 Octave = (Pitch / 12) - 1;
	return GetChordNote(Note, bUseSharps) + FString::FromInt(Octave);
}

int32 UMIDIFunctionLib::GetPitchFromNote(EChordNote Note, int32 Octave)
{
	return static_cast<int32>(Note) + 12 + (Octave * 12);
}

TArray<FChord> UMIDIFunctionLib::GetDiatonicScaleFromRoot(const EChordNote& Root, const EDiatonicScale& Scale)
{

	TArray<FChord> DiatonicScale{};

	#pragma region Scale Map

	const TMap<EDiatonicScale, TTuple<TArray<EChordType>, TArray<int>>> ScaleData = {
		{EDiatonicScale::Major,			MakeTuple(TArray<EChordType>{EChordType::Maj, EChordType::min, EChordType::min, EChordType::Maj, EChordType::Maj, EChordType::min, EChordType::Dim}, TArray<int>{2, 2, 1, 2, 2, 2, 1})},
		{EDiatonicScale::NaturalMinor,	MakeTuple(TArray<EChordType>{EChordType::min, EChordType::Dim, EChordType::Maj, EChordType::min, EChordType::min, EChordType::Maj, EChordType::Maj}, TArray<int>{2, 1, 2, 2, 1, 2, 2})},
		{EDiatonicScale::Dorian,		MakeTuple(TArray<EChordType>{EChordType::min, EChordType::Dim, EChordType::Maj, EChordType::min, EChordType::min, EChordType::Dim, EChordType::Maj}, TArray<int>{2, 1, 2, 2, 2, 1, 2})},
		{EDiatonicScale::Phrygian,		MakeTuple(TArray<EChordType>{EChordType::min, EChordType::Maj, EChordType::Maj, EChordType::min, EChordType::Dim, EChordType::Maj, EChordType::Maj}, TArray<int>{1, 2, 2, 2, 1, 2, 2})},
		{EDiatonicScale::Lydian,		MakeTuple(TArray<EChordType>{EChordType::Maj, EChordType::Maj, EChordType::min, EChordType::Dim, EChordType::Maj, EChordType::min, EChordType::min}, TArray<int>{2, 2, 2, 1, 2, 2, 1})},
		{EDiatonicScale::Mixolydian,	MakeTuple(TArray<EChordType>{EChordType::Maj, EChordType::min, EChordType::Dim, EChordType::Maj, EChordType::min, EChordType::min, EChordType::Maj}, TArray<int>{2, 2, 1, 2, 2, 1, 2})},
		{EDiatonicScale::Locrian,		MakeTuple(TArray<EChordType>{EChordType::Dim, EChordType::min, EChordType::min, EChordType::Maj, EChordType::Maj, EChordType::Maj, EChordType::min}, TArray<int>{1, 2, 2, 1, 2, 2, 2})}
	};

	#pragma endregion

	const TTuple<TArray<EChordType>, TArray<int>>& ScaleInfo = ScaleData.FindRef(Scale);
	const TArray<EChordType>& ChordTypes = ScaleInfo.Get<0>();
	const TArray<int>& ScalePattern = ScaleInfo.Get<1>();

	int CurrentNoteIndex = static_cast<int>(Root);

	const int NumStepsInScale = ChordTypes.Num();

	for (int i = 0; i < NumStepsInScale; ++i)
	{

		// Create a chord with the current note, chord type, and root inversion
		FChord ChordInScale;
		ChordInScale.Note = static_cast<EChordNote>(CurrentNoteIndex);
		ChordInScale.Type = ChordTypes[i];
		ChordInScale.Inversion = EChordInversion::Root;

		// Log the current chord information for debugging
		DiatonicScale.Add(ChordInScale);

		// Move to the next note index based on the scale pattern and chord type interval
		CurrentNoteIndex = (CurrentNoteIndex + ScalePattern[i]) % 12;
	}

	// Return the generated diatonic scale
	return DiatonicScale;
}

TArray<int32> UMIDIFunctionLib::GetChordPitches(const FChord& Chord, int32 Octave, EChordInversion BassInversion)
{
	if (Chord.Type == EChordType::None)
	{
		return TArray<int32>();
	}

	Octave = FMath::Clamp(Octave, 1, 7);

	TArray<FChordIntervals> ChordPatterns = GetChordPatterns();
	TArray<int32> IntervalPattern;

	TArray<int32> ChordNotes;
	int32 RootNote = static_cast<int32>(Chord.Note) + 12 + (Octave * 12);

	for (const FChordIntervals& ChordPattern : ChordPatterns)
	{
		if (ChordPattern.Type == Chord.Type && ChordPattern.Inversion == EChordInversion::Root)
		{
			IntervalPattern = ChordPattern.Intervals;
			break;
		}
	}

	if (IntervalPattern.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No interval pattern found for chord."));
		return ChordNotes;
	}

	int32 CurrentNote = RootNote;
	ChordNotes.Add(CurrentNote);

	for (int32 Interval : IntervalPattern)
	{
		CurrentNote += Interval;
		ChordNotes.Add(CurrentNote);
	}

	switch (Chord.Inversion)
	{
	case EChordInversion::First:
		ChordNotes[0] += 12;
		break;

	case EChordInversion::Second:
		ChordNotes[0] += 12;
		ChordNotes[1] += 12;
		break;

	case EChordInversion::Third:
		if (ChordNotes.Num() > 3) // Only applicable for seventh chords
		{
			ChordNotes[0] += 12;
			ChordNotes[1] += 12; 
			ChordNotes[2] += 12; 
		}
		break;
	}

	int32 BassNote = RootNote - 24;

	switch (BassInversion)
	{
	case EChordInversion::First:
		// Bass note is the first note above the root in the chord
		BassNote += IntervalPattern[0];
		break;

	case EChordInversion::Second:
		// Bass note is the second note above the root in the chord
		BassNote += IntervalPattern[0] + IntervalPattern[1];
		break;

	case EChordInversion::Third:
		// This case is typically for seventh chords
		if (ChordNotes.Num() > 3)
		{
			BassNote += IntervalPattern[0] + IntervalPattern[1] + IntervalPattern[2];
		}
		break;
	}

	ChordNotes.Add(BassNote);

	ChordNotes.Sort();

	return ChordNotes;
}

EInterval UMIDIFunctionLib::GetInterval(const int32 Note1, const int32 Note2, int32& OutInterval)
{
	int Interval = Note2 - Note1;

	Interval = abs(Interval);
	OutInterval = Interval;

	switch (Interval)
	{
	case 0: return EInterval::PerfectUnison;
	case 1: return EInterval::MinorSecond;
	case 2: return EInterval::MajorSecond;
	case 3: return EInterval::MinorThird;
	case 4: return EInterval::MajorThird;
	case 5: return EInterval::PerfectFourth;
	case 6: return EInterval::Tritone;
	case 7: return EInterval::PerfectFifth;
	case 8: return EInterval::MinorSixth;
	case 9: return EInterval::MajorSixth;
	case 10: return EInterval::MinorSeventh;
	case 11: return EInterval::MajorSeventh;
	case 12: return EInterval::PerfectOctave;
	default: return EInterval::None; // Fallback case
	}
}
