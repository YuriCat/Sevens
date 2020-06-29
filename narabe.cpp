#include "narabe.hpp"

using namespace std;


string move_string(const std::pair<int, int>& m)
{
    if (m.first < 0) return "PASS";
    return string(1, char('A' + m.first)) + to_string(m.second);
}

ostream& operator <<(ostream& os, const std::set<std::pair<int, int>>& ms)
{
    for (auto m : ms) os << " " << move_string(m);
    return os;
}

ostream& operator <<(ostream& os, const State& s)
{
    os << " ";
    for (int m = 0; m < M; m++) os << " " << m;
    os << endl;

    for (int k = 0; k < K; k++)
    {
        os << char('A' + k);
        for (int m = 0; m < M; m++)
        {
            auto card = make_pair(k, m);
            bool any = s.b.find(card) != s.b.end();
            bool black = s.h[0].find(card) != s.h[0].end();
            bool white = s.h[1].find(card) != s.h[1].end();
            os << " " << (any ? "*" : (black ? "b" : (white ? "w" : " ")));
        }
        os << endl;
    }
    return os;
}

int search(State& s, int p, bool agari = false, int alpha = -100, int beta = 100)
{
    if (agari)
    {
        if (s.h[s.color].size() == 0) return 1;
        if (s.h[1 - s.color].size() == 0) return -1;
    }
    if (s.pass[s.color] > p) return -1;
    if (s.pass[1 - s.color] > p) return 1;

    auto ms = s.moves();
    for (auto m : ms)
    {
        s.move(m);
        int result = -search(s, p, agari, -beta, -alpha);
        s.unmove(m);
        alpha = max(alpha, result);
        if (alpha >= beta) break;
    }
    return alpha;
}

int algo1r(const State& s, int k, int m = 0)
{
    auto card = make_pair(k, m);
    bool board = s.b.count(card);
    bool left = s.h[0].count(card);
    bool right = s.h[1].count(card);

    if (board || left || right)
    {
        int next = algo1r(s, k, m + 1);
        if (board) return next;
        if (left) return max(0, next + 1);
        if (right) return min(0, next - 1);
    }

    return 0;
}

int algo2(const State& s)
{
    int sum = 0;
    for (int k = 0; k < K; k++) sum += algo1r(s, k);
    int winner = (sum > 0 || (sum == 0 && s.color != 0)) ? 0 : 1;
    return s.color == winner ? 1 : -1;
}

int main()
{
    State s;
    for (int g = 0; g < 100000; g++)
    {
        s.clear();
        cerr << s;
        cerr << s.moves() << endl;
        cerr << search(s, 0) << " " << algo2(s) << endl;
        cerr << search(s, 0, true) << endl;
        if (search(s, 1) != search(s, 2)) getchar();
        if (search(s, 0, true) != search(s, 1, true)) getchar();
        if (search(s, 0) != algo2(s)) getchar();
    }
}