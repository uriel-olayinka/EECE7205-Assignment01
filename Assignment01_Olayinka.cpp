#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <functional>

using namespace std;


void printVec(const vector<int>& v) {
    cout << "[";
    for (size_t j = 0; j < v.size(); j++) {
        cout << v[j];
        if (j + 1 < v.size()) {
            cout << ", ";
        }
    }
    cout << "]";
}

void printVec(const vector<string>& v) {
    cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        cout << "\"" << v[i] << "\"";
        if (i + 1 < v.size()) cout << ", ";
    }
    cout << "]";
}

void printGroups(const vector<vector<string>>& groups) {
    cout << "[";
    for (size_t i = 0; i < groups.size(); i++) {
        cout << "[";
        for (size_t j = 0; j < groups[i].size(); j++) {
            cout << "\"" << groups[i][j] << "\"";
            if (j + 1 < groups[i].size()) {
                cout << ", ";
            }
        }
        cout << "]";
        if (i + 1 < groups.size()) {
            cout << ", ";
        }
    }
    cout << "]";
}


// ---------------------------------------------------------------
// Problem 1: Merge Intervals
// ---------------------------------------------------------------

struct Interval {
    int start;
    int end;
};

vector<Interval> mergeIntervals(vector<Interval> intervals) {
    vector<Interval> merged;

    if (intervals.empty()) {
        return merged;
    }

    sort(intervals.begin(), intervals.end(), [](const Interval& a, const Interval& b) {
        return a.start < b.start;
    });

    merged.push_back(intervals[0]);

    for (size_t i = 1; i < intervals.size(); i++) {
        Interval& last = merged.back();

        if (intervals[i].start <= last.end) {
            last.end = max(last.end, intervals[i].end);
        } else {
            merged.push_back(intervals[i]);
        }
    }

    return merged;
}

void printIntervals(const vector<Interval>& v) {
    cout << "[";
    for (size_t j = 0; j < v.size(); j++) {
        cout << "[" << v[j].start << "," << v[j].end << "]";
        if (j + 1 < v.size()) {
            cout << ", ";
        }
    }
    cout << "]";
}

void runTest(string label, vector<Interval> input) {
    cout << "\nTest: " << label << "\n";
    cout << "  Input:  ";
    printIntervals(input);
    cout << "\n  Output: ";
    printIntervals(mergeIntervals(input));
    cout << "\n";
}

void runProblem1() {
    cout << "\n=== Problem 1: Merge Intervals ===\n";
    runTest("Example", {{1, 3}, {2, 6}, {8, 10}, {15, 18}});
    runTest("Unsorted + fully contained interval", {{8, 10}, {1, 10}, {2, 3}, {15, 18}});
    runTest("Touching endpoints", {{1, 2}, {2, 3}, {5, 6}});
    runTest("Edge case: empty input", {});
    runTest("Edge case: single interval", {{4, 7}});
}

// ---------------------------------------------------------------
// Problem 2: Sort Items
// ---------------------------------------------------------------

void countingSortItems(vector<int>& items) {
    int counts[3] = {0, 0, 0};   // counts[0] = how many 0s, counts[1] = 1s, counts[2] = 2s

    for (int c : items) {
        counts[c]++;
    }

    int index = 0;
    for (int color = 0; color < 3; color++) {
        for (int i = 0; i < counts[color]; i++) {
            items[index++] = color;
        }
    }
}

void dutchFlagSort(vector<int>& items) {
    int low = 0;
    int mid = 0;
    int high = items.size() - 1;

    while (mid <= high) {
        if (items[mid] == 0) {
            swap(items[low], items[mid]);
            low++;
            mid++;
        } else if (items[mid] == 1) {
            mid++;
        } else {
            swap(items[mid], items[high]);
            high--;
        }
    }
}

void runTest2(string label, vector<int> input) {
    vector<int> original = input;
    cout << "\nTest: " << label << "\n";
    cout << "  Input:  ";
    printVec(input);
    countingSortItems(input);
    cout << "\n  Output (counting sort): ";
    printVec(input);

    // Reset input for Dutch Flag sort
    vector<int> input2 = original;
    dutchFlagSort(input2);
    cout << "\n  Output (Dutch Flag sort): ";
    printVec(input2);
    cout << "\n";
}

void runProblem2() {
    cout << "\n=== Problem 2: Sort Items ===\n";
    runTest2("Example", {2, 0, 2, 1, 1, 0});
    runTest2("Already sorted", {0, 0, 1, 1, 2, 2});
    runTest2("Reverse order", {2, 2, 1, 1, 0, 0});
    runTest2("Edge case: all the same value", {1, 1, 1});
    runTest2("Edge case: single element", {2});
    runTest2("Edge case: empty input", {});
}

// ---------------------------------------------------------------
// Problem 3: Largest Number
// ---------------------------------------------------------------

string largestNumber(const vector<int>& nums) {
    vector<string> parts;
    for (int n : nums) {
        parts.push_back(to_string(n));
    }

    sort(parts.begin(), parts.end(), [](const string& a, const string& b) {
        return a + b > b + a;
    });

    if (parts.empty()) {
        return "";
    }

    if (parts[0] == "0") {
        return "0";
    }

    string result;
    for (const string& part : parts) {
        result += part;
    }
    return result;
}

void runTest3(string label, const vector<int>& nums) {
    cout << "\nTest: " << label << "\n";
    cout << "  Input:  ";
    printVec(nums);
    cout << "\n  Output: \"" << largestNumber(nums) << "\"\n";
}

void runProblem3() {
    cout << "\n=== Problem 3: Largest Number ===\n";
    runTest3("Example", {3, 30, 34, 5, 9});
    runTest3("Simple pair", {10, 2});
    runTest3("Shared prefix", {121, 12});
    runTest3("Edge case: all zeros", {0, 0, 0});
    runTest3("Edge case: single number", {7});
    runTest3("Edge case: empty input", {});
}

// ---------------------------------------------------------------
// Problem 4: Group Anagrams
// ---------------------------------------------------------------

vector<vector<string>> groupAnagrams(const vector<string>& words) {
    unordered_map<string, int> groupIndex;   // fingerprint -> position in groups
    vector<vector<string>> groups;

    for (const string& word : words) {
        string key = word;
        sort(key.begin(), key.end());

        auto found = groupIndex.find(key);
        if (found == groupIndex.end()) {
            groupIndex[key] = static_cast<int>(groups.size());
            groups.push_back({word});
        } else {
            groups[found->second].push_back(word);
        }
    }
    return groups;
}

void runTest4(string label, const vector<string>& words) {
    cout << "\nTest: " << label << "\n";
    cout << "  Input:  ";
    printVec(words);
    cout << "\n  Output: ";
    printGroups(groupAnagrams(words));
    cout << "\n";
}

void runProblem4() {
    cout << "\n=== Problem 4: Group Anagrams ===\n";
    runTest4("Example", {"eat", "tea", "tan", "ate", "nat", "bat"});
    runTest4("Single group", {"abc", "cab", "bca"});
    runTest4("No anagrams", {"dog", "cat", "fish"});
    runTest4("Edge case: empty string", {""});
    runTest4("Edge case: single word", {"a"});
    runTest4("Edge case: empty input", {});
}

// ---------------------------------------------------------------
// Problem 5: Meetings
// ---------------------------------------------------------------

bool canAttendAllMeetings(vector<Interval> meetings) {
    sort(meetings.begin(), meetings.end(), [](const Interval& a, const Interval& b) {
        return a.start < b.start;
    });

    for (size_t i = 1; i < meetings.size(); i++) {
        if (meetings[i].start < meetings[i - 1].end) {
            return false;
        }
    }
    return true;
}

void runTest5(string label, vector<Interval> meetings) {
    cout << label << ": ";
    printIntervals(meetings);
    cout << " -> " << (canAttendAllMeetings(meetings) ? "true" : "false") << endl;
}

void runProblem5() {
    cout << "\n=== Problem 5: Meetings ===\n";
    runTest5("Example (overlap)", {{0, 30}, {5, 10}, {15, 20}});
    runTest5("No overlap, unsorted", {{7, 10}, {2, 4}});
    runTest5("Back-to-back meetings", {{1, 5}, {5, 8}, {8, 9}});
    runTest5("Meeting inside a longer one", {{1, 10}, {2, 3}, {4, 5}});
    runTest5("Edge case: empty input", {});
    runTest5("Edge case: single meeting", {{3, 6}});
}

// ---------------------------------------------------------------
// Problem 6: K Closest Points
// ---------------------------------------------------------------

struct Point {
    int x;
    int y;
};

long long squaredDistance(const Point& p) {
    return static_cast<long long>(p.x) * p.x + static_cast<long long>(p.y) * p.y;
}

vector<Point> kClosestBySorting(vector<Point> points, int k) {
    if (k <= 0) {
        return {};
    }

    sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
        return squaredDistance(a) < squaredDistance(b);
    });

    if (k < static_cast<int>(points.size())) {
        points.resize(k);
    }
    return points;
}

vector<Point> kClosestByHeap(const vector<Point>& points, int k) {
    if (k <= 0) {
        return {};
    }
    
    priority_queue<pair<long long, int>> heap;
    
    for (int i = 0; i < static_cast<int>(points.size()); i++) {
        heap.push({squaredDistance(points[i]), i});
        if (heap.size() > static_cast<size_t>(k)) {
            heap.pop();
        }
    }

    vector<Point> result;
    while (!heap.empty()) {
        result.push_back(points[heap.top().second]);
        heap.pop();
    }
    reverse(result.begin(), result.end());
    return result;
}

void printPoints(const vector<Point>& points) {
    cout << "[";
    for (size_t i = 0; i < points.size(); i++) {
        cout << "(" << points[i].x << "," << points[i].y << ")";
        if (i + 1 < points.size()) {
            cout << ", ";
        }
    }
    cout << "]";
}

void runTest6(string label, const vector<Point>& points, int k) {
    cout << "\nTest: " << label << "\n";
    cout << "  Input:  ";
    printPoints(points);
    cout << ", k = " << k;

    vector<Point> sortedResult = kClosestBySorting(points, k);
    cout << "\n  Output (sorting): ";
    printPoints(sortedResult);

    vector<Point> heapResult = kClosestByHeap(points, k);
    cout << "\n  Output (heap): ";
    printPoints(heapResult);
    cout << "\n";
}

void runProblem6() {
    cout << "\n=== Problem 6: K Closest Points ===\n";
    runTest6("Example", {{1, 3}, {-2, 2}, {5, 8}, {0, 1}}, 2);
    runTest6("All points same distance", {{1, 1}, {-1, -1}, {1, -1}, {-1, 1}}, 3);
    runTest6("Edge case: k = 0", {{1, 2}, {3, 4}}, 0);
    runTest6("Edge case: k = number of points", {{2, 3}, {4, 5}}, 2);
    runTest6("Edge case: k larger than number of points", {{1, 2}, {3, 4}}, 5);
    runTest6("Edge case: single point", {{7, 8}}, 1);
    runTest6("Edge case: empty input", {}, 3);
}

// ---------------------------------------------------------------
// Problem 7: Top K Frequent Elements
// ---------------------------------------------------------------

vector<int> topKFrequent(const vector<int>& nums, int k) {
    unordered_map<int, int> counts;   // value -> how many times it appears

    for (int v : nums) {
        counts[v]++;
    }

    vector<pair<int, int>> entries(counts.begin(), counts.end());

    sort(entries.begin(), entries.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        if (a.second != b.second) {
            return a.second > b.second;  // Higher count goes first
        }
        return a.first < b.first;  // Smaller value goes first
    });

    vector<int> result;
    for (int i = 0; i < min(k, static_cast<int>(entries.size())); i++) {
        result.push_back(entries[i].first);
    }
    return result;
}

void runTest7(string label, const vector<int>& nums, int k) {
    cout << "\nTest: " << label << "\n";
    cout << "  Input:  ";
    printVec(nums);
    cout << ", k = " << k;
    vector<int> result = topKFrequent(nums, k);
    cout << "\n  Output: ";
    printVec(result);
    cout << "\n";
}

void runProblem7() {
    cout << "\n=== Problem 7: Top K Frequent Elements ===\n";
    runTest7("Example", {1, 1, 1, 2, 2, 3}, 2);
    runTest7("Tie: smaller value first", {5, 5, 3, 3, 1}, 2);
    runTest7("Negative numbers", {-1, -1, 2, 2, 2, -3}, 2);
    runTest7("Edge case: single element", {1}, 1);
    runTest7("Edge case: k larger than distinct values", {4, 4, 6}, 5);
    runTest7("Edge case: empty input", {}, 2);
}

// ---------------------------------------------------------------
// Problem 8: Merge K Sorted Arrays
// ---------------------------------------------------------------

vector<int> mergeKSortedArrays(const vector<vector<int>>& arrays) {
    priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> minHeap;

    for (int i = 0; i < static_cast<int>(arrays.size()); i++) {
        if (!arrays[i].empty()) {
            minHeap.push({arrays[i][0], i, 0});
        }
    }

    vector<int> merged;

    while (!minHeap.empty()) {
        vector<int> entry = minHeap.top();
        minHeap.pop();
        merged.push_back(entry[0]);

        int a = entry[1];
        int p = entry[2];
        if (p + 1 < static_cast<int>(arrays[a].size())) {
            minHeap.push({arrays[a][p + 1], a, p + 1});
        }
    }

    return merged;
}

void printVecOfVec(const vector<vector<int>>& v) {
    cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        cout << "[";
        for (size_t j = 0; j < v[i].size(); j++) {
            cout << v[i][j];
            if (j + 1 < v[i].size()) {
                cout << ", ";
            }
        }
        cout << "]";
        if (i + 1 < v.size()) {
            cout << ", ";
        }
    }
    cout << "]";
}

void runTest8(string label, const vector<vector<int>>& arrays) {
    cout << "\nTest: " << label << "\n";
    cout << "  Input:  ";
    printVecOfVec(arrays);
    vector<int> result = mergeKSortedArrays(arrays);
    cout << "\n  Output: ";
    printVec(result);
    cout << "\n";
}

void runProblem8() {
    cout << "\n=== Problem 8: Merge K Sorted Arrays ===\n";
    runTest8("Example", {{1, 4, 7}, {2, 5, 8}, {3, 6, 9}});
    runTest8("Different lengths, duplicates", {{1, 3, 3}, {2, 3}, {0, 10}});
    runTest8("Edge case: one array is empty", {{1, 3}, {}, {2, 2, 4}});
    runTest8("Edge case: single array", {{5, 6, 7}});
    runTest8("Edge case: no arrays", {});
    runTest8("Edge case: arrays with empty arrays", {{}, {}, {}});
}

// ---------------------------------------------------------------
// Problem 9: Kth Largest Element
// ---------------------------------------------------------------

bool findKthLargest(const vector<int>& nums, int k, int& result) {
    if (k < 1 || k > static_cast<int>(nums.size())) {
        return false;
    }

    priority_queue<int, vector<int>, greater<int>> minHeap;

    for (int num : nums) {
        minHeap.push(num);
        if (static_cast<int>(minHeap.size()) > k) {
            minHeap.pop();
        }
    }

    result = minHeap.top();
    return true;
}

void runTest9(string label, const vector<int>& nums, int k) {
    cout << "\nTest: " << label << "\n";
    cout << "  Input:  ";
    printVec(nums);
    cout << ", k = " << k;

    int result;
    if (findKthLargest(nums, k, result)) {
        cout << "\n  Output: " << result << "\n";
    } else {
        cout << "\n  Output: Invalid k\n";
    }
}

void runProblem9() {
    cout << "\n=== Problem 9: Kth Largest Element ===\n";
    runTest9("Example", {3, 2, 1, 5, 6, 4}, 2);
    runTest9("Single element", {10}, 1);
    runTest9("All elements same", {7, 7, 7, 7}, 3);
    runTest9("Negative numbers", {-1, -2, -3, -4}, 2);
    runTest9("Edge case: k larger than array size", {1, 2}, 5);
    runTest9("Edge case: k = 0", {1, 2, 3}, 0);
    runTest9("Duplicates count separately", {3, 2, 3, 1, 2, 4, 5, 5, 6}, 4);
}

// ---------------------------------------------------------------
// Problem 10: Count Inversions
// ---------------------------------------------------------------

long long mergeAndCount(vector<int>& values, vector<int>& temp, int left, int mid, int right) {
    long long count = 0;
    int i = left;       // walks the left half
    int j = mid + 1;    // walks the right half
    int t = left;       // where the next item goes in temp

    while (i <= mid && j <= right) {
        if (values[i] <= values[j]) {
            temp[t++] = values[i++];
        } else {
            temp[t++] = values[j++];
            count += (mid - i + 1);
        }
    }

    while (i <= mid) {
        temp[t++] = values[i++];
    }
    while (j <= right) {
        temp[t++] = values[j++];
    }

    for (i = left; i <= right; i++) {
        values[i] = temp[i];
    }
    return count;
}

long long sortAndCount(vector<int>& values, vector<int>& temp, int left, int right) {
    if (left >= right) {
        return 0;
    }

    int mid = left + (right - left) / 2;
    long long count = 0;
    count += sortAndCount(values, temp, left, mid);
    count += sortAndCount(values, temp, mid + 1, right);
    count += mergeAndCount(values, temp, left, mid, right);
    return count;
}

long long countInversions(vector<int> values) {
    if (values.size() < 2) {
        return 0;
    }

    vector<int> temp(values.size());
    return sortAndCount(values, temp, 0, static_cast<int>(values.size()) - 1);
}

long long countInversionsBruteForce(const vector<int>& values) {
    long long count = 0;
    for (size_t i = 0; i < values.size(); i++) {
        for (size_t j = i + 1; j < values.size(); j++) {
            if (values[i] > values[j]) {
                count++;
            }
        }
    }
    return count;
}

void runTest10(string label, vector<int> values) {
    cout << "\nTest: " << label << "\n";
    cout << "  Input:  ";
    printVec(values);
    long long inversions = countInversions(values);
    long long inversionsBF = countInversionsBruteForce(values);
    cout << "\n  Output: " << inversions << " (optimized), " << inversionsBF << " (brute force)\n";
    if (inversions == inversionsBF) {
        cout << " [match]";
    } else {
        cout << " [MISMATCH]";
    }
    cout << "\n";
}

void runProblem10() {
    cout << "\n=== Problem 10: Count Inversions ===\n";
    runTest10("Example", {2, 4, 1, 3, 5});
    runTest10("Already sorted", {1, 2, 3, 4, 5});
    runTest10("Reverse order", {5, 4, 3, 2, 1});
    runTest10("Duplicates", {1, 3, 2, 3, 1});
    runTest10("Edge case: single element", {42});
    runTest10("Edge case: empty input", {});
    runTest10("Edge case: all same elements", {7, 7, 7, 7});
}

// ---------------------------------------------------------------
// Menu
// ---------------------------------------------------------------

int main() {
    int choice = -1;

    while (choice != 0) {
        cout << "\n===== EECE 7205 Assignment 01 =====\n";
        cout << "1. Merge Intervals" << endl;
        cout << "2. Sort Items" << endl;
        cout << "3. Largest Number" << endl;
        cout << "4. Group Anagrams" << endl;
        cout << "5. Meetings" << endl;
        cout << "6. K Closest Points" << endl;
        cout << "7. Top K Frequent Elements" << endl;
        cout << "8. Merge K Sorted Arrays" << endl;
        cout << "9. Kth Largest Element" << endl;
        cout << "10. Count Inversions" << endl;
        cout << "0. Exit" << endl;
        cout << "Choose a problem: ";

        cin >> choice;

        switch (choice) {
            case 1: runProblem1(); break;
            case 2: runProblem2(); break;
            case 3: runProblem3(); break;
            case 4: runProblem4(); break;
            case 5: runProblem5(); break;
            case 6: runProblem6(); break;
            case 7: runProblem7(); break;
            case 8: runProblem8(); break;
            case 9: runProblem9(); break;
            case 10: runProblem10(); break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice.\n";
        }
    }
    return 0;
}