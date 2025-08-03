#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <iomanip>
#include <limits>
#include <utility>
#include <string>
#include <map>
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

//Assigns a ranking to the metric and assigns it to a map
map<int, string> rankMetric(const vector<string> &metricsVec) {
    map<int, string> metricsRank;
    for (const auto &metric : metricsVec) {
        int rank = 0;
        while (true) {
            cout << "Rank the importance of the metric " << "[" << metric << "]" << " 1-5: ";
            // Validate input
            if (!(cin >> rank)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Enter a number between 1 and 5.\n";
                continue;
            }
            if (rank < 1 || rank > 5) {
                cout << "Rank must be between 1 and 5.\n";
                continue;
            }
            if (metricsRank.find(rank) != metricsRank.end()) {
                cout << "That rank is already used for [" << metricsRank[rank] << "]. Pick a different one.\n";
                continue;
            }
            break;
        }
        metricsRank[rank] = metric;
    }
    return metricsRank;
}

// Function to run the menu and display metrics
vector<string> runMenu(vector<pair<char, string>> core, vector<pair<char, string>> full) {
    cout << "Welcome to County Rank!" << endl;
    int metricType;
    cout << "Enter 0 for core metrics list, enter 1 for full metrics list:" << endl;
    cin >> metricType;

    vector<pair<char, string>> selectedList;
    if (metricType == 0) {
        selectedList = core;
    } else {
        selectedList = full;
    }

    // Display the list
    cout << "Select your top 5 most important metrics!" << endl;
    for (auto &pair : selectedList) {
        cout << pair.first << ") " << pair.second << endl;
    }

    unordered_map<char, string> validMetrics;
    for (auto &p : selectedList) {
        validMetrics[p.first] = p.second;
    }

    vector<string> chosenMetrics;
    char input;
    while (chosenMetrics.size() < 5) {
        cout << "Enter choice #" << (chosenMetrics.size() + 1) << ": ";
        cin >> input;

        if (validMetrics.find(input) != validMetrics.end()) {
            // Check for duplicates
            if (find(chosenMetrics.begin(), chosenMetrics.end(), validMetrics[input]) == chosenMetrics.end()) {
                chosenMetrics.push_back(validMetrics[input]);
            } else {
                cout << "You've already selected that metric. Try a different one." << endl;
            }
        } else {
            cout << "Invalid selection. Try again." << endl;
        }
    }

    return chosenMetrics;
}


// Function to trim whitespace from the start and end of a string
static inline string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t\r\n\"");
    if (start == string::npos) {
        return "";
    }
    size_t end = s.find_last_not_of(" \t\r\n\"");
    return s.substr(start, end - start + 1);
}

// Struct to hold county data
struct County {
    string name;
    string state;
    unordered_map<string, double> metrics;
    double score = 0.0;
};

// Function to load counties from a CSV file
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

    // Check if headers are present
    int idxCounty = -1, idxState = -1;
    for (int i = 0; i < static_cast<int>(headers.size()); i++) {
        if (headers[i] == "County") idxCounty = i;
        else if (headers[i] == "State") idxState = i;
    }

    // Check if County and State columns are present
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
        if (static_cast<int>(cells.size()) != static_cast<int>(headers.size())) {
            continue;
        }
        County c;
        c.name = cells[idxCounty];
        c.state = cells[idxState];

        // Skip if county or state is empty
        for (int i = 0; i < static_cast<int>(headers.size()); i++) {
            if (i == idxCounty || i == idxState) continue;
            string key = headers[i];
            if (cells[i].empty()) continue;
            try {
                double val = stod(cells[i]);
                c.metrics[key] = val;
            } catch (...) {
                // skips invalid
                cout << "Warning: non-numeric value for county '" << c.name << "', metric '" << key << "': \"" << cells[i] << "\"\n";
            }
        }
        counties.push_back(move(c));
    }
    return counties;
}

// Function to compute the score for a county based on selected metrics and their weights
double compute_score(const County &c, const unordered_map<string, double> &weights) {
    double score = 0;
    double totalWeight = 0;
    for (const auto &pair : weights) {
        const string &metric = pair.first;
        double weight = pair.second;

        // Check if the metric exists in the county's data
        auto it = c.metrics.find(metric);
        if (it != c.metrics.end()) {
            score += it->second * weight; // Add weighted score
            totalWeight += weight; // Accumulate total weight
        }
    }
    if (totalWeight == 0) {
        return 0.0; // Avoid division by zero
    }
    return score / totalWeight; // Normalize the score
}

void mergeSortArr(
    vector<pair<double, const County*>> &arr,
    int l,
    int r,
    vector<pair<double, const County*>> &tmp,
    bool wantMin
) {
    // Base case: if the range has 1 or 0 elements, it's already sorted
    if (r - l <= 1) {
        return;
    }

    // Find the middle point
    int m = (l + r) / 2;

    // Recursively sort both halves
    mergeSortArr(arr, l, m, tmp, wantMin);
    mergeSortArr(arr, m, r, tmp, wantMin);

    // Merge the two halves
    int i = l;     // pointer for the left half
    int j = m;     // pointer for the right half
    int k = l;     // pointer for the merged array (tmp)

    // Merge elements into tmp based on wantMin
    while (i < m || j < r) {
        bool takeLeft;

        // If the right half is empty, take from left
        if (j >= r) {
            takeLeft = true;
        }
        // If left half is empty, take from right
        else if (i >= m) {
            takeLeft = false;
        }
        // Compare values and decide
        else if (wantMin) {
            takeLeft = arr[i].first < arr[j].first; // Sort ascending
        } else {
            takeLeft = arr[i].first > arr[j].first; // Sort descending
        }

        // Copy the appropriate element to tmp
        if (takeLeft) {
            tmp[k] = arr[i];
            i++;
        } else {
            tmp[k] = arr[j];
            j++;
        }

        k++;
    }

    // Copy merged result back to original array
    for (int x = l; x < r; x++) {
        arr[x] = tmp[x];
    }
}
vector<pair<double,const County*>> get_ranked_counties(
    const vector<County> &counties,
    const unordered_map<string, double> &weights,
    bool wantMin,
    int K
) {
    int N = counties.size();
    // 1) Build a scored array
    vector<pair<double,const County*>> scored;
    scored.reserve(N);
    for (const auto &c : counties) {
        double s = compute_score(c, weights);
        scored.emplace_back(s, &c);
    }

    // 2) Prepare temp buffer & sort
    vector<pair<double,const County*>> tmp(N);
    mergeSortArr(scored, 0, N, tmp, wantMin);

    // 3) Slice out the top‐K or bottom‐K
    vector<pair<double,const County*>> result;
    result.reserve(K);
    if (wantMin) {
        // smallest-K: first K
        for (int i = 0; i < K && i < N; ++i)
            result.push_back(scored[i]);
    } else {
        // largest-K: first K (because arr[0] is largest)
        for (int i = 0; i < K && i < N; ++i)
            result.push_back(scored[i]);
    }
    return result;
}


void printRankedFromCSV(const string &filename) {
    ifstream in(filename);
    if (!in) {
        cerr << "Could not open " << filename << "\n";
        return;
    }
    string line;
    // skip header
    getline(in, line);
    // print each data line
    while (getline(in, line)) {
        cout << line << "\n";
    }
}

int main() {
    vector<pair<char, string>> coreMetricsList = loadCoreMetrics();
    vector<pair<char, string>> fullMetricsList = loadFullMetrics();

    vector<string> selectedMetrics = runMenu(coreMetricsList, fullMetricsList);
    //creates a sorted map with rank 1 first and rank 5 last
    map<int, string> metricsRankMap = rankMetric(selectedMetrics);
    // Display selected metrics and their respective rankings
    cout << "\nYou selected the following metrics:\n";
    for (const auto &pair : metricsRankMap) {
        cout << "- Rank " << pair.first << ": " << pair.second << endl;
    }

    // Load county data
    vector<County> counties = loadCounties("county_demographics.csv");

    //creates metric weight map
    unordered_map<string, double> weights;
    for (auto &p : metricsRankMap) {
        double rank = p.first; //the ranking in double
        string name = p.second; //the metric
        weights[name] = 6.0 - rank;
    }

    cout << "Enter 0 to get the highest ranked counties, enter 1 to get the lowest ranked counties:" << endl;
    int choice = 0;
    cin >> choice;
    bool wantMin = (choice == 1);

    cout << "How many counties do you want to rank? (default 10): ";
    int K = 10;
    cin >> K;
    if (K <= 0) {
        K = 10; // Default to 10 if invalid input
    }

    auto ranked = get_ranked_counties(counties, weights, wantMin, K);

    // Write to CSV
    ofstream out("ranked.csv");
    if (!out.is_open()) {
        cout << "\nFailed to write ranked.csv" << endl;
        return 1;
    }
    out << "County,State,Score\n";
    for (const auto &entry : ranked) {
        double score = entry.first;
        const County *c = entry.second;
        out << "\"" << c->name << "\",\"" << c->state << "\"," << fixed << setprecision(2) << score << "\n";
    }
    out.close();

    cout << "\nRanking complete! Results written to 'ranked.csv'.\n";
    cout << "Would you like to print them out? (Y/N)" << endl;
    char printOut;
    cin >> printOut;
    if (printOut == 'Y') {
        printRankedFromCSV("ranked.csv");
    }
    return 0;
}