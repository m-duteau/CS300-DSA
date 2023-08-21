//============================================================================
// Name        : ABCU_Course_Listing.cpp
// Author      : Michael Duteau
// Version     : 1.0
// Description : Hello World in C++, Ansi-style; primarily utilizes
//             : BinarySearchTree.cpp, CSVparser.cpp, and CSVparser.hpp
//             : from module 5 as references
//============================================================================

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// structure to hold course information
struct Course {
    string courseNumber; // unique identifier
    string courseName;
    vector<string> PreReqs;
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/*
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;
    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseNumber);
};

/*
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

/*
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
}

/*
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // starts inOrder() traversal at the root node
    inOrder(root);
}

/*
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    if (root == nullptr) {
        // course in course param becomes the root node when the root is null
        root = new Node(course);
    }
    else {
        // addNode() function will iterate through the tree to find the correct placement for course in course param
        addNode(root, course);
    }
}

/*
 * Search for a course
 */
Course BinarySearchTree::Search(string courseNumber) {
    // Search() method will start at the root
    Node* currNode = root;

    while (currNode != nullptr) {
        // the course being searched for is found and returned
        if (currNode->course.courseNumber == courseNumber) {
            return currNode->course;
        }
        // if courseNumber param is less than the curent node, traverses left
        if (courseNumber < currNode->course.courseNumber) {
            currNode = currNode->left;
        }
        // otherwise courseNumber param is greater than current node, traverses right
        else {
            currNode = currNode->right;
        }
    }
    // return an empty course if courseNumber param is not found within the tree
    Course course;
    return course;
}

/*
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // node param's courseNumber greater than course param's, so course will be left of node
    if (node->course.courseNumber > course.courseNumber) {
        // if node param's left is null, course param becomes the left node
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        // otherwise node param's left is not null and continue traversing left with recursive call
        else {
            this->addNode(node->left, course);
        }
    }
    // otherwise node param's courseNumber is less than course param, therefore course will be right of node
    else {
        // if node param's right is null, course param becomes the right node
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        // otherwise node param's right is not null and continue traversing right with recursive call
        else {
            this->addNode(node->right, course);
        }
    }
}

/*
 * recursively outputs the tree's contents inorder
 * per the nature of inorder traversal, this will return the tree's contents in alphanumeric order
 */
void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        //output courseNumber, courseName
        cout << node->course.courseNumber << ": " << node->course.courseName << endl;
        inOrder(node->right);
    }
}

/*
 * Display the course information to the console (std::out)
 */
void displayCourse(Course course) {
    cout << course.courseNumber << ": " << course.courseName << endl;
    // if PreReqs empty, then no PreReqs were stored and printing of PreReqs will be skipped
    if (!course.PreReqs.empty()) {
        cout << "Prerequisites for " << course.courseName << ": ";
        for (int i = 0; i < course.PreReqs.size(); ++i) {
            // if{} branch will append ", " if not the last element of PreReqs
            if (i != course.PreReqs.size() - 1) {
                cout << course.PreReqs.at(i) << ", ";
            }
            else {
                cout << course.PreReqs.at(i) << endl;
            }
        }
    }
    return;
}

/*
 * method for string splitting in loadCourses() method
 * reference: https://stackoverflow.com/a/46931770/19361783
 * 
 * @param str String to be split
 * @param delimiter Char used to determine where the string is to be split
 */
vector<string> splitString(string str, char delimiter) {
    vector<string> results; // stores individual substrings of str param resulting from the split
    stringstream ss(str); // creates stream to parse the string in str param 
    string item; // holds the current substring to be stored in results vector

    while (getline(ss, item, delimiter)) {
        results.push_back(item);
    }

    return results;
}

/*
 * Load a .txt file containing course information into a container
 *
 * @param csvPath the path to the .csv file to load
 * @return a container holding all the courses read
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "\n--- Loading file " << csvPath << " ---\n" << endl;

    vector<string> fileLines; // holds individual lines of the .csv being read
    ifstream courseFile; // instance to load the .csv
    courseFile.open(csvPath);

    if (courseFile.is_open()) {
        while (courseFile.good()) {
            string line; // holds the current line being read
            getline(courseFile, line);
            // add the line to fileLines if the line is not empty/blank
            if (line != "")
                fileLines.push_back(line);
        }
        courseFile.close();

        // checks that data was actually read from the file
        // i.e. if fileLines is empty, then nothing was read from the file
        if (fileLines.size() == 0) {
            cout << "No data was found in " << csvPath << "." << endl;
        }

        cout << "File read successfully.\n" << endl;
    }    

    // if fileLines isn't empty, data will be allocated to course objects
    if (fileLines.size() != 0) {
        cout << "Allocating data...\n" << endl;
        // iterate through each element of fileLines
        for (unsigned int i = 0; i < fileLines.size(); ++i) {
            Course course; // new course object
            string currLine = fileLines.at(i); // holds the current element of fileLines
            // a vector is created using the splitString() function
            // the vector brokenStrings will hold the substrings obtained from the string held in currLine
            vector<string> brokenStrings = splitString(currLine, ',');
            // if there are less than two elements in brokenStrings, then the file is not formatted correctly and the function terminates
            if (brokenStrings.size() < 2) {
                cout << "Incorrect file format." << endl;
                return;
            }
            // iterate through each substring stored in brokenStrings
            // if file format correct, .at(0) will always be courseNumber, .at(1) courseName, and anything else a PreReq
            for (unsigned int j = 0; j < brokenStrings.size(); ++j) {
                if (j == 0) {
                    course.courseNumber = brokenStrings.at(j);
                }
                else if (j == 1) {
                    course.courseName = brokenStrings.at(j);
                }
                else {
                    // PreReq will only be stored if brokenStrings.at(j) is not empty/blank
                    if (brokenStrings.at(j) != "") {
                        course.PreReqs.push_back(brokenStrings.at(j));
                    }
                }
            }
            // empty the brokenStrings vector
            brokenStrings.clear();
            // course is then added to the tree using Insert() method
            bst->Insert(course);
        }
        cout << "Course data has been stored successfully.\n" << endl;
    }
    else {
        cout << "Unable to open " << csvPath << ".\n" << endl;
    }
    //empty the fileLines vector
    fileLines.clear();
}

int main() {
    string csvPath = "ABCU_Advising_Program_Input.csv"; // default file path
    string input; // stores user input

    // Define a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    int choice = 0;

    cout << "Welcome to the ABC University Advising Program\n" << endl;
    while (choice != 9) {       
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Print Individual Course Information" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "\nEnter the file name of the course list you wish to load:" << endl;
            cin >> input;
            // entering "default" will load ABCU_Advising_Program_Input.csv (mainly for testing)
            if (input != "default") {
                csvPath = input;
            }
            loadCourses(csvPath, bst);
        }

        else if (choice == 2) {
            cout << "\n----------------------------------------------------" << endl;;
            bst->InOrder();
            cout << "----------------------------------------------------\n" << endl;
        }

        else if (choice == 3) {
            cout << "\nEnter the course number to be displayed (case-sensitive): ";
            cin >> input;
            cout << endl;

            course = bst->Search(input);
            if (!course.courseNumber.empty()) {
                cout << "\n------------------------------------------------------------------" << endl;
                displayCourse(course);
                cout << "------------------------------------------------------------------\n" << endl;
            }
            else {
                cout << "The entered course number was not found.\n" << endl;
            }
        }

        else if (choice != 9) {
            cout << "\nInvalid choice.\nPlease enter a number corresponding to the menu's available options.\n" << endl;
        }
    }

    cout << "\nGood bye." << endl;
    return 0;
}
