#pragma once
#include "NPCBase.h"
#include "BattleNPCInterface.h"

// ���� :
class Green : public NPCBase, public BattleNPCInterface
{
public:
	// constrcuter destructer
	Green();
	~Green();

	// delete Function
	Green(const Green& _Other) = delete;
	Green(Green&& _Other) noexcept = delete;
	Green& operator=(const Green& _Other) = delete;
	Green& operator=(Green&& _Other) noexcept = delete;

	static Green* NPCGreen;
	static Green* GetInst()
	{
		return NPCGreen;
	}
	
	bool IsRedSelectFinish_;
	bool IsGreenMove_;

	inline void SetRedSelectFinish(const bool _Value)
	{
		IsRedSelectFinish_ = _Value;
	}

	inline void SetGreenMove(const bool _Value)
	{
		IsGreenMove_ = _Value;
	}

	inline void SetSelectDialog(const bool _Value)
	{
		IsSelectDialog_ = _Value;
	}

	inline bool GetRedSelectFinish()
	{
		return IsRedSelectFinish_;
	}
	
	inline bool GetGreenMove()
	{
		return IsGreenMove_;
	}

	inline bool GetSelectDialog()
	{
		return IsSelectDialog_;
	}
protected:

private:
	void Start() override;
	void Update() override;

	std::vector<bool> IsStart_;
	bool IsSelectDialog_;

	// ���� ���ϸ� ���� ���� �׸� ���ϸ� ���� �� ��ȭ
	void RedSelectPokemonAfter();

	// ����� �׸� ��Ʋ ������ ���� ������
	void RedGreenBattle();
};

