#include "LocationalDamage.h"



namespace LocDamageNamespace {
    constexpr std::string_view PapyrusClass = "LDHitZones";

	int armHitCalc(float facing, float headingAngle, double armRange) {
		if (abs(facing) < 45) {
			if (headingAngle > armRange) {
				return rArm;
			}
			if (headingAngle < -armRange) {
				return lArm;
			}

		    return chest;
		}
		if (facing < -45 && facing > -135) {
			return lArm;
		}
		if (facing > 45 && facing < 135) {
			return rArm;
		}
		if (abs(facing) >= 135) {
			if (headingAngle > armRange) {
				return lArm;
			}
			if (headingAngle < -armRange) {
				return rArm;
			}

		    return back;
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
			if (weaponType != 7) {
				if (powerAttack && weaponType != 3 && weaponType != 4 && weaponType != 6) {
					return heart;
				}

			    return chest;
			}

		    return heart;
		}
		if (facing < -45 && facing > -135) {
			return lShoulder;
		}
		if (facing > 45 && facing < 135) {
			return rShoulder;
		}
		if (abs(facing) >= 135) {
			// Behind the target
			if (headingAngle > shoulderRange) {
				return lShoulder;
			}
			if (headingAngle < -shoulderRange) {
				return rShoulder;
			}

		    return back;
		}
	    return 0;
	}

	uint32_t hitDetection(Dimensions d, Angles a, Positions p, Actions s) {
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
		double zHeightMe = p.zPosMe;

		if (s.isSneaking) {
			net_height = net_height * .85;
		}

		if (s.isSneakingMe || s.isRecoilingMe) {
			net_height_me = net_height_me * .75;
		}
		else if (s.isSprintingMe) {
			net_height_me = net_height_me * .98;
		}

		zHeight = p.zPos + net_height;

		// Y-AXIS ZONES
		dispHeight = zHeight - (zHeightMe + net_height_me * 0.900);
		headZone = atan(dispHeight / p.dist) * 180 / PI;

		// Head strike
		if (a.ax < headZone) {
			return head;
		}

		dispHeight = zHeight - (zHeightMe + net_height_me * 0.850);
		neckZone = atan(dispHeight / p.dist) * 180 / PI;

		// Neck strike
		if (a.ax < neckZone) {
			if (a.facing <= -135 || a.facing >= 135) {
				return nape;
			}

		    return neck;
		}

		// X-AXIS ZONES
		dispWidth = d.width - d.widthMe * .900;
		shoulderRange = abs(asin(dispWidth / p.dx) * 180 / PI);
		dispWidth = d.width - d.widthMe * .800;
		armRange = abs(asin(dispWidth / p.dx) * 180 / PI);

		dispHeight = zHeight - (zHeightMe + net_height_me * 0.750);
		shoulderZone = atan(dispHeight / p.dist) * 180 / PI;

		// Shoulder strike
		if (a.ax < shoulderZone) {
			return shoulderhitCalc(s.isPowerAttack, a.facing, a.headingAngle, shoulderRange, s.weaponType);
		}

		dispHeight = zHeight - (zHeightMe + net_height_me * 0.500);
		chestZone = atan(dispHeight / p.dist) * 180 / PI;

		// Chest strike
		if (a.ax < chestZone) {
			return armHitCalc(a.facing, a.headingAngle, armRange);
		}

		dispHeight = zHeight - (zHeightMe + net_height_me * 0.400);
		groinZone = atan(dispHeight / p.dist) * 180 / PI;

		// Groin strike
		if (a.ax < groinZone) {
			if (abs(a.facing) < 45) {
				return groin;
			}

		    return legs;
		}

		dispHeight = zHeight - (zHeightMe + net_height_me * 0.200);
		legZone = atan(dispHeight / p.dist) * 180 / PI;

		// Leg strike
		if (a.ax < legZone) {
			return legs;
		}

		// Feet strike
		return feet;
	}

	uint32_t GetHitZone(RE::StaticFunctionTag*, std::vector<float> d, std::vector<float> a, std::vector<float> p, std::vector<uint32_t> s) {
		// Dimensions
        const auto scale = d[0];
        const auto scaleMe = d[1];
	    const auto width = d[2];
	    const auto widthMe = d[3];
	    const auto height = d[4];
	    const auto heightMe = d[5];

		// Angles
		const auto facing = a[0];
		const auto headingAngle = a[1];
		const auto ax = a[2];

		// Positions
		const auto dist = p[0];
		const auto dx = p[1];
		const auto dy = p[2];
		const auto zPos = p[3];
		const auto zPosMe = p[4];

		// States
		const auto isSneaking = s[0];
		const auto isSneakingMe = s[1];
		const auto isSprintingMe = s[2];
		const auto isRecoilingMe = s[3];
		const auto isPowerAttack = s[4];
		const auto weaponType = s[5];

		Dimensions dimensions(scale, scaleMe, width, widthMe, height, heightMe);
		Angles angles(facing, headingAngle, ax);
		Positions pos(dist, dx, dy, zPos, zPosMe);
		Actions actions(isSneaking, isSneakingMe, isSprintingMe, isRecoilingMe, isPowerAttack, weaponType);

		return hitDetection(dimensions, angles, pos, actions);
	}

	uint32_t ArmHitCalcSKSE(RE::StaticFunctionTag*, float facing, float headingAngle, double armRange) {
		return (uint32_t)armHitCalc(facing, headingAngle, armRange);
	}

    bool RegisterFuncs(RE::BSScript::IVirtualMachine* vm) {
	    vm->RegisterFunction("GetHitZone", PapyrusClass, LocDamageNamespace::GetHitZone);
	    vm->RegisterFunction("ArmHitCalcSKSE", PapyrusClass, LocDamageNamespace::ArmHitCalcSKSE);

	    return true;
	}
}