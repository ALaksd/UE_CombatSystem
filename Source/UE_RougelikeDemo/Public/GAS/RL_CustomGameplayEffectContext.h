#pragma once
#include "GameplayEffectTypes.h"
#include "RL_CustomGameplayEffectContext.generated.h"

class UGameplayEffect;
USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() {};

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemConponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemConponent;

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	UPROPERTY(BlueprintReadWrite)

	float KnockBackMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector KnockBackImpulse = FVector::ZeroVector;
};


USTRUCT(BlueprintType)
struct FRLGameplayEffectContext :public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	FORCEINLINE FVector GetKnockBackImpulse() const { return KnockBackImpulse; }
	FORCEINLINE FName GetHitBoneName() const { return HitBoneName; }

	FORCEINLINE void SetKnockBackImpulse(FVector& InKnockBackImpulse) { KnockBackImpulse = InKnockBackImpulse; }
	FORCEINLINE void SetHitBoneName(FName& InHitBoneName) { HitBoneName = InHitBoneName; }

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}

	virtual FRLGameplayEffectContext* Duplicate() const
	{
		FRLGameplayEffectContext* NewContext = new FRLGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:

	UPROPERTY()
	FVector KnockBackImpulse = FVector::ZeroVector;

	UPROPERTY()
	FName HitBoneName = FName();
};

template<>
struct TStructOpsTypeTraits< FRLGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FRLGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true                // Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};
