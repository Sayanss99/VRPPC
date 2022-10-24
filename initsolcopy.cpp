/**
 * ! Notes:       This is a file for initial solution, contains functions which are being called from the main.cpp
 * *           importing "function.h" (contains the class structure and definitions)
 */

#include "function.h"
#include <chrono>
using namespace std;
using namespace std::chrono;

/**
 * *    sortcol(const vector<float> &v1, const vector<float> &v2) is used to sort the ratio of (Ext Cost/Demand) column wise
 *
 */
bool sortcol(const vector<float> &v1, const vector<float> &v2)
{
    return v1[1] < v2[1];
}
/*
? This function is used to find the nodes that are not present after all vehicles complete their routes
*/
vector<int> findMissingnodes(vector<int> allnode, int low, int high)
{
    vector<int> result;
    unordered_set<int> s(allnode.begin(), allnode.end());
    for (int x = low; x <= high; x++)
    {
        if (s.find(x) == s.end())
        {
            result.push_back(x);
        }
    }
    return result;
}

/**
 * *    search(vector<float> v, int num) is used to search a particular element in an array
 */
int search(vector<int> v, int num)
{
    for (int i = 0; i < v.size(); i++)
        if (v[i] == num)
            return 1;
    return 0;
}
/*
? checkequal function is used to find if all the elements of the vector are equal or not
*/
int checkequal(vector<int> v)
{
    if (std::equal(v.begin() + 1, v.end(), v.begin()))
    {
        return 1;
    }
    else
        return 0;
}
/*
? this function helps to sort a vector and keeps track of the original indexes after the sorting
*/
vector<pair<int, int>> sortArr(vector<int> vec, int n)
{

    // Vector to store element
    // with respective present index
    vector<pair<int, int>> vp;

    // Inserting element in pair vector
    // to keep track of previous indexes
    for (int i = 0; i < n; ++i)
    {
        vp.push_back(make_pair(vec[i], i));
    }

    // Sorting pair vector
    sort(vp.begin(), vp.end());

    // Displaying sorted element
    // with previous indexes
    // corresponding to each element
    return vp;
}

/**
 * *    in this function we're passing an array containing all the elements of each row of distance matrix
 * *    and searching for the minimum distance in each row and storing the minimum index in an another array
 * ?    also checking if the index is already present or not
 */

int VRPPC ::min_func(vector<float> distmatrixrow, vector<int> served)
{
    for (int i = 0; i < customer + 1; i++)
    {
        if (search(served, i) == 1)
        {
            distmatrixrow[i] = 999999.00;
        }
    }
    int min = min_element(distmatrixrow.begin(), distmatrixrow.end()) - distmatrixrow.begin();
    return min;
}

/**
 * *    we're passing the customer details, distance matrix, truck details and it will return a structure containing nodes visited by internal fleet and external fleet
 * *    and the initial solution
 */
sol VRPPC::initialsol(vector<vector<int>> &customerdata, vector<vector<float>> &distMatrix, truck *truckarr)
{
    // auto start = high_resolution_clock::now();
    sol s1;

    /*  calculating sum of demands for all customer */
    int sumofdemand = 0;
    for (int j = 0; j < customer + 1; j++)
    {
        sumofdemand += customerdata[j][3];
    }

    /*  calculating sum of capacity for all vehicle */
    int sumofcapacity = 0;
    for (int j = 0; j < vehicle; j++)
    {
        sumofcapacity += truckarr[j].capacity;
    }
    // std::cout << sumofdemand << endl
    //           << sumofcapacity << endl;

    /*  calculating ratio of Extern Cost and Demands of customers   */
    vector<vector<float>> ratio_extCostbyDemand(customer, vector<float>(2));
    for (int i = 1, j = 0; i < customer + 1; i++, j++)
    {
        ratio_extCostbyDemand[j][0] = customerdata[i][0];
        ratio_extCostbyDemand[j][1] = float(customerdata[i][4]) / float(customerdata[i][3]);
    }

    /*  sorting the ratio   */
    sort(ratio_extCostbyDemand.begin(), ratio_extCostbyDemand.end(), sortcol);
    // for (int i = 0; i < customer; i++)
    // {
    //     for (int j = 0; j < 2; j++)
    //     {
    //         std::cout << ratio_extCostbyDemand[i][j] << "\t";
    //     }
    //     std::cout << "\n";
    // }

    /*  storing the nodes which have lower ratio in a vector    */
    vector<int> visitedbyExternalVehicle, visitedbyinternalfleet;
    int demand = sumofdemand;
    int sum = 0;
    int i = 0;
    // cout << int(ratio_extCostbyDemand[i][0]) << endl;
    while (demand > sumofcapacity)
    {
        visitedbyExternalVehicle.push_back(int(ratio_extCostbyDemand[i][0]));
        sum += customerdata[visitedbyExternalVehicle[i]][3];
        demand -= sum;
        i++;
    }

    s1.notvisited = visitedbyExternalVehicle;
    // std::cout << "Customers visited by External Fleet" << endl;
    // for (int x : visitedbyExternalVehicle)
    // {
    //     std::cout << x << " ";
    // }

    /*  since we've nodes visited by external fleet storing the remaining nodes from 1 to number of customer in another vector  */
    unordered_set<int> s(visitedbyExternalVehicle.begin(), visitedbyExternalVehicle.end());
    for (int x = 1; x <= customer; x++)
    {
        if (s.find(x) == s.end())
        {
            s1.visited.push_back(x);
        }
    }
    // std::cout << "\nCustomers visited by Internal Fleet" << endl;
    visitedbyinternalfleet = s1.visited;
    // for (int x : visitedbyinternalfleet)
    // {
    //     std::cout << x << " ";
    // }

    /*  creating a copy of distance matrix in another 2-D vector    */
    /*  and increasing all the rows and columns of the nodes visited by external fleet by a high number */
    vector<vector<float>> distanceCopy(customer + 1, vector<float>(customer + 1));
    distanceCopy = distMatrix;

    for (int i = 0; i < customer + 1; i++)
    {
        for (int j = 0; j < s1.notvisited.size(); j++)
        {
            distanceCopy[s1.notvisited[j]][i] = 999999.00;
            distanceCopy[i][s1.notvisited[j]] = 999999.00;
        }
        distanceCopy[i][i] = 999999.00;
    }

    int k, min = 0, iterator = 0;
    vector<float> checkingarr;

    /**
     * !  finding the routes for the internal fleet and calculating the initial value
     */

    /*
    ? stored all capacity and varcost of truck in two different vectors and sorted them pairwise with thier indexes
    */
    vector<int> TrCapcty, varcost;
    for (int i = 0; i < vehicle; i++)
    {
        TrCapcty.push_back(truckarr[i].capacity);
        varcost.push_back(truckarr[i].variable_cost);
    }

    /*
?  kept a copy of all customer nodes who will be visited by internal fleet
*/
    vector<float> unservedCustomer(visitedbyinternalfleet.begin(), visitedbyinternalfleet.end());
    vector<float> demandOfUnservedCustomer; //! this will store all the demands of customers to be visited
                                            //! by internal fleet
    for (int i = 0; i < customer; i++)
    {
        demandOfUnservedCustomer.push_back(customerdata[i + 1][3]);
    }
    vector<int> route{0};
    vector<int> visited;

    float totaldistance = 0;
    i = 0;
    int v = 0;
    int remcapacity = 0;
    vector<float> distanceofEachfleet;
    // sol *routearr = new sol[vehicle];
    vector<pair<int, int>> Capacitypair, varcostpair;
    Capacitypair = sortArr(TrCapcty, TrCapcty.size());
    varcostpair = sortArr(varcost, varcost.size());

    /*
! Start of route finding, here two loops have been used one runs upto no of vehicle and the inner loop continues until the remaining capacity becomes > 0
? a break conditions is there -  a) if the remaining capacity remains > 0 but any other unserved customer dont have that amount of demand or  less than that

*/
    while (v < vehicle)
    {
        totaldistance = 0;
        if (checkequal(TrCapcty) == 1)
        {
            if (checkequal(varcost) == 1)
            {
                remcapacity = truckarr[v].capacity;
            }
            else
            {
                remcapacity = truckarr[varcostpair[v].second].capacity;
            }
        }
        else
        {
            remcapacity = truckarr[Capacitypair[v].second].capacity;
        }
        while (remcapacity > 0)
        {

            for (int j = 0; j < customer + 1; j++)
            {
                checkingarr.push_back(distanceCopy[i][j]);
            }
            min = min_func(checkingarr, visited);
            checkingarr.clear();
            int minelemnt = *min_element(demandOfUnservedCustomer.begin(), demandOfUnservedCustomer.end());
            if (remcapacity < minelemnt)
            {
                break;
            }
            if (customerdata[min][3] > remcapacity)
            {
                break;
            }
            route.push_back(min);
            unservedCustomer.erase(std::remove(unservedCustomer.begin(), unservedCustomer.end(), float(min)), unservedCustomer.end());
            demandOfUnservedCustomer.erase(std::remove(demandOfUnservedCustomer.begin(), demandOfUnservedCustomer.end(), customerdata[min][3]), demandOfUnservedCustomer.end());
            remcapacity -= customerdata[min][3];
            i = min;
            visited.insert(visited.end(), route.begin(), route.end());
        }
        //! end of inner while, which will give one route
        route.push_back(0);
        s1.vehicleroute.push_back(route);
        route.clear();
        route.push_back(0);
        s1.remcapacityofvehicle.push_back(remcapacity);
        std::cout << "\nRoute of Vehicle " << v + 1 << ": " << endl;
        for (int i = 0; i < s1.vehicleroute[v].size(); i++)
        {
            std::cout << s1.vehicleroute[v][i] << "\t";
        }
        std::cout << "\nRemaining Capacity for vehicle " << v + 1 << ": " << remcapacity << endl;
        //? keeping distance of each fleet stored in a vector
        for (int j = 0, k = 1; j < s1.vehicleroute[v].size() - 1; j++, k++)
        {
            totaldistance += distMatrix[s1.vehicleroute[v][j]][s1.vehicleroute[v][k]];
        }
        s1.routeDistance.push_back(totaldistance);
        v++;
    }
    //! end of outer loop, which will provide all the routes

    //? checking if there are some customers where inter fleet not visited and adding them to external fleet
    visitedbyExternalVehicle = findMissingnodes(visited, 1, customer);

    s1.notvisited = visitedbyExternalVehicle;
    // std::cout << "Customers visited by External Fleet after all vehicle routed: " << endl;
    // for (int x : visitedbyExternalVehicle)
    // {
    //     std::cout << x << " ";
    // }
    k = 0;
    // for (float x : distanceofEachfleet)
    // {
    //     std::cout << "\nDistance of route" << k + 1 << " : " << x;
    //     k++;
    // }

    //! Now the rest is calculation for the solution value

    // External Transportation cost
    for (int x = 0; x < visitedbyExternalVehicle.size(); x++)
    {
        s1.ext_transportcost += customerdata[visitedbyExternalVehicle[x]][4];
    }
    // std::cout << "\nExternal Transportation cost: " << s1.ext_transportcost << endl;

    float totalvariablecost = 0;
    for (int x = 0; x < vehicle; x++)
    {
        totalvariablecost += s1.routeDistance[x] * truckarr[x].variable_cost;
    }
    int totalfixedcostofTruck = 0;
    for (int x = 0; x < vehicle; x++)
    {
        totalfixedcostofTruck += truckarr[x].fixed_cost;
    }
    float costofInternalfleet = totalfixedcostofTruck + totalvariablecost;
    s1.initcost = costofInternalfleet + s1.ext_transportcost;
    // std::cout << "Total cost for the internal fleet: " << costofInternalfleet << endl;
    // std::cout << "\nObjective Value for Initial Solution: " << s1.initcost << endl;

    // for(int i = 0; i < s1.vehicleroute.size(); i++)
    // {
    //     cout << "\nfor route " << i << endl; 
    //     for(int j = 0; j < s1.vehicleroute[i].size(); j++)
    //     {
    //         cout << customerdata[s1.vehicleroute[i][j]][1] << " ";
    //     }
    // }

    // for(int i = 0; i < s1.vehicleroute.size(); i++)
    // {
    //     cout << "\nfor route " << i << endl; 
    //     for(int j = 0; j < s1.vehicleroute[i].size(); j++)
    //     {
    //         cout << customerdata[s1.vehicleroute[i][j]][2] << " ";
    //     }
    // }

    // auto stop = high_resolution_clock::now();
    // auto duration = duration_cast<microseconds>(stop - start);
    // std::cout << "\nTime taken by function : " << duration.count() << " microseconds" << endl;

    return s1;
}