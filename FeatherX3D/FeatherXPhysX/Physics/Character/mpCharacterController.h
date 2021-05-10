#pragma once
/*
	Create	: 2011, 11 ... dobal
	Last	: 2011, 11 ... dobal
	Desc	: Character Controller
*/

class mpCharacterController
{
public:
	enum CollisionFlags
	{
		None = 0,
		Sides = 1,
		CollidedSides = 1,
		CollidedAbove = 2,
		Above = 2,
		CollidedBelow = 4,
		Below = 4,
	};

	mpCharacterController( const F3dVector& vtxCenter, float radius, float height, float slopeLimitDegree, float skinWidth, float stepOffset );
	~mpCharacterController();

	CollisionFlags Move( const F3dVector& motion );
	void SetWorldTransform( const F3dMatrix& mtxTrans );
	const NxActor* GetNxActor();

private:
	NxController*	m_pNxController;
};

typedef boost::shared_ptr< mpCharacterController >	mpCharacterControllerPtr;