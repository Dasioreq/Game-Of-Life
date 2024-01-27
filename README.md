General: 
  This is a (poorly made) recreation of Conway's Game of Life written in c++ with SDL2. The algorithm is based on the one from [here](https://www.youtube.com/watch?v=ndAfWKmKF34).
  Let's be real, the source code sucks, but I'll try to keep it updated and fix any issues. It (the src code) ***WILL NOT*** work without the used libraries installed properly.

Controls:
  LMB: Toggle cell state (Alive/dead)
  Space: Start/Stop simulation
  Right arrow: Advance to next simulation step
  Esc: Exit (duh)

Rules:
  1. If an alive cell has less than 2 or more than 3 alive neighbo(u)rs it dies
  2. If a dead cell has exactly 3 alive neighbo(u)rs it becomes alive 
