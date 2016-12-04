#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <crtdbg.h>

#include "Engine/Engine.h"
#include "Engine/AssetsManager.h"
#include "Engine/LOD.h"
#include "Engine/MMT.h"
#include "Engine/texts.h"
#include "Engine/Graphics/IRender.h"
#include "Engine/Graphics/Viewport.h"
#include "Engine/Graphics/PaletteManager.h"
#include "Engine/Tables/IconFrameTable.h"

#include "IO/Mouse.h"
#include "IO/Keyboard.h"

#include "GUI/GUIFont.h"
#include "GUI/UI/UIMainMenu.h"
#include "GUI/UI/UIGame.h"
#include "GUI/UI/UIPartyCreation.h"

#include "Media/Audio/AudioPlayer.h"

#include "Game/Game.h"
#include "Game/MainMenu.h"



GUIButton *pMainMenu_BtnExit = nullptr;
GUIButton *pMainMenu_BtnCredits = nullptr;
GUIButton *pMainMenu_BtnLoad = nullptr;
GUIButton *pMainMenu_BtnNew = nullptr;

Image *ui_mainmenu_new = nullptr;
Image *ui_mainmenu_load = nullptr;
Image *ui_mainmenu_credits = nullptr;
Image *ui_mainmenu_exit = nullptr;


GUIWindow_MainMenu::GUIWindow_MainMenu() :
    GUIWindow(0, 0, window->GetWidth(), window->GetHeight(), 0, nullptr)
{
// -----------------------------------
// 004627B7 void MainMenu_Loop -- part
    ui_mainmenu_new = assets->GetImage_16BitColorKey(L"title_new", 0x7FF);
    ui_mainmenu_load = assets->GetImage_16BitColorKey(L"title_load", 0x7FF);
    ui_mainmenu_credits = assets->GetImage_16BitColorKey(L"title_cred", 0x7FF);
    ui_mainmenu_exit = assets->GetImage_16BitColorKey(L"title_exit", 0x7FF);

    pMainMenu_BtnNew = CreateButton(495, 172, ui_mainmenu_new->GetWidth(), ui_mainmenu_new->GetHeight(), 1, 0, UIMSG_MainMenu_ShowPartyCreationWnd, 0, 'N', "", ui_mainmenu_new, 0);
    pMainMenu_BtnLoad = CreateButton(495, 227, ui_mainmenu_load->GetWidth(), ui_mainmenu_load->GetHeight(), 1, 0, UIMSG_MainMenu_ShowLoadWindow, 1, 'L', "", ui_mainmenu_load, 0);
    pMainMenu_BtnCredits = CreateButton(495, 282, ui_mainmenu_credits->GetWidth(), ui_mainmenu_credits->GetHeight(), 1, 0, UIMSG_ShowCredits, 2, 'C', "", ui_mainmenu_credits, 0);
    pMainMenu_BtnExit = CreateButton(495, 337, ui_mainmenu_exit->GetWidth(), ui_mainmenu_exit->GetHeight(), 1, 0, UIMSG_ExitToWindows, 3, 0, "", ui_mainmenu_exit, 0);
}


void GUIWindow_MainMenu::Update()
{
// -----------------------------------
// 004627B7 void MainMenu_Loop -- part
    POINT pt;
    pMouse->GetCursorPos(&pt);
    GUIWindow *pWindow = this;//pWindow_MainMenu;
    //if (GetCurrentMenuID() == MENU_SAVELOAD)
    //    pWindow = pGUIWindow_CurrentMenu;

    if (GetCurrentMenuID() == MENU_MAIN)
    {
        Image *pTexture = nullptr;
        if (!pModalWindow)// ???
        {
            auto pButton = pWindow->pControlsHead;
            for (pButton = pWindow->pControlsHead; pButton; pButton = pButton->pNext)
            {
                if (pt.x >= (signed int)pButton->uX && pt.x <= (signed int)pButton->uZ
                    && pt.y >= (signed int)pButton->uY && pt.y <= (signed int)pButton->uW
                    && pWindow == pWindow_MainMenu)
                {
                    auto pControlParam = pButton->msg_param;
                    int pY = 0;
                    switch (pControlParam) // backlight for buttons
                    {
                    case 0:
                        pTexture = assets->GetImage_16BitColorKey("title_new", 0x7FF);
                        pY = 172;
                        break;
                    case 1:
                        pTexture = assets->GetImage_16BitColorKey("title_load", 0x7FF);
                        pY = 227;
                        break;
                    case 2:
                        pTexture = assets->GetImage_16BitColorKey("title_cred", 0x7FF);
                        pY = 282;
                        break;
                    case 3:
                        pTexture = assets->GetImage_16BitColorKey("title_exit", 0x7FF);
                        pY = 337;
                        break;
                    }
                    pRenderer->DrawTextureAlphaNew(495/640.0f, pY/480.0f, pTexture);
                }
            }
        }
    }
}


//----- (0041B578) --------------------------------------------------------
void MainMenuUI_LoadFontsAndSomeStuff()
{
  //pIcons_LOD->SetupPalettes(pRenderer->uTargetRBits, pRenderer->uTargetGBits, pRenderer->uTargetBBits);
  pIcons_LOD->SetupPalettes(5, 6, 5);
  //pPaletteManager->SetColorChannelInfo(pRenderer->uTargetRBits, pRenderer->uTargetGBits, pRenderer->uTargetBBits);
  pPaletteManager->SetColorChannelInfo(5, 6, 5);
  pPaletteManager->RecalculateAll();

  for (uint i = 0; i < window->GetHeight(); ++i)
    pSRZBufferLineOffsets[i] = window->GetWidth() * i;

  pFontArrus = LoadFont("arrus.fnt", "FONTPAL", nullptr);
  pFontArrus->field_3 = 0;

  pFontLucida = LoadFont("lucida.fnt", "FONTPAL", nullptr);
  pFontLucida->field_3 = 0;

  pFontCreate = LoadFont("create.fnt", "FONTPAL", nullptr);
  pFontCreate->field_3 = 0;

  pFontSmallnum = LoadFont("smallnum.fnt", "FONTPAL", nullptr);
  pFontComic = LoadFont("comic.fnt", "FONTPAL", nullptr);
}

//----- (004415C5) --------------------------------------------------------
static void LoadPartyBuffIcons()
{
  for (uint i = 0; i < 14; ++i)
  {
    char filename[200];
    sprintf(filename, "isn-%02d", i + 1);
    pTextureIDs_PartyBuffIcons[i] = pIcons_LOD->LoadTexture(filename, TEXTURE_16BIT_PALETTE);
  }

  uIconIdx_FlySpell = pIconsFrameTable->FindIcon("spell21");
  uIconIdx_WaterWalk = pIconsFrameTable->FindIcon("spell27");
}

//----- (0041B690) --------------------------------------------------------
void MainMenuUI_Create()
{
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("wizeyeC"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("wizeyeB"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("wizeyeA"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("torchC"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("torchB"));
  pIconsFrameTable->InitializeAnimation(pIconsFrameTable->FindIcon("torchA"));

  game_ui_minimap_dirs[0] = assets->GetImage_16BitAlpha("MAPDIR1");
  game_ui_minimap_dirs[1] = assets->GetImage_16BitAlpha("MAPDIR2");
  game_ui_minimap_dirs[2] = assets->GetImage_16BitAlpha("MAPDIR3");
  game_ui_minimap_dirs[3] = assets->GetImage_16BitAlpha("MAPDIR4");
  game_ui_minimap_dirs[4] = assets->GetImage_16BitAlpha("MAPDIR5");
  game_ui_minimap_dirs[5] = assets->GetImage_16BitAlpha("MAPDIR6");
  game_ui_minimap_dirs[6] = assets->GetImage_16BitAlpha("MAPDIR7");
  game_ui_minimap_dirs[7] = assets->GetImage_16BitAlpha("MAPDIR8");

  game_ui_bar_blue = assets->GetImage_16BitColorKey(L"ib-statB", 0x7FF);
  game_ui_bar_green = assets->GetImage_16BitColorKey(L"ib-statG", 0x7FF);
  game_ui_bar_yellow = assets->GetImage_16BitColorKey(L"ib-statY", 0x7FF);
  game_ui_bar_red = assets->GetImage_16BitColorKey(L"ib-statR", 0x7FF);
  game_ui_monster_hp_background = assets->GetImage_16BitColorKey(L"mhp_bg", 0x7FF);
  game_ui_monster_hp_border_left = assets->GetImage_16BitColorKey(L"mhp_capl", 0x7FF);
  game_ui_monster_hp_border_right = assets->GetImage_16BitColorKey(L"mhp_capr", 0x7FF);
  game_ui_monster_hp_green = assets->GetImage_16BitColorKey(L"mhp_grn", 0x7FF);
  game_ui_monster_hp_red = assets->GetImage_16BitColorKey(L"mhp_red", 0x7FF);
  game_ui_monster_hp_yellow = assets->GetImage_16BitColorKey(L"mhp_yel", 0x7FF);
  ui_leather_mm7 = assets->GetImage_16Bit("LEATHER");
  ui_leather_mm6 = assets->GetImage_16Bit("ibground");
  dialogue_ui_x_x_u = assets->GetImage_16BitColorKey(L"x_x_u", 0x7FF);
  ui_buttdesc2 = assets->GetImage_16BitAlpha(L"BUTTESC2");
  dialogue_ui_x_ok_u = assets->GetImage_16BitColorKey(L"x_ok_u", 0x7FF);
  ui_buttyes2 = assets->GetImage_16BitAlpha(L"BUTTYES2");
  ui_partycreation_buttmake = assets->GetImage_16BitAlpha(L"BUTTMAKE");
  ui_partycreation_buttmake2 = assets->GetImage_16BitAlpha(L"BUTTMAKE2");

  pPrimaryWindow = new GUIWindow(0, 0, window->GetWidth(), window->GetHeight(), 0, 0);
  pPrimaryWindow->CreateButton(7, 8, 460, 343, 1, 0, UIMSG_MouseLeftClickInGame, 0, 0, "", 0);

  pPrimaryWindow->CreateButton(61, 424, 31, 80, 2, 94, UIMSG_SelectCharacter, 1, '1', "", 0);//buttons for portraits
  pPrimaryWindow->CreateButton(177, 424, 31, 80, 2, 94, UIMSG_SelectCharacter, 2, '2', "", 0);
  pPrimaryWindow->CreateButton(292, 424, 31, 40, 2, 94, UIMSG_SelectCharacter, 3, '3', "", 0);
  pPrimaryWindow->CreateButton(407, 424, 31, 40, 2, 94, UIMSG_SelectCharacter, 4, '4', "", 0);

  pPrimaryWindow->CreateButton(24, 404, 5, 49, 1, 93, UIMSG_0, 1, 0, "", 0);//buttons for HP
  pPrimaryWindow->CreateButton(139, 404, 5, 49, 1, 93, UIMSG_0, 2, 0, "", 0);
  pPrimaryWindow->CreateButton(255, 404, 5, 49, 1, 93, UIMSG_0, 3, 0, "", 0);
  pPrimaryWindow->CreateButton(370, 404, 5, 49, 1, 93, UIMSG_0, 4, 0, "", 0);

  pPrimaryWindow->CreateButton(97, 404, 5, 49, 1, 93, UIMSG_0, 1, 0, "", 0);//buttons for SP
  pPrimaryWindow->CreateButton(212, 404, 5, 49, 1, 93, UIMSG_0, 2, 0, "", 0);
  pPrimaryWindow->CreateButton(328, 404, 5, 49, 1, 93, UIMSG_0, 3, 0, "", 0);
  pPrimaryWindow->CreateButton(443, 404, 5, 49, 1, 93, UIMSG_0, 4, 0, "", 0);

  game_ui_tome_quests = assets->GetImage_16BitAlpha(L"ib-td1-A");
  pBtn_Quests = pPrimaryWindow->CreateButton(
      491, 353,
      game_ui_tome_quests->GetWidth(),
      game_ui_tome_quests->GetHeight(),
      1, 0, UIMSG_OpenQuestBook, 0, pKeyActionMap->GetActionVKey(INPUT_Quest),
      pGlobalTXT_LocalizationStrings[174], game_ui_tome_quests, 0
  ); //Quests

  game_ui_tome_autonotes = assets->GetImage_16BitAlpha(L"ib-td2-A");
  pBtn_Autonotes = pPrimaryWindow->CreateButton(
      527, 353,
      game_ui_tome_autonotes->GetWidth(),
      game_ui_tome_autonotes->GetHeight(),
      1, 0, UIMSG_OpenAutonotes, 0, pKeyActionMap->GetActionVKey(INPUT_Autonotes),
     pGlobalTXT_LocalizationStrings[154], game_ui_tome_autonotes, 0
  );//Autonotes

  game_ui_tome_maps = assets->GetImage_16BitAlpha("ib-td3-A");
  pBtn_Maps = pPrimaryWindow->CreateButton(
      546, 353,
      game_ui_tome_maps->GetWidth(),
     game_ui_tome_maps->GetHeight(),
      1, 0, UIMSG_OpenMapBook, 0, pKeyActionMap->GetActionVKey(INPUT_Mapbook),
     pGlobalTXT_LocalizationStrings[139], game_ui_tome_maps, 0
  ); //Maps

  game_ui_tome_calendar = assets->GetImage_16BitAlpha("ib-td4-A");
  pBtn_Calendar = pPrimaryWindow->CreateButton(
      570, 353,
      game_ui_tome_calendar->GetWidth(),
     game_ui_tome_calendar->GetHeight(),
      1, 0, UIMSG_OpenCalendar, 0, pKeyActionMap->GetActionVKey(INPUT_TimeCal),
     pGlobalTXT_LocalizationStrings[78], game_ui_tome_calendar, 0
  );//Calendar

  game_ui_tome_storyline = assets->GetImage_16BitAlpha("ib-td5-A");
  pBtn_History = pPrimaryWindow->CreateButton(
      600, 361,
      game_ui_tome_storyline->GetWidth(),
      game_ui_tome_storyline->GetHeight(),
      1, 0, UIMSG_OpenHistoryBook, 0, 72,//ascii
      pGlobalTXT_LocalizationStrings[602], game_ui_tome_storyline, 0
  );//History

  bFlashAutonotesBook = 0;
  bFlashQuestBook = 0;
  bFlashHistoryBook = 0;

  pBtn_ZoomIn = pPrimaryWindow->CreateButton(574, 136, game_ui_btn_zoomin->GetWidth(),
     game_ui_btn_zoomin->GetHeight(), 2, 0, UIMSG_ClickZoomInBtn, 0, pKeyActionMap->GetActionVKey(INPUT_ZoomIn),
     pGlobalTXT_LocalizationStrings[252], game_ui_btn_zoomin, 0); // Zoom In

  pBtn_ZoomOut = pPrimaryWindow->CreateButton(519, 136, game_ui_btn_zoomout->GetWidth(),
     game_ui_btn_zoomout->GetHeight(), 2, 0, UIMSG_ClickZoomOutBtn, 0, pKeyActionMap->GetActionVKey(INPUT_ZoomOut),
     pGlobalTXT_LocalizationStrings[251], game_ui_btn_zoomout, 0); // Zoom Out

  pPrimaryWindow->CreateButton(481, 0, 153, 67, 1, 92, UIMSG_0, 0, 0, "", 0);
  pPrimaryWindow->CreateButton(491, 149, 64, 74, 1, 0, UIMSG_StartHireling1Dialogue, 0, '5', "", 0);
  pPrimaryWindow->CreateButton(561, 149, 64, 74, 1, 0, UIMSG_StartHireling2Dialogue, 0, '6', "", 0);
  pPrimaryWindow->CreateButton(476, 322, 77, 17, 1, 100, UIMSG_0, 0, 0, "", 0);
  pPrimaryWindow->CreateButton(555, 322, 77, 17, 1, 101, UIMSG_0, 0, 0, "", 0);

  pBtn_CastSpell = pPrimaryWindow->CreateButton(476, 450,
      game_ui_btn_cast->GetWidth(),
      game_ui_btn_cast->GetHeight(),
      1, 0, UIMSG_SpellBookWindow, 0, 67, pGlobalTXT_LocalizationStrings[38], game_ui_btn_cast, 0);
  pBtn_Rest = pPrimaryWindow->CreateButton(518, 450,
      game_ui_btn_rest->GetWidth(),
      game_ui_btn_rest->GetHeight(),
      1, 0, UIMSG_RestWindow, 0, 82, pGlobalTXT_LocalizationStrings[182], game_ui_btn_rest, 0);
  pBtn_QuickReference = pPrimaryWindow->CreateButton(560, 450,
      game_ui_btn_quickref->GetWidth(),
      game_ui_btn_quickref->GetHeight(),
      1, 0, UIMSG_QuickReference, 0, 90, pGlobalTXT_LocalizationStrings[173], game_ui_btn_quickref, 0);
  pBtn_GameSettings = pPrimaryWindow->CreateButton(602, 450,
      game_ui_btn_settings->GetWidth(),
      game_ui_btn_settings->GetHeight(),
      1, 0, UIMSG_GameMenuButton, 0, 0, pGlobalTXT_LocalizationStrings[93], game_ui_btn_settings, 0);

  pBtn_NPCLeft = pPrimaryWindow->CreateButton(469, 178,
      ui_btn_npc_left->GetWidth(),
      ui_btn_npc_left->GetHeight(),
      1, 0, UIMSG_ScrollNPCPanel, 0, 0, "", ui_btn_npc_left, 0);
  pBtn_NPCRight = pPrimaryWindow->CreateButton(626, 178,
      ui_btn_npc_right->GetWidth(),
      ui_btn_npc_right->GetHeight(),
      1, 0, UIMSG_ScrollNPCPanel, 1, 0, "", ui_btn_npc_right, 0);
  LoadPartyBuffIcons();
}




//----- (00452AF3) --------------------------------------------------------
void __fastcall fill_pixels_fast(unsigned int a1, unsigned __int16 *pPixels, unsigned int uNumPixels)
{
  void *v3; // edi@1
  unsigned int v4; // eax@1
  unsigned __int16 *v5; // edi@3
  unsigned int i; // ecx@3

  if (for_refactoring)
  {
    MessageBoxA(nullptr, "Nomad: sub operates on 16 bit pixels, we have 32 bits.", "", 0);
    __debugbreak();
  }

  v3 = pPixels;
  v4 = a1 | (a1 << 16);
  if ( (unsigned __int8)pPixels & 2 )           // first 2 pixels
  {
    *pPixels = v4;
    v3 = pPixels + 1;
    --uNumPixels;
  }
  memset32(v3, v4, uNumPixels >> 1);            // 4 pixels at once
  v5 = (unsigned __int16 *)((char *)v3 + 4 * (uNumPixels >> 1));
  for ( i = uNumPixels & 1; i; --i )            // leftover pixels
  {
    *v5 = v4;
    ++v5;
  }
}

//----- (004979D2) --------------------------------------------------------
MENU_STATE MainMenuUI_Credits_Loop()
{
  char *cred_texturet; // edi@5
  FILE *pFile; // eax@5
  unsigned int pSize; // esi@7
  MSG Msg; // [sp+84h] [bp-B8h]@10
  GUIWindow credit_window;
  int move_Y; // [sp+128h] [bp-14h]@1
  char *pString; // [sp+12Ch] [bp-10h]@9
  GUIFont *pFontQuick; // [sp+134h] [bp-8h]@1
  GUIFont *pFontCChar; // [sp+138h] [bp-4h]@1
  RGBTexture cred_texture; // [sp+100h] [bp-3Ch]@1
  Texture_MM7 pTemporaryTexture; // [sp+Ch] [bp-130h]@5

  pFontQuick = LoadFont("quick.fnt", "FONTPAL", NULL);
  pFontCChar = LoadFont("cchar.fnt", "FONTPAL", NULL);

  if ( pMessageQueue_50CBD0->uNumMessages )
    pMessageQueue_50CBD0->uNumMessages = pMessageQueue_50CBD0->pMessages[0].field_8 != 0;
  ++pIcons_LOD->uTexturePacksCount;
  if ( !pIcons_LOD->uNumPrevLoadedFiles )
    pIcons_LOD->uNumPrevLoadedFiles = pIcons_LOD->uNumLoadedFiles;
  //dword_A74C88 = 0;//??? ����� ����������� ���� �����������, ��� �������������� ������ ����������


  if ( use_music_folder )
	PlayAudio(L"Music\\15.mp3");
  else
    pAudioPlayer->PlayMusicTrack(MUSIC_Credits);

  Image *mm6title = assets->GetImage_PCXFromIconsLOD(L"mm6title.pcx");

  cred_texturet = (char *)pEvents_LOD->LoadRaw("credits.txt", 0);
  pFile = pEvents_LOD->FindContainer("credits.txt", 0);
  if ( !pFile )
    Error(pGlobalTXT_LocalizationStrings[63]); // "Might and Magic VII is having trouble loading files. 

  // Please re-install to fix this problem. Note: Re-installing will not destroy your save games."

  //��� ��������� �������-----------------------
  fread(&pTemporaryTexture, 1, 0x30, pFile);
  pSize = pTemporaryTexture.uDecompressedSize;
  if ( !pSize )
    pSize = pTemporaryTexture.uTextureSize;
  memset(&pTemporaryTexture, 0, 0x48);//���������
  cred_texturet[pSize] = 0;//����� ������

  credit_window.uFrameWidth = 250;
  credit_window.uFrameHeight = 440;
  credit_window.uFrameX = 389;
  credit_window.uFrameY = 19;

  cred_texture.uWidth = 250;
  cred_texture.uHeight = pFontQuick->GetStringHeight2(pFontCChar, cred_texturet, &credit_window, 0, 1) + 2 * credit_window.uFrameHeight;
  cred_texture.uNumPixels = cred_texture.uWidth * cred_texture.uHeight;
  cred_texture.pPixels = (unsigned __int16 *)malloc(2 * cred_texture.uNumPixels);
  fill_pixels_fast(Color16(0, 0xFFu, 0xFFu), cred_texture.pPixels, cred_texture.uNumPixels);
  cred_texture._allocation_flags = 0;

  //���� ����� � ����� ������
  pString = (char *)malloc(2 * pSize);
  strncpy(pString, cred_texturet, pSize);
  pString[pSize] = 0;
  pFontQuick->_44D2FD_prolly_draw_credits_entry(pFontCChar, 0, credit_window.uFrameHeight, cred_texture.uWidth,
    cred_texture.uHeight, Color16(0x70u, 0x8Fu, 0xFEu), Color16(0xECu, 0xE6u, 0x9Cu), pString, cred_texture.pPixels, cred_texture.uWidth);
  free(pString);

  pWindow_MainMenu = new GUIWindow(0, 0, window->GetWidth(), window->GetHeight(), 0, cred_texturet);
  pWindow_MainMenu->CreateButton(0, 0, 0, 0, 1, 0, UIMSG_Escape, 0, 27, "", 0);
  current_screen_type = SCREEN_CREATORS;
  SetCurrentMenuID(MENU_CREDITSPROC);

  move_Y = 0;
  do
  {
    while ( PeekMessageA(&Msg, 0, 0, 0, 1) )
    {
      if ( Msg.message == 18 )
        Engine_DeinitializeAndTerminate(0);
      TranslateMessage(&Msg);
      DispatchMessageA(&Msg);
    }
    if (dword_6BE364_game_settings_1 & GAME_SETTINGS_APP_INACTIVE)
    {
      WaitMessage();
    }
    else
    {
      pRenderer->BeginScene();
      pRenderer->DrawTextureNew(0, 0, mm6title);
      pRenderer->SetUIClipRect(credit_window.uFrameX, credit_window.uFrameY, credit_window.uFrameX + credit_window.uFrameWidth,
           credit_window.uFrameY + credit_window.uFrameHeight);
      pRenderer->CreditsTextureScroll(credit_window.uFrameX, credit_window.uFrameY, 0, move_Y, &cred_texture);
      pRenderer->ResetUIClipRect();
      pRenderer->EndScene();
      ++move_Y;
      if ( move_Y >= cred_texture.uHeight )
        SetCurrentMenuID(MENU_MAIN);
      pRenderer->Present();
      current_screen_type = SCREEN_GAME;//Ritor1: temporarily, must be corrected MainMenu_EventLoop()
      MainMenu_EventLoop();
    }
  }
  while ( GetCurrentMenuID() == MENU_CREDITSPROC );
  if ( use_music_folder )
	alSourceStop(mSourceID);
  pAudioPlayer->_4AA258(1);
  free(cred_texturet);
  free(pFontQuick);
  free(pFontCChar);
  pWindow_MainMenu->Release();
  pIcons_LOD->RemoveTexturesPackFromTextureList();

  if (mm6title)
  {
      mm6title->Release();
      mm6title = nullptr;
  }
    
  cred_texture.Release();
  return MENU_MAIN;     // return MENU_Main
}