#pragma once
#include<functional>

#include "playField.h"
GAME_MODULE_BEGIN
class Cubic_Bezier
{
	float p1, p2, p3, p4;
public:
	Cubic_Bezier(float v1, float v2, float v3, float v4);
	float GetProcess(float time);
};


class Animated_Play_Field : public Play_Field
{
	template<class T>
	class Animation
	{
		Cubic_Bezier cb;
		bool finished = false;
		std::function<void(void)>* end;
		T& _target;
		T _to;
		T _from;
	public:
		Animation(T& target, T to):_target(target),_to(to),_from(target)
		{
			
		}

		void play(float time)
		{
			auto proc = cb.GetProcess(time);
			if (proc >= 1)
			{
				_target = _to;
				finished = true;
				if (end != nullptr) (*end)();
			}
			else _target = _from + (_to - _from) * proc;
		}

		void SetCb(const Cubic_Bezier& cb)
		{
			this->cb = cb;
		}

		void whenEnd(std::function<void(void)>* cb)
		{
			end = cb;
		}

		bool Finished()
		{
			return finished;
		}
	};

	Animation<int>* drop_animation;
	int drop_off;

	void StartHarDropAnimation();

	void StartEilAnimation();
public:
	bool HasAnim();
	void Play();
	bool HardDrop();
	bool LockAndEil()
	{
		Lock();
		Eli();
	}
};
GAME_MODULE_END