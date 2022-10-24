#include "function.h"
using namespace std;

float distanceCheck(vector<int> route, vector<vector<float>> &distMatrix)
{
    float totaldistance = 0;
    for (int j = 0, k = 1; j < route.size() - 1; j++, k++)
    {
        totaldistance += distMatrix[route[j]][route[k]];
    }
    return totaldistance;
}

int checkCapacity(vector<int> route, vector<vector<int>> &customerdata)
{
    int capacity = 0;
    for (int i = 0; i < route.size(); i++)
    {
        capacity += customerdata[route[i]][3];
    }
    return capacity;
}
float objfunc(vector<vector<int>> &customerdata, vector<vector<float>> &distMatrix, truck *truckarr, int vehicle, vector<float> distanceofEachfleet, int externalTransportationCost)
{
    float totalvariablecost = 0;
    for (int x = 0; x < vehicle; x++)
    {
        totalvariablecost += distanceofEachfleet[x] * truckarr[x].variable_cost;
    }
    int totalfixedcostofTruck = 0;
    for (int x = 0; x < vehicle; x++)
    {
        totalfixedcostofTruck += truckarr[x].fixed_cost;
    }
    float costofInternalfleet = totalfixedcostofTruck + totalvariablecost;
    float objvalue = 0;
    objvalue = costofInternalfleet + externalTransportationCost;
    return objvalue;
}

int findMinFromMatrix(vector<vector<float>> matrix)
{
    float minm = matrix[0][0];
    int minIndex = 0;

    // Run the inner loop for columns
    for (int j = 0; j < matrix.size(); j++)
    {

        // check if any element is smaller
        // than the minimum element of the column
        // and replace it
        if (matrix[j][0] < minm)
        {
            minm = matrix[j][0];
            minIndex = j;
        }
    }
    return minIndex;
}

int findMaxFromMatrix(vector<vector<float>> matrix)
{
    float max = matrix[0][0];
    int maxIndex = 0;

    // Run the inner loop for columns
    for (int j = 0; j < matrix.size(); j++)
    {

        // check if any element is smaller
        // than the minimum element of the column
        // and replace it
        if (matrix[j][0] > max)
        {
            max = matrix[j][0];
            maxIndex = j;
        }
    }
    return maxIndex;
}

sol VRPPC::adjacentSwapping(vector<vector<int>> &customerdata, vector<vector<float>> &distMatrix, truck *truckarr, int externalTransportationCost, sol s1)
{
    sol s;
    vector<int> routeCopy;
    vector<int> routeCopy1;
    vector<vector<int>> route(vehicle);
    route = s1.vehicleroute;
    vector<float> prevRouteDistances;
    vector<float> newRouteDistances;
    prevRouteDistances = s1.routeDistance;
    cout << "\nRoute Distances before applying Adjacent" << endl;
    for (float x : prevRouteDistances)
    {
        cout << x << "\t";
    }
    int var = 0;
    float distance = 0, distance_1 = 0;
    for (int x = 0; x < vehicle; x++)
    {
        // cout << "\n ----------------------------------------------------------------\n";
        routeCopy = route[x];
        for (int i = 1; i < route[x].size() - 2; i++)
        {
            var = routeCopy[i];
            routeCopy[i] = routeCopy[i + 1];
            routeCopy[i + 1] = var;
            distance = distanceCheck(routeCopy, distMatrix);
            if (distance < prevRouteDistances[x])
            {
                cout << distance << "----------" << prevRouteDistances[x] << endl;
                route[x] = routeCopy;
                prevRouteDistances[x] = distance;
            }
            else
            {
                routeCopy = route[x];
            }
        }
        // distance_1 = distanceCheck(route[x], distMatrix);
        cout << "Final route distance " << prevRouteDistances[x] << endl;
        newRouteDistances.push_back(prevRouteDistances[x]);
    }
    float objvalue = objfunc(customerdata, distMatrix, truckarr, vehicle, newRouteDistances, externalTransportationCost);
    // cout << "objective value after adjacent swapping " << objvalue << endl;
    s.vehicleroute = route;
    s.initcost = objvalue;
    s.routeDistance = newRouteDistances;
    s.ext_transportcost = s1.ext_transportcost;
    s.notvisited = s1.notvisited;
    s.remcapacityofvehicle = s1.remcapacityofvehicle;
    s.visited = s1.visited;
    cout << "\nRoute Distances after applying Adjacent" << endl;
    for (float x : s.routeDistance)
    {
        cout << x << "\t";
    }
    return s;
}

sol VRPPC::one_oneSwapping(vector<vector<int>> &customerdata, vector<vector<float>> &distMatrix, truck *truckarr, int externalTransportationCost, sol s1)
{
    sol s;
    vector<int> routeCopy;
    vector<int> routeCopy1;
    vector<vector<int>> route(vehicle);
    route = s1.vehicleroute;
    int var;
    float distance = 0, distance_1 = 0;
    vector<float> prevRouteDistances;
    prevRouteDistances = s1.routeDistance;
    vector<float> newRouteDistances;
    vector<float> storeDist;
    for (int x = 0; x < vehicle; x++)
    {
        routeCopy = route[x];
        if (route[x].size() == 3)
        {
            newRouteDistances.push_back(prevRouteDistances[x]);
            continue;
        }
        for (int i = 1; i < route[x].size() - 1; i++)
        {
            for (int j = 1; j < route[x].size() - 1; j++)
            {
                if (i == j)
                {
                    continue;
                }
                var = routeCopy[i];
                routeCopy[i] = routeCopy[j];
                routeCopy[j] = var;
                distance = distanceCheck(routeCopy, distMatrix);
                storeDist.push_back(distance);
                routeCopy = route[x];
            }

            float min = *min_element(storeDist.begin(), storeDist.end());
            int min_index = min_element(storeDist.begin(), storeDist.end()) - storeDist.begin();
            if (min < prevRouteDistances[x])
            {
                var = route[x][i];
                route[x][i] = route[x][min_index + 2];
                route[x][min_index + 2] = var;
                prevRouteDistances[x] = min;
            }
            storeDist.clear();
        }
        // distance_1 = distanceCheck(route[x], distMatrix);
        newRouteDistances.push_back(prevRouteDistances[x]);
    }
    float objvalue = objfunc(customerdata, distMatrix, truckarr, vehicle, newRouteDistances, externalTransportationCost);
    s.vehicleroute = route;
    s.initcost = objvalue;
    s.routeDistance = newRouteDistances;
    s.ext_transportcost = s1.ext_transportcost;
    s.notvisited = s1.notvisited;
    s.remcapacityofvehicle = s1.remcapacityofvehicle;
    s.visited = s1.visited;
    cout << "\nRoute Distances after applying One-One Swapping" << endl;
    for (float x : s.routeDistance)
    {
        cout << x << "\t";
    }
    return s;
}

sol VRPPC::singleInsertionSwapping(vector<vector<int>> &customerdata, vector<vector<float>> &distMatrix, truck *truckarr, int externalTransportationCost, sol s1)
{
    sol s;
    vector<int> routeCopy;
    vector<int> routeCopy1;
    vector<vector<int>> route(vehicle);
    route = s1.vehicleroute;
    int var;
    float distance = 0, distance_1 = 0;
    vector<float> prevRouteDistances;
    prevRouteDistances = s1.routeDistance;
    vector<float> newRouteDistances;
    vector<float> storeDist;

    vector<vector<int>> routesAfterInsert;
    for (int x = 0; x < vehicle; x++)
    {
        if (route[x].size() == 3)
        {
            newRouteDistances.push_back(prevRouteDistances[x]);
            continue;
        }
        for (int i = 1; i < route[x].size() - 1; i++)
        {
            routeCopy = route[x];
            for (int j = 0; j < route[x].size() - 1; j++)
            {
                if (j <= i - 2)
                {
                    routeCopy.insert(routeCopy.begin() + j + 1, routeCopy[i]);
                    routeCopy.erase(routeCopy.begin() + (i + 1));
                }
                if (j == i - 1)
                {
                    continue;
                }
                if (j == i)
                {
                    continue;
                }
                if (j >= i + 1)
                {
                    routeCopy.insert(routeCopy.begin() + j + 1, routeCopy[i]);
                    routeCopy.erase(routeCopy.begin() + (i));
                }
                routesAfterInsert.push_back(routeCopy);
                distance = distanceCheck(routeCopy, distMatrix);
                storeDist.push_back(distance);
                routeCopy = route[x];
            }
            float min = *min_element(storeDist.begin(), storeDist.end());
            int min_index = min_element(storeDist.begin(), storeDist.end()) - storeDist.begin();
            if (min < prevRouteDistances[x])
            {
                prevRouteDistances[x] = min;
                route[x] = routesAfterInsert[min_index];
            }
            storeDist.clear();
            routesAfterInsert.clear();
        }
        // distance_1 = distanceCheck(route[x], distMatrix);
        newRouteDistances.push_back(prevRouteDistances[x]);
    }
    float objvalue = objfunc(customerdata, distMatrix, truckarr, vehicle, newRouteDistances, externalTransportationCost);
    // cout << "objective value after Single-Insertion swapping " << objvalue << endl;
    // cout << "Routes after 3 operations :\n";
    // for (int i = 0; i < route.size(); i++)
    // {
    //     for (int j = 0; j < route[i].size(); j++)
    //     {
    //         cout << route[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    s.vehicleroute = route;
    s.initcost = objvalue;
    s.routeDistance = newRouteDistances;
    s.ext_transportcost = s1.ext_transportcost;
    s.notvisited = s1.notvisited;
    s.remcapacityofvehicle = s1.remcapacityofvehicle;
    s.visited = s1.visited;
    cout << "\nRoute Distances after applying Single Insertion" << endl;
    for (float x : s.routeDistance)
    {
        cout << x << "\t";
    }
    return s;
}

sol VRPPC::externalNodeInsertion(vector<vector<int>> &customerdata, vector<vector<float>> &distMatrix, truck *truckarr, int externalTransportationCost, sol s1)
{
    sol s;
    vector<int> routeCopy;
    vector<int> routeCopy1;
    vector<vector<int>> route(vehicle);
    route = s1.vehicleroute;
    int var;
    float distance = 0, distance_1 = 0;
    vector<float> prevRouteDistances, prevRouteDistances1;
    prevRouteDistances = s1.routeDistance;
    prevRouteDistances1 = s1.routeDistance;
    vector<int> remcap;
    remcap = s1.remcapacityofvehicle;
    vector<float> newRouteDistances;
    vector<float> storeDist;
    vector<int> recordOfInsertedNodes;
    vector<vector<float>> newRouteDistances2d(vehicle, vector<float>(s1.notvisited.size(), 0));
    int index, flag = 0;
    int totalCapacity;

    vector<int> track1;   // track of nodes
    vector<float> track2; // track of min distances
    vector<int> track3;   // track of route
    float randMultiplier = 1.05 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.2 - 1.05)));
    for (int k = 0; k < s1.notvisited.size(); k++)
    {
        // cout << "Not visited iteration " << k << endl;
        for (int i = 0; i < route.size(); i++)
        {
            // cout << "no of route iteration " << i << endl;
            routeCopy = route[i];
            for (int j = 0; j < route[i].size() - 1; j++)
            {
                // cout << "Inside route iteration" << endl;
                routeCopy.insert(routeCopy.begin() + j + 1, s1.notvisited[k]);
                totalCapacity = checkCapacity(routeCopy, customerdata);
                if (totalCapacity <= truckarr[i].capacity)
                {

                    distance = distanceCheck(routeCopy, distMatrix) - customerdata[s1.notvisited[k]][4];
                    storeDist.push_back(distance);
                    routeCopy.erase(routeCopy.begin() + j + 1);
                }
                else
                {
                    distance = 999999;
                    storeDist.push_back(distance);
                    routeCopy.erase(routeCopy.begin() + j + 1);
                    continue;
                }
            }
            float min = *min_element(storeDist.begin(), storeDist.end());
            int min_index = min_element(storeDist.begin(), storeDist.end()) - storeDist.begin();
            if (min < (prevRouteDistances[i]) * randMultiplier)
            {
                // cout << "Condition : TRUE" << endl;
                flag = 1;
                track1.push_back(min_index);
                track2.push_back(min);
                track3.push_back(i);
                prevRouteDistances[i] = min;
            }
            else
            {
                // cout << "Condition : FALSE" << endl;
                storeDist.clear();
                continue;
            }
            storeDist.clear();
        }
        // cout << "Out of two for loop" << endl;
        if (flag == 1)
        {
            // cout << "HIBRU" << endl;
            int minIndex_track2 = min_element(track2.begin(), track2.end()) - track2.begin();
            route[track3[minIndex_track2]].insert(route[track3[minIndex_track2]].begin() + (track1[minIndex_track2] + 1), s1.notvisited[k]);
            distance_1 = distanceCheck(route[track3[minIndex_track2]], distMatrix);
            newRouteDistances2d[track3[minIndex_track2]][k] = (distance_1);
            externalTransportationCost -= customerdata[s1.notvisited[k]][4];
            recordOfInsertedNodes.push_back(s1.notvisited[k]);
        }
        else
        {
            // cout << "URDU" << endl;
            track1.clear();
            track2.clear();
            track3.clear();
            continue;
        }
        track1.clear();
        track2.clear();
        track3.clear();
        flag = 0;
    }
    int count = 0;
    for (int i = 0; i < newRouteDistances2d.size(); i++)
    {
        count = 0;
        for (int j = 0; j < newRouteDistances2d[i].size(); j++)
        {
            if (newRouteDistances2d[i][j] == 0)
            {
                count++;
            }
            else
            {
                flag = 1;
                newRouteDistances.push_back(newRouteDistances2d[i][j]);
            }
        }
        if (count == newRouteDistances2d[i].size())
        {
            newRouteDistances.push_back(prevRouteDistances[i]);
        }
    }
    // cout << "objective value after Inserting nodes from External Vehicle " << objvalue << endl;
    // cout << "New current routes :\n";
    for (int i = 0; i < route.size(); i++)
    {
        s1.remcapacityofvehicle[i] = truckarr[i].capacity - checkCapacity(route[i], customerdata);
        s1.routeDistance[i] = distanceCheck(route[i], distMatrix);
    }
    float objvalue = objfunc(customerdata, distMatrix, truckarr, vehicle, s1.routeDistance, externalTransportationCost);

    s.vehicleroute = route;
    s.initcost = objvalue;
    s.routeDistance = s1.routeDistance;
    s.ext_transportcost = externalTransportationCost;
    s.notvisited = s1.notvisited;
    s.remcapacityofvehicle = s1.remcapacityofvehicle;
    s.visited = s1.visited;
    s.insertedNode = recordOfInsertedNodes;

    cout << "\nRoute Distances after applying External Node Insertion" << endl;
    for (float x : s.routeDistance)
    {
        cout << x << "\t";
    }
    return s;
}

sol VRPPC::externalNodeSwapping(vector<vector<int>> &customerdata, vector<vector<float>> &distMatrix, truck *truckarr, int externalTransportationCost, sol s1)
{
    sol s;
    vector<int> routeCopy;
    vector<int> routeCopy1;
    vector<vector<int>> route(vehicle);
    route = s1.vehicleroute;
    int var;
    int count;
    float distance = 0, distance_1 = 0;
    vector<float> prevRouteDistances;
    prevRouteDistances = s1.routeDistance;
    vector<float> newRouteDistances;
    vector<float> storeDist;
    vector<int> recordOfInsertedNodes;
    externalTransportationCost = s1.ext_transportcost;
    recordOfInsertedNodes = s1.insertedNode;
    for (int i = 0; i < recordOfInsertedNodes.size(); i++)
    {
        vector<int>::iterator position = find(s1.notvisited.begin(), s1.notvisited.end(), recordOfInsertedNodes[i]);
        if (position != s1.notvisited.end())
        {
            s1.notvisited.erase(position);
        }
    }
    vector<vector<float>> newRouteDistances2d(vehicle, vector<float>(s1.notvisited.size(), 0));
    vector<int> trackof_j;
    vector<int> trackof_i;
    vector<vector<float>> trackOfrecords(vehicle, vector<float>(3));
    float randMultiplier = 1.05 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.2 - 1.05)));

    for (int k = 0; k < s1.notvisited.size(); k++)
    {
        // cout << "Not visited iteration " << k << endl;
        for (int i = 0; i < route.size(); i++)
        {
            // cout << "no of route iteration " << i << endl;
            routeCopy = route[i];
            for (int j = 1; j < route[i].size() - 1; j++)
            {
                storeDist.push_back(distMatrix[routeCopy[j]][s1.notvisited[k]]);
                trackof_j.push_back(j);
            }
            float min = *min_element(storeDist.begin(), storeDist.end());
            int min_index = min_element(storeDist.begin(), storeDist.end()) - storeDist.begin();
            trackOfrecords[i][0] = (min);
            trackOfrecords[i][1] = (trackof_j[min_index]);
            trackOfrecords[i][2] = (i);
            storeDist.clear();
            trackof_j.clear();
        }
        int final_minIndex = findMinFromMatrix(trackOfrecords);
        // cout << final_minIndex << "-- " << int(trackOfrecords[final_minIndex][2]) << endl;
        routeCopy1 = route[int(trackOfrecords[final_minIndex][2])];
        routeCopy = route[int(trackOfrecords[final_minIndex][2])];
        // cout << "Before checking conditions" << endl;
        routeCopy1[int(trackOfrecords[final_minIndex][1])] = s1.notvisited[k];
        float dist = distanceCheck(routeCopy1, distMatrix);
        int capacity = checkCapacity(routeCopy1, customerdata);
        if (capacity <= truckarr[int(trackOfrecords[final_minIndex][2])].capacity - s1.remcapacityofvehicle[int(trackOfrecords[final_minIndex][2])])
        {
            // cout << "1st IF condition" << endl;
            if (dist < prevRouteDistances[int(trackOfrecords[final_minIndex][2])] * randMultiplier)
            {
                // cout << "2nd IF condition" << endl;
                route[int(trackOfrecords[final_minIndex][2])] = routeCopy1;
                newRouteDistances2d[int(trackOfrecords[final_minIndex][2])][k] = dist;
                s1.notvisited[k] = routeCopy[int(trackOfrecords[final_minIndex][1])];
                externalTransportationCost = externalTransportationCost + customerdata[routeCopy1[int(trackOfrecords[final_minIndex][1])]][4] - customerdata[s1.notvisited[k]][4];
                prevRouteDistances[int(trackOfrecords[final_minIndex][2])] = dist;
                s1.remcapacityofvehicle[int(trackOfrecords[final_minIndex][2])] = truckarr[int(trackOfrecords[final_minIndex][2])].capacity - capacity;
            }
        }
        for (int x = 0; x < trackOfrecords.size(); x++)
        {
            for (int y = 0; y < trackOfrecords[x].size(); y++)
            {
                fill(trackOfrecords[x].begin(), trackOfrecords[x].end(), 0);
            }
        }
    }
    for (int i = 0; i < newRouteDistances2d.size(); i++)
    {
        count = 0;
        for (int j = 0; j < newRouteDistances2d[i].size(); j++)
        {
            if (newRouteDistances2d[i][j] == 0)
            {
                count++;
            }
            else
            {
                newRouteDistances.push_back(newRouteDistances2d[i][j]);
            }
        }
        if (count == newRouteDistances2d[i].size())
        {
            newRouteDistances.push_back(prevRouteDistances[i]);
        }
    }
    vector<int> temp;
    unordered_set<int> set1(s1.notvisited.begin(), s1.notvisited.end());
    for (int x = 1; x <= customer; x++)
    {
        if (set1.find(x) == set1.end())
        {
            temp.push_back(x);
        }
    }
    float objvalue = objfunc(customerdata, distMatrix, truckarr, vehicle, newRouteDistances, externalTransportationCost);
    // cout << "objective value after swapping nodes(min-distance) from External Vehicle " << objvalue << endl;
    // cout << "objective value after swapping nodes(max-distance) from External Vehicle " << objvalue << endl;
    // cout << "Final current routes :\n";
    // for (int i = 0; i < route.size(); i++)
    // {
    //     for (int j = 0; j < route[i].size(); j++)
    //     {
    //         cout << route[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    s.vehicleroute = route;
    s.initcost = objvalue;
    s.routeDistance = newRouteDistances;
    s.ext_transportcost = externalTransportationCost;
    s.notvisited = s1.notvisited;
    s.remcapacityofvehicle = s1.remcapacityofvehicle;
    s.visited = temp;
    s.insertedNode = recordOfInsertedNodes;

    cout << "\nRoute Distances after applying External Node Swapping" << endl;
    for (float x : s.routeDistance)
    {
        cout << x << "\t";
    }
    // cout << endl;
    // for (int k : s1.notvisited)
    // {
    //     cout << s1.notvisited[k] << " ";
    // }
    return s;
}