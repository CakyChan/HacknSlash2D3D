// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "GameplayAbilityInfo.h"
#include "HacknSlash2D3D/HacknSlash2D3D.h"
#include "HSPaperCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AHSPaperCharacter*, Character);

UCLASS()
class HACKNSLASH2D3D_API AHSPaperCharacter : public APaperZDCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "HSPaperCharacter | Attributes | Name")
	FText CharacterName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "HSGameplayAbility | Ability")
	class UHSAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "HSGameplayAbility | Attribute")
	const class UHSAttributeSet* AttributeSet;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "HSGameplayAbility | HSAnimation")
	UAnimMontage* DeathAnimation;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "HSGameplayAbility | HSAbilities")
	TArray<TSubclassOf<class UGameplayAbility>> DefaultCharacterAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "HSGameplayAbility | HSAbilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	virtual void InitializeAbility(TSubclassOf<UGameplayAbility> Ability, int32 Level);
	virtual void InitializeAbilities(TArray<TSubclassOf<UGameplayAbility>> Abilities, int32 Level);
	virtual void InitializeAttributes();

#pragma region Change Delegates

	UFUNCTION()
	virtual void OnHealthChangedNative(float Health, int32 Stack);

	UFUNCTION()
	virtual void OnMaxHealthChangedNative(float MaxHealth, int32 Stack);

	UFUNCTION()
	virtual void OnHealthRegenChangedNative(float HealthRegen, int32 Stack);

	UFUNCTION()
	virtual void OnManaChangedNative(float Mana, int32 Stack);

	UFUNCTION()
	virtual void OnMaxManaChangedNative(float MaxMana, int32 Stack);

	UFUNCTION()
	virtual void OnManaRegenChangedNative(float ManaRegen, int32 Stack);

	UFUNCTION()
	virtual void OnBaseAttackDamageChangedNative(float BaseDamage, int32 Stack);

	UFUNCTION()
	virtual void OnMoveSpeedChangedNative(float MoveSpeed, int32 Stack);

	UFUNCTION()
	virtual void OnCharacterLevelChangedNative(float Level, int32 Stack);

	UFUNCTION()
	virtual void OnXPChangedNative(float XP, int32 Stack);

	UFUNCTION()
	virtual void OnGoldChangedNative(float Gold, int32 Stack);

	UFUNCTION()
	virtual void OnArmorChangedNative(float Armor, int32 Stack);

#pragma endregion

#pragma region Change Trigger

	UFUNCTION(BlueprintImplementableEvent, Category = "HSGameplayAbility")
	void OnHealthChange(float Health, int32 Stack);

	UFUNCTION(BlueprintImplementableEvent, Category = "HSGameplayAbility")
	void OnMaxHealthChange(float MaxHealth, int32 Stack);

	UFUNCTION(BlueprintImplementableEvent, Category = "HSGameplayAbility")
	void OnHealthRegenChange(float HealthRegen, int32 Stack);

	UFUNCTION(BlueprintImplementableEvent, Category = "HSGameplayAbility")
	void OnManaChange(float Mana, int32 Stack);

	UFUNCTION(BlueprintImplementableEvent, Category = "HSGameplayAbility")
	void OnMaxManaChange(float MaxMana, int32 Stack);

	UFUNCTION(BlueprintImplementableEvent, Category = "HSGameplayAbility")
	void OnManaRegenChange(float ManaRegen, int32 Stack);

	UFUNCTION(BlueprintImplementableEvent, Category = "HSGameplayAbility")
	void OnBaseAttackDamageChange(float BaseDamage, int32 Stack);

	UFUNCTION(BlueprintImplementableEvent, Category = "HSGameplayAbility")
	void OnMoveSpeedChange(float MoveSpeed, int32 Stack);

	UFUNCTION(BlueprintImplementableEvent, Category = "HSGameplayAbility")
	void OnCharacterLevelChange(float Level, int32 Stack);

	UFUNCTION(BlueprintImplementableEvent, Category = "HSGameplayAbility")
	void OnXPChange(float XP, int32 Stack);

	UFUNCTION(BlueprintImplementableEvent, Category = "HSGameplayAbility")
	void OnGoldChange(float Gold, int32 Stack);

	UFUNCTION(BlueprintImplementableEvent, Category = "HSGameplayAbility")
	void OnArmorChange(float Armor, int32 Stack);

#pragma endregion

#pragma region Setters

	UFUNCTION(BlueprintCallable, Category = "HSPaperCharacter | Attributes | Name")
	void SetCharacterName(FText NewName);

	UFUNCTION(BlueprintCallable, Category = "HSPaperCharacter | Attributes | Health")
	void SetHealth(float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "HSPaperCharacter | Attributes | Health")
	void SetMaxHealth(float NewMaxHealth);

	UFUNCTION(BlueprintCallable, Category = "HSPaperCharacter | Attributes | Health")
	void SetHealthRegen(float NewHealthRegen);

	UFUNCTION(BlueprintCallable, Category = "HSPaperCharacter | Attributes | Mana")
	void SetMana(float NewMana);

	UFUNCTION(BlueprintCallable, Category = "HSPaperCharacter | Attributes | Mana")
	void SetMaxMana(float NewMaxMana);

	UFUNCTION(BlueprintCallable, Category = "HSPaperCharacter | Attributes | Mana")
	void SetManaRegen(float NewManaRegen);

	UFUNCTION(BlueprintCallable, Category = "HSPaperCharacter | Attributes | Damage")
	void SetBaseAttackDamage(float NewBaseDamage);

	UFUNCTION(BlueprintCallable, Category = "HSPaperCharacter | Attributes | Level")
	void SetCharacterLevel(float NewLevel);

	UFUNCTION(BlueprintCallable, Category = "HSPaperCharacter | Attributes | XP")
	void SetXP(float NewXP);

	UFUNCTION(BlueprintCallable, Category = "HSPaperCharacter | Attributes | Gold")
	void SetGold(float NewGold);

	UFUNCTION(BlueprintCallable, Category = "HSPaperCharacter | Attributes | Armor")
	void SetArmor(float NewArmor);

	UFUNCTION(BlueprintCallable, Category = "HSPaperCharacter | Attributes | Move")
	void SetMoveSpeed(float NewSpeed);

#pragma endregion

#pragma region Tags

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

#pragma endregion


public:
	AHSPaperCharacter(const class FObjectInitializer& ObjectInitializer);
	virtual void Tick(float Delta) override;
	virtual	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UFUNCTION(BlueprintCallable, Category = "HSPaperCharacter")
	virtual bool IsAlive() const;

	UPROPERTY(BlueprintAssignable, Category = "HSPaperCharacter")
	FCharacterDiedDelegate OnCharacterDied;

	UFUNCTION(BlueprintCallable, Category = "HSPaperCharacter")
	virtual int32 GetAbilityLevel(EHSAbilityInputID AbilityID) const;

	UFUNCTION(BlueprintCallable, Category = "HSPaperCharacter")
	FGameplayAbilityInfo GetAbilityInfoFromAbilityClass(TSubclassOf<UGameplayAbility> AbilityClass, int32 AtAbilityLevel);

	virtual void RemoveAllCharacterAbilities();

#pragma region Getters

	UFUNCTION(BlueprintPure, Category = "HSPaperCharacter | Attributes | Name")
	FText GetCharacterName() const;

	UFUNCTION(BlueprintPure, Category = "HSPaperCharacter | Attributes | Health")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintPure, Category = "HSPaperCharacter | Attributes | Health")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "HSPaperCharacter | Attributes | Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure, Category = "HSPaperCharacter | Attributes | Health")
	float GetHealthRegen() const;

	UFUNCTION(BlueprintPure, Category = "HSPaperCharacter | Attributes | Mana")
	float GetMana() const;

	UFUNCTION(BlueprintPure, Category = "HSPaperCharacter | Attributes | Mana")
	float GetMaxMana() const;

	UFUNCTION(BlueprintPure, Category = "HSPaperCharacter | Attributes | Mana")
	float GetManaRegen() const;

	UFUNCTION(BlueprintPure, Category = "HSPaperCharacter | Attributes | Damage")
	float GetBaseAttackDamage() const;

	UFUNCTION(BlueprintPure, Category = "HSPaperCharacter | Attributes | Move")
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintPure, Category = "HSPaperCharacter | Attributes | Move")
	float GetBaseMoveSpeed() const;

	UFUNCTION(BlueprintPure, Category = "HSPaperCharacter | Attributes | XP")
	float GetXP() const;

	UFUNCTION(BlueprintPure, Category = "HSPaperCharacter | Attributes | Gold")
	float GetGold() const;

	UFUNCTION(BlueprintPure, Category = "HSPaperCharacter | Attributes | Armor")
	float GetArmor() const;

#pragma endregion

	virtual void Die();
};