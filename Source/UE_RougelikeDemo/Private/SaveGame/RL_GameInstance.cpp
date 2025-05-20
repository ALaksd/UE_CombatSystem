// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/RL_GameInstance.h"

ECharacterType URL_GameInstance::GetSelectCharacter()
{
	return SelectCharacter;
}

void URL_GameInstance::SetSelectCharacter(ECharacterType CharacterType)
{
	SelectCharacter = CharacterType;
}
