//
// Created by frank on 7/7/2024.
//

#ifndef LOCATIONALDAMAGE_H
#define LOCATIONALDAMAGE_H

#include <cmath>       /* atan */
#include <vector>

#include <SKSE/SKSE.h>
#include <RE/T/TypeTraits.h>

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
		Actions(uint32_t bIsSneaking, uint32_t bIsSneakingMe, uint32_t bIsSprintingMe, uint32_t bIsRecoilingMe, uint32_t bIsPowerAttack, uint32_t uWeaponType)
		{
			isSneaking = bIsSneaking;
			isSneakingMe = bIsSneakingMe;
			isSprintingMe = bIsSprintingMe;
			isRecoilingMe = bIsRecoilingMe;
			isPowerAttack = bIsPowerAttack;
			weaponType = uWeaponType;
		}

		uint32_t isSneaking;
		uint32_t isSneakingMe;
		uint32_t isSprintingMe;
		uint32_t isRecoilingMe;
		uint32_t isPowerAttack;
		uint32_t weaponType;
	};

	uint32_t hitDetection(Dimensions d, Angles a, Positions p, Actions s);

	uint32_t GetHitZone(RE::StaticFunctionTag*, std::vector<float> d, std::vector<float> a, std::vector<float> p, std::vector<uint32_t> s);

	uint32_t ArmHitCalcSKSE(RE::StaticFunctionTag*, float facing, float headingAngle, double armRange);

    bool RegisterFuncs(RE::BSScript::IVirtualMachine* vm);

}

#endif //LOCATIONALDAMAGE_H
