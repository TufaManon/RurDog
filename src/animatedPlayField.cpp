#include "animatedPlayField.h"
GAME_MODULE_BEGIN
Cubic_Bezier::Cubic_Bezier(float v1, float v3, float v2, float v4) :p1(v1), p2(v2), p3(v3), p4(v4)
{

}

float Cubic_Bezier::GetProcess(float time)
{
	float f1, f2, f3;
	float s1, s2;
	f1 = p1 * (1 - time) + p2 * time;
	f2 = p2 * (1 - time) + p3 * time;
	f3 = p3 * (1 - time) + p4 * time;
	s1 = f1 * (1 - time) + f2 * time;
	s2 = f2 * (1 - time) + f3 * time;
	return s1 * (1 - time) + s2 * time;
}

void Animated_Play_Field::StartHarDropAnimation()
{
	drop_animation = new Animation<int>(drop_off,0);
	drop_animation->SetCb(Cubic_Bezier(.03f, .53f, 1, 1));                        
}
bool Animated_Play_Field::HardDrop()
{
	while (Drop())
	{
		drop_off--;
	}
	StartHarDropAnimation();
	return drop_off < 0;
}
GAME_MODULE_END


