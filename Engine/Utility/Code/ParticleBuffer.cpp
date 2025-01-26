#include "ParticleBuffer.h"

CParticleBuffer::CParticleBuffer(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 _vecOrigin)
	:CComponent(pGraphicDev), _vbSize(2045), _vbBatchSize(512),_vbOffset(0)
{
}

CParticleBuffer::CParticleBuffer(const CParticleBuffer& rhs)
	:CComponent(rhs), m_vecOrigin(rhs.m_vecOrigin)
{
}

CParticleBuffer::~CParticleBuffer()
{
}

_int CParticleBuffer::Update_Component(const _float& fTimeDelta)
{
	std::list<Attribute>::iterator i;

	for (i = m_listParticles.begin(); i != m_listParticles.end(); i++)
	{
		// 살아있는 파티클만 업데이트
		if (i->_isAlive)
		{
			i->_position += i->_velocity * fTimeDelta;

			i->_age += fTimeDelta;

			if (i->_age > i->_lifeTime) // 파티클 죽임
				i->_isAlive = false;
		}
	}

	return 0;
}

HRESULT CParticleBuffer::Ready_Buffer()
{
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer
	(_vbSize * sizeof(Particle),	// 생성할 버텍스 버퍼의 크기
		D3DUSAGE_DYNAMIC|D3DUSAGE_POINTS|D3DUSAGE_WRITEONLY,			// 버텍스 버퍼의 종류(0인 경우 정적 버퍼, d3dusage_dynamic인 경우 동적 버퍼)
		FVF_Particle,	// 버텍스 속성 옵션
		D3DPOOL_DEFAULT, // 동적버퍼
		&m_vb,	
		NULL),	// 공유 설정
		E_FAIL);


	return S_OK;
}

void CParticleBuffer::Render_Buffer(_ulong AttribId)
{
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, _ulong(AttribId));
	if (!m_listParticles.empty())
	{
		m_pGraphicDev->SetFVF(FVF_Particle);
		m_pGraphicDev->SetStreamSource(0, m_vb, 0, sizeof(Particle));

		if (_vbOffset >= _vbSize)
			_vbOffset = 0;

		Particle* v = 0;

		m_vb->Lock(
			_vbOffset * sizeof(Particle),
			_vbBatchSize * sizeof(Particle),
			(void**)&v,
			_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		_ulong numParticlesInBatch = 0;

		list<Attribute>::iterator i;
		for (i = m_listParticles.begin(); i != m_listParticles.end(); i++)
		{
			if (i->_isAlive)
			{
				v->_posiotion = i->_position;
				v->_color = (D3DCOLOR)i->_color;
				v++;
				numParticlesInBatch++;

				if (numParticlesInBatch==_vbBatchSize)
				{
					m_vb->Unlock();

					m_pGraphicDev->DrawPrimitive(
						D3DPT_POINTLIST,
						_vbOffset,
						_vbBatchSize);

					_vbOffset += _vbBatchSize;

					if (_vbOffset>=-_vbSize)
					{
						_vbOffset = 0;
					}

					m_vb->Lock
					(
						_vbOffset * sizeof(Particle),
						_vbBatchSize * sizeof(Particle),
						(void**)&v,
						_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD
					);

					numParticlesInBatch = 0;
				}
			}
		}
		
		m_vb->Unlock();

		if (numParticlesInBatch)
		{
			m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST,
																_vbOffset,
																numParticlesInBatch);
		}

		_vbOffset += _vbBatchSize;

	}

}

void CParticleBuffer::Reset()
{
		std::list<Attribute>::iterator i;
		for (i = m_listParticles.begin(); i != m_listParticles.end(); i++)
		{
			ResetParticle(&(*i));
		}
}

void CParticleBuffer::RemoveDeadParticles()
{
	list<Attribute>::iterator	i = m_listParticles.begin();

	while (i != m_listParticles.end())
	{
		if (i->_isAlive == FALSE)
		{
			i = m_listParticles.erase(i);
		}
		else
		{
			i++;
		}
	}
}

void CParticleBuffer::AddParticle()
{
	Attribute	attribute;
	ResetParticle(&attribute);
	m_listParticles.push_back(attribute);
}

void CParticleBuffer::ResetParticle(Attribute* att)
{
	att->_isAlive = true;
	att->_position = m_vecOrigin;

	D3DXVECTOR3 min = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	D3DXVECTOR3 max = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	GetRandomVector(
		&att->_velocity,
		&min,
		&max);

	// normalize to make spherical
	D3DXVec3Normalize(
		&att->_velocity,
		&att->_velocity);

	att->_velocity *= 100.0f;

	att->_color = D3DXCOLOR(
		GetRandomFloat(0.0f, 1.0f),
		GetRandomFloat(0.0f, 1.0f),
		GetRandomFloat(0.0f, 1.0f),
		1.0f);

	att->_age = 0.0f;
	att->_lifeTime = 2.0f; // lives for 2 seconds
}

_float CParticleBuffer::GetRandomFloat(_float lowBound, _float highBound)
{
	if (lowBound>=highBound)
	{
		return lowBound;
	}
	_float f = (rand() % 10000) * 0.0001f;
	return (f * (highBound - lowBound)) + lowBound;
}

void CParticleBuffer::GetRandomVector(_vec3* out, _vec3* min, _vec3* max)
{
	out->x = GetRandomFloat(min->x, max->x);	
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}



CParticleBuffer* CParticleBuffer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CParticleBuffer* pInstance = new CParticleBuffer(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Particle Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CParticleBuffer::Clone()
{

	return new CParticleBuffer(*this);
}

void CParticleBuffer::Free()
{
	CComponent::Free();
}
