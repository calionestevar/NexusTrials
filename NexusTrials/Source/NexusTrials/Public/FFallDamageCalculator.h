// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * FFallDamageCalculator: A pure, testable utility for calculating fall damage.
 * This class is designed to be easily unit-tested via the NexusQA framework.
 */
class FFallDamageCalculator
{
public:
	/** 
	 * Threshold velocity below which fall damage isn't applied (cm/s)
	 * Default: 1000 cm/s (10 meters/second)
	 */
	static constexpr float FALL_DAMAGE_THRESHOLD = 1000.0f;

	/** Damage multiplier per 100 cm/s of excess fall velocity */
	static constexpr float DAMAGE_MULTIPLIER = 0.1f;

	/**
	 * Accumulate fall velocity and compute damage.
	 * 
	 * @param CurrentVelocityZ Current Z velocity component (negative = falling)
	 * @param DeltaTime Time since last frame in seconds
	 * @param InOutFallDamageTimer Accumulator for fall damage (modified in place)
	 * @return Damage to apply, or 0.0f if no damage
	 */
	static float AccumulateAndComputeDamage(float CurrentVelocityZ, float DeltaTime, float& InOutFallDamageTimer)
	{
		// Only accumulate if falling (negative Z velocity)
		if (CurrentVelocityZ < 0.0f)
		{
			InOutFallDamageTimer += FMath::Abs(CurrentVelocityZ) * DeltaTime;
		}
		else
		{
			// Reset accumulator when not falling
			float Damage = CalculateDamage(InOutFallDamageTimer);
			InOutFallDamageTimer = 0.0f;
			return Damage;
		}

		return 0.0f;
	}

	/**
	 * Calculate damage from accumulated fall distance/velocity.
	 * 
	 * @param AccumulatedFallValue Total accumulated fall velocity*time
	 * @return Damage amount, or 0.0f if below threshold
	 */
	static float CalculateDamage(float AccumulatedFallValue)
	{
		if (AccumulatedFallValue <= FALL_DAMAGE_THRESHOLD)
		{
			return 0.0f;
		}

		float ExcessFall = AccumulatedFallValue - FALL_DAMAGE_THRESHOLD;
		return ExcessFall * DAMAGE_MULTIPLIER;
	}
};
