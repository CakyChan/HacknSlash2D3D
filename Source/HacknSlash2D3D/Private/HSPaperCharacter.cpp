#include "HSPaperCharacter.h"
#include "HSAttributeSet.h"
#include "HSAbilitySystemComponent.h"
#include "HSGameplayAbility.h"
#include "Components/CapsuleComponent.h"

AHSPaperCharacter::AHSPaperCharacter(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	bAlwaysRelevant = true;
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UHSAbilitySystemComponent>(this, TEXT("AbilitySystemComp"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	NetUpdateFrequency = 100.0f;

}


UAbilitySystemComponent* AHSPaperCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void AHSPaperCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AHSPaperCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (AbilitySystemComponent) {
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAbilities(DefaultCharacterAbilities, 0);
	}
}

void AHSPaperCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}


void AHSPaperCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent) {
		AttributeSet = AbilitySystemComponent->GetSet<UHSAttributeSet>();

		const_cast<UHSAttributeSet*>(AttributeSet)->HealthChangeDelegate.AddDynamic(this, &AHSPaperCharacter::OnHealthChangedNative);
		const_cast<UHSAttributeSet*>(AttributeSet)->MaxHealthChangeDelegate.AddDynamic(this, &AHSPaperCharacter::OnMaxHealthChangedNative);
		const_cast<UHSAttributeSet*>(AttributeSet)->HealthRegenChangeDelegate.AddDynamic(this, &AHSPaperCharacter::OnHealthRegenChangedNative);

		const_cast<UHSAttributeSet*>(AttributeSet)->ManaChangeDelegate.AddDynamic(this, &AHSPaperCharacter::OnManaChangedNative);
		const_cast<UHSAttributeSet*>(AttributeSet)->MaxManaChangeDelegate.AddDynamic(this, &AHSPaperCharacter::OnMaxManaChangedNative);
		const_cast<UHSAttributeSet*>(AttributeSet)->ManaRegenChangeDelegate.AddDynamic(this, &AHSPaperCharacter::OnManaRegenChangedNative);

		const_cast<UHSAttributeSet*>(AttributeSet)->BaseAttackDamageChangeDelegate.AddDynamic(this, &AHSPaperCharacter::OnBaseAttackDamageChangedNative);

		const_cast<UHSAttributeSet*>(AttributeSet)->MoveSpeedChangeDelegate.AddDynamic(this, &AHSPaperCharacter::OnMoveSpeedChangedNative);

		const_cast<UHSAttributeSet*>(AttributeSet)->CharacterLevelChangeDelegate.AddDynamic(this, &AHSPaperCharacter::OnCharacterLevelChangedNative);

		const_cast<UHSAttributeSet*>(AttributeSet)->XPChangeDelegate.AddDynamic(this, &AHSPaperCharacter::OnXPChangedNative);

		const_cast<UHSAttributeSet*>(AttributeSet)->ArmorChangeDelegate.AddDynamic(this, &AHSPaperCharacter::OnArmorChangedNative);

		const_cast<UHSAttributeSet*>(AttributeSet)->GoldChangeDelegate.AddDynamic(this, &AHSPaperCharacter::OnGoldChangedNative);

	}
}


void AHSPaperCharacter::Tick(float Delta)
{
	Super::Tick(Delta);
}

void AHSPaperCharacter::Die()
{
	RemoveAllCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAllAbilities();

		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);

		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	if (DeathAnimation)
	{
		PlayAnimMontage(DeathAnimation);
	}
	else
	{
		Destroy();
	}
}

void AHSPaperCharacter::InitializeAbility(TSubclassOf<UGameplayAbility> Ability, int32 Level)
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, Level, 0));

	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void AHSPaperCharacter::InitializeAbilities(TArray<TSubclassOf<UGameplayAbility>> Abilities, int32 Level)
{
	for (TSubclassOf<UGameplayAbility> AbilityItem : Abilities)
	{
		InitializeAbility(AbilityItem, Level);
	}
}


void AHSPaperCharacter::RemoveAllCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || !AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && DefaultCharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = false;
}

void AHSPaperCharacter::InitializeAttributes()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
	}
}

FGameplayAbilityInfo AHSPaperCharacter::GetAbilityInfoFromAbilityClass(TSubclassOf<UGameplayAbility> Ability, int32 Level)
{
	UGameplayAbility* AbilityCDO = Ability.GetDefaultObject();
	if (AbilityCDO)
	{
		UGameplayEffect* CooldownEffect = AbilityCDO->GetCooldownGameplayEffect();
		UGameplayEffect* CostEffect = AbilityCDO->GetCostGameplayEffect();

		if (CostEffect && CooldownEffect)
		{
			float CooldownDuration = 0;
			TArray<float> Cost;
			TArray<FString> CostName;
			float SingleCost = 0.0f;
			FString SingleCostName;
			Cost.Empty();
			CostName.Empty();
			CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(Level, CooldownDuration);
			if (CostEffect->Modifiers.Num() > 0)
			{
				for (auto& EffectInfo : CostEffect->Modifiers)
				{
					EffectInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level, SingleCost);
					FGameplayAttribute CostAttribute = EffectInfo.Attribute;
					SingleCostName = CostAttribute.AttributeName;
					Cost.Add(SingleCost);
					CostName.Add(SingleCostName);

				}
			}
			return FGameplayAbilityInfo(CooldownDuration, Cost, CostName);
		}
		else
		{
			return FGameplayAbilityInfo();
		}
	}
	else
	{
		return FGameplayAbilityInfo();
	}
}


bool AHSPaperCharacter::IsAlive() const
{
	return GetHealth() > 0.0f;
}

#pragma region Getters

int32 AHSPaperCharacter::GetAbilityLevel(EHSAbilityInputID AbilityID) const
{
	return 1;
}

int32 AHSPaperCharacter::GetCharacterLevel() const
{
	if (AttributeSet)
	{
		return static_cast<int32>(AttributeSet->GetCharacterLevel());
	}

	return 0;
}

FText AHSPaperCharacter::GetCharacterName() const
{
	if (!CharacterName.IsEmpty()) 
	{
		return CharacterName;
	}

	return FText::FromString(TEXT("John Doe"));
}

float AHSPaperCharacter::GetHealth() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetHealth();
	}

	return 0.0f;
}

float AHSPaperCharacter::GetMaxHealth() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMaxHealth();
	}

	return 0.0f;
}

float AHSPaperCharacter::GetHealthRegen() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetHealthRegen();
	}

	return 0.0f;
}

float AHSPaperCharacter::GetMana() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMana();
	}

	return 0.0f;
}

float AHSPaperCharacter::GetMaxMana() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMaxMana();
	}

	return 0.0f;
}

float AHSPaperCharacter::GetManaRegen() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetManaRegen();
	}

	return 0.0f;
}

float AHSPaperCharacter::GetBaseAttackDamage() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetBaseAttackDamage();
	}

	return 0.0f;
}

float AHSPaperCharacter::GetMoveSpeed() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMoveSpeed();
	}

	return 0.0f;
}

float AHSPaperCharacter::GetBaseMoveSpeed() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMoveSpeedAttribute().GetGameplayAttributeData(AttributeSet)->GetBaseValue();
	}

	return 0.0f;
}

float AHSPaperCharacter::GetXP() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetXP();
	}

	return 0.0f;
}

float AHSPaperCharacter::GetGold() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetGold();
	}

	return 0.0f;
}

float AHSPaperCharacter::GetArmor() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetArmor();
	}

	return 0.0f;
}

#pragma endregion

#pragma region Setters

void AHSPaperCharacter::SetCharacterName(FText NewName)
{
	CharacterName = NewName;
}

void AHSPaperCharacter::SetHealth(float Health)
{
	if (AttributeSet)
	{
		AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetHealthAttribute(), EGameplayModOp::Override, Health);
	}
}

void AHSPaperCharacter::SetMaxHealth(float MaxHealth)
{
	if (AttributeSet)
	{
		AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetMaxHealthAttribute(), EGameplayModOp::Override, MaxHealth);
	}
}

void AHSPaperCharacter::SetHealthRegen(float HealthRegen)
{
	if (AttributeSet)
	{
		AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetHealthRegenAttribute(), EGameplayModOp::Override, HealthRegen);
	}
}

void AHSPaperCharacter::SetMana(float Mana)
{
	if (AttributeSet)
	{
		AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetManaAttribute(), EGameplayModOp::Override, Mana);
	}
}

void AHSPaperCharacter::SetMaxMana(float MaxMana)
{
	if (AttributeSet)
	{
		AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetMaxManaAttribute(), EGameplayModOp::Override, MaxMana);
	}
}

void AHSPaperCharacter::SetManaRegen(float ManaRegen)
{
	if (AttributeSet)
	{
		AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetManaRegenAttribute(), EGameplayModOp::Override, ManaRegen);
	}
}

void AHSPaperCharacter::SetBaseAttackDamage(float BaseAttack)
{
	if (AttributeSet)
	{
		AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetBaseAttackDamageAttribute(), EGameplayModOp::Override, BaseAttack);
	}
}

void AHSPaperCharacter::SetMoveSpeed(float Speed)
{
	if (AttributeSet)
	{
		AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetMoveSpeedAttribute(), EGameplayModOp::Override, Speed);
	}
}
void AHSPaperCharacter::SetCharacterLevel(float Level)
{
	if (AttributeSet)
	{
		AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetCharacterLevelAttribute(), EGameplayModOp::Override, Level);
	}
}
void AHSPaperCharacter::SetXP(float XP)
{
	if (AttributeSet)
	{
		AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetXPAttribute(), EGameplayModOp::Override, XP);
	}
}
void AHSPaperCharacter::SetGold(float Gold)
{
	if (AttributeSet)
	{
		AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetGoldAttribute(), EGameplayModOp::Override, Gold);
	}
}
void AHSPaperCharacter::SetArmor(float Armor)
{
	if (AttributeSet)
	{
		AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetArmorAttribute(), EGameplayModOp::Override, Armor);
	}
}

#pragma endregion

#pragma region Change Delegates

void AHSPaperCharacter::OnHealthChangedNative(float Health, int32 Stack)
{
	OnHealthChange(Health, Stack);
}

void AHSPaperCharacter::OnMaxHealthChangedNative(float MaxHealth, int32 Stack)
{
	OnMaxHealthChange(MaxHealth, Stack);
}

void AHSPaperCharacter::OnHealthRegenChangedNative(float HealthRegen, int32 Stack)
{
	OnHealthRegenChange(HealthRegen, Stack);
}

void AHSPaperCharacter::OnManaChangedNative(float Mana, int32 Stack)
{
	OnManaChange(Mana, Stack);
}

void AHSPaperCharacter::OnMaxManaChangedNative(float MaxMana, int32 Stack)
{
	OnMaxManaChange(MaxMana, Stack);
}

void AHSPaperCharacter::OnManaRegenChangedNative(float ManaRegen, int32 Stack)
{
	OnManaRegenChange(ManaRegen, Stack);
}

void AHSPaperCharacter::OnBaseAttackDamageChangedNative(float BaseDamage, int32 Stack)
{
	OnBaseAttackDamageChange(BaseDamage, Stack);
}

void AHSPaperCharacter::OnMoveSpeedChangedNative(float Speed, int32 Stack)
{
	OnMoveSpeedChange(Speed, Stack);
}

void AHSPaperCharacter::OnCharacterLevelChangedNative(float Level, int32 Stack)
{
	OnCharacterLevelChange(Level, Stack);
}

void AHSPaperCharacter::OnXPChangedNative(float XP, int32 Stack)
{
	OnXPChange(XP, Stack);
}

void AHSPaperCharacter::OnGoldChangedNative(float Gold, int32 Stack)
{
	OnGoldChange(Gold, Stack);
}

void AHSPaperCharacter::OnArmorChangedNative(float Armor, int32 Stack)
{
	OnArmorChange(Armor, Stack);
}

#pragma endregion