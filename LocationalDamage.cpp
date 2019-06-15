#include "LocationalDamage.h"

namespace LocDamageNamespace {
	int armHitCalc(float facing, float headingAngle, double armRange) {
		if (abs(facing) < 45) {
			if (headingAngle > armRange) {
				return rArm;
			}
			else if (headingAngle < -armRange) {
				return lArm;
			}
			else {
				return chest;
			}
		}
		else if (facing < -45 && facing > -135) {
			return lArm;
		}
		else if (facing > 45 && facing < 135) {
			return rArm;
		}
		else if (abs(facing) >= 135) {
			if (headingAngle > armRange) {
				return lArm;
			}
			else if (headingAngle < -armRange) {
				return rArm;
			}
			else {
				return back;
			}
		}

		return back;
	}

	int shoulderhitCalc(bool powerAttack, float facing, float headingAngle, float shoulderRange, int weaponType) {
		if (abs(facing) < 45) {
			// In front of the target
			if (headingAngle > shoulderRange) {
				return rShoulder;
			}
			if (headingAngle < -shoulderRange) {
				return lShoulder;
			}
			else if (weaponType != 7) {
				if (powerAttack && weaponType != 3 && weaponType != 4 && weaponType != 6) {
					return heart;
				}
				else {
					return chest;
				}
			}
			else {
				return heart;
			}

		}
		else if (facing < -45 && facing > -135) {
			return lShoulder;
		}
		else if (facing > 45 && facing < 135) {
			return rShoulder;
		}
		else if (abs(facing) >= 135) {
			// Behind the target
			if (headingAngle > shoulderRange) {
				return lShoulder;
			}
			else if (headingAngle < -shoulderRange) {
				return rShoulder;
			}
			else {
				return back;
			}
		}
	}

	UInt32 hitDetection(Dimensions d, Angles a, Positions p, Actions s) {
		double net_height = d.height;
		double net_height_me = d.heightMe;
		double dispWidth;
		double dispHeight;

		double shoulderRange;
		double armRange;
		double headZone;
		double neckZone;
		double shoulderZone;
		double chestZone;
		double groinZone;
		double legZone;

		double zHeight;
		double zHeightMe;

		if (s.isSneaking) {
			net_height = net_height * .75;
		}

		if (s.isSneakingMe || s.isRecoilingMe) {
			net_height_me = net_height_me * .75;
		}
		else if (s.isSprintingMe) {
			net_height_me = net_height_me - d.scaleMe * 5;
		}

		zHeight = p.zPos + net_height;
		zHeightMe = p.zPosMe + net_height_me;

		// Y-AXIS ZONES
		dispHeight = zHeight - (p.zPosMe + net_height_me * 0.900);
		headZone = atan(dispHeight / p.dist) * 180 / PI;

		// Head strike
		if (a.ax < headZone) {
			return head;
		}

		dispHeight = zHeight - (p.zPosMe + net_height_me * 0.850);
		neckZone = atan(dispHeight / p.dist) * 180 / PI;

		// Neck strike
		if (a.ax < neckZone) {
			if (a.facing <= -135 || a.facing >= 135) {
				return nape;
			}
			else {
				return neck;
			}
		}

		// X-AXIS ZONES
		dispWidth = d.width - d.widthMe * .900;
		shoulderRange = abs(asin(dispWidth / p.dx) * 180 / PI);
		dispWidth = d.width - d.widthMe * .800;
		armRange = abs(asin(dispWidth / p.dx) * 180 / PI);

		dispHeight = zHeight - (p.zPosMe + net_height_me * 0.750);
		shoulderZone = atan(dispHeight / p.dist) * 180 / PI;

		// Shoulder strike
		if (a.ax < shoulderZone) {
			return shoulderhitCalc(s.isPowerAttack, a.facing, a.headingAngle, shoulderRange, s.weaponType);
		}

		dispHeight = zHeight - (p.zPosMe + net_height_me * 0.500);
		chestZone = atan(dispHeight / p.dist) * 180 / PI;

		// Chest strike
		if (a.ax < chestZone) {
			return armHitCalc(a.facing, a.headingAngle, armRange);
		}

		dispHeight = zHeight - (p.zPosMe + net_height_me * 0.400);
		groinZone = atan(dispHeight / p.dist) * 180 / PI;

		// Groin strike
		if (a.ax < groinZone) {
			if (abs(a.facing) < 45) {
				return groin;
			}
			else {
				return legs;
			}
		}

		dispHeight = zHeight - (p.zPosMe + net_height_me * 0.200);
		legZone = atan(dispHeight / p.dist) * 180 / PI;

		// Leg strike
		if (a.ax < legZone) {
			return legs;
		}

		// Feet strike
		return feet;
	}

	UInt32 GetHitZone(StaticFunctionTag *base, VMArray<float> d, VMArray<float> a, VMArray<float> p, VMArray<UInt32> s) {
		gLog.SetLogLevel(IDebugLog::kLevel_Message);

		// Dimensions
		float scale;
		float scaleMe;
		float width;
		float widthMe;
		float height;
		float heightMe;

		// Angles
		float facing;
		float headingAngle;
		float ax;

		// Positions
		float dist;
		float dx;
		float dy;
		float zPos;
		float zPosMe;

		// States
		UInt32 isSneaking;
		UInt32 isSneakingMe;
		UInt32 isSprintingMe;
		UInt32 isRecoilingMe;
		UInt32 isPowerAttack;
		UInt32 weaponType;

		d.Get(&scale, 0);
		d.Get(&scaleMe, 1);
		d.Get(&width, 2);
		d.Get(&widthMe, 3);
		d.Get(&height, 4);
		d.Get(&heightMe, 5);

		a.Get(&facing, 0);
		a.Get(&headingAngle, 1);
		a.Get(&ax, 2);

		p.Get(&dist, 0);
		p.Get(&dx, 1);
		p.Get(&dy, 2);
		p.Get(&zPos, 3);
		p.Get(&zPosMe, 4);

		s.Get(&isSneaking, 0);
		s.Get(&isSneakingMe, 1);
		s.Get(&isSprintingMe, 2);
		s.Get(&isRecoilingMe, 3);
		s.Get(&isPowerAttack, 4);
		s.Get(&weaponType, 5);

		Dimensions dimensions(scale, scaleMe, width, widthMe, height, heightMe);
		Angles angles(facing, headingAngle, ax);
		Positions pos(dist, dx, dy, zPos, zPosMe);
		Actions actions(isSneaking, isSneakingMe, isSprintingMe, isRecoilingMe, isPowerAttack, weaponType);

		return hitDetection(dimensions, angles, pos, actions);
	}

	UInt32 ArmHitCalcSKSE(StaticFunctionTag *base, float facing, float headingAngle, float armRange) {
		return (UInt32)armHitCalc(facing, headingAngle, armRange);
	}

	bool RegisterFuncs(VMClassRegistry* registry) {
		registry->RegisterFunction(new NativeFunction4 <StaticFunctionTag, UInt32, VMArray<float>, VMArray<float>, VMArray<float>, VMArray<UInt32>>("GetHitZone", "LDHitZones", LocDamageNamespace::GetHitZone, registry));
		registry->RegisterFunction(new NativeFunction3 <StaticFunctionTag, UInt32, float, float, float>("ArmHitCalcSKSE", "LDHitZones", LocDamageNamespace::ArmHitCalcSKSE, registry));
		return true;
	}
}
