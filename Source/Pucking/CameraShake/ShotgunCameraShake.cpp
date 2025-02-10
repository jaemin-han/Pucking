// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraShake/ShotgunCameraShake.h"

UShotgunCameraShake::UShotgunCameraShake()
{
	OscillationDuration = 0.2f;
	OscillationBlendInTime = 0.1f;
	OscillationBlendOutTime = 0.1f;
	
	RotOscillation.Pitch.Amplitude = 2.f;
	RotOscillation.Pitch.Frequency = 8.f;
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;

	RotOscillation.Yaw.Amplitude = 2.f;
	RotOscillation.Yaw.Frequency = 8.f;
	RotOscillation.Yaw.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
}
