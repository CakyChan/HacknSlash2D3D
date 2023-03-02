// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilityInfo.h"
#include "HSGameplayAbility.generated.h"

/**
 *
 */
UCLASS()
class HACKNSLASH2D3D_API UHSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "HSGameplayAbility")
		FGameplayAbilityInfo GetAbilityInfo();
};