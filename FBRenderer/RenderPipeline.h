#pragma once
#include "RenderSteps.h"

namespace fastbird
{
	DECLARE_SMART_PTR(RenderPipeline);
	class RenderPipeline
	{
		bool mSteps[RenderSteps::Num];

	public:
		RenderPipeline();
		RenderPipeline(const RenderPipeline& other);
		RenderPipeline(bool steps[]);
		~RenderPipeline();

	public:

		RenderPipeline* Clone() const;
		virtual RenderPipeline& SetStep(RenderSteps::Enum step, bool enable);
		bool GetStep(RenderSteps::Enum step) const;

		virtual RenderPipeline& SetMaximum();
		virtual RenderPipeline& SetMinimum();
	};

}