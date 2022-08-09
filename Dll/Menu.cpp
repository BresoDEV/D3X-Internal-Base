

#include "Header.h"
#include "Menu.h"  
#include <sstream>
using namespace std;

std::unique_ptr<MenuManager> g_Menu;




MenuManager::MenuManager(int x, int y, int w, int h)
{
	bVisible = true;
	bDir = true;
	bRun = false;
	backupX = x;
	startX = x;
	startY = y;
	startW = w;
	startH = h;
	currentSel = 0;
}

MenuManager::~MenuManager() = default;

 



void MenuManager::SetAnimation(bool b)
{
#ifdef _ANIMATION
	bDir = b;
	bRun = true;
#else
	bVisible = b;
#endif // _ANIMATION
}













enum Submenus
{
	Principal = 1,
	Configuracoes,
	Posicao,
};











//Loop
void MenuManager::OnUpdate(LPDIRECT3DDEVICE9 pDevice)
{


	if (IsKeyPressed(VK_INSERT, 0) & 1)
	{
		SetAnimation(!bVisible);
	}

	HackActions();

	if (!bVisible)
		return;


}


int Posicao_Menu[4] = { 50, 50, 255, 350 };
int opcaoAtual = 1;
int maxOpcoes = 10;
bool rolagemRapida = false;
bool selecionou = false;
int menuAtual = 1;
int menuAnterior;

void MenuPrincipal();
void MenuSettings();
void MenuPosicao();

//Roda Sempre
void MenuManager::Render(LPDIRECT3DDEVICE9 pDevice, IDirect3DTexture9 * TextFundoMenu)
{

	OnUpdate(pDevice);

	if (!bVisible)
		return;

	switch (menuAtual)
	{
	case Submenus::Principal:			MenuPrincipal(); break;
	case Submenus::Configuracoes:		MenuSettings(); break;
	case Submenus::Posicao:				MenuPosicao(); break;
	}



}







int pox[4] = { 50, 50, 255, 350 };
int pt = 0;
int aa = 0;

void Controles()
{
	if (rolagemRapida)
	{
		if (GetAsyncKeyState(VK_UP) == -32767)
		{
			if (opcaoAtual == 1)
				opcaoAtual = maxOpcoes;
			else
				opcaoAtual--;
		}
		if (GetAsyncKeyState(VK_DOWN) == -32767)
		{
			if (opcaoAtual != maxOpcoes)
				opcaoAtual++;
			else
				opcaoAtual = 1;
		}
	}
	else
	{
		if (IsKeyPressed(VK_UP, 0) & 1)
		{
			if (opcaoAtual == 1)
				opcaoAtual = maxOpcoes;
			else
				opcaoAtual--;
		}
		if (IsKeyPressed(VK_DOWN, 0) & 1)
		{
			if (opcaoAtual != maxOpcoes)
				opcaoAtual++;
			else
				opcaoAtual = 1;
		}
	}


}

void DrawRects()
{
	//topo
	g_Draw->DrawBox(Posicao_Menu[0],
		Posicao_Menu[1],
		Posicao_Menu[2],
		350 + (-334),
		Vermelho);

	//baixo
//	g_Draw->DrawBox(Posicao_Menu[0],
//		Posicao_Menu[1] + 350,
//		Posicao_Menu[2],
//		350 + (-348),
//		Vermelho); //Fundo
//
//	//esquerda
//	g_Draw->DrawBox(Posicao_Menu[0] + (-2),
//		Posicao_Menu[1],
//		255 + (-253),
//		Posicao_Menu[3] + 2,
//		Vermelho); //Fundo
//
//	//direita
//	g_Draw->DrawBox(Posicao_Menu[0] + 254,
//		Posicao_Menu[1],
//		255 + (-253),
//		Posicao_Menu[3] + 2,
//		Vermelho); //Fundo
}

void AddTitle(const char* Titulo, int MaxOpcoesMenu, int _menuatual, int menuanterior_ = 1)
{
	DrawRects();
	Controles();
	menuAnterior = menuanterior_;
	menuAtual = _menuatual;
	maxOpcoes = MaxOpcoesMenu;
	g_Draw->DrawBox(Posicao_Menu[0], Posicao_Menu[1], Posicao_Menu[2], Posicao_Menu[3], Fundo); //Fundo
	char buff[128];
	sprintf_s(buff, "%s", Titulo);
	g_Draw->DrawTextA(buff, Posicao_Menu[0] + (Posicao_Menu[2] / 2), Posicao_Menu[1] + 3, Branco, 1);
	g_Draw->DrawTextA(buff, Posicao_Menu[0] + (Posicao_Menu[2] / 2), Posicao_Menu[1] + 3, Branco, 1);

	if (IsKeyPressed(VK_BACK, 0) & 1)
	{
		if (menuAtual != 1)
		{
			menuAtual = menuAnterior;
			opcaoAtual = 1;
		}
	}
	if (IsKeyPressed(VK_RETURN, 0) & 1)
	{
		selecionou = true;
	}
}

void AddOption(const char* Texto, int index)
{
	char buff[128];
	sprintf_s(buff, "%s", Texto);
	int cont = 0;
	int cont2 = 0;
	for (int i = 35; i  <= 10000; i += 15)
	{
		cont2++;
		if (cont2 == index)
		{
			cont = (cont2 * 15);
			break;
		}
	}
	if (opcaoAtual == index)
		g_Draw->DrawTextA(buff, Posicao_Menu[0] + 3, Posicao_Menu[1] + cont, Cyano, 2);
	else
		g_Draw->DrawTextA(buff, Posicao_Menu[0] + 3, Posicao_Menu[1] + cont, Branco, 2);
}

void AddFloatOption(const char* Texto, int index, float& valor, float min, float max, bool fast, float step = 1.0)
{

	char buff[300];
	sprintf_s(buff, "< %.04f >", valor);
	int cont = 0;
	int cont2 = 0;
	for (int i = 35; i  <= 10000; i += 15)
	{
		cont2++;
		if (cont2 == index)
		{
			cont = (cont2 * 15);
			break;
		}
	}
	if (opcaoAtual == index)
	{
		AddOption(Texto, index);
		g_Draw->DrawTextA(buff, Posicao_Menu[0] + (Posicao_Menu[2] - 4), Posicao_Menu[1] + cont, Cyano, 0);
		if (fast)
		{
			if (GetAsyncKeyState(VK_LEFT) == -32767)
			{
				if (valor <= min)
					valor = max;
				else
					valor -= step;
			}
			if (GetAsyncKeyState(VK_RIGHT) == -32767)
			{
				if (valor <= max)
					valor += step;
				else
					valor = min;
			}
		}
		else
		{
			if (IsKeyPressed(VK_LEFT, 0) & 1)
			{
				if (valor <= min)
					valor = max;
				else
					valor -= step;
			}
			if (IsKeyPressed(VK_RIGHT, 0) & 1)
			{
				if (valor <= max)
					valor += step;
				else
					valor = min;
			}
		}
	}
	else
	{
		AddOption(Texto, index);
		g_Draw->DrawTextA(buff, Posicao_Menu[0] + (Posicao_Menu[2] - 4), Posicao_Menu[1] + cont, Branco, 0);
	}
}

void AddIntOption(const char* Texto, int index, int& valor, int min, int max, bool fast, int step = 1)
{
	AddOption(Texto, index);
	char buff[300];
	sprintf_s(buff, "< %i >", valor);
	int cont = 0;
	int cont2 = 0;
	for (int i = 35; i  <= 10000; i += 15)
	{
		cont2++;
		if (cont2 == index)
		{
			cont = (cont2 * 15);
			break;
		}
	}
	if (opcaoAtual == index)
	{
		g_Draw->DrawTextA(buff, Posicao_Menu[0] + (Posicao_Menu[2] - 4), Posicao_Menu[1] + cont, Cyano, 0);
		if (fast)
		{
			if (GetAsyncKeyState(VK_LEFT) == -32767)
			{
				if (valor <= min)
					valor = max;
				else
					valor -= step;
			}
			if (GetAsyncKeyState(VK_RIGHT) == -32767)
			{
				if (valor <= max)
					valor += step;
				else
					valor = min;
			}
		}
		else
		{
			if (IsKeyPressed(VK_LEFT, 0) & 1)
			{
				if (valor <= min)
					valor = max;
				else
					valor -= step;
			}
			if (IsKeyPressed(VK_RIGHT, 0) & 1)
			{
				if (valor <= max)
					valor += step;
				else
					valor = min;
			}
		}
	}
	else
		g_Draw->DrawTextA(buff, Posicao_Menu[0] + (Posicao_Menu[2] - 4), Posicao_Menu[1] + cont, Branco, 0);
}

void AddBoolOption(const char* Texto, int index, bool& valor)
{


	AddOption(Texto, index);
	const char* retorno;
	if (valor)
		retorno = "ON ";
	else
		retorno = "OFF ";
	int cont = 0;
	int cont2 = 0;
	for (int i = 35; i  <= 10000; i += 15)
	{
		cont2++;
		if (cont2 == index)
		{
			cont = (cont2 * 15);
			break;
		}
	}
	
	if (opcaoAtual == index)
	{
		g_Draw->DrawTextA(retorno, Posicao_Menu[0] + (Posicao_Menu[2] - 4), Posicao_Menu[1] + cont, valor ? Verde : Vermelho, 0);
		if (selecionou)
			valor = !valor;
	}
	else
		g_Draw->DrawTextA(retorno, Posicao_Menu[0] + (Posicao_Menu[2] - 4), Posicao_Menu[1] + cont, valor ? Verde : Vermelho, 0);
}

void AddSubMenuOption(const char* Texto, int index, int MenuAtual, int ProxMenu)
{
	 
	AddOption(Texto, index);
	const char* buff = ">>";
	int cont = 0;
	int cont2 = 0;
	for (int i = 35; i  <= 10000; i += 15)
	{
		cont2++;
		if (cont2 == index)
		{
			cont = (cont2 * 15);
			break;
		}
	}
	if (opcaoAtual == index)
	{
		g_Draw->DrawTextA(buff, Posicao_Menu[0] + (Posicao_Menu[2] - 4), Posicao_Menu[1] + cont, Cyano, 0);
		if (selecionou)
		{
			menuAtual = ProxMenu;
			menuAnterior = MenuAtual;
			opcaoAtual = 1;
		}
	}
	else
		g_Draw->DrawTextA(buff, Posicao_Menu[0] + (Posicao_Menu[2] - 4), Posicao_Menu[1] + cont, Branco, 0);
}



float teste = 0.000;
int teste2 = 1;
bool aaaaasdasdasd = false;





void MenuPrincipal()
{
	AddTitle("Menu Base by BresoDEV", 7, Principal);
	AddFloatOption("Float Lento", 1, teste, -20.0, 20.0, false, 1.0);
	AddFloatOption("Float Rapido", 2, teste, -20.0, 20.0, true, 2.0); 
	AddIntOption("Int Lento", 3, teste2, -100, 100, false, 1);
	AddIntOption("Int Rapido", 4, teste2, -100, 100, true, 1);
	AddBoolOption("Rolagem Rapida", 5, rolagemRapida);
	AddSubMenuOption("Settings", 6, Principal, Configuracoes);  
	AddBoolOption("Rolagem Radida", 7, aaaaasdasdasd);
	if (selecionou)
	{
		switch (opcaoAtual)
		{
		//case 1: Posicao_Menu[pt]--; break;
		//case 2:Posicao_Menu[pt]++; break;
		}
		selecionou = false;
	}
}

void MenuSettings()
{
	AddTitle("Configs.", 1, Configuracoes, Principal);
	AddSubMenuOption("Posicao", 1, Configuracoes, Posicao);
	if (selecionou)
	{
		//switch (opcaoAtual)
		//{
		//case 1: Posicao_Menu[pt]--; break;
		//case 2:Posicao_Menu[pt]++; break;
		//}
		selecionou = false;
	}
}

void MenuPosicao()
{
	AddTitle("Posicao", 4, Posicao, Configuracoes);
	AddIntOption("X", 1, Posicao_Menu[0], 0, 20000, true, 1);
	AddIntOption("Y", 2, Posicao_Menu[1], 0, 20000, true, 1);
	AddIntOption("Largura", 3, Posicao_Menu[2], 0, 20000, true, 1);
	AddIntOption("Altura", 4, Posicao_Menu[3], 0, 20000, true, 1);
}


//Loop mesmo fechado
void MenuManager::HackActions()
{


 
	//	if (GetAsyncKeyState(VK_LEFT) == -32767)
	//		pox[pt]--;
	//	if (GetAsyncKeyState(VK_RIGHT) == -32767)
	//		pox[pt]++;
	//	if (IsKeyPressed(VK_RETURN, 0) & 1)
	//	{
	//		if (pt == 3)
	//			pt = 0;
	//		else
	//			pt++;
	//	}
	// 
	//	
	//
	//
	//char buff[128];
	//sprintf_s(buff, "%i, %i, %i, %i", pox[0], pox[1], pox[2], pox[3]);
	//g_Draw->DrawBox(Posicao_Menu[0] + pox[0],
	//	Posicao_Menu[1] + pox[1],
	//	Posicao_Menu[2] + pox[2],
	//	Posicao_Menu[3] + pox[3],
	//	Vermelho); //Fundo
	//Posicao_Menu[0], Posicao_Menu[1], Posicao_Menu[2], Posicao_Menu[3]
	//g_Draw->DrawTextA(buff, 500, 500, Vermelho, 0);




}