#pragma once

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
	DRAGON //�巡��
};

//���� �̻�
enum class StatusEffect
{
	NONE,
	PSN, //�͵�
	BRN, //ȭ��
	PAR, //����
	SLP, //���
	FRZ, //����
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