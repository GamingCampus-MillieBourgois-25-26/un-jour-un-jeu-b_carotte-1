#pragma once

class Game
{
public:
    void Run(int argc, const char** argv);

private:
    void Init(int argc, const char** argv);
    void LoadScene();
};