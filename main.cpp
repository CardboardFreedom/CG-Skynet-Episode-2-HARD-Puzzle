#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;

int secondNode = -1;
int firstNode = -1;

class node
{
public:

    node()
    {}

    int index;
    bool exit = false;
    vector<node*> parent;

    vector<int> connected;
};


void search(vector<node> &nodes, int index)
{
    int gates = 0;
    list<pair<node*, pair<int, bool>>> queue;
    pair<int, bool> level = {0, false};

    cerr << "ENTERED SEARCH:\n";

    queue.push_back({&nodes[index], level});

    auto qwe = queue.front();
    
    for(auto &i:qwe.first->connected)
    {
        if(nodes[i].exit)
        {
            secondNode = qwe.first->index;
            firstNode = nodes[i].index;

            auto sev = &nodes[secondNode].connected;
            auto search = find(sev->begin(), sev->end(), firstNode);

            sev->erase(search);

            sev = &nodes[firstNode].connected;
            search = find(sev->begin(), sev->end(), secondNode);

            sev->erase(search);
            return;
        }
    }

    queue.pop_front();

    for(auto &i : qwe.first->connected)
    {
        if(nodes[i].parent.size()) continue;
        queue.push_back({&nodes[i], {qwe.second.first+1, qwe.second.second}});
        nodes[i].parent.push_back(qwe.first);
    }

    bool first = true;

    while (!queue.empty())
    {
        auto check = queue.front();
        if(check.second.first != level.first)
            level.first++;

        if(check.first->exit || (level.first > 2 && secondNode != -1 && !check.second.second))
        {
            queue.pop_front();
            continue;
        }

        int temp = 0;

        int fN, sN;

        for(auto &i:check.first->connected)
        {
            if(nodes[i].exit)
            {
                fN = nodes[i].index;
                sN = check.first->index;
                temp++;
            }
        }

        if(!temp)
            check.second.second = false;
        else if(temp && check.second.first == 1)
        {
            check.second.second = true;
            first = false;
        }

        if(temp > gates || (temp == gates && check.second.second))
        {
            cerr << "\nGREATER HERE!\n";
            gates = temp;
            firstNode = fN;
            secondNode = sN;

            if(temp >= 2 && check.second.second)
            {
                cerr << "PLEASE NO FATHER\n";

                auto sev = &nodes[secondNode].connected;
                auto search = find(sev->begin(), sev->end(), firstNode);

                sev->erase(search);

                sev = &nodes[firstNode].connected;
                search = find(sev->begin(), sev->end(), secondNode);

                sev->erase(search);
                return;
            }
        }

        queue.pop_front();

        for(auto &i : check.first->connected)
        {
            if(nodes[i].parent.size())
                continue;

            queue.push_back({&nodes[i], {check.second.first+1, check.second.second}});
            nodes[i].parent.push_back(check.first);
        }
    }


    auto sev = &nodes[secondNode].connected;
    auto search = find(sev->begin(), sev->end(), firstNode);

    sev->erase(search);

    sev = &nodes[firstNode].connected;
    search = find(sev->begin(), sev->end(), secondNode);

    sev->erase(search);
}


int main()
{
    int N; // the total number of nodes in the level, including the gateways
    int L; // the number of links
    int E; // the number of exit gateways
    cin >> N >> L >> E;

    vector<node> nodes;
    nodes.resize(N);

    int g = 0;

    for(auto &i:nodes)
    {
        i.index = g;
        g++;
    }

    for (int i = 0; i < L; i++)
    {
        int N1; // N1 and N2 defines a link between these nodes
        int N2;
        cin >> N1 >> N2;

        nodes[N1].connected.push_back(N2);
        nodes[N2].connected.push_back(N1);
    }
    for (int i = 0; i < E; i++)
    {
        int EI; // the index of a gateway node
        cin >> EI;

        nodes[EI].exit = true;
    }

    // game loop
    while (1)
    {
        int SI; // The index of the node on which the Skynet agent is positioned this turn
        cin >> SI;

        search(nodes, SI);

        cout << firstNode << ' ' << secondNode << '\n';

        firstNode = -1;
        secondNode = -1;

        for(auto &i:nodes)
            i.parent.clear();
    }
}
