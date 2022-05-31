#pragma once
#include <GameEngine/GameEngineActor.h>
#include "ContentEnum.h"
#include <vector>

enum class BagState
{
	ListMenu,
	DialogMenu,
	ItemGive,
	ItemToss
};

class Bag : public GameEngineActor
{
public:
	Bag();
	~Bag();

	Bag(const Bag& _Other) = delete;
	Bag(Bag&& _Other) noexcept = delete;
	Bag& operator=(const Bag& _Other) = delete;
	Bag& operator=(Bag&& _Other) noexcept = delete;

	void BagInit();
	void DestroyBag();

	void MoveBag(); //������ Ÿ�� �̵�
	void MoveArrow();
	void ChangeBag();
	void ShowItemList();
	void ShowKeyItemList();
	void ShowBallList();

	void MoveItem(); //������ �̵�
	void MoveSelectArrow();
	void ShowItemInfo(std::vector<class Item*>& _List); //���� ������ �������� ����

	void ActiveDialog(); //���̾�α� Ȱ��ȭ üũ
	void MoveDialog();
	void SelectDialog();
	void OnDialog();
	void CloseDialog();

	void UpFonts(); //�������� 5�� �̻��� ��� ��Ʈ ������
	void DownFonts();
	void HideFonts();
	void ShowFonts(std::vector<class Item*>& _List);
	void DestroyFonts(); //��ü ��Ʈ ����
	void DestroyNameFonts(); //������ �̸� ����
	void DestroyDescFonts(); //������ ���� ����
	void DestroyOverlapFonts(); //������ �ߺ� ����
	void DestroyDialogFonts(); //���̾�α� ����

	inline void SetCurrentPokemon(class Pokemon* _Pokemon)
	{
		CurrentPokemon_ = _Pokemon;
	}

protected:
	void Start() override;
	void Update() override;

private:
	ItemType BagType_;
	BagState BagState_;

	class GameEngineRenderer* BagRedrerer_;
	GameEngineRenderer* BagName_;
	GameEngineRenderer* BagDialog_;
	GameEngineRenderer* DialogBox_;
	GameEngineRenderer* DialogArrow_;

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

	std::vector<class GameEngineContentFont*> ItemNameFonts_; //������ �̸�
	std::vector<class GameEngineContentFont*> ItemDescFonts_; //������ ����
	std::vector<class GameEngineContentFont*> ItemOverlapFonts_; //������ ����
	std::vector<class GameEngineContentFont*> DialogFonts_; //���̾�α� â 

	int BagIndex_; //���� ����

	//���� ������
	float BagMoveTime_;
	bool IsMove_;

	//ȭ��ǥ ������
	float ArrowMoveTime_;
	bool IsArrowSync_;

	//���̾�α� ���� �ε���
	int DialogIndex_;

	Item* CurrentItem_;
	Pokemon* CurrentPokemon_;

	GameEngineActor* ChildUI_;
};

