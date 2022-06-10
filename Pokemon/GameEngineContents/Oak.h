#pragma once
#include "NPCBase.h"

// ���� :
class Oak : public NPCBase
{
public:
	// constrcuter destructer
	Oak();
	~Oak();

	// delete Function
	Oak(const Oak& _Other) = delete;
	Oak(Oak&& _Other) noexcept = delete;
	Oak& operator=(const Oak& _Other) = delete;
	Oak& operator=(Oak&& _Other) noexcept = delete;

	inline void SetRedOut(bool _bRedOutCheck)
	{
		RedOutCheck_ = _bRedOutCheck;
	}

	inline bool GetRedOut()
	{
		return RedOutCheck_;
	}

protected:

private:
	void Start() override;
	void Update() override;

	std::vector<bool> IsStart_;
	bool RedOutCheck_;
	int OakCurrentIndexX_;
	int OakCurrentIndexY_;

	void OakMoveIndexPos(int _X, int _Y);

	// ������ �� �� ���ڻ� �θ�
	void RedCatch();
	// ��ȭ �� ���ڻ� ���� ���� �̵�
	void OakFollow();
	// ���ڻ� ���� ���嵵 ������ ����
	void Room4Enter();
	// ���ϸ� �̼��� �� ��ȣ�ۿ�
	//void Room4Idle();
	// �׸����� ���� ���� ��ȣ�ۿ�
	//void OakIdle();
	// ���� �̺�Ʈ
	//void ShopEvent();
	// ���� �̺�Ʈ ���� ��ȣ�ۿ�
	//void ShopEventIdle();

};
