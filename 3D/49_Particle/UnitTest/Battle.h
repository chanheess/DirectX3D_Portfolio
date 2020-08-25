#pragma once
#include "Systems/IExecute.h"
#define PLAYERCOUNT 1
#define WEAPONCOUNT 2
#define MONSTERCOUNT 6
#define RANGEMONSTERCOUNT 2
#define PARTICLECOUNT 4
#define ARROWCOUNT 1

class Battle : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Ready() override {}
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:

	void Mesh();
	void Player();
	void Monster();
	void NoAttachWeapons();

	void Pass(UINT mesh, UINT model, UINT anim);

	void PlayerAttackControl(UINT attackPlayer);
	void MonsterAttackControl(UINT attackMonster, Vector3 playerPos);
	void RangeAttacked(Vector3 pos);
	void PlayerArrowUpdates();
	void MonsterArrowUpdates(UINT input);

	void AddSpotLights();

	Vector3 GetPickedPosition();

private:
	Shader* shader;
	Shadow* shadow;

	ParticleSystem* explosion[PARTICLECOUNT];
	float rangeSkillDelay = 0;
	int rangeSkillCount = 0;
	bool rangeSkillOn = false;
	Vector3 rangeCameraPos{ 0,0,0 };
	Vector3 rangeSkillPos = { 0,0,0 };

	//Player & monsters
	enum PlayerAttackState
	{
		Idle = 0,
		SwordAttack = 1,
		PunchAttack = 2,
		RangeAttack = 3,
		DistanceAttack = 4
	} playerAttackState;

	vector<PlayerAttackState> AttackState;
	vector<PlayerAttackState> MonsterAttackState;
	vector<UINT> noAttack;
	vector<UINT> monsterNoAttack;

	class HpBar* playerHp;
	class HpBar* monsterHp[MONSTERCOUNT + RANGEMONSTERCOUNT];

	bool playerDeath = false;	//�÷��̾� �׾���

	UINT dontAttack = 0;
	vector<float> playerTime;
	vector<float> monsterTime;	//���߿� �������� ������!
	vector<float> rangeMonsterTime;	//Ȱ ����
	float timing = 0.0f;

	ColliderObjectDesc** playerColliders;
	ColliderObjectDesc** playerArrowColliders;
	ColliderObjectDesc** weaponColliders;
	ColliderObjectDesc** dropWeaponColliders;

	ColliderObjectDesc** monsterColliders;
	ColliderObjectDesc** monsterAttackColliders;
	ColliderObjectDesc** monsterArrowColliders;
	ColliderObjectDesc** rangeMonsterColliders;

	Model* weapon[WEAPONCOUNT];
	Model* monsterRangeWeapon[2];

	//�¾����� �ѹ��� �°�
	bool monsterToPlayer[MONSTERCOUNT + RANGEMONSTERCOUNT]{ false };	//���Ͱ� �÷��̾� ����
	bool playerToMonster[MONSTERCOUNT + RANGEMONSTERCOUNT]{ false };	//�÷��̾ ���� ����

	ModelRender* dropWeapons[WEAPONCOUNT];
	ModelRender* playerArrows[ARROWCOUNT];
	ModelRender* monsterArrows[RANGEMONSTERCOUNT];
	bool getWeaponed[2] = { false };

	ModelAnimator* player = NULL;
	ModelAnimator* monster = NULL;
	ModelAnimator* rangeMonster = NULL;

	ModelControler* playerControler[PLAYERCOUNT];
	ModelControler* monsterControler[MONSTERCOUNT];
	ModelControler* rangeMonsterControler[RANGEMONSTERCOUNT];
	///=================================


	class CubeSky* sky;

	Material* floor;
	MeshRender* grid;

	vector<MeshRender *> meshes;
	vector<ModelRender *> models;
	vector<ModelAnimator *> animators;

	VertexTextureNormalTangent* vertices;

	bool isRender = false;
};