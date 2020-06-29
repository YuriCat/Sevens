#pragma once

#include <iostream>
#include <set>
#include <utility>


const int K = 3;
const int M = 9;

struct State
{
    std::set<std::pair<int, int>> b;
    int color;

    std::set<std::pair<int, int>> h[2];
    int pass[2];

    void clear()
    {
        b.clear();
        color = 0;
        for (int c = 0; c < 2; c++)
        {
            h[c].clear();
            pass[c] = 0;
        }

        for (int k = 0; k < K; k++) b.insert(std::make_pair(k, 0));

        for (int k = 0; k < K; k++) for (int m = 1; m < M; m++)
        {
            auto card = std::make_pair(k, m);
            int o = rand() % 3;
            if (o < 2) h[o].insert(card);
        }
    }

    bool legal(std::pair<int, int> m)
    {
        if (m.first < 0) return true;

        bool same = b.count(m);
        bool left = b.count(std::make_pair(m.first, m.second - 1));

        return !same && left;
    }

    std::set<std::pair<int, int>> moves()
    {
        std::set<std::pair<int, int>> ms;
        ms.insert(std::make_pair(-1, -1));

        for (auto m : h[color]) if (legal(m)) ms.insert(m);

        return ms;
    }

    void move(std::pair<int, int> m)
    {
        if (m.first < 0) pass[color] += 1;
        else
        {
            h[color].erase(m);
            b.insert(m);
        }
        color = 1 - color;
    }

    void unmove(std::pair<int, int> m)
    {
        color = 1 - color;
        if (m.first < 0) pass[color] -= 1;
        else
        {
            b.erase(m);
            h[color].insert(m);
        }
    }
};


std::string move_string(const std::pair<int, int>& m);
std::ostream& operator <<(std::ostream& os, const std::set<std::pair<int, int>>& ms);
std::ostream& operator <<(std::ostream& os, const State& s);
