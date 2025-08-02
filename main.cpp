#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
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

static inline string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

struct County {
    string name;
    string state;
    unordered_map<string, double> metrics;
};

vector<County> loadCounties(const string &filename) {
    vector<County> counties;
    ifstream in(filename);
    if (!in.is_open()) {
        cout << "Failed to open " << filename << endl;
        return counties;
    }
    string line;
    vector<string> headers;
    if (!getline(in, line)) return counties;

    // normalize the header while converting tabs to spaces and split on commas
    {
        string normalized = line;
        replace(normalized.begin(), normalized.end(), '\t', ' ');
        stringstream ss(normalized);
        string cell;
        while (getline(ss, cell, ',')) {
            headers.push_back(trim(cell));
        }
    }

    int idxCounty = -1, idxState = -1;
    for (int i = 0; i < (int)headers.size(); ++i) {
        if (headers[i] == "County") idxCounty = i;
        else if (headers[i] == "State") idxState = i;
    }
    if (idxCounty == -1 || idxState == -1) {
        cout << "CSV missing County or State column" << endl;
        return counties;
    }

    while (getline(in, line)) {
        stringstream ss(line);
        vector<string> cells;
        string cell;
        while (getline(ss, cell, ',')) {
            cells.push_back(trim(cell));
        }
        // if malformed row then it skips
        if ((int)cells.size() != (int)headers.size()) {
            
            continue;
        }

        County c;
        c.name = cells[idxCounty];
        c.state = cells[idxState];

        for (int i = 0; i < (int)headers.size(); ++i) {
            if (i == idxCounty || i == idxState) continue;
            string key = headers[i];
            if (cells[i].empty()) continue;
            try {
                double val = stod(cells[i]);
                c.metrics[key] = val;
            } catch (...) {
                // skips invalid
                cout << "Warning: non-numeric value for county '" << c.name;
            }
        }

        counties.push_back(move(c));
    }

    return counties;
}

// Function to compute the score for a county based on selected metrics and their weights
double compute_score(const County &c, const unordered_map<string, double> &weights) {
    double score = 0.0;
    double totalWeight = 0.0;
    for (auto &[metric, weight] : weights) {
        auto it = c.metrics.find(metric);
        if (it != c.metrics.end()) {
            score += it->second * weight;
            totalWeight += weight;
        }
    }
    if (totalWeight == 0.0) {
        return 0.0; // Avoid division by zero
    }
    return score / totalWeight; // Normalize the score
}

int main() {
    //Load all the parameters into a vector of pairs
    vector<pair<char, string>> coreMetricsList = loadCoreMetrics();
    vector<pair<char, string>> fullMetricsList = loadFullMetrics();
    runMenu(coreMetricsList, fullMetricsList);
    return 0;
}