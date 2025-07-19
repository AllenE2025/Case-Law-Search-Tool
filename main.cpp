#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Case {
    string id;
    string title;
    string summary;
    string tags;
};

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == string::npos) ? "" : str.substr(first, last - first + 1);
}

vector<Case> loadCases(const string& filename) {
    vector<Case> cases;
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cerr << "Error: Could not open file '" << filename << "'\n";
        return cases;
    }

    // Skip header
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string id, title, summary, tags;

        getline(ss, id, ',');
        getline(ss, title, ',');
        getline(ss, summary, ',');
        getline(ss, tags, ',');

        cases.push_back({id, title, summary, tags});
    }

    return cases;
}

bool containsIgnoreCase(const string& text, const string& keyword) {
    string textLower = text, keywordLower = keyword;
    transform(textLower.begin(), textLower.end(), textLower.begin(), ::tolower);
    transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(), ::tolower);
    return textLower.find(keywordLower) != string::npos;
}

vector<Case> searchCases(const vector<Case>& cases, const string& keyword) {
    vector<Case> matches;

    for (const auto& c : cases) {
        if (containsIgnoreCase(c.title, keyword) ||
            containsIgnoreCase(c.summary, keyword) ||
            containsIgnoreCase(c.tags, keyword)) {
            matches.push_back(c);
        }
    }

    return matches;
}

void displayCases(const vector<Case>& cases) {
    for (const auto& c : cases) {
        cout << "-------------------------------\n";
        cout << "Case ID: " << c.id << "\n";
        cout << "Title: " << c.title << "\n";
        cout << "Summary: " << c.summary << "\n";
        cout << "Tags: " << c.tags << "\n";
        cout << "-------------------------------\n";
    }
}

void exportResults(const vector<Case>& cases, const string& filename) {
    ofstream out(filename);

    if (!out.is_open()) {
        cerr << "Error: Could not write to file '" << filename << "'\n";
        return;
    }

    for (const auto& c : cases) {
        out << "Case ID: " << c.id << "\n";
        out << "Title: " << c.title << "\n";
        out << "Summary: " << c.summary << "\n";
        out << "Tags: " << c.tags << "\n";
        out << "-------------------------------\n";
    }

    out.close();
    cout << "Results exported to '" << filename << "' successfully.\n";
}

int main() {
    string filename = "cases.csv";
    vector<Case> cases = loadCases(filename);

    if (cases.empty()) {
        cerr << "No cases loaded. Please check your file.\n";
        return 1;
    }

    while (true) {
        string keyword;
        cout << "\n🔍 Enter keyword to search for (or type 'exit' to quit): ";
        getline(cin, keyword);
        keyword = trim(keyword);

        if (keyword == "exit") break;

        vector<Case> matches = searchCases(cases, keyword);

        if (matches.empty()) {
            cout << "❌ No matching cases found for \"" << keyword << "\".\n";
        } else {
            cout << "✅ Found " << matches.size() << " matching case(s):\n";
            displayCases(matches);

            // Ask to export
            string exportChoice;
            cout << "📤 Export results to file? (y/n): ";
            getline(cin, exportChoice);

            if (exportChoice == "y" || exportChoice == "Y") {
                string outFile;
                cout << "Enter output filename (e.g., results.txt): ";
                getline(cin, outFile);
                exportResults(matches, outFile);
            }
        }
    }

    cout << "👋 Exiting Case Search Tool. Goodbye!\n";
    return 0;
}
