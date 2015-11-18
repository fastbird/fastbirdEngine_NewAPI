#pragma once
namespace fastbird{
	namespace ResourceTypes{
		namespace Textures{
			enum Enum{				
				Noise,
				GGXGenTarget,				
				ToneMap,
				LuminanceMap,				
				Last = LuminanceMap,
			};
		}

		namespace Shaders{
			enum Enum{
				FullscreenQuadNearVS,
				FullscreenQuadFarVS,
				CopyPS,
				CopyPSMS,
				ShadowMapVSPS,
				DepthWriteVSPS,
				CloudDepthWriteVSPS,
				
				SampleLumInitialPS,
				SampleLumIterativePS,
				SampleLumFinalPS,
				CalcAdaptedLumPS,
				ToneMappingPS,
				BrightPassPS,
				BloomPS,
				Blur5x5PS,
				StarGlarePS,
				MergeTextures2PS,

				GodRayPS,
				OcclusionPrePassVSPS,
				OcclusionPrePassVSGSPS,

				GlowPS,
				SilouettePS,

				GGXGenPS,

				Last = GGXGenPS,



			};
		}

		namespace Materials{
			enum Enum{
				Missing,
				Quad,
				QuadTextured,
				BillboardQuad,

				Last = BillboardQuad,
			};
		}

		namespace RasterizerStates{
			enum Enum{
				Default,
				CullFrontFace,
				OneBiased,
				WireFrame,			

				Last = WireFrame,
			};
		}

		namespace BlendStates{
			enum Enum{
				Default,
				Additive,
				AlphaBlend,
				MaxBlend,				
				RedAlphaMask,
				GreenAlphaMask,
				GreenAlphaMaskMax,
				RedAlphaMaskAddMinus,
				GreenAlphaMaskAddAdd,
				RedAlphaMaskAddAdd,
				GreenAlphaMaskAddMinus,
				GreenMask,
				BlueMask,
				NoColorWrite,
			};
		}

		namespace DepthStencilStates{
			enum Enum{
				Default,
				NoDepthStencil,
				NoDepthWrite_LessEqual,
				LessEqual,
			};
		}

		namespace SamplerStates{
			enum Enum{
				Point,
				Linear,
				Anisotropic,
				Shadow,
				PointWrap,
				LinearWrap,
				BlackBorder,
				PointBlackBorder,
			};
		}
	}
}