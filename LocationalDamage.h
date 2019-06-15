#include "skse64/PapyrusNativeFunctions.h"
#include <math.h>       /* atan */
#define PI 3.14159265
#define M_PI_4     0.785398163397448309616

namespace LocDamageNamespace
{
	const int head = 1;
	const int neck = 2;
	const int nape = 3;
	const int heart = 4;
	const int rShoulder = 5;
	const int lShoulder = 6;
	const int rArm = 7;
	const int lArm = 8;
	const int chest = 9;
	const int back = 10;
	const int groin = 11;
	const int legs = 12;
	const int feet = 13;

	struct Dimensions { /* ... */

		Dimensions(float fScale, float fScaleMe, float fWidth, float fwidthMe, float fHeight, float fHeightMe)
		{
			scale = fScale;
			scaleMe = fScaleMe;
			width = fWidth;
			widthMe = fwidthMe;
			height = fHeight;
			heightMe = fHeightMe;
		}

		float scale;
		float scaleMe;
		float width;
		float widthMe;
		float height;
		float heightMe;
	};
	struct Angles { /* ... */
		Angles(float fFacing, float fHeadingAngle, float fAx)
		{
			facing = fFacing;
			headingAngle = fHeadingAngle;
			ax = fAx;
		}

		float facing;
		float headingAngle;
		float ax;
	};

	struct Positions { /* ... */
		Positions(float fDist, float fDx, float fDy, float fZPos, float fZPosMe)
		{
			dist = fDist;
			dx = fDx;
			dy = fDy;
			zPos = fZPos;
			zPosMe = fZPosMe;
		}

		float dist;
		float dx;
		float dy;
		float zPos;
		float zPosMe;

	};
	struct Actions {
		Actions(UInt32 bIsSneaking, UInt32 bIsSneakingMe, UInt32 bIsSprintingMe, UInt32 bIsRecoilingMe, UInt32 bIsPowerAttack, UInt32 uWeaponType)
		{
			isSneaking = bIsSneaking;
			isSneakingMe = bIsSneakingMe;
			isSprintingMe = bIsSprintingMe;
			isRecoilingMe = bIsRecoilingMe;
			isPowerAttack = bIsPowerAttack;
			weaponType = uWeaponType;
		}

		UInt32 isSneaking;
		UInt32 isSneakingMe;
		UInt32 isSprintingMe;
		UInt32 isRecoilingMe;
		UInt32 isPowerAttack;
		UInt32 weaponType;
	};

	UInt32 hitDetection(Dimensions d, Angles a, Positions p, Actions s);

	UInt32 GetHitZone(StaticFunctionTag *base, VMArray<float> d, VMArray<float> a, VMArray<float> p, VMArray<UInt32> s);

	UInt32 ArmHitCalcSKSE(StaticFunctionTag *base, float facing, float headingAngle, double armRange);

	bool RegisterFuncs(VMClassRegistry* registry);
}
