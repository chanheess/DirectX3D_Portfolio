#include "Framework.h"
#include "ModelControler.h"

ModelControler::ModelControler()
{
	move = Vector3(0, 0, 0);
	arrival = Vector3(-5, 0, -5);

	position1 = Vector3(0, 0, 0);
	rotation1 = Vector3(0, 0, 0);

	movePosition[0] = Vector3(0, 0, 0);
	movePosition[1] = Vector3(0, 0, 0);
	movePosition[2] = Vector3(0, 0, 0);
	movePosition[3] = Vector3(0, 0, 0);

	savePosition = Vector3(0, 0, 0);
	tempPosition = Vector3(0, 0, 0);
	randomPosition = Vector2(0, 0);

	patrolState = PatrolState::pIdle;

	angle = 0;
	currAngle = 0;
	prevAngle = 0;
	autoAttack = 0;

	srand((UINT)time(0));
}

ModelControler::~ModelControler()
{

}

void ModelControler::Patrol(Transform* pos, Vector3 pos2, bool input)
{
	patrolScale = 30.0f;
	pos->Position(&tempPosition);

	if (randomOn == false)
	{
		int random = 0;
		random = rand() % 8;

		savePosition = tempPosition;
		{
			//East
			movePosition[0].x = savePosition.x + patrolScale;
			movePosition[0].z = savePosition.z;
			movePosition[0].y = savePosition.y;
			//South
			movePosition[1].x = savePosition.x;
			movePosition[1].z = savePosition.z - patrolScale;
			movePosition[1].y = savePosition.y;
			//West
			movePosition[2].x = savePosition.x - patrolScale;
			movePosition[2].z = savePosition.z;
			movePosition[2].y = savePosition.y;
			//North
			movePosition[3].x = savePosition.x;
			movePosition[3].z = savePosition.z + patrolScale;
			movePosition[3].y = savePosition.y;
		}
		switch (random)
		{
		case 0:
			randomPos[0] = 0;
			randomPos[1] = 1;
			randomPos[2] = 2;
			randomPos[3] = 3;
			break;
		case 1:
			randomPos[0] = 1;
			randomPos[1] = 2;
			randomPos[2] = 3;
			randomPos[3] = 0;
			break;
		case 2:
			randomPos[0] = 2;
			randomPos[1] = 3;
			randomPos[2] = 0;
			randomPos[3] = 1;
			break;
		case 3:
			randomPos[0] = 3;
			randomPos[1] = 0;
			randomPos[2] = 1;
			randomPos[3] = 2;
			break;
		case 4:
			randomPos[0] = 3;
			randomPos[1] = 2;
			randomPos[2] = 1;
			randomPos[3] = 0;
			break;
		case 5:
			randomPos[0] = 2;
			randomPos[1] = 1;
			randomPos[2] = 0;
			randomPos[3] = 3;
			break;
		case 6:
			randomPos[0] = 1;
			randomPos[1] = 0;
			randomPos[2] = 3;
			randomPos[3] = 2;
			break;
		case 7:
			randomPos[0] = 0;
			randomPos[1] = 3;
			randomPos[2] = 2;
			randomPos[3] = 1;
			break;
		}

		randomOn = true;
	}
	if (randomOn == true)
	{
		if (state != AnimationState::Death)
		{
			//1 현재위치 2 플레이어위치 3첫시작위치
			autoAttack = PatrolAutoAttack(tempPosition, pos2, savePosition, input);

			switch (autoAttack)
			{
			case 1:	//플레이어 따라가기
				MoveOn = true;
				Move(pos, pos2, true);
				break;
			case 2:	//때리기
				if (input == false)
				{
					if (state != AnimationState::PunchAttack)
						state = AnimationState::PunchAttack;	//때리기
				}
				else if (input == true)
				{
					if (state != AnimationState::DistanceAttack)
						state = AnimationState::DistanceAttack;	//때리기
				}
				MoveOn = true;
				break;
			case 3:	//정찰
				if (MoveOn == true) //딜레이 후에만 움직이게
					Move(pos, movePosition[randomPos[patrolNum]], true);

				if (patrolState == PatrolState::pIdle)
				{
					MoveOn = false;
					patrolTime += Time::Get()->Delta();
					if (patrolTime >= 3.0f)	// 움직인 후 딜레이 후 움직임
					{
						patrolTime = 0;
						patrolNum++;
						patrolNum %= 4;
						MoveOn = true;
					}
				}
				break;

			case 4:	//지정범위이탈
				MoveOn = true;
				Move(pos, savePosition, true);

				if (patrolState == PatrolState::pIdle)
				{
					outRange = false;
					lockOn = false;
				}
				break;
			}
		}//if (state != AnimationState::Death)
	}
}

UINT ModelControler::PatrolAutoAttack(Vector3 pos, Vector3 pos2, Vector3 savepos, bool input)
{
	//거리
	float playerDis = sqrt((pos2.z - pos.z) * (pos2.z - pos.z) +
		(pos2.x - pos.x) * (pos2.x - pos.x));

	float saveDis = sqrt((savepos.z - pos.z) * (savepos.z - pos.z) +
		(savepos.x - pos.x) * (savepos.x - pos.x));

	//감지 범위
	float sensor = 25.0f;
	//공격 범위
	float attackRange;

	if (input == false)
		attackRange = 10.0f;
	if (input == true)
		attackRange = 20.0f;

	//patrolScale밖으로 나갈수있는 허용 범위
	float possibility = sensor + patrolScale;

	//플레이어가 지정범위 바깥에 도착했을경우
	if (saveDis >= possibility)
	{
		outRange = true;
	}

	//센서에 감지될 경우 지정범위 안에서 플레이어 따라가기
	if (playerDis < sensor)	
	{
		lockOn = true;
	}
	//원래 위치로 돌아간다.
	if (outRange == true)
	{
		return 4;
	}
	//공격
	else if (playerDis <= attackRange)
	{
		return 2;
	}
	else if (lockOn == true)
	{
		return 1;
	}
	else
	{
		return 3;
	}
	
}

void ModelControler::Attack(UINT attack, Transform* pos, Vector3 mousePos, bool turn)
{
	switch(attack)
	{
	case 1:	//Sword
		state = AnimationState::SwordAttack;
		break;
	case 2:	//Punch
		state = AnimationState::PunchAttack;
		break;
	case 3:	//Range
		state = AnimationState::RangeAttack;
		break;
	case 4:	//Distance
		state = AnimationState::DistanceAttack;
		break;
	}

	if (turn == true)
	{
		pos->RotationDegree(&rotation1);

		Vector3 normal1;
		normal1 = -pos->Forward();

		Vector3 normal2;
		normal2 = mousePos - position1;

		Vector3 normal3;
		normal3 = -pos->Right();

		D3DXVec3Normalize(&normal1, &normal1);
		D3DXVec3Normalize(&normal2, &normal2);
		D3DXVec3Normalize(&normal3, &normal3);

		angle = D3DXVec3Dot(&normal1, &normal2);

		float angle3_2 = D3DXVec3Dot(&normal3, &normal2);
		angle3_2 = acosf(angle3_2);
		angle3_2 = Math::ToDegree(angle3_2);

		if (angle3_2 > 90)
		{
			angle = -acosf(angle);
			angle = Math::ToDegree(angle);
		}
		else if (angle3_2 <= 90)
		{
			angle = acosf(angle);
			angle = Math::ToDegree(angle);
		}

		prevAngle = rotation1.y;
		currAngle = rotation1.y + angle;

		if ((currAngle > prevAngle + 0.5f || currAngle < prevAngle - 0.5f) == true)
			rotation1.y += angle;

		pos->RotationDegree(rotation1);
	}
}

void ModelControler::Dash(Transform* pos, Vector3 pos2, bool input)
{
	pos->Position(&position1);
	pos->RotationDegree(&rotation1);

	if (input == true)
	{
		state = AnimationState::Dashed;

		Vector3 normal1;
		normal1 = -pos->Forward();

		Vector3 normal2;
		normal2 = pos2 - position1;

		Vector3 normal3;
		normal3 = -pos->Right();

		D3DXVec3Normalize(&normal1, &normal1);
		D3DXVec3Normalize(&normal2, &normal2);
		D3DXVec3Normalize(&normal3, &normal3);

		angle = D3DXVec3Dot(&normal1, &normal2);

		float angle3_2 = D3DXVec3Dot(&normal3, &normal2);
		angle3_2 = acosf(angle3_2);
		angle3_2 = Math::ToDegree(angle3_2);

		if (angle3_2 > 90)
		{
			angle = -acosf(angle);
			angle = Math::ToDegree(angle);
		}
		else if (angle3_2 <= 90)
		{
			angle = acosf(angle);
			angle = Math::ToDegree(angle);
		}

		prevAngle = rotation1.y;
		currAngle = rotation1.y + angle;

		if ((currAngle > prevAngle + 0.5f || currAngle < prevAngle - 0.5f) == true)
			rotation1.y += angle;

		//도착지 설정
		arrival = position1 + normal2 * 30.0f;

		move = normal2 * 0.2f;
	}

	//ArriveDistance
	distance = sqrt((arrival.z - position1.z) * (arrival.z - position1.z) +
		(arrival.x - position1.x) * (arrival.x - position1.x));


	if (state == AnimationState::Dashed)
	{
		if (arrival != Vector3(-5, 0, -5) && (distance < 0.15f) != true)
		{
			position1 += move;
		}
	}

	pos->RotationDegree(rotation1);
	pos->Position(position1);
}

void ModelControler::DistanceAttacked(Transform* pos, Vector3 pos2)
{
	pos->Position(&position2);

	if (bfirstArrow == false)
	{
		firstArrow = position2;
		arriveArrow = pos2;

		state = AnimationState::DistanceAttack;

		Vector3 normal1;
		normal1 = -pos->Forward();

		Vector3 normal2;
		normal2 = arriveArrow - firstArrow;

		D3DXVec3Normalize(&normal1, &normal1);
		D3DXVec3Normalize(&normal2, &normal2);

		//도착지 설정
		arrival = firstArrow + normal2 * 100.0f;

		move = normal2 * 0.8f;

		bfirstArrow = true;
	}

	//ArriveDistance
	distance = sqrt((arrival.z - position2.z) * (arrival.z - position2.z) +
		(arrival.x - position2.x) * (arrival.x - position2.x));


	if (arrival != Vector3(-5, 0, -5) && (distance < 0.15f) != true)
	{
		position2 += move;
	}

	pos->Position(position2);
}

void ModelControler::Move(Transform* pos, Vector3 pos2, bool patrol)
{
	if (state == AnimationState::Dashed ||
		state == AnimationState::SwordAttack || 
		state == AnimationState::PunchAttack ||
		state == AnimationState::DistanceAttack ||
		state == AnimationState::RangeAttack)
	{
		return;
	}

	pos->Position(&position1);
	pos->RotationDegree(&rotation1);

	if (patrol == false)
	{
		if (Mouse::Get()->Press(0))
		{
			//도착지 설정
			arrival = pos2;

			Vector3 normal1;
			normal1 = -pos->Forward();

			Vector3 normal2;
			normal2 = pos2 - position1;

			Vector3 normal3;
			normal3 = -pos->Right();

			D3DXVec3Normalize(&normal1, &normal1);
			D3DXVec3Normalize(&normal2, &normal2);
			D3DXVec3Normalize(&normal3, &normal3);

			angle = D3DXVec3Dot(&normal1, &normal2);

			float angle3_2 = D3DXVec3Dot(&normal3, &normal2);
			angle3_2 = acosf(angle3_2);
			angle3_2 = Math::ToDegree(angle3_2);

			if (angle3_2 > 90)
			{
				angle = -acosf(angle);
				angle = Math::ToDegree(angle);
			}
			else if (angle3_2 <= 90)
			{
				angle = acosf(angle);
				angle = Math::ToDegree(angle);
			}

			prevAngle = rotation1.y;
			currAngle = rotation1.y + angle;

			if((currAngle > prevAngle + 0.5f || currAngle < prevAngle - 0.5f) == true)
				rotation1.y += angle;
		}
	}
	else if (patrol == true)
	{
		//도착지 설정
		arrival = pos2;

		Vector3 normal1;
		normal1 = -pos->Forward();

		Vector3 normal2;
		normal2 = pos2 - position1;

		Vector3 normal3;
		normal3 = -pos->Right();

		D3DXVec3Normalize(&normal1, &normal1);
		D3DXVec3Normalize(&normal2, &normal2);
		D3DXVec3Normalize(&normal3, &normal3);

		float angle = D3DXVec3Dot(&normal1, &normal2);

		float angle3_2 = D3DXVec3Dot(&normal3, &normal2);
		angle3_2 = acosf(angle3_2);
		angle3_2 = Math::ToDegree(angle3_2);

		if (angle3_2 > 90)
		{
			angle = -acosf(angle);
			angle = Math::ToDegree(angle);
		}
		else if (angle3_2 <= 90)
		{
			angle = acosf(angle);
			angle = Math::ToDegree(angle);
		}

		prevAngle = rotation1.y;
		currAngle = rotation1.y + angle;

		if ((currAngle > prevAngle + 0.5f || currAngle < prevAngle - 0.5f) == true)
			rotation1.y += angle;
	}
	
	if (patrol == false)
		move = pos->Forward() * 250.0f * Time::Get()->Delta();
	else if (patrol == true)
	{
		if(autoAttack != 4)
			move = pos->Forward() * 100.0f * Time::Get()->Delta();
		else if(autoAttack == 4)
			move = pos->Forward() * 250.0f * Time::Get()->Delta();
	}

	//ArriveDistance
	distance = sqrt((arrival.z - position1.z) * (arrival.z - position1.z) +
		(arrival.x - position1.x) * (arrival.x - position1.x));

	if (arrival != Vector3(-5, 0, -5) && (distance < 0.15f) != true)
	{
		position1 -= move;
		state = AnimationState::Moving;

		if (patrol == true)
			patrolState = PatrolState::pMoving;
	}
	else
	{
		if(state != AnimationState::SwordAttack ||
			state != AnimationState::PunchAttack ||
			state != AnimationState::DistanceAttack)
			state = AnimationState::Idle;

		if (patrol == true)
		{
			state = AnimationState::Idle;
			patrolState = PatrolState::pIdle;
		}
	}

	pos->RotationDegree(rotation1);
	pos->Position(position1);
}
