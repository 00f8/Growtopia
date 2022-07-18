
#include <enet.h>

#include "ENetClient.h"
#include "wrapper/Gui.h"
#include "wrapper/FileBrowser.h"
#include "wrapper/TextEditor.h"
#include "wrapper/HTTPRequest.hpp"

#include "Utils.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <Windows.h>
#include <string>
#pragma comment(lib, "urlmon.lib")
 
// PUBLIC CONFIG
std::vector<ENetClient*> bots;

int tab = 1;

int botselected;

std::string version = "1.0";
std::string linkversion = "https://raw.githubusercontent.com/IniTyoo/umbrlla-config/main/version";

//



using namespace std;

#ifdef _CONSOLE
int main() {
#else
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif
	
	int eStatus = enet_initialize();
#ifdef _CONSOLE
	std::cout << "enet initialize status: " << eStatus << std::endl;
	if (eStatus < 0) {
		system("pause");
		return 0;
	}
#endif
	std::string itemsdats = "C:\\items.dat";
	std::ifstream itemdat(itemsdats.c_str());

	if (!itemdat) {
		LPCTSTR url = TEXT("https://raw.githubusercontent.com/IniTyoo/umbrlla-config/main/Updater.exe");
		LPCTSTR fPath = TEXT("C:\\items.dat");
		HRESULT hr = URLDownloadToFile (NULL, url, fPath, 0, NULL);
		if (!FAILED(hr))
		{
			itemDefs->LoadFromFile();
		}
	}else{
		itemDefs->LoadFromFile();
	}
	
	
	
	Gui::CreateHWindow(L"main window");
	if (Gui::CreateDevice()) {
		Gui::CreateImGui();
		
		std::vector<ENetClient*> cachedBots; // pending to remove
		bool popup_open = false;
		
		
		ImGui::FileBrowser m_browser;
		m_browser.SetWindowPos(0, 0);
		m_browser.SetTitle("Browse script");
		m_browser.SetTypeFilters({".lua"});
		
		TextEditor m_editor;
		Api::INIT_EDITOR(&m_editor);
		m_editor.SetPalette(TextEditor::GetDarkPalette());
		ImGui::GetStyle().WindowRounding = 4;
		ImGui::GetStyle().FrameRounding = 4;
		ImGui::GetStyle().PopupRounding = 4;
		ImGui::GetStyle().ScrollbarRounding = 6;
		ImGui::GetStyle().GrabRounding = 4;
		ImGui::GetStyle().TabRounding = 4;
		
		//Font
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		ImFont* font;
		font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 20);

		//Style
		ImVec4* colors = ImGui::GetStyle().Colors;
		ImGuiStyle& style = ImGui::GetStyle();
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.73f, 0.75f, 0.74f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.44f, 0.44f, 0.60f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.57f, 0.57f, 0.57f, 0.70f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.76f, 0.76f, 0.76f, 0.80f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.65f, 0.55f, 0.40f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.13f, 0.75f, 0.55f, 0.50f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
		colors[ImGuiCol_Button] = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
		colors[ImGuiCol_Header] = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
		colors[ImGuiCol_Separator] = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
		colors[ImGuiCol_Tab] = ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.80f);
		colors[ImGuiCol_TabActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.36f, 0.36f, 0.36f, 0.54f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.13f, 0.13f, 0.13f, 0.80f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		
		while (Gui::Instance) {
			if (Gui::BeginRender())
				continue;
			
			ImGui::SetNextWindowPos(ImVec2(120, 120), ImGuiCond_::ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(650 + 100, 415 + 70), ImGuiCond_::ImGuiCond_Once);
			DWORD window_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;
			ImGui::SetNextWindowCollapsed(false, ImGuiCond_::ImGuiCond_Once);
			static ENetClient* bot = NULL;
			
			if (ImGui::Begin("MultiBot V.1", &Gui::Instance, window_flags)) {
				//ImGui::BeginTabBar("tb");
				static ImVec4 active = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
				static ImVec4 inactive = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);

				ImGui::PushStyleColor(ImGuiCol_Button, tab == 1 ? active : inactive);
				if (ImGui::Button("Bots", ImVec2(ImGui::GetWindowSize().x / 3 - 11, 40))) {
					tab = 1;
				}
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Button, tab == 2 ? active : inactive);
				if (ImGui::Button("Executor", ImVec2(ImGui::GetWindowSize().x / 3 - 11, 40))) {
					tab = 2;
				}
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Button, tab == 3 ? active : inactive);
				if (ImGui::Button("Item DB", ImVec2(ImGui::GetWindowSize().x / 3 - 11, 40))) {
					tab = 3;
				}
				ImGui::PopStyleColor(3);

				//Bots
				if (tab == 1) {
					
					
					ImGui::BeginChild("ls", ImVec2(200, 345 + 45), false);
					static float availx = ImGui::GetContentRegionAvail().x;
					
					if (ImGui::BeginListBox("##lbls", ImVec2(availx, 325.f))) {
						for (uint32_t i = 0; i < bots.size(); i++) {
							ImGui::PushID(i);
							if (botselected > 0){
								bot = bots[botselected];
							}
							if (ImGui::Selectable(bots[i]->data.name.empty() ? "[Guest Acccount]" : bots[i]->data.name.c_str(), bot == bots[i])){
								bot = bots[i];
								botselected = i;
								}
							ImGui::PopID();
						}
						ImGui::EndListBox();
					}
					
					if (ImGui::Button("Add", ImVec2(availx, 0)))
						popup_open = true;
					
					if (ImGui::Button("Remove", ImVec2(availx, 0))) {
						if (bot) {
							for (uint32_t i = 0; i < bots.size(); i++) {
								if (bots[i] == bot) {
									bots.erase(bots.begin() + i);
									cachedBots.push_back(bot);
									break;
								}
							}
							bot = NULL;
						}
					}
					
					ImGui::EndChild();
					ImGui::SameLine();
					
					ImGui::BeginChild("rs", ImVec2(420 + 100, 345 + 45), false);
					if (bot){
						bool connected = true;
						ImGui::BeginTabBar("tbm");
						if (ImGui::BeginTabItem("Info##1")) {
							ImGui::TextUnformatted("Selected:");
							ImGui::SameLine();
							if (bot->data.name.empty()){
							ImGui::TextUnformatted("[Guest Acccount]");
							}else{
							ImGui::TextUnformatted(bot->data.name.c_str());
							}
							ImGui::TextUnformatted("Status:");
							ImGui::SameLine();
							ImVec4 backup_color = style.Colors[ImGuiCol_Text];
							switch (bot->GetState()) {
								case 0: {
									style.Colors[ImGuiCol_Text] = ImVec4(1, 0, 0, 1);
									ImGui::TextUnformatted("Disconnected");
									connected = false;
								} break;
								case 1: {
									style.Colors[ImGuiCol_Text] = ImVec4(1, 1, 0, 1);
									ImGui::TextUnformatted("Connecting");
									connected = false;
								} break;
								case 5: {
									style.Colors[ImGuiCol_Text] = ImVec4(0, 1, 0, 1);
									ImGui::TextUnformatted("Connected");
									connected = true;
								} break;
								case 7: {
									style.Colors[ImGuiCol_Text] = ImVec4(1, 0.3f, 0, 1);
									ImGui::TextUnformatted("Disconnecting");
									connected = false;
								} break;
								default: {
									ImGui::TextUnformatted("Unknown status");
									connected = false;
								} break;
							}
							style.Colors[ImGuiCol_Text] = backup_color;
							ImGui::SameLine();
							ImGui::Text("Ping: %d", bot->GetPing());
							if (connected) {
								ImGui::Text("Current world: %s", bot->local.worldname.c_str());
								ImGui::Text("Server: %s:%d", bot->data.ip.c_str(), bot->data.port);
								ImGui::Text("Position X: %d, Y: %d", (int)((bot->local.pos.m_x + 10) / 32), (int)((bot->local.pos.m_y + 15) / 32));
								ImGui::Text("Country : %s", bot->local.country.c_str());
								ImGui::Text("UserID: %d", bot->local.userid);
								ImGui::Text("NetID: %d", bot->local.netid);
								ImGui::Text("Gems: %d", bot->local.gems);
								ImGui::Text("Level: %d", bot->local.level);
								ImGui::Text("Players In World: %d", bot->local.players.size());
							}
							ImGui::Text("Other status: %s", bot->data.status.c_str());
							if (ImGui::Button("Connect"))
								bot->Connect(true);
							ImGui::SameLine();
							if (ImGui::Button("Disconnect"))
								bot->Disconnect(true);
							ImGui::EndTabItem();
						}
						
						if (ImGui::BeginTabItem("Control")) {
							ImGui::Spacing();
							static char worldname[30];
							static char doorid[30];
							static uint64_t warp_delay = 0;
							ImGui::SetNextItemWidth(200);
							ImGui::InputTextWithHint("##itwn", "World name", worldname, 33, ImGuiInputTextFlags_CharsUppercase);
							ImGui::SameLine();
							if (ImGui::Button("Warp")) {
								std::string nwn(worldname);
								if (nwn != bot->local.worldname && Utils::HandleDelay(warp_delay, 3000))
									bot->SendPacket(3, "action|join_request\nname|" + nwn);
							}
							ImGui::SetNextItemWidth(200);
							ImGui::InputTextWithHint("##itwnn", "Door ID", doorid, 33, ImGuiInputTextFlags_CharsUppercase);
							ImGui::SameLine();
							if (ImGui::Button("Enter")) {
								
							}
							
							static int buttonsize = 50;
							static int moveradius = 1;
							
							ImGui::Text("Current tile pos: %d, %d", (int)((bot->local.pos.m_x + 10) / 32), (int)((bot->local.pos.m_y + 15) / 32));
							ImVec2 buttonsizeF = ImVec2(50, 50);
							ImGui::BeginChild("cm", ImVec2(0, 0));
							if (ImGui::Button("1##Right UP", buttonsizeF))
								bot->Move(-1 * moveradius, -1 * moveradius);
							ImGui::SameLine();
							if (ImGui::Button("2##Up", buttonsizeF))
								bot->Move(0, -1 * moveradius);
							ImGui::SameLine();
							if (ImGui::Button("3##Left Up", buttonsizeF))
								bot->Move(1 * moveradius, -1 * moveradius);
							if (ImGui::Button("4##Left", buttonsizeF))
								bot->Move(-1 * moveradius, 0);
							ImGui::SameLine();
							if (ImGui::Button("5##Door", buttonsizeF))
								bot->Door();
							ImGui::SameLine();
							if (ImGui::Button("6##Right", buttonsizeF))
								bot->Move(1 * moveradius, 0);
							if (ImGui::Button("7##Rightdown", buttonsizeF))
								bot->Move(-1 * moveradius, 1 * moveradius);
							ImGui::SameLine();
							if (ImGui::Button("8##down", buttonsizeF))
								bot->Move(0, 1 * moveradius);
							ImGui::SameLine();
							if (ImGui::Button("9##ldown", buttonsizeF))
								bot->Move(1 * moveradius, 1 * moveradius);
							ImGui::SliderInt("##simr", &moveradius, 1, 3, "Move radius: %d");
							if (ImGui::Button("Respawn")) {
								if (bot->InWorld())
									bot->SendPacket(2, "action|respawn");
							}
							ImGui::SameLine();
							if (ImGui::Button("Exit world")) {
								if (bot->InWorld())
									bot->SendPacket(3, "action|quit_to_exit");
							}
							ImGui::Checkbox("Auto accept access", &bot->cheat.auto_aa);
								ImGui::SameLine();
								if (ImGui::Button("Unaccess")) {
									if (bot->InWorld())
										bot->SendPacket(2, "action|input\n|text|/unaccess");
								}
							ImGui::EndChild();
							ImGui::EndTabItem();
						}
						if (ImGui::BeginTabItem("Scanner")){
						ImGui::BeginTabBar("aowkoawkw");
						if (ImGui::BeginTabItem("Inventory")) {
								static int dt_count = 1;
								static uint64_t dt_time = 0;
								
								ImGui::Text("Gems: %d, Level: %d", bot->local.gems, bot->local.level);
								ImGui::SliderInt("##iiic", &dt_count, 1, 200, "Drop/trash count: %d");
								float availx = ImGui::GetContentRegionAvail().x - 3 * style.ItemSpacing.x - 3 * 50;
								
								ImGui::BeginChild("iin", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
								for (auto& item : bot->local.items) {
									ItemInfo* info = itemDefs->Get(item.id);
									if (info)
										ImGui::Text("%dx %s", item.amount, info->name.c_str());
									else
										ImGui::Text("%dx (id: %d)", item.amount, item.id);
									
									ImGui::SameLine();
									ImGui::PushID(item.id);
									if (ImGui::Button("Drop")) {
										if (bot->InWorld() && Utils::HandleDelay(dt_time, 2127)) {
											bot->dt_amount = dt_count > item.amount ? item.amount : dt_count;
											bot->SendPacket(2, "action|drop\n|itemID|" + std::to_string(item.id));
										}
									}
									ImGui::PopID();
									ImGui::SameLine();
									ImGui::PushID(item.id + 20000);
									if (ImGui::Button("Trash")) {
										if (bot->InWorld() && Utils::HandleDelay(dt_time, 2127)) {
											bot->dt_amount = dt_count > item.amount ? item.amount : dt_count;
											bot->SendPacket(2, "action|trash\n|itemID|" + std::to_string(item.id));
										}
									}
									ImGui::PopID();ImGui::SameLine();
									ImGui::PushID(item.id + 40000);
									if (ImGui::Button("Wear")) {
										if (bot->InWorld()) {
											TankPacketStruct tank;
											tank.type = 10;
											tank.value = item.id;
											bot->SendPacketRaw(4, (uint8_t*)&tank);
										}
									}
									ImGui::PopID();
								}
								ImGui::EndChild();
								ImGui::EndTabItem();
							}
						if (ImGui::BeginTabItem("Floating Object")) {
								if (ImGui::Button("Rescan"))
									bot->local.RescanObject();
								ImGui::SameLine();
								ImGui::Text("Count: %d, Actual Count: %d", bot->local.objects.size(), bot->local.objectcount);
								ImGui::BeginChild("iwo", ImVec2(0, 0), true);
								for (auto& scanned : bot->local.scanned_object) {
									ItemInfo* info = itemDefs->Get(scanned.first);
									if (info)
										ImGui::Text("%dx %s", scanned.second, info->name.c_str());
									else
										ImGui::Text("%dx (id: %d)", scanned.second, scanned.first);
								}
								ImGui::EndChild();
								ImGui::EndTabItem();
							}					
						if (ImGui::BeginTabItem("World Tile")) {
								ImGui::Text("World Name: %s" "\n" "Width: %d" "\n" "Height: %d" "\n" "Tile count: %d" "\n" ,
								bot->local.worldname.c_str(), bot->local.width, bot->local.height, bot->local.tilecount );
								if (ImGui::Button("Rescan"))
									bot->local.RescanTile();
								ImGui::BeginChild("iwt", ImVec2(0, 0), true);
								for (auto& scanned : bot->local.scanned_tile) {
									ItemInfo* info = itemDefs->Get(scanned.first);
									if (info)
										ImGui::Text("%dx %s", scanned.second, info->name.c_str());
									else
										ImGui::Text("%dx (id: %d)", scanned.second, scanned.first);
								}
								ImGui::EndChild();
								ImGui::EndTabItem();
							}
							ImGui::EndTabBar();
							ImGui::EndTabItem();
						}
						if (ImGui::BeginTabItem("Automation")) {
							ImGui::BeginTabBar("ta");
							
							if (ImGui::BeginTabItem("Collect")) {
								ImGui::BeginChild("ac", ImVec2(0, 0));
								ImGui::Checkbox("Auto collect", &bot->cheat.auto_c);
								ImGui::Checkbox("Force collect all", &bot->cheat.auto_c_f);
								ImGui::SliderInt("##sicd", &bot->cheat.auto_c_d, 100, 1000, "Collect delay: %d");
								ImGui::SliderInt("##sir", &bot->cheat.auto_c_r, 0, 10, "Collect radius: %d");
								ImGui::EndChild();
								ImGui::EndTabItem();
							}
							
							if (ImGui::BeginTabItem("Farm")) {
								ImGui::BeginChild("af", ImVec2(0, 0));
								
								static const char* farmmode[] = {"Above bot (Y -2)", "Above head (Y -1)", "Under bot (Y +2)", "Under feet (Y +1)"};
								ImGui::Combo("Farm mode", &bot->cheat.auto_f_m, farmmode, 4);
								if (ImGui::InputInt("Item ID", &bot->cheat.auto_f_id, 1)) {
									ItemInfo* info = itemDefs->Get(bot->cheat.auto_f_id);
									bot->cheat.auto_f_bhit = info ? info->breakHits/6 : 0;
								}
								ImGui::SliderInt("##sipd", &bot->cheat.auto_f_pd, 200, 1000, "Place delay: %d");
								ImGui::SliderInt("##sibd", &bot->cheat.auto_f_bd, 180, 1000, "Break delay: %d");
								ImGui::Checkbox("Auto farm", &bot->cheat.auto_f);
								ImGui::Checkbox("Auto place", &bot->cheat.auto_f_p);
								ImGui::Checkbox("Auto break", &bot->cheat.auto_f_b);
								
								ImGui::EndChild();
								ImGui::EndTabItem();
							}
							if (ImGui::BeginTabItem("Spam")) {
								ImGui::BeginChild("as", ImVec2(0, 0));
								static char spamtext[100];
								static int spam_selected = -1;
								float availx = ImGui::GetContentRegionAvail().x;
								
								ImGui::SetNextItemWidth(availx);
								ImGui::InputTextWithHint("##itst", "Spam text", spamtext, 100);
								
								if (ImGui::Button("Add")) {
									std::string spamtextS = std::string(spamtext);
									if (!spamtextS.empty())
										bot->cheat.auto_s_t.push_back(spamtextS);
								}
								ImGui::SameLine();
								if (ImGui::Button("Remove")) {
									if (spam_selected > -1 && spam_selected < bot->cheat.auto_s_t.size())
										bot->cheat.auto_s_t.erase(bot->cheat.auto_s_t.begin() + spam_selected);
									spam_selected = -1;
								}
								ImGui::SameLine();
								if (ImGui::Button("Clear")) {
									bot->cheat.auto_s_t.clear();
									spam_selected = -1;
								}
								
								if (ImGui::BeginListBox("##lbs", ImVec2(availx, 160))) {
									for (int i = 0; i < bot->cheat.auto_s_t.size(); i++) {
										ImGui::PushID(i);
										if (ImGui::Selectable(bot->cheat.auto_s_t[i].c_str(), spam_selected == i))
											spam_selected = i;
										ImGui::PopID();
									}
									ImGui::EndListBox();
								}
								
								ImGui::SliderInt("##sisd", &bot->cheat.auto_s_d, 2000, 8000, "Spam delay: %d");
								ImGui::Checkbox("Spam", &bot->cheat.auto_s);
								
								
								ImGui::EndChild();
								ImGui::EndTabItem();
							}
							ImGui::EndTabBar();
							ImGui::EndTabItem();
						}
						
						ImGui::EndTabBar();
					} else {
						ImGui::TextColored(ImVec4(255.0f, 0.0f, 0.0f, 1.00f), "No bots selected.");
					}
					
					ImGui::EndChild();
					//ImGui::EndTabItem();
				}
				
				if (tab == 2){
					
					ImGui::BeginChild("ls", ImVec2(200, 345 + 45), false);
					static float availx = ImGui::GetContentRegionAvail().x;
					
					if (ImGui::BeginListBox("##lbls", ImVec2(availx, 325.f))) {
						for (uint32_t i = 0; i < bots.size(); i++) {
							ImGui::PushID(i);
							if (botselected > 0){
								bot = bots[botselected];
							}
							if (ImGui::Selectable(bots[i]->data.name.empty() ? "[Guest Acccount]" : bots[i]->data.name.c_str(), bot == bots[i])){
								bot = bots[i];
								botselected = i;
								}
							ImGui::PopID();
						}
						ImGui::EndListBox();
					}
					
					if (ImGui::Button("Add", ImVec2(availx, 0)))
						popup_open = true;
					
					if (ImGui::Button("Remove", ImVec2(availx, 0))) {
						if (bot) {
							for (uint32_t i = 0; i < bots.size(); i++) {
								if (bots[i] == bot) {
									bots.erase(bots.begin() + i);
									cachedBots.push_back(bot);
									break;
								}
							}
							bot = NULL;
						}
					}
					
					ImGui::EndChild();
					ImGui::SameLine();
					ImGui::BeginChild("rs", ImVec2(420 + 100, 345 + 45), false);
					if (bot){
							ImGui::BeginTabBar("te");
							if (ImGui::BeginTabItem("Main")) {
								if (!bot->l_t) {
									if (ImGui::Button("Execute")) {
										kembali1:
										if (bot->l) {
											std::thread lua_thread(Api::THREAD, bot->l, &bot->l_t, m_editor.GetText());
											lua_thread.detach();
										} else {
											bot->l = luaL_newstate();
											Api::OPEN(bot->l, (uintptr_t)bot);
											goto kembali1;
										}
									}
									ImGui::SameLine();
								}
								if (ImGui::Button("Terminate")) {
									bot->hook_var = false;
									if (bot->l) {
										if (bot->l_t) {
											Api::TERMINATE(bot->l);
											bot->l = NULL;
										} else {
											lua_close(bot->l);
											bot->l = NULL;
										}
									}
									for (auto& s : bot->t)
										lua_close(s);
									bot->t.clear();
								}
								ImGui::SameLine();
								if (ImGui::Button("Browse"))
									m_browser.Open();
								
								m_editor.Render("ee", ImVec2(0, 0));
								ImGui::EndTabItem();
							}
							if (ImGui::BeginTabItem("Console")) {
								if (ImGui::Button("Clear"))
									bot->logger.textList[7].clear();
								ImGui::BeginChild("ec", ImVec2(0, 0));
								for (auto& text : bot->logger.textList[7])
									ImGui::TextUnformatted(text.c_str());
								ImGui::EndChild();
								ImGui::EndTabItem();
							}
							ImGui::EndTabBar();
					} else {
						ImGui::TextColored(ImVec4(255.0f, 0.0f, 0.0f, 1.00f), "No bots selected.");
					}
					ImGui::EndChild();
						
						
				}
				
				//Item DB
				if (tab == 3) {
					ImGui::Spacing();
					if (itemDefs->Instance) {
						
						static char itemname[30];
						static std::vector<ItemInfo*> itemList;
						static int showMuch = 20;
						static ItemInfo* item = NULL;
						
						ImGui::Text("items.dat version: %d, item count: %d", itemDefs->version, itemDefs->itemcount);
						ImGui::Separator();
						ImGui::SetNextItemWidth(250);
						if (ImGui::InputTextWithHint("##intit", "Find item name", itemname, 30))
							itemDefs->GetItemsByPartialName(itemList, itemname);
						ImGui::SameLine();
						ImGui::Text("found result: %d", itemList.size());
						
						ImGui::SetNextItemWidth(170);
						ImGui::InputInt("Show count", &showMuch, 1);
						
						if (ImGui::BeginListBox("##ls", ImVec2(300, ImGui::GetContentRegionAvail().y))) {
							for (int i = 0; i < showMuch; i++) {
								if (i > -1 && i < (int)itemList.size()) {
									ImGui::PushID(i);
									if (ImGui::Selectable(itemList[i]->name.c_str(), item == itemList[i]))
										item = itemList[i];
									ImGui::PopID();
								} else break;
							}
							ImGui::EndListBox();
						}
						
						ImGui::SameLine();
						ImGui::BeginChild("rs", ImVec2(0, 0), true);
						
						if (item) {
							ImGui::Text( "name: %s""\n" "id: %d""\n" "type: %d""\n" "collision type: %d""\n" "break hit: %d""\n" "clothing type: %d""\n" "rarity: %d""\n" "grow time: %d",
							item->name.c_str(), item->id, item->type, item->collisionType, item->breakHits/6, item->clothingType, item->rarity, item->growTime);
						}
						
						ImGui::EndChild();
					} else {
						ImGui::TextUnformatted("items.dat not found!\n");
						if (ImGui::Button("Download items.dat")){
							
							LPCTSTR url = TEXT("http://cdn.discordapp.com/attachments/914033740651569203/997673251658530956/items.dat");
							LPCTSTR fPath = TEXT("C:\\items.dat");

							HRESULT hr = URLDownloadToFile (NULL, url, fPath, 0, NULL);
							if (!FAILED(hr))
							{
								itemDefs->LoadFromFile();
							}
						}
					}
					
					//ImGui::EndTabItem();
				}
				
				//ImGui::EndTabBar();
			}
			
			// popup for add bot
			if (popup_open) {
				popup_open = false;
				ImGui::OpenPopup("Add bot");
			}
			
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x + 100, ImGui::GetWindowPos().y + 300), ImGuiCond_Once);
			if (ImGui::BeginPopupModal("Add bot", NULL, ImGuiWindowFlags_NoDecoration)) {
				static char name[20];
				static char pass[20];
				static bool showpassword = false;
				
				ImGui::SetNextItemWidth(300);
				ImGui::InputTextWithHint("##itn", "GrowID (empty for guest)", name, 20);
				
				ImGui::SetNextItemWidth(300);
				ImGui::InputTextWithHint("##itp", "Password", pass, 20, showpassword ? 0 : ImGuiInputTextFlags_Password);
				ImGui::Checkbox("Show password", &showpassword);
				
				float ww = (ImGui::GetContentRegionAvail().x - style.ItemSpacing.x) / 2;
				if (ImGui::Button("Cancel", ImVec2(ww, 0)))
					ImGui::CloseCurrentPopup();
				ImGui::SameLine();
				if (ImGui::Button("Add", ImVec2(ww, 0))) {
					bots.push_back(new ENetClient(name, pass));
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			
			m_browser.Display();
			if (m_browser.HasSelected()) {
				std::string path = m_browser.GetSelected().string();
				std::vector<std::string> content;
				std::string lines;
				std::ifstream filee(path);
				while (getline(filee, lines))
					content.push_back(lines);
				filee.close();
				m_editor.SetTextLines(content);
				m_browser.ClearSelected();
			}
			
			ImGui::End();
			Gui::EndRender();
			
			for (auto& bot : bots)
				bot->Poll();
			
			srand(time(NULL));
			//std::this_thread::sleep_for(std::chrono::milliseconds(5));
			Sleep(5);
		}
		Gui::DestroyImGui();
	}
	
	Gui::DestroyDevice();
	Gui::DestroyHWindow();
#ifdef _CONSOLE
	system("pause");
#endif
	return 1;
}
