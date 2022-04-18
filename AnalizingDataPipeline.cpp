/*
    Problema 3
     Sofia Neves - 2019220082
     Tatiana Almeida - 2019219581
*/

#include <bits/stdc++.h>
using namespace std;
vector<vector<bool>> adjacent_matrix;
vector<int> vector_, time_, aux;
vector<bool> visited, stack_;
priority_queue<int, vector<int>, greater<int>> pq;

int first;

int number_of_tasks;

bool one_initial_task()
{
    int counter = 0;
    bool aux = false;
    for (int i = 1; i <= number_of_tasks; ++i)
    {
        aux = false;
        for (int j = 1; j <= number_of_tasks; ++j)
        {
            if (adjacent_matrix[i][j] == true)
            {
                aux = true;
                break;
            }
        }
        if (!aux)
        {
            first = i;
            counter++;
        }
    }

    if (counter != 1)
    {
        return false;
    }
    return true;
}

bool one_final_task()
{
    int counter = 0;
    bool aux = false;
    for (int i = 1; i <= number_of_tasks; ++i)
    {
        aux = false;
        for (int j = 1; j <= number_of_tasks; ++j)
        {
            if (adjacent_matrix[j][i] == true)
            {
                aux = true;
                break;
            }
        }
        if (!aux)
        {
            counter++;
        }
    }

    if (counter != 1)
    {
        return false;
    }
    return true;
}

bool pipeline_connected()
{
    if (number_of_tasks != 1)
    {
        int counter = 0;
        bool aux = false, aux2 = false;
        for (int i = 1; i <= number_of_tasks; ++i)
        {
            aux = false;
            aux2 = false;
            for (int j = 1; j <= number_of_tasks; ++j)
            {
                if (adjacent_matrix[j][i] == true)
                    aux = true;

                if (adjacent_matrix[i][j] == true)
                    aux2 = true;
            }
            if (!aux && !aux2)
            {
                counter++;
            }
        }

        if (counter != 0)
        {
            return false;
        }
    }
    return true;
}

bool dfs(int v)
{
    if (visited[v] == false)
    {
        // Mark the current node as visited and part of recursion stack
        visited[v] = true;
        stack_[v] = true;

        // Recur for all the vertices adjacent to this vertex
        for (int i = 1; i <= number_of_tasks; ++i)
        {
            if (adjacent_matrix[i][v] == true)
            {
                if (!visited[i] && dfs(i))
                    return true;
                else if (stack_[i])
                    return true;
            }
        }
    }
    stack_[v] = false; // remove the vertex from recursion stack
    return false;
}

bool acyclic_pipeline()
{
    visited = vector<bool>(number_of_tasks + 1, false);
    stack_ = vector<bool>(number_of_tasks + 1, false);

    return !dfs(first);
}

bool check_validity_of_pipeline()
{
    if (pipeline_connected() && one_initial_task() && one_final_task() && acyclic_pipeline())
    {
        return true;
    }
    return false;
}

bool check_if_parents_are_completed(int n)
{
    aux = vector<int>();
    for (int i = 1; i <= number_of_tasks; ++i)
    {
        if (adjacent_matrix[n][i] == true)
        {
            aux.push_back(i);
        }
    }

    unsigned long int count = 0;
    for (unsigned long int i = 0; i < aux.size(); ++i)
    {
        if (visited[aux[i]] == true)
        {
            count++;
        }
    }

    if (count == aux.size())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void print_childs(int n, int stat)
{
    if (check_if_parents_are_completed(n))
    {
        if (stat == 1 || (stat == 3 && stack_[n] == false))
        {
            pq.pop();
            visited[n] = true;
            cout << n << endl;
        }

        // call childs
        for (int i = 1; i <= number_of_tasks; ++i)
        {
            if (adjacent_matrix[i][n] == true && visited[i] == false && check_if_parents_are_completed(i))
            {
                pq.push(i);
            }
        }

        if (!pq.empty())
        {
            print_childs(pq.top(), stat);
        }
    }
}

void mininum_amount_of_time_AND_sequence()
{

    visited = vector<bool>(number_of_tasks + 1, false);

    cout << accumulate(time_.begin(), time_.end(), 1) << endl;

    visited[first] = true;
    cout << first << endl;

    // call childs
    for (int i = 1; i <= number_of_tasks; ++i)
    {
        if (adjacent_matrix[i][first] == true && check_if_parents_are_completed(i))
        {
            pq.push(i);
        }
    }

    print_childs(pq.top(), 1);
}

void minimum_amount_of_time()
{
}

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

    cout << first << endl;
    print_childs(first, 3);
}

int main()
{
    string line, aux;
    int number, statistic;

    while (getline(cin, aux))
    {
        stringstream ss_aux(aux);
        ss_aux >> number_of_tasks;

        time_ = vector<int>(number_of_tasks + 1, -1);
        adjacent_matrix = vector<vector<bool>>(number_of_tasks + 1, vector<bool>(number_of_tasks + 1, false));

        for (int i = 1; i <= number_of_tasks; ++i)
        {
            getline(cin, line);
            stringstream ss(line);

            ss >> number;
            time_[i] = number;

            ss >> number;
            while (ss >> number)
            {
                adjacent_matrix[i][number] = true;
            }
        }

        cin >> statistic;

        if (check_validity_of_pipeline())
        {
            cout << "first: " << first << endl;
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