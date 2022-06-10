#pragma once
#include <GameEngine/GameEngineActor.h>
#include "ContentEnum.h"
#include <vector>
#include "Item.h"

enum class BagState
{
	ListMenu,
	DialogMenu,
	ItemUse,
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
	void ShowItemInfo(std::vector<Item*>& _List); //���� ������ �������� ����

	void ActiveDialog(); //���̾�α� Ȱ��ȭ üũ
	void MoveDialog();
	void SelectDialog();
	void OnDialog();
	void CloseDialog();

	void UpFonts(); //�������� 5�� �̻��� ��� ��Ʈ ������
	void DownFonts();
	void HideFonts();
	void ShowFonts(std::vector<Item*>& _List);
	void DestroyFonts(); //��ü ��Ʈ ����
	void DestroyNameFonts(); //������ �̸� ����
	void DestroyDescFonts(); //������ ���� ����
	void DestroyOverlapFonts(); //������ �ߺ� ����
	void DestroyDialogFonts(); //���̾�α� ����

	void Use(std::vector<Item*>& _List);
	void Give(std::vector<Item*>& _List);
	void Toss(std::vector<Item*>& _List);

	inline void SetCurrentPokemon(class Pokemon* _Pokemon)
	{
		CurrentPokemon_ = _Pokemon;
	}

	inline void SetPlayerItemList(std::vector<Item*>& _List)
	{
		for (int i = 0; i < _List.size(); ++i)
		{
			if (ItemType::ITEM == _List[i]->GetInfo()->GetType())
			{
				ItemList_.push_back(_List[i]);
			}
			
			else if (ItemType::KEYITEM == _List[i]->GetInfo()->GetType())
			{
				KeyItemList_.push_back(_List[i]);
			}

			else if (ItemType::BALL == _List[i]->GetInfo()->GetType())
			{
				BallList_.push_back(_List[i]);
			}
		}
	}

protected:
	void Start() override;
	void Update() override;

	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _NextLevel) override;

private:
	ItemType BagType_;
	BagState BagState_;

	class FadeActor* FadeActor_;

	class GameEngineRenderer* BagRedrerer_;
	GameEngineRenderer* Title_;
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

	//�Է� ��� �ð� 
	float InputTime_;
};

