// P93 - Sequencer ：C++ 播放 LevelSequence
//
// ALevelSequenceActor ：场景中放置的"播放器" ；持有 LevelSequence 资产
// ULevelSequencePlayer ：实际播放控制 ；Play / Pause / Stop / OnFinished
//
// 用法 ：
//   ALevelSequenceActor* Actor = UP93_SequencePlayerHelper::PlayLevelSequence(this, MySequence);
//   if (Actor && Actor->GetSequencePlayer()) {
//       Actor->GetSequencePlayer()->OnFinished.AddDynamic(this, &::HandleSeqDone);
//   }

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P93_SequencePlayer.generated.h"

class ULevelSequence;
class ALevelSequenceActor;

UCLASS()
class UETEST_API UP93_SequencePlayerHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 创建 Sequence Actor + Player 并自动 Play ；返回 Actor 句柄
	// 调用方可通过返回的 Actor->GetSequencePlayer() 绑定 OnFinished
	UFUNCTION(BlueprintCallable, Category = "P93|Sequencer",
		meta = (WorldContext = "WorldContext"))
	static ALevelSequenceActor* PlayLevelSequence(
		const UObject* WorldContext, ULevelSequence* Sequence);

	UFUNCTION(BlueprintCallable, Category = "P93|Sequencer")
	static void PauseSequence(ALevelSequenceActor* SeqActor);

	UFUNCTION(BlueprintCallable, Category = "P93|Sequencer")
	static void StopSequence(ALevelSequenceActor* SeqActor);
};
