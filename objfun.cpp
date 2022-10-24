#include "function.h"

/*
? In findMissing function we pass the vector returned from the 'returnvisitednodes' function, it checks all the elements from 1 to number of customer present and the eleemnts which is not found are stored in a vector and returned, which is basically the nodes visited by external fleet
*/
vector<int> findMissing(vector<int> allnode, int low, int high)
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

/*
? since we have all the routes available for objective value finding, we pass each route and the whole distance matrix, so that we get the respective distance for each node and after adding we get the distance of the complete route and return it
*/
float findDistanceofroute(vector<int> vect, vector<vector<float>> resultantvec)
{
    float distance = 0;
    for (int j = 0, k = 1; j < vect.size() - 1; j++, k++)
    {
        distance += resultantvec[vect[j]][vect[k]];
    }
    return distance;
}

/*
? in this function we pass two variable, i.e, total distance of all the routes and the respective vehicle's varaible cost and return the product from the function
*/

float calculatevariablecost(float x, int y)
{
    return x * y;
}
/*
? Here we calculate the total fixed cost for all the vehicles present in depot
*/
int totalFixedcost(truck *truckarr, int size)
{
    int totalFixedcost = 0;
    for (int x = 0; x < size; x++)
    {
        totalFixedcost += truckarr[x].fixed_cost;
    }
    return totalFixedcost;
}

/*
?Calculating the objective function value:
!   Variable cost of a vehicle (A) = total distance traveled * VAR. COST 
!   Total cost for the internal fleet (B) = FIXED COST for the used vehicles + variable cost for the used
!   vehicles (A) Objective value = B + Cost of external transporter for the external vehicles 
*/

float objectivevalue(float x, int y)
{
    return x + y;
}

float VRPPC::objectivefunction(vector<vector<int>> &customerdata, vector<vector<float>> &distancematrix, truck *truckarr, sol *initroute)
{
    /*
? we have stored all the given routes in some vectors and we are passing to this function, since all the routes contain the node '0' twice so this function will copy all the nodes of each route and finally it will sort and remove the zeroes and return the nodes visited by internal fleet in a vector
*/
    cout << "\n----------This is Objective function--------\n"<< endl;
    vector<int> visitednodes;
    for (int i = 0; i < vehicle; i++)
    {
        visitednodes.insert(visitednodes.end(), initroute[i].vehicleroute.begin(), initroute[i].vehicleroute.end());
    }
    sort(visitednodes.begin(), visitednodes.end());
    for (auto j = visitednodes.begin(); j != visitednodes.end(); ++j)
    {
        if (*j == 0)
        {
            visitednodes.erase(j);
            j--;
        }
        else
        {
            break;
        }
    }
    for(int x : visitednodes)
    {
        cout << x << " ";
    }
    cout << endl;
    vector<int> missing;
    missing = findMissing(visitednodes, 1, customer);
    for(int x : missing)
    {
        cout << x << " ";
    }
    /*
? Since we get the customer number who are visited by external node, we pass that vector to this function and calculate their respective external cost and make sum of them 
*/
    int total_cost_for_externalTransporter = 0;
    for (int x = 0; x < missing.size(); x++)
    {
        total_cost_for_externalTransporter += customerdata[missing[x]][4];
    }
    int i = 0;
    float distanceofroute1 = findDistanceofroute(initroute[i].vehicleroute, distancematrix);
    float distanceofroute2 = findDistanceofroute(initroute[i + 1].vehicleroute, distancematrix);
    float distanceofroute3 = findDistanceofroute(initroute[i + 2].vehicleroute, distancematrix);
    float distanceofroute4 = findDistanceofroute(initroute[i + 3].vehicleroute, distancematrix);

    cout << "\nDistance of route 1: " << distanceofroute1 << "\nDistance of route 2: " << distanceofroute2 << "\nDistance of route 3: " << distanceofroute3 << "\nDistance of route 4: " << distanceofroute4 << endl;
    float totalDistance = distanceofroute1 + distanceofroute2 + distanceofroute3 + distanceofroute4;

    cout << "\nTotal distance for all the routes : " << totalDistance << endl;
    float variableCostofVehicle1 = calculatevariablecost(distanceofroute1, truckarr[0].variable_cost);
    float variableCostofVehicle2 = calculatevariablecost(distanceofroute2, truckarr[1].variable_cost);
    float variableCostofVehicle3 = calculatevariablecost(distanceofroute3, truckarr[2].variable_cost);
    float variableCostofVehicle4 = calculatevariablecost(distanceofroute4, truckarr[3].variable_cost);

    cout << "\nVariable cost of vehicle 1: " << variableCostofVehicle1 << "\nVariable cost of vehicle 2: " << variableCostofVehicle2 << "\nVariable cost of vehicle 3: " << variableCostofVehicle3 << "\nVariable cost of vehicle 4: " << variableCostofVehicle4 << endl;
    cout << "\nTotal FIXED COST for the used vehicles: " << totalFixedcost(truckarr, vehicle) << endl;
    float totalCostofInternalFleet = totalFixedcost(truckarr, vehicle) + (variableCostofVehicle1 + variableCostofVehicle2 + variableCostofVehicle3 + variableCostofVehicle4);
    cout << "Total cost for the internal fleet: " << totalCostofInternalFleet << endl;
    return objectivevalue(totalCostofInternalFleet, total_cost_for_externalTransporter);
}