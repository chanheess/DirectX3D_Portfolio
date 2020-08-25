#pragma once
#include "Systems/IExecute.h"

class ConvModel : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Ready() override {}
	virtual void Destroy() override {}
	virtual void Update() override {}
	virtual void PreRender() override {}
	virtual void Render() override {}
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	void Airplane();
	void Tower();
	void Tank();
	void Kachujin();
	void Weapons();
	void Player();
	void Monster();
	void Trees();
};