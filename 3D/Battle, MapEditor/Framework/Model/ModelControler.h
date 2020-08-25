#pragma once

class ModelControler
{
private:
	enum AnimationState
	{
		Idle = 1,
		Moving,
		SwordAttack,
		PunchAttack,
		RangeAttack,
		DistanceAttack,
		Damaged,
		Death,
		Dashed,
		Running
	} state;

	enum PatrolState
	{
		pIdle = 1,
		pMoving,
		pStandBy,
	} patrolState;

public:
	ModelControler();
	~ModelControler();

	void Patrol(Transform* pos, Vector3 pos2, bool input, float posY);
	UINT PatrolAutoAttack(Vector3 pos, Vector3 pos2, Vector3 savepos, bool input);
	void Attack(UINT attack, Transform* pos, Vector3 mousePos, bool turn);
	void Dash(Transform* pos, Vector3 pos2, bool input);
	void DistanceAttacked(Transform* pos, Vector3 pos2);
	void StopDash() { state = AnimationState::Idle; }
	void DeathUnit() { state = AnimationState::Death; }
	void MoveStart() { state = AnimationState::Moving; }
	void RunStart() { state = AnimationState::Running; }
	void NoArrowShoot() { bfirstArrow = false; }
	void ArriveArrowShoot(Vector3 pos) { arriveArrow = pos; }
	Vector3 AtArrow() { return arriveArrow; }

	void Move(Transform* pos, Vector3 pos2, bool patrol, UINT currEq, float posY);
	void Move2(Transform* pos, UINT currEq, float posY);
	UINT AnimationState() { return state; }
	UINT autoAttackState() { return autoAttack; }
	bool MoveMode() { return moveMode; }
	void InputMoveMode(bool input) { moveMode = input; }

private:
	//��Ʈ���� ����
	int patrolScale;

	float angle;	//����
	float currAngle;	//���簢
	float prevAngle;	//������
	float patrolTime = 0;	//��Ʈ�ѿ� 3��

	int patrolNum = 0;	//���° ��Ʈ���ΰ�
	bool MoveOn = true;	//��Ʈ���� �̵�

	//ȭ���
	bool bfirstArrow = false;
	Vector3 firstArrow{ 0,0,0 };
	Vector3 arriveArrow{ 0,0,0 };

	//���������� �Ÿ�
	float distance = 0;
	bool outRange = false;
	bool lockOn = false;

	bool noAttack = false; 

	bool randomOn = false;
	bool moveStop = false;
	bool fullRange = false;

	bool moveMode = false; // true : move2 / false : move

	UINT randomPos[4];
	UINT autoAttack;

	Vector3 move;
	Vector3 arrival;

	Vector3 position1;
	Vector3 rotation1;

	Vector3 position2;
	Vector3 rotation2;

	Vector3 movePosition[4];
	Vector3 savePosition;
	Vector3 tempPosition;
	Vector2 randomPosition;
};