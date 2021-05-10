#pragma once

class mpFixedJointDesc : public mpJointDesc
{
public:
	mpFixedJointDesc();	
	void SetToDefault();
	bool IsValid() const;

	mpJointDesc* Copy() const;
	void Copy( const mpJointDesc* pJointDesc );
	void GetNxFixedJointDesc( NxFixedJointDesc& JointDesc, const F3dMatrix& mtxWorld ) const;
};
