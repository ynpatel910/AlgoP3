#include <iostream>
#include <vector>
#include <string>
using namespace std;
vector<pair<char, string>> loadCoreMetrics() {
    vector<pair<char, string>> coreMetrics = {
        {'A', "Income.Median Household Income"},
        {'B', "Income.Poverty Rate"},
        {'C', "Education.Bachelor's Degree or Higher"},
        {'D', "Labor.Unemployment Rate"},
        {'E', "Age.Percent Under 18 Years"},
        {'F', "Age.Percent 65 and Older"},
        {'G', "Housing.Homeownership Rate"},
        {'H', "Housing.Median Home Value"},
        {'I', "Population.Population per Square Mile"},
        {'J', "Health Insurance Coverage %"},
    };
    return coreMetrics;
}
vector<pair<char, string>> loadFullMetrics() {
    vector<pair<char, string>> fullMetrics = {
        {'A', "Age.Percent 65 and Older"},
        {'B', "Age.Percent Under 18 Years"},
        {'C', "Age.Percent Under 5 Years"},
        {'D', "Education.Bachelor's Degree or Higher"},
        {'E', "Education.High School or Higher"},
        {'F', "Employment.Nonemployer Establishments"},
        {'G', "Ethnicities.American Indian and Alaska Native Alone"},
        {'H', "Ethnicities.Asian Alone"},
        {'I', "Ethnicities.Black Alone"},
        {'J', "Ethnicities.Hispanic or Latino"},
        {'K', "Ethnicities.Native Hawaiian and Other Pacific Islander Alone"},
        {'L', "Ethnicities.Two or More Races"},
        {'M', "Ethnicities.White Alone"},
        {'N', "Ethnicities.White Alone not Hispanic or Latino"},
        {'O', "Housing.Homeownership Rate"},
        {'P', "Housing.Median Home Value"},
        {'Q', "Housing.Percent Renter Occupied"},
        {'R', "Income.Median Household Income"},
        {'S', "Income.Median Family Income"},
        {'T', "Income.Poverty Rate"},
        {'U', "Labor.Force Participation Rate"},
        {'V', "Labor.Unemployment Rate"},
        {'W', "Miscellaneous.Percent Female"},
        {'X', "Miscellaneous.Veterans"},
        {'Y', "Population.2020 Population"},
        {'Z', "Population.2010 Population"},
        {'a', "Population.Population per Square Mile"},
        {'b', "Sales.Accommodation and Food Services Sales"},
        {'c', "Sales.Retail Sales"},
        {'d', "Employment.Firms.Total"},
        {'e', "Employment.Firms.Women-Owned"},
        {'f', "Employment.Firms.Men-Owned"},
        {'g', "Employment.Firms.Minority-Owned"},
        {'h', "Employment.Firms.Nonminority-Owned"},
        {'i', "Employment.Firms.Veteran-Owned"},
        {'j', "Employment.Firms.Nonveteran-Owned"},
        };
    return fullMetrics;
}
void runMenu(vector<pair<char,string>> core, vector<pair<char,string>> full) {
    cout << "Welcome to County Rank!" << endl;
    int metricType;
    cout << "Enter 0 for core metrics list, enter 1 for full metrics list" << endl;
    cin >> metricType;
    cout << "Select your top 5 most important metrics!" << endl;
    if (metricType == 0) {
        for (auto pair : core) {
            cout << pair.first << ") " << pair.second << endl;
        }
    }
    else if (metricType == 1) {
        for (auto pair : full) {
            cout << pair.first << ") " << pair.second << endl;
        }
    }

}
int main() {
    //Load all the parameters into a vector of pairs
    vector<pair<char, string>> coreMetricsList = loadCoreMetrics();
    vector<pair<char, string>> fullMetricsList = loadFullMetrics();
    runMenu(coreMetricsList, fullMetricsList);
    return 0;
}