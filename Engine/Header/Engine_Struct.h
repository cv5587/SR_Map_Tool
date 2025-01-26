#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"




namespace Engine
{



	typedef struct tagVertexColor
	{
		_vec3		vPosition;	
		_ulong		dwColor;

	}VTXCOL;

	const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTexture
	{
		_vec3		vPosition;
		_vec3		vNormal;
		_vec2		vTexUV;

	}VTXTEX;

	const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexCubeTexture
	{
		_vec3		vPosition;
		_vec3		vTexUV;

	}VTXCUBE;

	const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0); // �ؽ�ó�� UV ��ǥ ���� FLOAT�� 3���� ǥ���ϰڴٴ� ��ũ��(��ȣ���� ���� 0�� �ǹ̴� ���� ���ؽ��� �ؽ��� UV���� �������� �� �� �ִµ� ���� 0��° ���� �����ϰڴٴ� �ǹ�)

	typedef struct tagIndex16
	{
		_ushort  _0;
		_ushort  _1;
		_ushort  _2;

	}INDEX16;

	typedef struct tagIndex32
	{
		_ulong	_0;
		_ulong	_1;
		_ulong	_2;

	}INDEX32;	

	struct FbxMeshInfo
	{
		std::wstring								name;
		std::vector<VTXTEX>						vertices;
		std::vector<INDEX32>			indices;
	};

	const _ulong	FVF_Particle = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_PSIZE;
	struct TileType
	{
		std::wstring TileName;
		int	Tilenum;
	};

	struct Particle
	{
		_vec3 _posiotion;
		D3DCOLOR	_color;
		_float	_size;
	};
	struct Attribute
	{
		_vec3 _position;//��ƼŬ��ġ
		_vec3 _velocity;//��ƼŬ�ӵ�
		_vec3 _acceleration;//��ƼŬ����
		_float  _lifeTime;//��ƼŬ �����ð�
		_float  _age;//��ƼŬ ����ð�
		D3DXCOLOR	_color;//��ƼŬ ��
		D3DXCOLOR	_colorFade;//��ƼŬ �ð��� �帧�� ���� ����ȯ
		_bool	_isAlive;// ���� true ,�Ҹ� false
	};
}
#endif // Engine_Struct_h__
