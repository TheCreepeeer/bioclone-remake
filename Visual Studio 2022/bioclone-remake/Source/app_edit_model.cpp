/*
*
*	Megan Grass
*	July 26, 2025
*
*/

#include "app.h"

void Global_Application::ModelEditor(void)
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File##ModelFileMenu"))
		{
			if (ImGui::MenuItem("Open##ModelFileMenu"))
			{
				OpenModel();
			}
			if (ImGui::MenuItem("Save As##ModelFileMenu"))
			{
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Close##ModelFileMenu"))
			{
				Player->Close();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Assemble##ModelFileMenu"))
			{
			}
			if (ImGui::MenuItem("Disassemble##ModelFileMenu"))
			{
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Texture##ModelTextureMenu"))
		{
			if (ImGui::MenuItem("Open##ModelTextureMenu"))
			{
				OpenModelTexture();
			}
			if (ImGui::MenuItem("Save As##ModelTextureMenu"))
			{
				SaveModelTexture();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Object##ModelObjectMenu"))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::CollapsingHeader("File##ModelEditor", ImGuiTreeNodeFlags_None))
	{
		bool b_Bio1Alpha = Player->GameType() & (AUG95 | OCT95);
		bool b_Bio1 = Player->GameType() & (BIO1);
		bool b_Bio2Nov96 = Player->GameType() & (BIO2NOV96);
		bool b_Bio2 = Player->GameType() & (BIO2);
		bool b_Bio3 = Player->GameType() & (BIO3);

		bool b_Bio1Enemy = false;

		if (ImGui::BeginTable("Table##ModelType", 2))
		{
			TooltipOnHover("Game Type on Open/Save");
			ImGui::TableNextColumn();
			if (ImGui::Checkbox(" Bio1 Alpha##ModelType", &b_Bio1Alpha)) { Player->SetGame(Video_Game::Resident_Evil_Aug_4_1995); }
			TooltipOnHover("Resident Evil (Aug/Oct 1995) Prototype");
			ImGui::TableNextColumn();
			if (ImGui::Checkbox(" Bio1 Enemy##ModelType", &b_Bio1Enemy)) {  }
			TooltipOnHover("Bio1 EMD is enemy type on Open\r\nAssume player type otherwise");
			ImGui::TableNextColumn();
			if (ImGui::Checkbox(" Bio1##ModelType", &b_Bio1)) { Player->SetGame(Video_Game::Resident_Evil); }
			TooltipOnHover("Resident Evil");
			if (ImGui::Checkbox(" Bio2 Nov '96##ModelType", &b_Bio2Nov96)) { Player->SetGame(Video_Game::Resident_Evil_2_Nov_6_1996); }
			TooltipOnHover("Resident Evil 2 (Nov 1996) Prototype");
			if (ImGui::Checkbox(" Bio2##ModelType", &b_Bio2)) { Player->SetGame(Video_Game::Resident_Evil_2); }
			TooltipOnHover("Resident Evil 2");
			if (ImGui::Checkbox(" Bio3##ModelType", &b_Bio3)) { Player->SetGame(Video_Game::Resident_Evil_3); }
			TooltipOnHover("Resident Evil 3");
			ImGui::EndTable();
		}
	}

	if (ImGui::CollapsingHeader("Render##ModelEditor", ImGuiTreeNodeFlags_None))
	{
		DrawVerticalLine(8.0f, 12.0f, 2.0f, m_BorderRed, m_BorderGreen, m_BorderBlue);

		ImGui::MenuItem(" Dither##ModelRender", NULL, &Player->b_Dither);
		TooltipOnHover("Sony PlayStation (1994) Dithering Pixel Shader");

		DrawVerticalLine(8.0f, 12.0f, 2.0f, m_BorderRed, m_BorderGreen, m_BorderBlue);

		bool b_NoFilter = Player->m_TextureFilter == D3DTEXF_NONE || Player->m_TextureFilter == D3DTEXF_POINT;
		bool b_LinearFilter = Player->m_TextureFilter == D3DTEXF_LINEAR;
		bool b_AnisotropicFilter = Player->m_TextureFilter == D3DTEXF_ANISOTROPIC;

		if (ImGui::MenuItem(" Point##ModelRender", NULL, &b_NoFilter)) { Player->m_TextureFilter = D3DTEXF_POINT; }
		TooltipOnHover("Texture Filter");

		if (ImGui::MenuItem(" Linear##ModelRender", NULL, &b_LinearFilter)) { Player->m_TextureFilter = D3DTEXF_LINEAR; }
		TooltipOnHover("Texture Filter");

		if (ImGui::MenuItem(" Anisotropic##ModelRender", NULL, &b_AnisotropicFilter)) { Player->m_TextureFilter = D3DTEXF_ANISOTROPIC; }
		TooltipOnHover("Texture Filter");

		DrawVerticalLine(8.0f, 12.0f, 2.0f, m_BorderRed, m_BorderGreen, m_BorderBlue);

		if (ImGui::MenuItem(" Wireframe##ModelRender", NULL, &Player->b_DrawWireframe))
		{
			Player->b_DrawWireframe = true;
			Player->b_DrawSolidColor = false;
			Player->b_DrawTextured = false;
		}
		TooltipOnHover("Polygons will be drawn as wireframe");

		if (ImGui::MenuItem(" Solid##ModelRender", NULL, &Player->b_DrawSolidColor))
		{
			Player->b_DrawWireframe = false;
			Player->b_DrawSolidColor = true;
			Player->b_DrawTextured = false;
		}
		TooltipOnHover("Polygons will be drawn as solid color");

		if (ImGui::MenuItem(" Textured##ModelRender", NULL, &Player->b_DrawTextured))
		{
			Player->b_DrawWireframe = false;
			Player->b_DrawSolidColor = false;
			Player->b_DrawTextured = true;
		}
		TooltipOnHover("Polygons will be drawn as textured");

		ImGui::MenuItem(" Skeleton##ModelRender", NULL, &Player->b_DrawSkeleton);
		TooltipOnHover("Skeleton mesh will be drawn");

		DrawVerticalLine(8.0f, 12.0f, 2.0f, m_BorderRed, m_BorderGreen, m_BorderBlue);
	}

	if (ImGui::CollapsingHeader("Transform##ModelEditor", ImGuiTreeNodeFlags_None))
	{
		TooltipOnHover("World Position and Rotation");
		if (ImGui::BeginTable("Transform##ModelTransform", 4))
		{
			ImGui::TableSetupColumn("Label##ModelTransform", ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("____").x);
			ImGui::TableSetupColumn("X##ModelTransform");
			ImGui::TableSetupColumn("Y##ModelTransform");
			ImGui::TableSetupColumn("Z##ModelTransform");
			ImGui::TableNextRow();

			{
				ImGui::TableNextColumn(); ImGui::Text(" Pos");

				ImGui::TableNextColumn(); ImGui::SetNextItemWidth(ImGui::CalcTextSize("_______").x);
				if (ImGui::InputScalar("##ModelTransformPosX", ImGuiDataType_S32, &Player->EditorPosition().x))
				{
					Player->EditorPosition().x = std::clamp(Player->EditorPosition().x, -32767, 32767);
				}
				ScrollOnHover(&Player->EditorPosition().x, ImGuiDataType_S32, 32, -32767, 32767);
				TooltipOnHover("Position X");

				ImGui::TableNextColumn(); ImGui::SetNextItemWidth(ImGui::CalcTextSize("_______").x);
				if (ImGui::InputScalar("##ModelTransformPosY", ImGuiDataType_S32, &Player->EditorPosition().y))
				{
					Player->EditorPosition().y = std::clamp(Player->EditorPosition().y, -32767, 32767);
				}
				ScrollOnHover(&Player->EditorPosition().y, ImGuiDataType_S32, 32, -32767, 32767);
				TooltipOnHover("Position Y");

				ImGui::TableNextColumn(); ImGui::SetNextItemWidth(ImGui::CalcTextSize("_______").x);
				if (ImGui::InputScalar("##ModelTransformPosZ", ImGuiDataType_S32, &Player->EditorPosition().z))
				{
					Player->EditorPosition().z = std::clamp(Player->EditorPosition().z, -32767, 32767);
				}
				ScrollOnHover(&Player->EditorPosition().z, ImGuiDataType_S32, 32, -32767, 32767);
				TooltipOnHover("Position Z");
			}

			{
				ImGui::TableNextColumn(); ImGui::Text(" Rot");

				ImGui::TableNextColumn(); ImGui::SetNextItemWidth(ImGui::CalcTextSize("_______").x);
				if (ImGui::InputScalar("##ModelTransformRotX", ImGuiDataType_S32, &Player->EditorRotation().x))
				{
					Player->EditorRotation().x = std::clamp(Player->EditorRotation().x, -4096, 4096);
				}
				ScrollOnHover(&Player->EditorRotation().x, ImGuiDataType_S32, 32, -4096, 4096);
				TooltipOnHover("Rotation X");

				ImGui::TableNextColumn(); ImGui::SetNextItemWidth(ImGui::CalcTextSize("_______").x);
				if (ImGui::InputScalar("##ModelTransformRotY", ImGuiDataType_S32, &Player->EditorRotation().y))
				{
					Player->EditorRotation().y = std::clamp(Player->EditorRotation().y, -4096, 4096);
				}
				ScrollOnHover(&Player->EditorRotation().y, ImGuiDataType_S32, 32, -4096, 4096);
				TooltipOnHover("Rotation Y");

				ImGui::TableNextColumn(); ImGui::SetNextItemWidth(ImGui::CalcTextSize("_______").x);
				if (ImGui::InputScalar("##ModelTransformRotZ", ImGuiDataType_S32, &Player->EditorRotation().z))
				{
					Player->EditorRotation().z = std::clamp(Player->EditorRotation().z, -4096, 4096);
				}
				ScrollOnHover(&Player->EditorRotation().z, ImGuiDataType_S32, 32, -4096, 4096);
				TooltipOnHover("Rotation Z");
			}

			{
				ImGui::TableNextColumn(); ImGui::Text(" Sx");

				ImGui::TableNextColumn(); ImGui::SetNextItemWidth(ImGui::CalcTextSize("_______").x);
				if (ImGui::InputScalar("##ModelTransformScaleX", ImGuiDataType_S32, &Player->EditorScale().x))
				{
					Player->EditorScale().x = std::clamp(Player->EditorScale().x, 0, 32768);
				}
				ScrollOnHover(&Player->EditorScale().x, ImGuiDataType_S32, 512, 0, 32768);
				TooltipOnHover("Scale X");

				ImGui::TableNextColumn(); ImGui::SetNextItemWidth(ImGui::CalcTextSize("_______").x);
				if (ImGui::InputScalar("##ModelTransformScaleY", ImGuiDataType_S32, &Player->EditorScale().y))
				{
					Player->EditorScale().y = std::clamp(Player->EditorScale().y, 0, 32768);
				}
				ScrollOnHover(&Player->EditorScale().y, ImGuiDataType_S32, 512, 0, 32768);
				TooltipOnHover("Scale Y");

				ImGui::TableNextColumn(); ImGui::SetNextItemWidth(ImGui::CalcTextSize("_______").x);
				if (ImGui::InputScalar("##ModelTransformScaleZ", ImGuiDataType_S32, &Player->EditorScale().z))
				{
					Player->EditorScale().z = std::clamp(Player->EditorScale().z, 0, 32768);
				}
				ScrollOnHover(&Player->EditorScale().z, ImGuiDataType_S32, 512, 0, 32768);
				TooltipOnHover("Scale Z");
			}

			ImGui::EndTable();
		}
	}

	if (ImGui::CollapsingHeader("Index##ModelEditor", ImGuiTreeNodeFlags_None))
	{
		bool b_Normal = Player->AnimIndex() == AnimationIndex::Normal;
		bool b_NormalEx0 = Player->AnimIndex() == AnimationIndex::NormalEx0;
		bool b_NormalEx1 = Player->AnimIndex() == AnimationIndex::NormalEx1;
		bool b_Damage = Player->AnimIndex() == AnimationIndex::Damage;
		bool b_Weapon = Player->AnimIndex() == AnimationIndex::Weapon;
		bool b_WeaponEx0 = Player->AnimIndex() == AnimationIndex::WeaponEx0;
		bool b_WeaponEx1 = Player->AnimIndex() == AnimationIndex::WeaponEx1;
		bool b_Room = Player->AnimIndex() == AnimationIndex::Room;

		if (ImGui::MenuItem(" Normal##ModelAnimation", NULL, &b_Normal)) { Player->iFrame = 0; Player->AnimIndex() = AnimationIndex::Normal; }
		TooltipOnHover("Normal Animation");
		if (ImGui::MenuItem(" Normal Ex0##ModelAnimation", NULL, &b_NormalEx0)) { Player->iFrame = 0; Player->AnimIndex() = AnimationIndex::NormalEx0; }
		TooltipOnHover("Normal Animation Extra\r\nEMD (Bio2/Bio3)");
		if (ImGui::MenuItem(" Normal Ex1##ModelAnimation", NULL, &b_NormalEx1)) { Player->iFrame = 0; Player->AnimIndex() = AnimationIndex::NormalEx1; }
		TooltipOnHover("Normal Animation Extra\r\nEMD (Bio3)");
		if (ImGui::MenuItem(" Damage##ModelAnimation", NULL, &b_Damage)) { Player->iFrame = 0; Player->AnimIndex() = AnimationIndex::Damage; }
		TooltipOnHover("Damage Animation\r\nEMD, for use with Player");
		if (ImGui::MenuItem(" Weapon##ModelAnimation", NULL, &b_Weapon)) { Player->iFrame = 0; Player->AnimIndex() = AnimationIndex::Weapon; }
		TooltipOnHover("Weapon Animation\r\nEMW (Bio1) or PLW (Bio2/Bio3)");
		if (ImGui::MenuItem(" Weapon Ex0##ModelAnimation", NULL, &b_WeaponEx0)) { Player->iFrame = 0; Player->AnimIndex() = AnimationIndex::WeaponEx0; }
		TooltipOnHover("Weapon Animation Ex0\r\nPLW/EMD (Bio3), unknown purpose");
		if (ImGui::MenuItem(" Weapon Ex1##ModelAnimation", NULL, &b_WeaponEx1)) { Player->iFrame = 0; Player->AnimIndex() = AnimationIndex::WeaponEx1; }
		TooltipOnHover("Weapon Animation Ex1\r\nPLW/EMD (Bio3)");
		if (ImGui::MenuItem(" Room##ModelAnimation", NULL, &b_Room)) { Player->iFrame = 0; Player->AnimIndex() = AnimationIndex::Room; }
		TooltipOnHover("Room Animation\r\nRDT");
	}
}