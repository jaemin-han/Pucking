// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraShake/BFGThirdCameraShake.h"

UBFGThirdCameraShake::UBFGThirdCameraShake()
{
	OscillationDuration = 0.5f;
	OscillationBlendInTime = 0.1f;
	OscillationBlendOutTime = 0.1f;
	
	RotOscillation.Pitch.Amplitude = 9.5f;
	RotOscillation.Pitch.Frequency = 10.f;
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;

	RotOscillation.Yaw.Amplitude = 9.5f;
	RotOscillation.Yaw.Frequency = 10.f;
	RotOscillation.Yaw.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
}

void UBFGThirdCameraShake::StartShake()
{
	
}
