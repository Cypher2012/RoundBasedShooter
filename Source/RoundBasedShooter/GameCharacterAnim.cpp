// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCharacterAnim.h"

#include "Animation/AnimSequence.h"

UGameCharacterAnim::UGameCharacterAnim()
{
	
}

void UGameCharacterAnim::UpdateIdleAnimation_Implementation(UAnimSequence* NewIdleAnim)
{
	IdleAnimation = NewIdleAnim;
}
