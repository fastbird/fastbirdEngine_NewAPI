#pragma once
namespace fastbird{
	class ITexture;

	struct DrawQuadParam{
		int x, y; 
		int width, height;
		float r, g, b, a;
		bool updateRs; // default true.
	};

	struct DrawQuadLineParam{
		int x, y; 
		int width, height;
		int x2, y2;
		int width2, height2;
		float r, g, b, a;
	};

	struct DrawQuadWithTextureParam{
		int x, y; 
		int width, height;
		float r, g, b, a;
		ITexture* texture; 
		//IMaterial* materialOverride = 0 -- should bind in Renderer layer.
	};

	struct DrawQuadWithTextureUVParam{
		int x, y;
		int width, height; 
		float uStart, vStart, uEnd, vEnd;		
		float r, g, b, a;
		ITexture* texture;
		//IMaterial* materialOverride = 0 -- should bind in Renderer layer.
	};

	struct DrawBillboardWorldQuadParam{
		float x, y, z;
		float width, height;
		float offsetX, offsetY;
		unsigned color;
		//IMaterial* pMat -- shold bind in Renderer layer.
	};

	struct DrawTriangleParam{
		float x, y, z;
		float x2, y2, z2;
		float x3, y3, z3;
		float r, g, b, a;
		//IMaterial* pMat -- shold bind in Renderer layer.
	};
}