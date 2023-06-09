#include "NPCBase.h"
#include "PlayerRed.h"
#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngine/GameEngineCollision.h>

NPCBase* NPCBase::NPC_ = nullptr;
bool NPCBase::IsTalk_ = false;
NPCBase::NPCBase() 
	: NPCLerpTime_(0)
	, NPCNextMoveTime_(0)
	, NPCLerpX_(0)
	, NPCLerpY_(0)
	, IsMove_()
	, RedCatchEndCheck_(false)
	, OakFollowCheck_(false)
	, Room4EnterCheck_(false)
{
	NPC_ = this;
}

NPCBase::~NPCBase() 
{
}

void NPCBase::Start()
{
	//NPCCollision_ = CreateCollision("NPCColBox", { 64,64 });
}

void NPCBase::Update()
{
	//NPCMove();
	NPCMoveAnim();
	NPCMove();

}

void NPCBase::Render()
{
}

void NPCBase::NPCMove()
{
	NPCInteractDir();
	
	if (false == PlayerRed::WMenuUICheck_ || true == IsTalk_)
	{
		NPCMoveDir_ = float4::ZERO;
		return;
	}
	if (nullptr == CurrentTileMap_)
	{
		return;
	}
	GameEngineRandom RandomValue_;
	int RandomStateNum_ = RandomValue_.RandomInt(0, 4);

	if (0 == RandomStateNum_)
	{
		State_ = NPCState::Idle;
		NPCAnimationName_ = "Idle";
		if ("" == NPCChangeDirText_)
		{
			NPCChangeDirText_ = "Down";
		}
		NPCMoveDir_ = float4::ZERO;
	}
	else if (1 == RandomStateNum_)
	{
		State_ = NPCState::Walk;
		CurrentDir_ = NPCDir::Up;
		NPCAnimationName_ = "Walk";
		NPCChangeDirText_ = "Up";
		NPCMoveDir_ = float4::UP;
	}
	else if (2 == RandomStateNum_)
	{
		State_ = NPCState::Walk;
		CurrentDir_ = NPCDir::Down;
		NPCAnimationName_ = "Walk";
		NPCChangeDirText_ = "Down";
		NPCMoveDir_ = float4::DOWN;
	}
	else if (3 == RandomStateNum_)
	{
		State_ = NPCState::Walk;
		CurrentDir_ = NPCDir::Left;
		NPCAnimationName_ = "Walk";
		NPCChangeDirText_ = "Left";
		NPCMoveDir_ = float4::LEFT;
	}
	else if (4 == RandomStateNum_)
	{
		State_ = NPCState::Walk;
		CurrentDir_ = NPCDir::Right;
		NPCAnimationName_ = "Walk";
		NPCChangeDirText_ = "Right";
		NPCMoveDir_ = float4::RIGHT;
	}

	// 간혹 Red랑 겹치는 문제 존재. 일정시간 지나면 NPC도 Red도 각자 갈 길 감.
	if (true == NPCBase::NPCCollision_->NextPosCollisionCheck("RedColBox", NPCMoveDir_ * 50))
	{
		NPCMoveDir_ = float4::ZERO;
		return;
	}
	
	if (GetAccTime() >= NPCNextMoveTime_)
	{
		NPCNextMoveTime_ = GetAccTime() + 0.5f;
		StartPos_ = GetPosition();
		float4 CheckPos_ = GetPosition() + (NPCMoveDir_ * 50) - CurrentTileMap_->GetPosition();
		TileIndex NextIndex = CurrentTileMap_->GetTileMap().GetTileIndex(CheckPos_);

		switch (CurrentTileMap_->CanMove(NextIndex.X, NextIndex.Y, (NPCMoveDir_ * 50)))
		{
		case TileState::False:
		{
			State_ = NPCState::Idle;
			NPCAnimationName_ = "Idle";
			if ("" == NPCChangeDirText_)
			{
				NPCChangeDirText_ = "Down";
			}
			NPCRender_->ChangeAnimation(NPCAnimationName_ + NPCChangeDirText_); 
			NPCMoveDir_ = float4::ZERO;
			break;
		}
		case TileState::True:
		{
			IsMove_ = true;
			NPCRender_->ChangeAnimation(NPCAnimationName_ + NPCChangeDirText_);
			GoalPos_ = CurrentTileMap_->GetWorldPostion(NextIndex.X, NextIndex.Y);
			if (false == IsInside(InSideLeftTop_, InSideRightBot_))
			{
				IsMove_ = false;
				State_ = NPCState::Idle;
				NPCAnimationName_ = "Idle";
				if ("" == NPCChangeDirText_)
				{
					NPCChangeDirText_ = "Down";
				}
				NPCRender_->ChangeAnimation(NPCAnimationName_ + NPCChangeDirText_);
				NPCMoveDir_ = float4::ZERO;
				return;
			}
			break;
		}
		case TileState::MoreDown:
			break;
		default:
			break;
		}
	}
}

void NPCBase::NPCMoveDir(NPCDir _Dir, int _Count)
{
	Count_ = _Count;
	NextDirMoveTimer_ = GetAccTime() + (0.35f * _Count);

	if (NPCDir::Up == _Dir)
	{
		State_ = NPCState::Walk;
		CurrentDir_ = NPCDir::Up;
		NPCAnimationName_ = "Walk";
		NPCChangeDirText_ = "Up";
		NPCMoveDir_ = float4::UP;
	}
	else if (NPCDir::Down == _Dir)
	{
		State_ = NPCState::Walk;
		CurrentDir_ = NPCDir::Down;
		NPCAnimationName_ = "Walk";
		NPCChangeDirText_ = "Down";
		NPCMoveDir_ = float4::DOWN;
	}
	else if (NPCDir::Left == _Dir)
	{
		State_ = NPCState::Walk;
		CurrentDir_ = NPCDir::Left;
		NPCAnimationName_ = "Walk";
		NPCChangeDirText_ = "Left";
		NPCMoveDir_ = float4::LEFT;
	}
	else if (NPCDir::Right == _Dir)
	{
		State_ = NPCState::Walk;
		CurrentDir_ = NPCDir::Right;
		NPCAnimationName_ = "Walk";
		NPCChangeDirText_ = "Right";
		NPCMoveDir_ = float4::RIGHT;
	}


	if (GetAccTime() >= NPCNextMoveTime_)
	{
		NPCNextMoveTime_ = GetAccTime() + 0.3f;
		StartPos_ = GetPosition();
		float4 CheckPos_ = GetPosition() + (NPCMoveDir_ * 50) - CurrentTileMap_->GetPosition();
		TileIndex NextIndex = CurrentTileMap_->GetTileMap().GetTileIndex(CheckPos_);

		switch (CurrentTileMap_->CanMove(NextIndex.X, NextIndex.Y, (NPCMoveDir_ * 50)))
		{
		case TileState::False:
		{
			State_ = NPCState::Idle;
			NPCAnimationName_ = "Idle";
			if ("" == NPCChangeDirText_)
			{
				NPCChangeDirText_ = "Down";
			}
			NPCRender_->ChangeAnimation(NPCAnimationName_ + NPCChangeDirText_);
			NPCMoveDir_ = float4::ZERO;
			break;
		}
		case TileState::True:
		{
			IsMove_ = true;
			NPCRender_->ChangeAnimation(NPCAnimationName_ + NPCChangeDirText_);
			GoalPos_ = CurrentTileMap_->GetWorldPostion(NextIndex.X, NextIndex.Y);
			break;
		}
		case TileState::MoreDown:
			break;
		default:
			break;
		}
	}
}

void NPCBase::NPCMoveAnim()
{
	if (true == IsMove_)
	{
		NPCLerpTime_ += GameEngineTime::GetDeltaTime() * 3.0f;
		NPCLerpX_ = GameEngineMath::LerpLimit(StartPos_.x, GoalPos_.x, NPCLerpTime_);
		NPCLerpY_ = GameEngineMath::LerpLimit(StartPos_.y, GoalPos_.y, NPCLerpTime_);
		SetPosition({ NPCLerpX_,NPCLerpY_ });

		if (NPCLerpTime_ > 1.0f)
		{
			NPCLerpTime_ = 0.0f;
			IsMove_ = false;
			State_ = NPCState::Idle;
			NPCAnimationName_ = "Idle";
			NPCRender_->ChangeAnimation(NPCAnimationName_ + NPCChangeDirText_);

			if (Count_ > 0)
			{
				Count_ -= 1;
				if (Count_ > 0)
				{
					NPCMoveDir(CurrentDir_, Count_);
				}
			}
		}
	}
}

void NPCBase::NPCInteractDir()
{
	if (NPCUpCollision_->CollisionCheck("RedFrontColBox"))
	{
		if (true == GameEngineInput::GetInst()->IsPress("Z"))
		{
			State_ = NPCState::Idle;
			CurrentDir_ = NPCDir::Up;
			NPCAnimationName_ = "Idle";
			NPCChangeDirText_ = "Up";
			NPCMoveDir_ = float4::ZERO;
			NPCRender_->ChangeAnimation(NPCAnimationName_ + NPCChangeDirText_);
			return;
		}
		return;
	}
	else if (NPCDownCollision_->CollisionCheck("RedFrontColBox"))
	{
		if (true == GameEngineInput::GetInst()->IsPress("Z"))
		{
			State_ = NPCState::Idle;
			CurrentDir_ = NPCDir::Down;
			NPCAnimationName_ = "Idle";
			NPCChangeDirText_ = "Down";
			NPCMoveDir_ = float4::ZERO;
			NPCRender_->ChangeAnimation(NPCAnimationName_ + NPCChangeDirText_);
			return;
		}
		return;
	}
	else if (NPCRightCollision_->CollisionCheck("RedFrontColBox"))
	{
		if (true == GameEngineInput::GetInst()->IsPress("Z"))
		{
			State_ = NPCState::Idle;
			CurrentDir_ = NPCDir::Right;
			NPCAnimationName_ = "Idle";
			NPCChangeDirText_ = "Right";
			NPCMoveDir_ = float4::ZERO;
			NPCRender_->ChangeAnimation(NPCAnimationName_ + NPCChangeDirText_);
			return;
		}
		return;
	}
	else if (NPCLeftCollision_->CollisionCheck("RedFrontColBox"))
	{
		if (true == GameEngineInput::GetInst()->IsPress("Z"))
		{
			State_ = NPCState::Idle;
			CurrentDir_ = NPCDir::Left;
			NPCAnimationName_ = "Idle";
			NPCChangeDirText_ = "Left";
			NPCMoveDir_ = float4::ZERO;
			NPCRender_->ChangeAnimation(NPCAnimationName_ + NPCChangeDirText_);
			return;
		}
		return;
	}
}

bool NPCBase::IsInside(float4 _LeftTop, float4 _RightBot)
{
	if (CurrentTileMap_->GetWorldPostion(static_cast<int>(_LeftTop.x), static_cast<int>(_LeftTop.y)).x > GoalPos_.x || CurrentTileMap_->GetWorldPostion(static_cast<int>(_RightBot.x), static_cast<int>(_RightBot.y)).x < GoalPos_.x)
	{
		return false;
	}
	if (CurrentTileMap_->GetWorldPostion(static_cast<int>(_LeftTop.x), static_cast<int>(_LeftTop.y)).y > GoalPos_.y || CurrentTileMap_->GetWorldPostion(static_cast<int>(_RightBot.x), static_cast<int>(_RightBot.y)).y < GoalPos_.y)
	{
		return false;
	}

	return true;
}
