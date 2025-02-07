// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraShake/BFGSecondCameraShake.h"

UBFGSecondCameraShake::UBFGSecondCameraShake()
{
	OscillationDuration = 0.75f;
	OscillationBlendInTime = 0.1f;
	OscillationBlendOutTime = 0.1f;
	
	RotOscillation.Pitch.Amplitude = 1.5f;
	RotOscillation.Pitch.Frequency = 30.f;
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_MAX;

	RotOscillation.Yaw.Amplitude = 1.5f;
	RotOscillation.Yaw.Frequency = 30.f;
	RotOscillation.Yaw.InitialOffset = EInitialOscillatorOffset::EOO_MAX;
}
