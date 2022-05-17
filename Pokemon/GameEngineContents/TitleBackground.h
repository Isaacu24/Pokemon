#pragma once
#include <GameEngine/GameEngineActor.h>

//�������� ������ �͵鵵 ��������� ĥ ���̴�
//���� ������ �ʾƵ� �ڵ����� �����Ǿ� ������ �ִ°͵�

//���� : 
class TitleBackground : public GameEngineActor
{
public:
	//����Ʈ ������
	TitleBackground();
	//����Ʈ �Ҹ���
	~TitleBackground();

	
	
	
	//======�Ʒ��͵��� ��������� �Ⱦ��ڽ��ϴ�(delete)======
	
	//����Ʈ ���� ������
	TitleBackground(const TitleBackground& _Other) = delete;
	//RValue Reference ������ (���߿� ����)
	TitleBackground(TitleBackground&& _Other) noexcept = delete;
	//operater= (�ڱ��ڽ��� �����ϴ�)
	TitleBackground& operator=(const TitleBackground& _Other) = delete;
	TitleBackground& operator=(TitleBackground&& _Other) noexcept = delete;


protected:

private:
	void Start() override;
};

