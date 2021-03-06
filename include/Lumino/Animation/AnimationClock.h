﻿
#pragma once
#include "Common.h"
#include <Lumino/Reflection/ReflectionObject.h>
#include <Lumino/Animation/AnimationCurve.h>
#include "AnimatableObject.h"


LN_NAMESPACE_BEGIN

struct AnimationClockArgs
{
	AnimationCurve*		curve;
	Object*				targetObject;
	tr::PropertyInfo*	targetProperty;
};

/**
	@brief		
*/
class AnimationClock
	: public Object
{
public:
	AnimationClock();
	virtual ~AnimationClock();
	// TODO: internal
	void Initialize(AnimatableObject* targetObject);
	const tr::WeakRefPtr<AnimatableObject>& GetTargetObject() const { return m_targetObject; }

	void SetTime(double time);
	void AdvanceTime(float deltaTime);
	bool IsFinished() const { return m_isFinished; }

	//template<typename TCurve, typename TValue>
	//void AddAnimationCurve(TCurve* curve, AnimatableObject* targetObject, const tr::PropertyInfo* targetProperty, const TValue& startValue)
	//{
	//	targetObject->DeactivatePropertyAnimation(targetProperty);

	//	RefPtr<AnimationCurveInstance> inst(curve->CreateAnimationCurveInstance(targetObject, targetProperty, startValue), false);


	//	m_instanceList.Add(inst);
	//}
	template<typename TCurve, typename TValue>
	void AddAnimationCurve(TCurve* curve, AnimatableObject* targetObject, const tr::TypedPropertyInfo<TValue>* targetPropertyInfo,/*const tr::PropertyRef<TValue>& targetProperty, */const TValue& startValue)
	{
		auto targetProperty = tr::PropertyInfo::GetProperty(targetObject, targetPropertyInfo);
		targetObject->DeactivatePropertyAnimation(targetPropertyInfo);

		RefPtr<AnimationCurveInstanceBase> inst(curve->CreateAnimationCurveInstance(targetProperty, startValue), false);


		m_instanceList.Add(inst);
	}


private:
	friend class AnimatableObject;

	tr::WeakRefPtr<AnimatableObject>	m_targetObject;
	List<RefPtr<AnimationCurveInstanceBase>>	m_instanceList;
	float					m_currentTime;
	bool					m_isFinished;
};


LN_NAMESPACE_END
