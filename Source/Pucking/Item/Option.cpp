// Fill out your copyright notice in the Description page of Project Settings.


#include "Option.h"

// todo: EOptionType 가 추가됨에 따라 수정 필요
void UOption::SetOptionDescription()
{
	// OptionType, OptionValue에 따라 OptionDescription을 설정
	// 예시: OptionType 이 EOptionType::Critical, OptionValue가 0.5일 경우 "치명타 0.5"로 설정
	switch (OptionType)
	{
	case EOptionType::Critical:
		OptionDescription = FString::Printf(TEXT("치명타 %.1f"), OptionValue);
		break;
	default:
		OptionDescription = FString::Printf(TEXT("Unknown Option"));
		break;
	}
}

FString UOption::GetOptionDescription()
{
	// OptionDescription 이 비어있으면 설정
	if (OptionDescription.IsEmpty())
	{
		SetOptionDescription();
	}
	return OptionDescription;
}
