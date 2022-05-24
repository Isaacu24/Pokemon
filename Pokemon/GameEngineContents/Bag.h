#pragma once
#include <GameEngine/GameEngineActor.h>
#include "ContentEnum.h"
#include <vector>

class Bag : public GameEngineActor
{
public:
	Bag();
	~Bag();

	Bag(const Bag& _Other) = delete;
	Bag(Bag&& _Other) noexcept = delete;
	Bag& operator=(const Bag& _Other) = delete;
	Bag& operator=(Bag&& _Other) noexcept = delete;

	void MoveBag(); //������ Ÿ�� �̵�
	void ChangeBag();
	void ShowItemList();
	void ShowKeyItemList();
	void ShowBallList();

	void MoveItem(); //������ �̵�
	void MoveSelectArrow();
	void ShowItemInfo(); //���� ������ �������� ����
	void ShowKeyItemInfo();
	void ShowBallInfo();

	void UpFonts(); //�������� 5�� �̻��� ��� ��Ʈ ������
	void DownFonts();
	void HideFonts();
	void ShowFonts(std::vector<class Item*>& _Fonts);
	void DestroyFonts(); //��ü ��Ʈ ����
	void DestroyNameFonts(); //������ �̸� ����
	void DestroyDescFonts(); //������ ���� ����
	void DestroyOverlapFonts(); //������ �ߺ� ����


protected:
	void Start() override;
	void Update() override;

	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _NextLevel) override;

private:
	ItemType BagType_;

	class GameEngineRenderer* BagRedrerer_;
	GameEngineRenderer* BagName_;
	GameEngineRenderer* BagDialog_;

	GameEngineRenderer* LeftArrow_;
	GameEngineRenderer* RightArrow_;
	GameEngineRenderer* UpArrow_;
	GameEngineRenderer* DownArrow_;

	GameEngineRenderer* SelectArrow_; //������ ���� ȭ��ǥ
	int SelectIndex_;

	std::vector<Item*> ItemList_;
	std::vector<Item*> KeyItemList_;
	std::vector<Item*> BallList_;

	GameEngineRenderer* ItemPreview_; //���� ������ ������ �̹���

	std::vector<class GameEngineContentFont*> ItemNameFonts_; 
	std::vector<class GameEngineContentFont*> ItemDescFonts_;
	std::vector<class GameEngineContentFont*> ItemOverlapFonts_;

	int BagIndex_; //���� ����

	//���� ������
	float BagMoveTime_;
	bool IsMove_;

	//ȭ��ǥ ������
	float ArrowMoveTime_;
	bool IsArrowSync_;
};

