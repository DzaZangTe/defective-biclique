std::vector<int> without(const std::vector<int> &S, const int w)
{
    std::set<int> set(S.begin(), S.end());
    set.erase(w);
    return std::vector<int>(set.begin(), set.end());
}

std::vector<int> without(const std::vector<int> &S, const std::vector<int> &T)
{
    std::set<int> set(S.begin(), S.end());
    for (int t : T)
        set.erase(t);
    return std::vector<int>(set.begin(), set.end());
}

std::vector<int> add(const std::vector<int> &S, const int w)
{
    std::set<int> set(S.begin(), S.end());
    set.insert(w);
    return std::vector<int>(set.begin(), set.end());
}

std::vector<int> intersect(const std::vector<int> &S, const std::vector<int> &T)
{
    std::set<int> set(S.begin(), S.end());
    std::set<int> res;
    for (int t : T)
        if (set.find(t) != set.end())
            res.insert(t);
    return std::vector<int>(res.begin(), res.end());
}

std::vector<int> neighbor(const int u)
{
    return std::vector<int>(edge[u].begin(), edge[u].end());
}

std::vector<int> neighbor(const std::vector<int> &S)
{
    std::set<int> set;
    for (int s : S)
        set.insert(edge[s].begin(), edge[s].end());
    return std::vector<int>(set.begin(), set.end());
}

std::vector<int> twoHopNeighbor(const int w)
{
    return without(neighbor(neighbor(w)), w);
}

bool in(const int u, const std::vector<int> &S)
{
    std::set<int> set(S.begin(), S.end());
    return set.find(u) != set.end();
}

int ndegree(const int u, const std::vector<int> &S)
{
    int res = 0;
    for (int v : S)
        if (edge[u].find(v) == edge[u].end())
            ++res;
    return res;
}