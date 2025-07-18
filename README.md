## Configurable game of Tic Tac Toe for two players or for playing with a bot made with ImGUI, GLFW and Vulkan

### What can be configured:
  * grid size (3x3, 5x5, 7x7)
  * two players or not
  * human starts the game or not
  * grid colour

### Other features:
  * the bot uses minmax algorithm with alpha-beta pruning
  * couple other optimizations like only checking the best empty cells and ordering them by most neighbours
  * the winner is highlighted yellow and there's a Game Over window with a Restart button
  * the system is very dynamic, so it can easily be configured for even bigger grids just by changing two config values
 
Run Setup-Windows.bat to create Visual Studio project files and solution.
