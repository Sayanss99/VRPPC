/*
 * * This is a header file for all other files *
 */

#include <iostream>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;

/*
? This structure 'sol' we have taken for storing the results from the initial solution
*/

struct sol
{
    vector<int> visited;
    vector<int> notvisited;
    vector<vector<int>> vehicleroute;
    vector<int> remcapacityofvehicle;
    int ext_transportcost = 0;
    vector<float> routeDistance;
    vector<int> insertedNode;
    float initcost;
};

struct liststruct
{
    int node;
    int capacity;
    float partialDistance;
    liststruct *next;
    liststruct *prev;
};

struct twoOpt
{
    vector<liststruct *> newRoute;
    float objvalue;
};

struct swapwithinRoute
{
    vector<vector<int>> route;
    float objvalue;
    int extCost;
};
/*
?  Structure fleet we have taken for storing each vehicle and its route, it has been used while in objective funtion
*/

struct fleet
{
    int vehicleNo;
    vector<int> route;
};

/*
? Structure Truck is used whle taking input from file to store the truck details
*/

struct truck
{
    int capacity, fixed_cost, variable_cost;
};

/*
! We have defined a class VRPPC where all kinds of function declaration lies as its member function. We continue to use these function throughout all the files of this program
*/

class VRPPC
{
public:
    int i;
    int customer, vehicle; //! defined customer and vehicle as two variables so that we can access these                         //! two values in any function
    int *customerArr, *vehicleArr;
    string str, str1;
    VRPPC()
    {
        i = 0;
        start = NULL;
        end = NULL;
    }
    int extractIntegerWords(string str);    //! this will extract integer from each line and return that
    void extractint(string str, int arr[]); //! this is similar to previous one except it will store the intergers in an array
    void takeInputFile(string);                   //! this is the function to open the file to read the data store the customer and truck details
    void showCustomerData();
    void showVehicleData();
    void showDistanceMatrix();
    void showTruckDetails();
    truck *getTruckArr();
    vector<vector<int>> getCustomerData();     //! storing all customer details in a 2d vector
    vector<vector<int>> getVehicleData();      //! storing all customer details in a 2d vector
    vector<vector<float>> getDistanceMatrix(); //! storing distance matrix in a 2d vector
    liststruct *insertatend(vector<vector<int>> &, vector<vector<float>> &, truck *, sol, int, int);
    liststruct *start;
    liststruct *end;

    /*
    ? following functions work in objective function file
    */
    // vector<int> returnvisitednodes(vector<int> &, vector<int> &, vector<int> &, vector<int> &); //! this stores all nodes where
    //                                                                                             //!truck has visited
    // vector<int> findMissing(vector<int> &, int, int);   //! this will give the record of customers who have not been visited by
    //                                                     //! internal vehicle
    // int costforExternalTransport(vector<int>);
    // float findDistanceofroute(vector<int> &, vector<vector<float>> &);
    // float calculatevariablecost(float, int);
    // int totalFixedcost(truck*);
    // float objectivevalue(float, int);
    // float objectivefunction(vector<vector<int>>&, vector<vector<float>> &, truck *, sol*);
    /*
    ? below function works for initial solution file, we are passing the customer array wehere all details of customer lies, distance matrix and the truck details and it will return a structure where initial solution and customer visited by internal fleet and external fleet will also be there
    */
    sol initialsol(vector<vector<int>> &, vector<vector<float>> &, truck *);
    sol newsol(vector<vector<int>> &, vector<vector<float>> &, truck *, sol);
    // swapwithinRoute newalgo(vector<vector<int>> &, vector<vector<float>> &, truck *, twoOpt, int, sol);
    sol adjacentSwapping(vector<vector<int>> &, vector<vector<float>> &, truck *, int, sol);
    sol one_oneSwapping(vector<vector<int>> &, vector<vector<float>> &, truck *, int, sol);
    sol singleInsertionSwapping(vector<vector<int>> &, vector<vector<float>> &, truck *, int, sol);
    sol externalNodeInsertion(vector<vector<int>> &, vector<vector<float>> &, truck *, int, sol);
    sol externalNodeSwapping(vector<vector<int>> &, vector<vector<float>> &, truck *, int, sol);
    int min_func(vector<float>, vector<int>);
};
