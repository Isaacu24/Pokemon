#pragma once
#include "NPCBase.h"

// ���� :
class Green : public NPCBase
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

	bool IsRedSelectFinish_;
	bool IsGreenMove_;

	inline void SetGreenMove(const bool _Value)
	{
		IsGreenMove_ = _Value;
	}

	inline bool GetGreenMove()
	{
		return IsGreenMove_;
	}

protected:

private:
	void Start() override;
	void Update() override;

	std::vector<bool> IsStart_;

	// ���� ���ϸ� ���� ���� �׸� ���ϸ� ���� �� ��ȭ
	void RedSelectPokemonAfter();

	// ����� �׸� ��Ʋ ������ ���� ������
	void RedGreenBattle();
};

