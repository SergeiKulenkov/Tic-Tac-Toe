#include "App/App.h"
#include "Game/Game.h"

int main()
{
    Game::Init("Tic Tac Toe");
    Game::Get().Run();
    Game::Shutdown();

    return 0;
}
