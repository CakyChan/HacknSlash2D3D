// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilityInfo.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct HACKNSLASH2D3D_API FGameplayAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HSGameplayAbility")
	float Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HSGameplayAbility")
	TArray<float> Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HSGameplayAbility")
	TArray<FString> CostName;

	FGameplayAbilityInfo();
	FGameplayAbilityInfo(float Cooldown, TArray<float> Cost, TArray<FString> CostName);
};
