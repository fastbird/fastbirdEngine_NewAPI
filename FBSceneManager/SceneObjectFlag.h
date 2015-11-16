#pragma once
namespace fastbird{
	namespace SceneObjectFlag{
		enum Enum{
			Hide = 1,
			Queryable = 1 << 1,
			Ignore = 1 << 2,
			NoDepthPath = 1 << 3, // todo: move to renderable flag
			HighlightDedi = 1 << 4, // todo: move to renderable flag			
			Last = HighlightDedi // Can define user flag from Last << 1
		};
	}
}