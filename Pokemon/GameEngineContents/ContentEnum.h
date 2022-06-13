#pragma once

//���ϸ� Ÿ��
enum class PokemonType
{
	NORMAL, //�븻
	FIRE, //��
	WATER, //��
	GRASS, // Ǯ
	ELECTRIC, //����
	ICE, //����
	FIGHTING, //����
	POISON, //��
	GROUND, //��
	FLYING, //����
	PSYCHIC, //������
	BUG, //����
	ROCK, //����
	GHOST, //��Ʈ
	DRAGON, //�巡��
	DARK, //��
	STEEL //��ö
};

//���ϸ� ���� �̻�
enum class StatusEffect
{
	NONE,
	PSN, //�͵�
	BRN, //ȭ��
	PAR, //����
	SLP, //���
	FRZ, //����
};

//���ϸ� ����
enum class PokemonNature
{
	Quirky, //����
	Bold, //���
	Modest, //����
	Calm, //����
	Timid, //������
	Lonely, //�ܷο�
	Bashful, //������
	Mild, //����
	Gentle, //����
	Hasty, //����
	Adamant, //��������
	Impish, //�峭�ٷ���
	Hardy, //���
	Careful, //����
	Jolly, //���
	Naughty, //��������
	Lax, //�Ͷ�
	Rash, //����
	Docile, //�¼�
	Naive, //õ������
	Brave, //�밨
	Relaxed, //��������
	Quiet, //����
	Sassy, //�ǹ�
	Serious, //����
	End 
};

//��ų ����
enum class SkillType
{
	Physical,
	Special,
	Status
};


//�������� ����
enum class ItemType
{
	ITEM,
	KEYITEM,
	BALL
};


enum class RenderOrder
{
	Background,
	Player = 1,
	UI = 15
};

// ����� ���� ����
enum class WorldBackgroundSoundEnum
{
	None,
	PalletTown,
	Route1,
	ViridianCity,
	Route22,
	Route2,
	PewterCity,
	OakLap,
	Gym,
	Battle,
	Max
};

enum class WorldSoundEffectEnum
{
	None,
	Block,
	Click,
	Menu,
	Max
};