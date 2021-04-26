#pragma once
#include "SingletonBase.h"
class TimeManager : public SingletonBase<TimeManager>
{
private:
	//FPS:�ʴ� ������ ��(frames per second)

	bool _isHardware;				//������ Ÿ�̸Ӹ� �����ϳ�?
	__int64 _curTime;				//����ð�
	__int64 _lastTime;				//�������ð�

	float _FPStimeElapsed;			//FPS �������ð��� ���� �ð��� �����
	float _timeScale;				//�ð� �����
	UINT _FPSFrameCount;			//FPS ī��Ʈ

	//���ϰ�
	UINT _frameRate;				//FPS
	float _timeElapsed;				//�� �����Ӵ� �����
	float _worldTime;				//��ü �ð� �����
public:
	TimeManager();
	~TimeManager();

	//�ʱ�ȭ ���� ������ �������� 60���������� ����
	void SetFrame(UINT lock = 60);

	//�� ������ ����ð� �������� 1/60
	float GetElapsedTime() const { return _timeElapsed; }
	//��ü �ð� ����� ��������
	float GetWorldTime() const { return _worldTime; }
	//���� FPS ��������
	unsigned long GetFrameRate() { return _frameRate; }

	void render();

};
