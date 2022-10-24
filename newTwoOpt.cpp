#include "function.h"
using namespace std;

float checkDistance(vector<int> route, vector<vector<float>> &distMatrix)
{
    float totaldistance = 0;
    for (int j = 0, k = 1; j < route.size() - 1; j++, k++)
    {
        totaldistance += distMatrix[route[j]][route[k]];
    }
    return totaldistance;
}

int capacityCheck(vector<int> route, vector<vector<int>> &customerdata)
{
    int capacity = 0;
    for (int i = 0; i < route.size(); i++)
    {
        capacity += customerdata[route[i]][3];
    }
    return capacity;
}
float objectivefunc(vector<vector<int>> &customerdata, vector<vector<float>> &distMatrix, truck *truckarr, int vehicle, vector<float> distanceofEachfleet, int externalTransportationCost)
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

vector<vector<float>> find_meandistmatrix(vector<vector<int>> &customerdata, sol s1, int vehicle)
{
    float xsum = 0, ysum = 0;
    vector<double> meanX, meanY;

    for (int i = 0; i < s1.vehicleroute.size(); i++)
    {
        for (int j = 0; j < s1.vehicleroute[i].size(); j++)
        {
            xsum += customerdata[s1.vehicleroute[i][j]][1];
            ysum += customerdata[s1.vehicleroute[i][j]][2];
        }
        meanX.push_back(xsum / s1.vehicleroute[i].size());
        meanY.push_back(ysum / s1.vehicleroute[i].size());
    }
    vector<vector<float>> meandistMatrix(vehicle, vector<float>(vehicle));
    for (int i = 0; i < vehicle; i++)
    {
        for (int j = 0; j < vehicle; j++)
        {
            meandistMatrix[i][j] = sqrt(pow((meanX[i] - meanX[j]), 2) + pow((meanY[i] - meanY[j]), 2));
        }
    }
    return meandistMatrix;
}

//? Function smmalestInRow takes a distance matrix(square) as argument and tries to find in which coulumn number minimum value lies. //

vector<int> smallestInRow(vector<vector<float>> mat)
{
    vector<int> arr;
    for (int i = 0; i < mat.size(); i++)
    {
        //! distance matrix will have 0 at diagonal
        //! to avoid problems in finding minimum values
        //! we make the diagonal elements a large positive no
        mat[i][i] = 9999.00;
    }
    for (int i = 0; i < mat.size(); i++)
    {

        //! initialize the minimum element
        //! as first element
        int minm = mat[i][0];
        int flag = 0;
        for (int j = 1; j < mat[i].size(); j++)
        {
            //! check if any element is smaller
            //! than the minimum element of the row
            //! and replace it with the respective column no
            if (mat[i][j] < minm)
            {
                minm = j;
                flag = 1;
            }
        }
        if (flag == 0)
        {
            minm = 0;
        }
        //! If we get say arr[1][2] as the minimum i.e., column 1 of row 2
        //! then arr[2][1] should be ignored
        //! as distance matrix have same values for arr[i][j] and arr[j][i]
        mat[minm][i] = 9999.00;
        arr.push_back(minm);
    }
    return arr;
}

sol VRPPC::newsol(vector<vector<int>> &customerdata, vector<vector<float>> &distMatrix, truck *truckarr, sol s1)
{
    sol s2;
    vector<vector<float>> meandistMatrix(vehicle, vector<float>(vehicle));
    for (int k = 0; k < vehicle; k++)
    {
        if (vehicle == 2)
        {
            break;
        }
        meandistMatrix = find_meandistmatrix(customerdata, s1, vehicle);
        vector<int> min_meandist = smallestInRow(meandistMatrix);
        int l = min_meandist[k];
        for (int i = 1; i < s1.vehicleroute[k].size() - 1; i++)

        {
            for (int j = 1; j < s1.vehicleroute[l].size() - 1; j++)
            {
                vector<int> routecopy1 = s1.vehicleroute[k];
                vector<int> routecopy2 = s1.vehicleroute[l];
                int temp = routecopy1[i];
                routecopy1[i] = routecopy2[j];
                routecopy2[j] = temp;
                int capacity1 = capacityCheck(routecopy1, customerdata);
                int capacity2 = capacityCheck(routecopy2, customerdata);
                float distance1 = checkDistance(routecopy1, distMatrix);
                float distance2 = checkDistance(routecopy2, distMatrix);
                if (capacity1 < truckarr[k].capacity - s1.remcapacityofvehicle[k] && capacity2 < truckarr[l].capacity - s1.remcapacityofvehicle[l])
                {
                    if (distance1 < s1.routeDistance[k] && distance2 < s1.routeDistance[l])
                    {
                        s1.vehicleroute[k] = routecopy1;
                        s1.vehicleroute[l] = routecopy2;
                        s1.routeDistance[k] = distance1;
                        s1.routeDistance[l] = distance2;
                        s1.remcapacityofvehicle[k] = truckarr[k].capacity - capacity1;
                        s1.remcapacityofvehicle[l] = truckarr[l].capacity - capacity2;
                    }
                }
            }
        }
    }
    float objvalue = objectivefunc(customerdata, distMatrix, truckarr, vehicle, s1.routeDistance, s1.ext_transportcost);
    // cout << "objective value after modified Two Opt swapping " << objvalue << endl;
    cout << "\nRoute Distances after applying Two opt" << endl;
    for (float x : s1.routeDistance)
    {
        cout << x << "\t";
    }
    s2.ext_transportcost = s1.ext_transportcost;
    s2.initcost = objvalue;
    s2.notvisited = s1.notvisited;
    s2.remcapacityofvehicle = s1.remcapacityofvehicle;
    s2.routeDistance = s1.routeDistance;
    s2.vehicleroute = s1.vehicleroute;
    s2.visited = s1.visited;
    return s2;
}