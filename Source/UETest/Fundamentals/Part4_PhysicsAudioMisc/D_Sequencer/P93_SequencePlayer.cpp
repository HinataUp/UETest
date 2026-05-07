#include "P93_SequencePlayer.h"
#include "Engine/World.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "../00_Common/FundP4LogMacros.h"

ALevelSequenceActor* UP93_SequencePlayerHelper::PlayLevelSequence(
	const UObject* WorldContext, ULevelSequence* Sequence)
{
	if (!WorldContext || !Sequence) { return nullptr; }
	UWorld* World = WorldContext->GetWorld();
	if (!World) { return nullptr; }

	// CreateLevelSequencePlayer ：一行同时创建 Actor + Player
	FMovieSceneSequencePlaybackSettings Settings;
	ALevelSequenceActor* SeqActor = nullptr;
	ULevelSequencePlayer* Player = ULevelSequencePlayer::CreateLevelSequencePlayer(
		World, Sequence, Settings, SeqActor);

	if (!Player)
	{
		P4_LOG("P93: 创建 Sequence Player 失败");
		return nullptr;
	}

	Player->Play();
	P4_LOG("P93: 开始播放 Sequence %s", *Sequence->GetName());
	return SeqActor;
}

void UP93_SequencePlayerHelper::PauseSequence(ALevelSequenceActor* SeqActor)
{
	if (SeqActor && SeqActor->GetSequencePlayer())
	{
		SeqActor->GetSequencePlayer()->Pause();
	}
}

void UP93_SequencePlayerHelper::StopSequence(ALevelSequenceActor* SeqActor)
{
	if (SeqActor && SeqActor->GetSequencePlayer())
	{
		SeqActor->GetSequencePlayer()->Stop();
	}
}
