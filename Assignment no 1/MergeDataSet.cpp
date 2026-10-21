#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm> // For std::count
#include <sstream>
#include <cstdio>

using namespace std;

// 1. Using a 'class' instead of a 'struct'
class TransactionRow {
public: // We must specify public so the merge sort can access these variables
    string text;
    vector<string> values;
    long long dateValue; 
    
    // Constructor to easily create a new row object
    TransactionRow(string t, vector<string> v, long long d) {
        text = t;
        values = v;
        dateValue = d;
    }
};

// Parse one CSV record, including commas inside quoted fields.
vector<string> parseCSV(const string& record) {
    vector<string> fields;
    string field;
    bool inQuotes = false;

    for (size_t i = 0; i < record.size(); ++i) {
        char ch = record[i];
        if (ch == '"') {
            if (inQuotes && i + 1 < record.size() && record[i + 1] == '"') {
                field += '"';
                ++i;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (ch == ',' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += ch;
        }
    }
    fields.push_back(field);
    return fields;
}

string csvEscape(const string& value) {
    if (value.find_first_of(",\"\n\r") == string::npos) return value;
    string escaped = "\"";
    for (char ch : value) {
        if (ch == '"') escaped += "\"\"";
        else escaped += ch;
    }
    return escaped + "\"";
}

// 2. Your EXACT merge sort logic, adapted for the class
void merge(vector<TransactionRow>& arr, int si, int mid, int ei) {
    vector<TransactionRow> temp;
    int i = si, j = mid + 1;
    
    while (i <= mid && j <= ei) {
        // Compare the numeric date values directly
        if (arr[i].dateValue < arr[j].dateValue) temp.push_back(arr[i++]);
        else temp.push_back(arr[j++]);
    }
    
    while (i <= mid) temp.push_back(arr[i++]);
    while (j <= ei) temp.push_back(arr[j++]);
    
    for (int idx = si, x = 0; idx <= ei; idx++) {
        arr[idx] = temp[x++];
    }
}

void mergeSort(vector<TransactionRow>& arr, int si, int ei) {
    if (si >= ei) return;
    int mid = si + (ei - si) / 2;
    mergeSort(arr, si, mid);
    mergeSort(arr, mid + 1, ei);
    merge(arr, si, mid, ei);
}

int main() {
    ifstream file("Retail_Transaction_Dataset.csv");
    vector<TransactionRow> dataset;
    string line, header;
    
    // Check if the file opened successfully
    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return 1;
    }
    
    getline(file, header);
    vector<string> headers = parseCSV(header);
    int dateColumn = -1;
    for (int i = 0; i < static_cast<int>(headers.size()); ++i) {
        if (headers[i] == "TransactionDate") {
            dateColumn = i;
            break;
        }
    }
    if (dateColumn == -1) {
        cout << "Error: TransactionDate column was not found." << endl;
        return 1;
    }
    // 3. Loop to read and parse the CSV
    while (getline(file, line)) {
        
        // Handle rows that break onto a new line because of quotes
        while (count(line.begin(), line.end(), '"') % 2 != 0) {
            string extra;
            getline(file, extra);
            line += "\n" + extra;
        }
        
        vector<string> values = parseCSV(line);
        if (dateColumn >= static_cast<int>(values.size())) continue;
        
        // Extract Month, Day, Year, Hour, and Minute from the date column.
        int m, d, y, h, min;
        if (sscanf(values[dateColumn].c_str(), "%d/%d/%d %d:%d", &m, &d, &y, &h, &min) != 5) continue;
        
        // The Math Trick: Turn the date into a single large number
        long long dateVal = y * 100000000LL + m * 1000000LL + d * 10000LL + h * 100LL + min;
        
        // Use the class constructor to add the data to our vector
        dataset.push_back(TransactionRow(line, values, dateVal));
    }

    // 4. Sort the dataset
    if (dataset.size() > 0) {
        mergeSort(dataset, 0, dataset.size() - 1);
    }

    // Print columns alphabetically by column name, followed by the first 10
    // transactions in ascending transaction-date order.
    vector<int> columnOrder(headers.size());
    for (int i = 0; i < static_cast<int>(columnOrder.size()); ++i) columnOrder[i] = i;
    sort(columnOrder.begin(), columnOrder.end(), [&](int a, int b) {
        return headers[a] < headers[b];
    });

    cout << "Transactions sorted by date; fields shown alphabetically by column name.\n";
    cout << "Top 10 transactions (earliest first):\n\n";
    size_t rowsToPrint = min<size_t>(10, dataset.size());
    for (size_t row = 0; row < rowsToPrint; ++row) {
        cout << "Transaction " << row + 1 << "\n";
        for (size_t col = 0; col < columnOrder.size(); ++col) {
            int sourceColumn = columnOrder[col];
            cout << "  " << headers[sourceColumn] << ": ";
            if (sourceColumn < static_cast<int>(dataset[row].values.size()))
                cout << dataset[row].values[sourceColumn];
            cout << '\n';
        }
        cout << '\n';
    }

    return 0;
}

