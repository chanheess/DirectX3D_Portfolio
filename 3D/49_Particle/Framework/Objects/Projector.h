#pragma once

class Projector
{
public:
	Projector(Shader* shader, wstring mapFile, UINT width, UINT height, UINT index);
	~Projector();

	void Update();
	void Render();

	Camera* GetCamera() { return (Camera *)camera; }

private:
	struct Desc
	{
		Matrix View;	//�ٸ���
		Matrix Projection;

		Color Color = D3DXCOLOR(1, 1, 1, 1);
	} desc;

private:
	Shader* shader;

	UINT width, height, index;	//�ؽ����� 

	class Fixity* camera;
	Projection* projection;	//�����

	Texture* map;
	ID3DX11EffectShaderResourceVariable* sMap;

	ConstantBuffer* buffer[2];
	ID3DX11EffectConstantBuffer* sBuffer[2];
};