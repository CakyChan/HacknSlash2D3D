// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityInfo.h"

FGameplayAbilityInfo::FGameplayAbilityInfo()
{
	Cooldown = 0.0f;
	Cost.Empty();
	CostName.Empty();
}

FGameplayAbilityInfo::FGameplayAbilityInfo(float Cooldown, TArray<float> Cost, TArray<FString> CostName)
{
	Cooldown = Cooldown;
	Cost = Cost;
	CostName = CostName;
}