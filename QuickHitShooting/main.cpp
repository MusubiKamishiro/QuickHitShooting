#include "Game.h"
#ifdef _DEBUG
int main(void)
{
#else
#include <Windows.h>
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
#endif
	Game& game = Game::Instance();
	game.Initialize();
	game.Run();
	game.Terminate();
}
