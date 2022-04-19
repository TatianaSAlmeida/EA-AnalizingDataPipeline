/*
    Problema 3
     Sofia Neves - 2019220082
     Tatiana Almeida - 2019219581
*/

#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> parents, childs;
vector<int> time_, dp, degree;
vector<bool> visited, stack_;
priority_queue<int, vector<int>, greater<int>> pq;

int first;

int number_of_tasks;

bool check_if_one_initial_task()
{
    int counter_initial_tasks = 0;
    for (int i = 1; i <= number_of_tasks; ++i)
    {
        if (parents[i].size() == 0)
        {
            first = i;
            counter_initial_tasks++;
        }
    }
    return counter_initial_tasks == 1;
}

bool check_if_one_final_task()
{
    int counter_final_tasks = 0;
    for (int i = 1; i <= number_of_tasks; ++i)
    {
        // if a node has no childs
        if (childs[i].size() == 0)
        {
            counter_final_tasks++;
        }
    }
    return counter_final_tasks == 1;
}

bool dfs(int v)
{
    if (visited[v] == false)
    {
        // Mark the current node as visited and part of recursion stack
        visited[v] = true;
        stack_[v] = true;

        // Recur for all the vertices adjacent to this vertex
        for (unsigned long int i = 0; i < childs[v].size(); ++i)
        {

            if (!visited[childs[v][i]] && dfs(childs[v][i]))
                return true;
            else if (stack_[childs[v][i]])
                return true;
        }
    }
    stack_[v] = false; // remove the vertex from recursion stack
    return false;
}

bool acyclic_and_connected_pipeline()
{
    visited = vector<bool>(number_of_tasks + 1, false);
    stack_ = vector<bool>(number_of_tasks + 1, false);

    if (dfs(first))
    {
        return false;
    }

    for (int i = 1; i <= number_of_tasks; ++i)
    {
        if (!visited[i])
        {
            return false;
        }
    }

    return true;
}

bool check_validity_of_pipeline()
{
    return check_if_one_final_task() && check_if_one_initial_task() && acyclic_and_connected_pipeline();
}

void print_childs(int n, int stat)
{

    if (stat == 1 || (stat == 3 && stack_[n] == false))
    {
        pq.pop();
        visited[n] = true;
        cout << n << endl;
    }

    if (stat == 3 && stack_[n] == true)
    {
        visited[n] = true;
        pq.pop();
    }

    // call childs
    for (unsigned long int i = 0; i < childs[n].size(); ++i)
    {
        int visit = childs[n][i];
        degree[visit]++;

        if (visited[visit] == false && (degree[visit] == (int)parents[visit].size()))
        {
            pq.push(visit);
        }
    }

    if (!pq.empty())
    {
        print_childs(pq.top(), stat);
    }
}

void mininum_amount_of_time_AND_sequence()
{

    degree = vector<int>(number_of_tasks + 1, 0);
    visited = vector<bool>(number_of_tasks + 1, false);
    visited[first] = true;

    cout << accumulate(time_.begin(), time_.end(), 1) << endl;
    cout << first << endl;

    // call childs
    for (unsigned long int i = 0; i < childs[first].size(); ++i)
    {
        degree[childs[first][i]]++;

        if (degree[childs[first][i]] == (int)parents[childs[first][i]].size())
        {
            pq.push(childs[first][i]);
        }
    }

    print_childs(pq.top(), 1);
}

void recursion(int node)
{
    bool leaf = true;
    for (unsigned long int i = 0; i < childs[node].size(); ++i)
    {
        leaf = false;

        if (dp[childs[node][i]] == -1)
        {
            recursion(childs[node][i]);
        }
    }

    if (leaf)
    {
        dp[node] = time_[node];
        return;
    }

    int max_time = 0;
    for (unsigned long int i = 0; i < childs[node].size(); ++i)
    {
        if (max_time < dp[childs[node][i]])
        {
            max_time = dp[childs[node][i]];
        }
    }
    dp[node] = max_time + time_[node];
    return;
}

void minimum_amount_of_time()
{
    dp = vector<int>(number_of_tasks + 1, -1);
    recursion(first);
    cout << dp[first] << endl;
}

/*
void bottleneck()
{
    int first_child = 0;
    bool aux = false;

    visited = vector<bool>(number_of_tasks + 1, false);
    stack_ = vector<bool>(number_of_tasks + 1, false);
    for (int i = 1; i <= number_of_tasks; ++i)
    {
        first_child = 0;
        aux = false;
        for (int j = 1; j <= number_of_tasks; ++j)
        {
            if (adjacent_matrix[i][j] == true)
            {
                if (first_child != 0)
                {
                    aux = true;
                    stack_[first_child] = true;
                }
                first_child = j;
            }
        }

        if (aux)
        {
            stack_[first_child] = true;
        }
    }

    // PRINT STACK
    for (unsigned long int i = 0; i < stack_.size(); ++i)
    {
        cout << stack_[i] << " ";
    }
    cout << endl
         << endl;

    visited[first] = true;
    // call childs
    for (int i = 1; i <= number_of_tasks; ++i)
    {
        if (adjacent_matrix[i][first] == true && check_if_parents_are_completed(i))
        {
            pq.push(i);
        }
    }

    cout << first << endl;
    print_childs(pq.top(), 3);
}
*/
int main()
{
    int number, statistic, number_of_parents;

    cin >> number_of_tasks;

    time_ = vector<int>(number_of_tasks + 1, -1);
    parents = vector<vector<int>>(number_of_tasks + 1, vector<int>());
    childs = vector<vector<int>>(number_of_tasks + 1, vector<int>());

    for (int i = 1; i <= number_of_tasks; ++i)
    {

        cin >> number;
        time_[i] = number;

        cin >> number_of_parents;

        for (int j = 0; j < number_of_parents; ++j)
        {
            cin >> number;
            parents[i].push_back(number);
            childs[number].push_back(i);
        }
    }
    cin >> statistic;

    if (check_validity_of_pipeline())
    {
        switch (statistic)
        {
        case 0:
            cout << "VALID" << endl;
            break;
        case 1:
            mininum_amount_of_time_AND_sequence();
            break;
        case 2:
            minimum_amount_of_time();
            break;
        case 3:
            bottleneck();
            break;
        }
    }
    else
    {
        cout << "INVALID" << endl;
    }

    /*
    cout << "-----------------------------TIME" << endl;
    for (int i = 1; i <= number_of_tasks; ++i)
    {
        cout << time_[i] << " ";
    }
    cout << endl;

    cout << "ADJACENT MATRIX " << endl;
    for (int i = 1; i <= number_of_tasks; i++)
    {
        cout << "NODE NUMBER: " << i << endl;
        for (int j = 1; j <= number_of_tasks; ++j)
        {
            cout << adjacent_matrix[i][j] << " ";
        }
        cout << endl
             << endl;
    }
    cout << statistic << endl;
    */

    return 0;
}