/*
*
*	Megan Grass
*	July 10, 2025
*
*/

#include "app.h"

void Global_Application::DrawBackground(void) const
{
	if (!Camera->b_ViewTopDown && !Camera->b_ViewModelEdit && Camera->b_ViewBackground && Camera->m_Background)
	{
		Render->SetDepthScale(1.0f, 1.0f);

		Render->AlphaBlending(FALSE, D3DBLEND_ZERO, D3DBLEND_ZERO);

		Render->Draw({ Camera->m_BackgroundVert.get(), nullptr, Camera->m_Background.get(), Render->PassthroughPixelShader.get(),
			{ TRUE, D3DZB_TRUE, D3DCMP_LESSEQUAL },
			{ sizeof(vec4t), D3DFILL_SOLID, D3DPT_TRIANGLESTRIP },
			{ Camera->m_TexWidth, Camera->m_TexHeight },
			{ nullptr, nullptr, (D3DXMATRIX*)Camera->Orthogonal.get() } });
	}
}

void Global_Application::DrawCamera(void)
{
	if (!Camera->b_DrawLine || Camera->b_ViewModelEdit || !IsRoomOpen()) { return; }

	std::vector<vec3> Shape;

	std::int32_t CrossSize = 900;
	std::int32_t ArrowSize = 900;

	vec3 Delta = vec3(GTE->ToFloat(Camera->m_At.x - Camera->m_Eye.x), 0.0f, GTE->ToFloat(Camera->m_At.z - Camera->m_Eye.z));
	vec3 DeltaNormal = vec3(Delta.x, Delta.y, Delta.z).Normalize();

	vec3 Perpendicular0 = vec3(-DeltaNormal.z, 0.0f, DeltaNormal.x).Normalize() * (GTE->ToFloat(ArrowSize) * 0.5f);
	vec3 Perpendicular1 = vec3(DeltaNormal.z, 0.0f, -DeltaNormal.x).Normalize() * (GTE->ToFloat(ArrowSize) * 0.5f);

	vec3 Arrow = vec3(
		GTE->ToFloat(Camera->m_At.x) - DeltaNormal.x * GTE->ToFloat(ArrowSize),
		GTE->ToFloat(Camera->m_At.y) - DeltaNormal.y * GTE->ToFloat(ArrowSize),
		GTE->ToFloat(Camera->m_At.z) - DeltaNormal.z * GTE->ToFloat(ArrowSize));

	{
		Shape.push_back(vec3(GTE->ToFloat(Camera->m_Eye.x - CrossSize), 0.0f, GTE->ToFloat(Camera->m_Eye.z)));
		Shape.push_back(vec3(GTE->ToFloat(Camera->m_Eye.x + CrossSize), 0.0f, GTE->ToFloat(Camera->m_Eye.z)));
		Shape.push_back(vec3(GTE->ToFloat(Camera->m_Eye.x), 0.0f, GTE->ToFloat(Camera->m_Eye.z)));
		Shape.push_back(vec3(GTE->ToFloat(Camera->m_Eye.x), 0.0f, GTE->ToFloat(Camera->m_Eye.z)));
		Shape.push_back(vec3(GTE->ToFloat(Camera->m_Eye.x), 0.0f, GTE->ToFloat(Camera->m_Eye.z - CrossSize)));
		Shape.push_back(vec3(GTE->ToFloat(Camera->m_Eye.x), 0.0f, GTE->ToFloat(Camera->m_Eye.z + CrossSize)));
	}

	{
		Shape.push_back(vec3(GTE->ToFloat(Camera->m_Eye.x), 0.0f, GTE->ToFloat(Camera->m_Eye.z)));
		Shape.push_back(vec3(GTE->ToFloat(Camera->m_At.x), 0.0f, GTE->ToFloat(Camera->m_At.z)));
	}

	{
		Shape.push_back(vec3(GTE->ToFloat(Camera->m_At.x), 0.0f, GTE->ToFloat(Camera->m_At.z)));
		Shape.push_back(vec3(Arrow.x + Perpendicular0.x, 0.0f, Arrow.z + Perpendicular0.z));
		Shape.push_back(vec3(GTE->ToFloat(Camera->m_At.x), 0.0f, GTE->ToFloat(Camera->m_At.z)));
		Shape.push_back(vec3(Arrow.x + Perpendicular1.x, 0.0f, Arrow.z + Perpendicular1.z));
	}

	std::unique_ptr<IDirect3DVertexBuffer9, IDirect3DDelete9<IDirect3DVertexBuffer9>> Vertices;
	Vertices.reset(Render->CreateVec3c(Shape, 0x00FF0000));

	Render->SetWorld(Standard_Matrix());

	Render->DrawVec3c(Vertices.get(), nullptr, nullptr, D3DFILL_WIREFRAME, D3DPT_LINELIST);

	Render->ResetWorld();
}

void Global_Application::DrawSprite(void)
{
	if (Camera->b_ViewTopDown || Camera->b_ViewModelEdit || !Camera->b_ViewSprite || !IsRoomOpen()) { return; }

	{
		Render->SetDepthScale(32.0f, 16384.0f);

		Render->AlphaBlending(TRUE, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA);
	}

	if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2))
	{
		for (std::size_t i = 0; i < Bio2->Rdt->Pri[Camera->Cut]->Count(); i++)
		{
			for (std::size_t x = 0; x < Bio2->Rdt->Pri[Camera->Cut]->Get(i)->Sprite.size(); x++)
			{
				auto& Sprite = Bio2->Rdt->Pri[Camera->Cut]->Get(i)->Sprite[x];
				auto& Layer = Bio2->Rdt->Pri[Camera->Cut]->Get(i)->Layer;

				float X = ((float)Sprite.x + (float)Layer.OfsX) * Camera->m_OrthoScaleX;
				float Y = ((float)Sprite.y + (float)Layer.OfsY) * Camera->m_OrthoScaleY;
				float W = (float)Sprite.w * Camera->m_OrthoScaleX;
				float H = (float)Sprite.h * Camera->m_OrthoScaleY;

				// adjust for direct-x 9 half texel
				X -= 0.5f;
				Y += 0.5f;

				float OffsetU = 0.5f / Camera->m_TexSprWidth;
				float OffsetV = 0.5f / Camera->m_TexSprHeight;

				float U0 = ((float)Sprite.u + OffsetU) / Camera->m_TexSprWidth;
				float V0 = ((float)Sprite.v + OffsetV) / Camera->m_TexSprHeight;
				float U1 = ((float)Sprite.u + (float)Sprite.w - OffsetU) / Camera->m_TexSprWidth;
				float V1 = ((float)Sprite.v + (float)Sprite.h - OffsetV) / Camera->m_TexSprHeight;

				float Z = (float)Sprite.otz;

				std::vector<vec4t> Vector = {
					{ vec4{ X,		Y + H,	Z, 1.0f }, vec2{ U0, V1 } },
					{ vec4{ X,		Y,		Z, 1.0f }, vec2{ U0, V0 } },
					{ vec4{ X + W,	Y + H,	Z, 1.0f }, vec2{ U1, V1 } },
					{ vec4{ X + W,	Y,		Z, 1.0f }, vec2{ U1, V0 } } };

				std::unique_ptr<IDirect3DVertexBuffer9, IDirect3DDelete9<IDirect3DVertexBuffer9>> Vertices;
				Vertices.reset(Render->CreateVec4t(Vector));

				Render->Draw({ Vertices.get(), nullptr, Camera->m_Sprite.get(), Render->PassthroughPixelShader.get(),
					{ TRUE, D3DZB_TRUE, D3DCMP_LESSEQUAL },
					{ sizeof(vec4t), D3DFILL_SOLID, D3DPT_TRIANGLESTRIP },
					{ Camera->m_TexSprWidth, Camera->m_TexSprHeight },
					{ nullptr, nullptr, (D3DXMATRIX*)Camera->Orthogonal.get() } });
			}
		}
	}
}

void Global_Application::DrawSwitch(void)
{
	if (!Camera->b_DrawSwitch || Camera->b_ViewModelEdit || !IsRoomOpen()) { return; }

	if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2))
	{
		for (std::size_t i = 0; i < Bio2->Rdt->Rvd->Count(); i++)
		{
			if (Bio2->Rdt->Rvd->Get(i)->Fcut == Camera->Cut)
			{
				Geometry->Draw4p(Bio2->Rdt->Rvd->Get(i)->Xz, 0, D3DCOLOR_ARGB(255, 0, 197, 0));
			}
		}
	}
}

void Global_Application::DrawCollision(void)
{
	if (!Geometry->b_DrawCollision || Camera->b_ViewModelEdit || !IsRoomOpen()) { return; }

	if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2))
	{
		for (std::size_t i = 0; i < Bio2->Rdt->Sca->Count(); i++)
		{
			std::int16_t XX = Bio2->Rdt->Sca->Get(i)->X;
			std::int16_t ZZ = Bio2->Rdt->Sca->Get(i)->Z;
			std::uint16_t WW = Bio2->Rdt->Sca->Get(i)->W;
			std::uint16_t DD = Bio2->Rdt->Sca->Get(i)->D;

			std::int32_t Ground = Bio2->Rdt->Sca->GetLow(i);
			std::int32_t High = Bio2->Rdt->Sca->GetHigh(i);

			if (Ground < High) { Ground = High; }

			bool b_Solid = false;

			bool b_Shape = true;

			if (!b_Shape) { High = Ground; }

			switch (Bio2->Rdt->Sca->Get(i)->Id.Bits.Shape)
			{
			case std::to_underlying(Resident_Evil_2_Collision_Shape::Box):
				Geometry->DrawBox({ XX, Ground, ZZ, WW, High, DD }, { }, 0x00C5C5C5, b_Solid);
				break;
			case std::to_underlying(Resident_Evil_2_Collision_Shape::Naname_a):
				Geometry->DrawTriangle({ XX, Ground, ZZ, WW, High, DD }, { }, 0x00C5C5C5, b_Solid, Resident_Evil_Triangle_Type::Naname_A);
				break;
			case std::to_underlying(Resident_Evil_2_Collision_Shape::Naname_b):
				Geometry->DrawTriangle({ XX, Ground, ZZ, WW, High, DD }, { }, 0x00C5C5C5, b_Solid, Resident_Evil_Triangle_Type::Naname_B);
				break;
			case std::to_underlying(Resident_Evil_2_Collision_Shape::Naname_c):
				Geometry->DrawTriangle({ XX, Ground, ZZ, WW, High, DD }, { }, 0x00C5C5C5, b_Solid, Resident_Evil_Triangle_Type::Naname_C);
				break;
			case std::to_underlying(Resident_Evil_2_Collision_Shape::Naname_d):
				Geometry->DrawTriangle({ XX, Ground, ZZ, WW, High, DD }, { }, 0x00C5C5C5, b_Solid, Resident_Evil_Triangle_Type::Naname_D);
				break;
			case std::to_underlying(Resident_Evil_2_Collision_Shape::Hishi):
				Geometry->DrawRhombus({ XX, Ground, ZZ, WW, High, DD }, { }, 0x00C5C5C5, b_Solid);
				break;
			case std::to_underlying(Resident_Evil_2_Collision_Shape::Circle):
			case std::to_underlying(Resident_Evil_2_Collision_Shape::Koban_x):
			case std::to_underlying(Resident_Evil_2_Collision_Shape::Koban_z):
				Geometry->DrawCylinder({ XX, Ground, ZZ, WW, High, DD }, { }, 0x00C5C5C5, b_Solid);
				break;
			case std::to_underlying(Resident_Evil_2_Collision_Shape::Slope):
				switch (Bio2->Rdt->Sca->GetSlopeHypotenuse(i))
				{
				case Resident_Evil_2_Slope_Hypotenuse::Type_A:
					Geometry->DrawTriangle({ XX, Ground, ZZ, WW, High, DD }, { }, 0x00C5C5C5, b_Solid, Resident_Evil_Triangle_Type::Slope_A);
					break;
				case Resident_Evil_2_Slope_Hypotenuse::Type_B:
					Geometry->DrawTriangle({ XX, Ground, ZZ, WW, High, DD }, { }, 0x00C5C5C5, b_Solid, Resident_Evil_Triangle_Type::Slope_B);
					break;
				case Resident_Evil_2_Slope_Hypotenuse::Type_C:
					Geometry->DrawTriangle({ XX, Ground, ZZ, WW, High, DD }, { }, 0x00C5C5C5, b_Solid, Resident_Evil_Triangle_Type::Slope_C);
					break;
				case Resident_Evil_2_Slope_Hypotenuse::Type_D:
					Geometry->DrawTriangle({ XX, Ground, ZZ, WW, High, DD }, { }, 0x00C5C5C5, b_Solid, Resident_Evil_Triangle_Type::Slope_D);
					break;
				}
				break;
			case std::to_underlying(Resident_Evil_2_Collision_Shape::Box_3):
				High = Bio2->Rdt->Sca->Get(i)->Type.Bits.nFloor * -1800;

				switch (Bio2->Rdt->Sca->GetSlopeHypotenuse(i))
				{
				case Resident_Evil_2_Slope_Hypotenuse::Type_A:
					Geometry->DrawTriangle({ XX, Ground, ZZ, WW, High, DD }, { }, 0x00C5C5C5, b_Solid, Resident_Evil_Triangle_Type::Slope_A | Resident_Evil_Triangle_Type::Stairs);
					break;
				case Resident_Evil_2_Slope_Hypotenuse::Type_B:
					Geometry->DrawTriangle({ XX, Ground, ZZ, WW, High, DD }, { }, 0x00C5C5C5, b_Solid, Resident_Evil_Triangle_Type::Slope_B | Resident_Evil_Triangle_Type::Stairs);
					break;
				case Resident_Evil_2_Slope_Hypotenuse::Type_C:
					Geometry->DrawTriangle({ XX, Ground, ZZ, WW, High, DD }, { }, 0x00C5C5C5, b_Solid, Resident_Evil_Triangle_Type::Slope_C | Resident_Evil_Triangle_Type::Stairs);
					break;
				case Resident_Evil_2_Slope_Hypotenuse::Type_D:
					Geometry->DrawTriangle({ XX, Ground, ZZ, WW, High, DD }, { }, 0x00C5C5C5, b_Solid, Resident_Evil_Triangle_Type::Slope_D | Resident_Evil_Triangle_Type::Stairs);
					break;
				}
				break;
			}
		}
	}
}

void Global_Application::DrawBlock(void)
{
	if (!Geometry->b_DrawBlock || Camera->b_ViewModelEdit || !IsRoomOpen()) { return; }

	if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2))
	{
		for (std::size_t i = 0; i < Bio2->Rdt->Blk->Count(); i++)
		{
			std::int16_t WW = Bio2->Rdt->Blk->Get(i)->Size_x - Bio2->Rdt->Blk->Get(i)->Pos_x;
			std::int16_t DD = Bio2->Rdt->Blk->Get(i)->Size_z - Bio2->Rdt->Blk->Get(i)->Pos_z;

			Geometry->DrawBox({ Bio2->Rdt->Blk->Get(i)->Pos_x, 0, Bio2->Rdt->Blk->Get(i)->Pos_z, WW, 0, DD }, { }, 0x00FFFF00, false);
		}
	}
}

void Global_Application::DrawFloor(void)
{
	if (!Geometry->b_DrawFloor || Camera->b_ViewModelEdit || !IsRoomOpen()) { return; }

	if (GameType() & (BIO2NOV96 | BIO2TRIAL | BIO2))
	{
		for (std::size_t i = 0; i < Bio2->Rdt->Flr->Count(); i++)
		{
			std::int16_t XX = Bio2->Rdt->Flr->Get(i)->X;
			std::int16_t ZZ = Bio2->Rdt->Flr->Get(i)->Z;
			std::uint16_t WW = Bio2->Rdt->Flr->Get(i)->W;
			std::uint16_t DD = Bio2->Rdt->Flr->Get(i)->D;

			Geometry->DrawBox({ XX, 0, ZZ, WW, 0, DD }, { }, 0x0000FFFF, false);
		}
	}
}