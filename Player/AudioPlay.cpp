#include "AudioPlay.h"
#include "CAudioPlay.h"

AudioPlay::AudioPlay()
{
}

AudioPlay::~AudioPlay()
{
}

AudioPlay* AudioPlay::Get()
{
	static CAudioPlay play;
	return &play;
}
