#include "function.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <bits/stdc++.h>

void VRPPC ::takeInputFile(string filename)
{
    ifstream input;

    char first[70];
    char second[70];
    char third[70];
    char fourth[70];
    char fifth[70];

    input.open(filename);

    cout << fixed << setprecision(2);

    input.getline(first, 70);
    input.getline(second, 70);
    input.getline(third, 70);
    input.getline(fourth, 70);
    input.getline(fifth, 70);

    string thirdString = third;
    string fourthString = fourth;
    str1 = fifth;

    customer = extractIntegerWords(thirdString);
    vehicle = extractIntegerWords(fourthString);

    cout << "Customers: " << customer << endl
         << "Vehicle: " << vehicle << endl;

    // // Reading data from file and storing all the customer info in a 1-D array //

    customerArr = new int[(customer + 1) * 5];
    for (int i = 0; i < customer + 1; i++)
    {
        char line[100];
        input.getline(line, 100);
        string fstring = line;
        extractint(fstring, customerArr);
    }

    char sixth[70];
    input.getline(sixth, 70);
    str = sixth;

    VRPPC p;
    vehicleArr = new int[(vehicle * 4)];
    for (int i = 0; i < vehicle; i++)
    {
        char line[100];
        input.getline(line, 100);
        string fstring = line;
        p.extractint(fstring, vehicleArr);
    }
}

void VRPPC ::extractint(string str, int arr[])
{
    stringstream ss;
    ss << str;
    string temp;
    int found;
    while (!ss.eof())
    {
        ss >> temp;
        if (stringstream(temp) >> found)
        {
            arr[i] = found;
            i++;
        }
        temp = "";
    }
}

int VRPPC ::extractIntegerWords(string str)
{
    stringstream ss;
    ss << str;
    string temp;
    int found, got;
    while (!ss.eof())
    {
        ss >> temp;
        if (stringstream(temp) >> found)
            got = found;
        temp = " ";
    }
    return got;
}

vector<vector<int>> VRPPC ::getCustomerData()
{
    vector<vector<int>> customervec(customer + 1, vector<int>(5));
    for (int i = 0, k = 0; i < customer + 1; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            customervec[i][j] = customerArr[k];
            k++;
        }
    }
    return customervec;
}

vector<vector<int>> VRPPC ::getVehicleData()
{
    vector<vector<int>> vehiclevec(vehicle, vector<int>(4));
    for (int i = 0, l = 0; i < vehicle; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            vehiclevec[i][j] = vehicleArr[l];
            l++;
        }
    }
    return vehiclevec;
}

void VRPPC ::showCustomerData()
{
    vector<vector<int>> customerVect = this->getCustomerData();
    cout << str1 << endl;
    for (int i = 0; i < customer + 1; i++)
    {
        for (int j = 0; j < 5; j++)
            cout << customerVect[i][j] << "\t\t";
        cout << endl;
    }
}

void VRPPC ::showVehicleData()
{
    vector<vector<int>> vehicleVect = this->getVehicleData();
    cout << str << endl;
    for (int i = 0; i < vehicle; i++)
    {
        for (int j = 0; j < 4; j++)
            cout << vehicleVect[i][j] << "\t\t";
        cout << "\n";
    }
}

vector<vector<float>> VRPPC ::getDistanceMatrix()
{
    vector<vector<int>> coordinatearr(customer + 1, vector<int>(2));
    vector<vector<int>> customervec = this->getCustomerData();
    for (int i = 0; i < customer + 1; i++)
    {
        coordinatearr[i][0] = customervec[i][1];
        coordinatearr[i][1] = customervec[i][2];
    }
    vector<vector<float>> resultantvec(customer + 1, vector<float>(customer + 1));
    for (int i = 0; i < customer + 1; i++)
    {
        for (int j = 0; j < customer + 1; j++)
        {
            resultantvec[i][j] = sqrt(pow((coordinatearr[i][0] - coordinatearr[j][0]), 2) + pow((coordinatearr[i][1] - coordinatearr[j][1]), 2));
        }
    }
    return resultantvec;
}
truck *VRPPC ::getTruckArr()
{
    truck *TruckArray = new truck[vehicle];
    vector<vector<int>> vehiclevec = this->getVehicleData();
    for (int i = 0; i < vehicle; i++)
    {
        TruckArray[i].capacity = vehiclevec[i][1];
        TruckArray[i].fixed_cost = vehiclevec[i][2];
        TruckArray[i].variable_cost = vehiclevec[i][3];
    }
    return TruckArray;
}

void VRPPC ::showDistanceMatrix()
{
    vector<vector<float>> resultantvec = this->getDistanceMatrix();
    cout << "\nDistance Matrix\n";
    for (int i = 0; i < customer + 1; i++)
    {
        for (int j = 0; j < customer + 1; j++)
        {
            cout << resultantvec[i][j] << "\t";
        }
        cout << endl;
    }
}

void VRPPC ::showTruckDetails()
{
    truck *arr = this->getTruckArr();
    cout << "\nTruck list\n";
    for (int i = 0; i < vehicle; i++)
    {
        cout << "Truck Array[" << i + 1 << "]\t" << arr[i].capacity << "\t" << arr[i].fixed_cost << "\t" << arr[i].variable_cost << endl;
    }
}
