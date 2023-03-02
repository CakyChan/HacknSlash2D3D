// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "HSAbilitySystemComponent.generated.h"

/**
 *
 */
UCLASS()
class HACKNSLASH2D3D_API UHSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	bool bCharacterAbilitiesGiven = false;
};
