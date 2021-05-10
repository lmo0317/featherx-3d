#pragma once

class CFeatherXCamera : public FBSingleton< CFeatherXCamera > 
{
public:

	CFeatherXCamera();
	~CFeatherXCamera();

	void Init();
	void Process();
	void Render();
	void Destroy();

	inline void SetEyeVector(D3DXVECTOR3 eye) { m_vEye = eye; }
	inline void SetLookatVector(D3DXVECTOR3 lookat) { m_vLook = lookat; }
	inline void SetUpVector(D3DXVECTOR3 up) { m_vUp = up; }

	inline D3DXVECTOR3* GetEyeVector() { return &m_vEye; }
	inline D3DXVECTOR3* GetLookatVector() { return &m_vLook; }
	inline D3DXVECTOR3* GetUpVector() { return &m_vUp; }

	inline void SetMinPhi(float phi) { MIN_PHI = phi; }
	inline void SetMaxPhi(float phi) { MAX_PHI = phi; }
	inline void SetMinZoom(float zoom) { MIN_ZOOM = zoom; }
	inline void SetMaxZoom(float zoom) { MAX_ZOOM = zoom; }

	inline void SetTarget(F3dVector vecTargetpos) { m_vecTargetPos = vecTargetpos; }
	inline F3dVector GetTarget() { return m_vecTargetPos; }

	inline void SetZoom(float zoom) { m_fZoom = zoom; }
	inline void SetPhi(float phi) { m_fPhi = phi; }
	inline void SetTheta(float theta) { m_fTheta = theta; }

	inline float GetZoom() { return m_fZoom; }
	inline float GetPhi() { return m_fPhi; }
	inline float GetTheta() { return m_fTheta; }

	bool RotateCamera(float dTheta, float dPhi);
	bool ZoomCamera(float dZoom);
	bool SetUpCamera();

	void Get2Dto3DVector( const F3dVector & v2D, F3dVector & v3D );
	void Get3Dto2DVector( const F3dVector & v3D, F3dVector & v2D );

private:

	D3DXVECTOR3 m_vEye;
	D3DXVECTOR3 m_vLook;
	D3DXVECTOR3 m_vUp;

	F3dVector  m_vecPos;

	D3DXMATRIXA16 m_matWorld;
	D3DXMATRIXA16 m_matView;
	D3DXMATRIXA16 m_matProj;

	F3dVector	  m_vecTargetPos;

	float			m_fZoom;
	float			m_fTheta;
	float			m_fPhi;

	float			MIN_PHI;
	float			MAX_PHI;
	float			MIN_ZOOM;
	float			MAX_ZOOM;
};