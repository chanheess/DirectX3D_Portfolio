#pragma once
#include "Systems/IExecute.h"

class TerrainLodDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Ready() override {}
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	Shader* shader;

	TerrainLod* terrain;

	//Billboard
	Shader* billboardShader;
	Billboard* billboard;
	bool startBillboard;

	UINT treeType;
	UINT count;
	bool erase;
};