// Fill out your copyright notice in the Description page of Project Settings.


#include "HSAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"


void UHSAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
	else if (Attribute == GetMoveSpeedAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 150, 1000);
	}
}


void UHSAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		HealthChangeDelegate.Broadcast(GetHealth(), Data.EffectSpec.StackCount);
	}
	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		HealthChangeDelegate.Broadcast(GetMaxHealth(), Data.EffectSpec.StackCount);
	}
	if (Data.EvaluatedData.Attribute == GetHealthRegenAttribute())
	{
		HealthChangeDelegate.Broadcast(GetHealthRegen(), Data.EffectSpec.StackCount);
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		ManaChangeDelegate.Broadcast(GetMana(), Data.EffectSpec.StackCount);
	}
	if (Data.EvaluatedData.Attribute == GetMaxManaAttribute())
	{
		ManaChangeDelegate.Broadcast(GetMaxMana(), Data.EffectSpec.StackCount);
	}
	if (Data.EvaluatedData.Attribute == GetManaRegenAttribute())
	{
		ManaChangeDelegate.Broadcast(GetManaRegen(), Data.EffectSpec.StackCount);
	}
	if (Data.EvaluatedData.Attribute == GetBaseAttackDamageAttribute())
	{
		BaseAttackDamageChangeDelegate.Broadcast(GetBaseAttackDamage(), Data.EffectSpec.StackCount);
	}
	if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
	{
		MoveSpeedChangeDelegate.Broadcast(GetMoveSpeed(), Data.EffectSpec.StackCount);
	}
	if (Data.EvaluatedData.Attribute == GetCharacterLevelAttribute())
	{
		MoveSpeedChangeDelegate.Broadcast(GetCharacterLevel(), Data.EffectSpec.StackCount);
	}
	if (Data.EvaluatedData.Attribute == GetXPAttribute())
	{
		MoveSpeedChangeDelegate.Broadcast(GetXP(), Data.EffectSpec.StackCount);
	}
	if (Data.EvaluatedData.Attribute == GetGoldAttribute())
	{
		MoveSpeedChangeDelegate.Broadcast(GetGold(), Data.EffectSpec.StackCount);
	}
	if (Data.EvaluatedData.Attribute == GetArmorAttribute())
	{
		MoveSpeedChangeDelegate.Broadcast(GetArmor(), Data.EffectSpec.StackCount);
	}
}

UHSAttributeSet::UHSAttributeSet()
{
}

void UHSAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UHSAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHSAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHSAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHSAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHSAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHSAttributeSet, ManaRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHSAttributeSet, BaseAttackDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHSAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHSAttributeSet, CharacterLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHSAttributeSet, XP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHSAttributeSet, Gold, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHSAttributeSet, Armor, COND_None, REPNOTIFY_Always);
}

void UHSAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHSAttributeSet, Health, OldHealth);
}

void UHSAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHSAttributeSet, MaxHealth, OldMaxHealth);
}

void UHSAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHSAttributeSet, HealthRegen, OldHealthRegen);
}

void UHSAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHSAttributeSet, Mana, OldMana);
}

void UHSAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHSAttributeSet, MaxMana, OldMaxMana);
}

void UHSAttributeSet::OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHSAttributeSet, ManaRegen, OldManaRegen);
}

void UHSAttributeSet::OnRep_BaseAttackDamage(const FGameplayAttributeData& OldBaseAttackDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHSAttributeSet, BaseAttackDamage, OldBaseAttackDamage);
}

void UHSAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHSAttributeSet, MoveSpeed, OldMoveSpeed);
}

void UHSAttributeSet::OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHSAttributeSet, CharacterLevel, OldCharacterLevel);
}

void UHSAttributeSet::OnRep_XP(const FGameplayAttributeData& OldXP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHSAttributeSet, XP, OldXP);
}

void UHSAttributeSet::OnRep_Gold(const FGameplayAttributeData& OldGold)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHSAttributeSet, Gold, OldGold);
}

void UHSAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHSAttributeSet, Armor, OldArmor);
}


void UHSAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;
		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}
