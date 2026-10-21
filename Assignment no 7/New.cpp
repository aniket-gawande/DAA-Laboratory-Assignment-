#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

using Graph = vector<set<int>>;

struct Course {
    string name;
    int students;
};

struct Room {
    string name;
    int capacity;
};

struct Schedule {
    vector<int> color;
    int slotCount;
};

Graph buildConflictGraph(
    int courseCount,
    const vector<vector<int>>& studentCourses
) {
    Graph graph(courseCount);

    for (const auto& enrolled : studentCourses) {
        for (size_t i = 0; i < enrolled.size(); ++i) {
            for (size_t j = i + 1; j < enrolled.size(); ++j) {
                int a = enrolled[i];
                int b = enrolled[j];

                if (a != b) {
                    graph[a].insert(b);
                    graph[b].insert(a);
                }
            }
        }
    }

    return graph;
}

int smallestAvailableColor(
    int vertex,
    const Graph& graph,
    const vector<int>& colors
) {
    set<int> used;
    for (int neighbor : graph[vertex]) {
        if (colors[neighbor] != -1) {
            used.insert(colors[neighbor]);
        }
    }

    int color = 0;
    while (used.count(color)) {
        ++color;
    }
    return color;
}

Schedule greedyColoring(const Graph& graph, vector<int> order) {
    int n = static_cast<int>(graph.size());
    vector<int> colors(n, -1);

    for (int vertex : order) {
        colors[vertex] = smallestAvailableColor(vertex, graph, colors);
    }

    int slotCount = 0;
    for (int color : colors) {
        slotCount = max(slotCount, color + 1);
    }

    return {colors, slotCount};
}

Schedule welshPowellColoring(const Graph& graph) {
    int n = static_cast<int>(graph.size());
    vector<int> order(n);
    for (int i = 0; i < n; ++i) {
        order[i] = i;
    }

    sort(order.begin(), order.end(), [&](int a, int b) {
        if (graph[a].size() != graph[b].size()) {
            return graph[a].size() > graph[b].size();
        }
        return a < b;
    });

    return greedyColoring(graph, order);
}

Schedule dsaturColoring(const Graph& graph) {
    int n = static_cast<int>(graph.size());
    vector<int> colors(n, -1);
    vector<bool> colored(n, false);
    int coloredCount = 0;
    int maxColor = -1;

    while (coloredCount < n) {
        int chosen = -1;
        size_t bestSaturation = 0;
        size_t bestDegree = 0;

        for (int vertex = 0; vertex < n; ++vertex) {
            if (colored[vertex]) {
                continue;
            }

            set<int> neighboringColors;
            for (int neighbor : graph[vertex]) {
                if (colors[neighbor] != -1) {
                    neighboringColors.insert(colors[neighbor]);
                }
            }

            size_t saturation = neighboringColors.size();
            size_t degree = graph[vertex].size();

            if (chosen == -1 ||
                saturation > bestSaturation ||
                (saturation == bestSaturation && degree > bestDegree)) {
                chosen = vertex;
                bestSaturation = saturation;
                bestDegree = degree;
            }
        }

        colors[chosen] = smallestAvailableColor(chosen, graph, colors);
        colored[chosen] = true;
        ++coloredCount;
        maxColor = max(maxColor, colors[chosen]);
    }

    return {colors, maxColor + 1};
}

bool assignRooms(
    const vector<Course>& courses,
    const vector<Room>& rooms,
    const vector<int>& colors,
    vector<string>& assignedRooms
) {
    int slotCount = 0;
    for (int color : colors) {
        slotCount = max(slotCount, color + 1);
    }

    assignedRooms.assign(courses.size(), "");
    vector<bool> roomUsed(rooms.size(), false);

    for (int slot = 0; slot < slotCount; ++slot) {
        fill(roomUsed.begin(), roomUsed.end(), false);

        vector<int> exams;
        for (int course = 0; course < static_cast<int>(courses.size()); ++course) {
            if (colors[course] == slot) {
                exams.push_back(course);
            }
        }

        // Schedule the largest exams first to improve room fit.
        sort(exams.begin(), exams.end(), [&](int a, int b) {
            return courses[a].students > courses[b].students;
        });

        for (int course : exams) {
            int bestRoom = -1;

            for (int room = 0; room < static_cast<int>(rooms.size()); ++room) {
                if (roomUsed[room] ||
                    rooms[room].capacity < courses[course].students) {
                    continue;
                }

                if (bestRoom == -1 ||
                    rooms[room].capacity < rooms[bestRoom].capacity) {
                    bestRoom = room;
                }
            }

            if (bestRoom == -1) {
                return false;
            }

            roomUsed[bestRoom] = true;
            assignedRooms[course] = rooms[bestRoom].name;
        }
    }

    return true;
}

void printSchedule(
    const string& algorithm,
    const Schedule& schedule,
    const vector<Course>& courses,
    const vector<Room>& rooms
) {
    vector<string> assignedRooms;
    bool roomsFit = assignRooms(
        courses, rooms, schedule.color, assignedRooms
    );

    cout << "\n============================================================\n";
    cout << algorithm << " TIMETABLE\n";
    cout << "============================================================\n";
    cout << "Number of exam slots (colors) used: " << schedule.slotCount << "\n";
    cout << "Each slot is one exam period. Courses in a slot have no shared-student conflicts.\n";

    for (int slot = 0; slot < schedule.slotCount; ++slot) {
        cout << "\nSLOT " << slot + 1 << "\n";
        cout << "------------------------------------------------------------\n";
        cout << "Course\tStudents\tRoom\tCapacity\tSeats left\n";
        bool hasCourse = false;
        for (int i = 0; i < static_cast<int>(courses.size()); ++i) {
            if (schedule.color[i] != slot) continue;
            hasCourse = true;
            cout << courses[i].name << '\t' << courses[i].students << '\t';
            if (roomsFit) {
                int roomCapacity = 0;
                for (const Room& room : rooms) {
                    if (room.name == assignedRooms[i]) {
                        roomCapacity = room.capacity;
                        break;
                    }
                }
                cout << assignedRooms[i] << '\t' << roomCapacity << '\t'
                     << roomCapacity - courses[i].students << '\n';
            } else {
                cout << "Unavailable\t-\t-\n";
            }
        }
        if (!hasCourse) cout << "No courses assigned.\n";
    }

    cout << "\nSchedule summary: " << courses.size() << " courses across "
         << schedule.slotCount << " slots.\n";
    if (!roomsFit) {
        cout << "Room assignment is infeasible: at least one slot does not have "
                "enough distinct rooms with sufficient capacity.\n";
    } else {
        cout << "Room assignment: feasible; all course and room capacities fit.\n";
    }
}

int main() {
    cout << "============================================================\n";
    cout << "       UNIVERSITY EXAM TIMETABLE SCHEDULER\n";
    cout << " Graph coloring with room allocation and algorithm comparison\n";
    cout << "============================================================\n\n";

    int courseCount;
    cout << "Enter number of courses: ";
    if (!(cin >> courseCount) || courseCount <= 0) {
        cerr << "Error: enter a positive number of courses.\n";
        return 1;
    }

    vector<Course> courses(courseCount);
    cout << "\nEnter each course name and enrolled student count.\n";
    for (int i = 0; i < courseCount; ++i) {
        cout << "Course " << i + 1 << " name (one word): ";
        if (!(cin >> courses[i].name)) {
            cerr << "Error: invalid course name.\n";
            return 1;
        }
        cout << "Students enrolled in " << courses[i].name << ": ";
        if (!(cin >> courses[i].students) || courses[i].students < 0) {
            cerr << "Error: student count must be zero or greater.\n";
            return 1;
        }
    }

    int studentCount;
    cout << "\nEnter number of students (for conflict detection): ";
    if (!(cin >> studentCount) || studentCount < 0) {
        cerr << "Error: student count must be zero or greater.\n";
        return 1;
    }

    vector<vector<int>> studentCourses(studentCount);
    cout << "For each student, enter how many courses they take, followed by "
            "the course numbers (1 to " << courseCount << ").\n";
    cout << "Example: 2 1 3 means this student takes Course 1 and Course 3.\n";
    for (int student = 0; student < studentCount; ++student) {
        int enrolledCount;
        cout << "Student " << student + 1 << " enrollment: ";
        if (!(cin >> enrolledCount) || enrolledCount < 0 || enrolledCount > courseCount) {
            cerr << "Error: enrollment count must be between 0 and "
                 << courseCount << ".\n";
            return 1;
        }
        set<int> uniqueCourses;
        for (int j = 0; j < enrolledCount; ++j) {
            int courseNumber;
            if (!(cin >> courseNumber) || courseNumber < 1 || courseNumber > courseCount) {
                cerr << "Error: course numbers must be between 1 and "
                     << courseCount << ".\n";
                return 1;
            }
            if (!uniqueCourses.insert(courseNumber - 1).second) {
                cerr << "Error: a course cannot be entered twice for one student.\n";
                return 1;
            }
            studentCourses[student].push_back(courseNumber - 1);
        }
    }

    int roomCount;
    cout << "\nEnter number of available rooms: ";
    if (!(cin >> roomCount) || roomCount <= 0) {
        cerr << "Error: enter at least one room.\n";
        return 1;
    }

    vector<Room> rooms(roomCount);
    cout << "Enter each room name and seating capacity.\n";
    for (int i = 0; i < roomCount; ++i) {
        cout << "Room " << i + 1 << " name (one word): ";
        if (!(cin >> rooms[i].name)) {
            cerr << "Error: invalid room name.\n";
            return 1;
        }
        cout << "Capacity of " << rooms[i].name << ": ";
        if (!(cin >> rooms[i].capacity) || rooms[i].capacity <= 0) {
            cerr << "Error: room capacity must be positive.\n";
            return 1;
        }
    }

    Graph graph = buildConflictGraph(
        static_cast<int>(courses.size()),
        studentCourses
    );

    size_t conflictCount = 0;
    cout << "\n================ CONFLICT GRAPH ================\n";
    cout << "Course conflicts (an edge means at least one shared student):\n";
    for (int i = 0; i < static_cast<int>(courses.size()); ++i) {
        cout << courses[i].name << " (" << graph[i].size() << " conflicts): ";
        if (graph[i].empty()) cout << "None";
        else {
            bool first = true;
            for (int neighbor : graph[i]) {
                if (i < neighbor) ++conflictCount;
                if (!first) cout << ", ";
                cout << courses[neighbor].name;
                first = false;
            }
        }
        cout << '\n';
    }
    cout << "Total unique conflict pairs: " << conflictCount << '\n';

    Schedule greedy = greedyColoring(
        graph,
        [&]() {
            vector<int> order(courseCount);
            for (int i = 0; i < courseCount; ++i) order[i] = i;
            return order;
        }()
    );
    Schedule dsatur = dsaturColoring(graph);
    Schedule welshPowell = welshPowellColoring(graph);

    printSchedule("Greedy Coloring", greedy, courses, rooms);
    printSchedule("DSATUR", dsatur, courses, rooms);
    printSchedule("Welsh-Powell", welshPowell, courses, rooms);

    cout << "\n================ ALGORITHM COMPARISON ================\n";
    cout << "Greedy: " << greedy.slotCount << " slots\n";
    cout << "DSATUR: " << dsatur.slotCount << " slots\n";
    cout << "Welsh-Powell: " << welshPowell.slotCount << " slots\n";
    cout << "Fewer slots means a more compact timetable for this input.\n";

    return 0;
}
