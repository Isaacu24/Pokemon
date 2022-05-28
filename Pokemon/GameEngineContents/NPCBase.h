#pragma once
#include <GameEngine/GameEngineActor.h>
#include "PokemonTileMap.h"

enum class NPCState
{
	Idle,
	Walk,

	Max
};
enum class NPCDir
{
	Up,
	Down,
	Left,
	Right,

};
// ���� :
class GameEngineCollision;
class NPCBase : public GameEngineActor
{
public:
	// constrcuter destructer
	NPCBase();
	~NPCBase();

	// delete Function
	NPCBase(const NPCBase& _Other) = delete;
	NPCBase(NPCBase&& _Other) noexcept = delete;
	NPCBase& operator=(const NPCBase& _Other) = delete;
	NPCBase& operator=(NPCBase&& _Other) noexcept = delete;

	static NPCBase* NPC_;
	
	void Start() override;
	void Update() override;
	void Render() override;

	bool IsTalk_;

	inline void SetNPCInteraction(const bool _Value)
	{
		IsTalk_ = _Value;
	}

protected:
	void NPCMove();
	void NPCMoveAnim();
	void NPCInteractDir();
	bool IsInside(float4 _LeftTop, float4 _RightBot);
	PokemonTileMap*			CurrentTileMap_;
	GameEngineRenderer*		NPCRender_;
	GameEngineCollision*	NPCCollision_;
	GameEngineCollision*	NPCUpCollision_;
	GameEngineCollision*	NPCDownCollision_;
	GameEngineCollision*	NPCRightCollision_;
	GameEngineCollision*	NPCLeftCollision_;
	std::string				NPCAnimationName_;
	std::string				NPCChangeDirText_;

	float4 InSideLeftTop_;
	float4 InSideRightBot_;
	float NPCLerpX_;
	float NPCLerpY_;
	float NPCLerpTime_;
	float NPCNextMoveTime_;
	bool  IsMove_;

private:
	float4	StartPos_;
	float4	GoalPos_;
	float4	NPCMoveDir_;

	NPCState State_;
	NPCDir CurrentDir_;
};

