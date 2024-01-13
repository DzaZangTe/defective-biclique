#include <cstdio>
#include <queue>
#include <vector>
#include <algorithm>
#include <set>

int n1,n2, m, k, ansSize;
std::vector< std::set<int> > edge, edge0, ans;
std::vector<int> order, inv;

#include "set.hpp"

void ordering(void)
{
    order = std::vector<int>(n1 + n2, -1);
    inv.resize(n1 + n2);
    edge0.resize(n1 + n2);
    std::priority_queue< std::pair<int, int> > pq;
    for (int i = 0; i < n1; ++i)
        pq.push(std::make_pair(-int(edge[i].size()), i));
    for (int i = 0; i < n2; ++i)
        pq.push(std::make_pair(-int(edge[i + n1].size()), i + n1));

    int idx = 0;
    while (!pq.empty()) {
        int p = pq.top().second;
        pq.pop();
        if (order[p] != -1)
            continue;
        order[p] = idx++;
        for (int q : edge[p]) {
            edge[q].erase(p);
            edge0[q].insert(p);
            pq.push(std::make_pair(-int(edge[q].size()), q));
        }
    }

    for (int i = 0; i < n1 + n2; ++i) {
        inv[order[i]] = i;
        edge0[i].insert(edge[i].begin(), edge[i].end());
        edge[i].clear();
    }
    for (int i = 0; i < n1 + n2; ++i) {
        for (int u : edge0[i]) {
            edge[order[i]].insert(order[u]);
            edge[order[u]].insert(order[i]);
        }
    }
}

void output(const std::vector<int> &RU, const std::vector<int> &RV, const int k)
{
    if (int(RU.size()) <= ::k || int(RV.size()) <= ::k)
        return;
    if (int(RU.size() * RV.size()) - ::k + k > ansSize) {
        ansSize = int(RU.size() * RV.size()) - ::k + k;
        ans.clear();
    }
    if (int(RU.size() * RV.size()) - ::k + k == ansSize) {
        std::set<int> tmp;
        for (int ru : RU)
            tmp.insert(inv[ru]);
        for (int rv : RV)
            tmp.insert(inv[rv]);
        ans.emplace_back(tmp);
    }
}

void update(const std::vector<int> &RU, const std::vector<int> &RV, const int u,
    std::vector<int> &CU, std::vector<int> &CV, const int k)
{
    std::vector<int> CU_, CV_;
    for (int v : CV)
        if (ndegree(v, add(RU, u)) <= k)
            CV_ = add(CV_, v);
    std::vector<int> range(without(CU, u));
    for (int u : range)
        if (ndegree(u, RV) <= k)
            CU_ = add(CU_, u);
    CU = CU_;
    CV = CV_;
}

int findpivot(const std::vector<int> &CU, const std::vector<int> &RV)
{
    for (int u : CU)
        if (ndegree(u, RV) == 0)
            return u;
    return -1;
}

void dfs(const std::vector<int>, const std::vector<int>,
    std::vector<int>, std::vector<int>,
    std::vector<int>, std::vector<int>, const int);
void branch(const std::vector<int>, const std::vector<int>, const int,
    std::vector<int>, std::vector<int>,
    std::vector<int>, std::vector<int>, const int);

void dfs(const std::vector<int> RU, const std::vector<int> RV,
    std::vector<int> CU, std::vector<int> CV,
    std::vector<int> XU, std::vector<int> XV,
    const int k)
{
    if (CU.size() == 0 && CV.size() == 0) {
        if (XU.size() == 0 && XV.size() == 0)
            output(RU, RV, k);
        return;
    }

    if (int((RU.size()+CU.size()) * (RV.size()+CV.size())) - ::k + k < ansSize)
        return;

    int pu = findpivot(CU, RV);
    if (pu != -1) {
        if (k - ndegree(pu, RV) >= 0 && in(pu, CU)) {
            branch(RU, RV, pu, without(CU, pu), CV,
                XU, XV, k - ndegree(pu, RV));
            CU = without(CU, pu);
            XU = add(XU, pu);
        }
        std::vector<int> CV_ = without(CV, neighbor(pu));
        for (int w : CV_) {
            if (k - ndegree(w, RU) >= 0 && in(w, CV)) {
                branch(RV, RU, w, without(CV, w), CU,
                    XV, XU, k - ndegree(w, RU));
                CV = without(CV, w);
                XV = add(XV, w);
            }
        }
    } else {
        std::vector<int> CU_ = CU;
        std::vector<int> CV_ = CV;
        for (int w : CU_) {
            if (k - ndegree(w, RV) >= 0 && in(w, CU)) {
                branch(RU, RV, w, without(CU, w), CV,
                    XU, XV, k - ndegree(w, RV));
                CU = without(CU, w);
                XU = add(XU, w);
            }
        }
        for (int w : CV_) {
            if (k - ndegree(w, RU) >= 0 && in(w, CV)) {
                branch(RV, RU, w, without(CV, w), CU,
                    XV, XU, k - ndegree(w, RU));
                CV = without(CV, w);
                XV = add(XV, w);
            }
        }
    }
}

void branch(const std::vector<int> RU, const std::vector<int> RV, const int u,
    std::vector<int> CU, std::vector<int> CV,
    std::vector<int> XU, std::vector<int> XV,
    const int k)
{
    update(RU, RV, u, CU, CV, k);
    update(RU, RV, u, XU, XV, k);
    dfs(add(RU, u), RV, CU, CV, XU, XV, k);
}

void search(void)
{
    std::vector<int> after(n1 + n2), before;
    for (int i = 0; i < n1 + n2; ++i)
        after[i] = i;
    for (int i = 0; i < n1 + n2; ++i) {
        after = without(after, i);
        if (inv[i] < n1) {
            std::vector<int> CU(intersect(after, twoHopNeighbor(i)));
            std::vector<int> CV(neighbor(add(CU, i)));
            std::vector<int> XU(intersect(before, twoHopNeighbor(i)));
            dfs(std::vector<int>{i}, std::vector<int>(), CU, CV,
                XU, std::vector<int>(), k);
        }
        before.emplace_back(i);
    }
}

int main(void)
{
    freopen("data.in", "r", stdin);
    freopen("data.out", "w", stdout);
    scanf("%d%d%d%d", &n1, &n2, &m, &k);
    edge.resize(n1 + n2);
    for (int i = 0; i < m; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        edge[u].insert(v + n1);
        edge[v + n1].insert(u);
    }
    ordering();
    search();
    for (auto a : ans) {
        int s = 0;
        for (int u : a) {
            printf("%d, ", u);
            for (int v : a)
                if (edge[order[u]].find(order[v]) != edge[order[u]].end())
                    ++s;
        }
        printf("sum = %d\n", s / 2);
    }
    return 0;
}