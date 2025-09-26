#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// create Student and BST Node
struct student {
    char studentNumber[20];
    char name[50];
    float grade;
};

struct BSTNode {
    struct student* element;
    struct BSTNode* left;
    struct BSTNode* right;
};


struct BSTNode* root = NULL;
int studentCount = 0;

// === FUNCTION PROTOTYPES ===
void displayMenu();
void addStudent();
void displayInOrderByGrade(struct BSTNode* current);

// we insert grades into the nodes, thats how it's sorted
struct BSTNode* insertByGrade(struct BSTNode* node, struct student* element); 

struct BSTNode* searchByNumber(struct BSTNode* node, char* number);
struct BSTNode* findMin(struct BSTNode* node);
struct BSTNode* findMax(struct BSTNode* node);
struct BSTNode* deleteNode(struct BSTNode* node, char* number);
void removeStudent();
void displayStatistics();

void calculateAverage(struct BSTNode* node, float* total, int* count);
void removeMinGrade();
void removeMaxGrade();
void freeTree(struct BSTNode* node);
void clearInputBuffer();


// === MAIN ===
int main() {
    int choice;
    printf("=== STUDENT GRADE MANAGEMENT SYSTEM ===\n");

    do {
        // display all options
        displayMenu();
        printf("Enter your choice: ");
        if (scanf_s("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer(); // clear after number input for as \n stays in the buffer

        // all choices
        switch (choice) {
        case 1: addStudent(); break;

        case 2:
            printf("\n=== ALL STUDENTS (Sorted by Grade - Highest to Lowest) ===\n");
            displayInOrderByGrade(root);
            break;

        case 3: {
            char number[20];
            printf("\nEnter student number to search: ");
            scanf_s("%10s", number, (unsigned)_countof(number));
            clearInputBuffer();
            struct BSTNode* found = searchByNumber(root, number);
            if (found) {
                printf("Student Found: %s | %s | %.2f\n",
                    found->element->studentNumber,
                    found->element->name,
                    found->element->grade);
            }
            else {
                printf("Student with number %s not found!\n", number);
            }
            break;
        }
        case 4: removeStudent(); break;
        case 5: displayStatistics(); break;
        case 6: removeMinGrade(); break;
        case 7: removeMaxGrade(); break;
        case 0: printf("Exiting program. Goodbye!\n"); break;
        default:
            printf("Invalid choice! Please try again.\n");
            clearInputBuffer();
        }
        printf("\n");
    } while (choice != 0);

    freeTree(root);
    return 0;
}

// === UTILITY FUNCTIONS ===
// note: consume the new line and clear it, this is like java's "scan.nextline();" except we need to create it
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// === MENU ===
void displayMenu() {
    printf("=== MAIN MENU ===\n");
    printf("1. Add New Student\n");
    printf("2. Display All Students (Sorted by Grade)\n");
    printf("3. Search Student by Number\n");
    printf("4. Remove Student\n");
    printf("5. Display Statistics\n");
    printf("6. Remove Student with Lowest Grade\n");
    printf("7. Remove Student with Highest Grade\n");
    printf("0. Exit\n");
}

// === 1. ADD STUDENT ==
void addStudent() {
    struct student* newStudent = (struct student*)malloc(sizeof(struct student));
    if (!newStudent) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("\n=== ADD NEW STUDENT ===\n");
    printf("Enter student number: ");
    scanf_s("%10s", newStudent->studentNumber, (unsigned)_countof(newStudent->studentNumber));
    clearInputBuffer();

    // check duplicates for student number
    if (searchByNumber(root, newStudent->studentNumber)) {
        printf("Error: Student number %s already exists!\n", newStudent->studentNumber);
        free(newStudent);
        return;
    }

    // input name
    printf("Enter student name: ");
    scanf_s("%49s", newStudent->name, (unsigned)_countof(newStudent->name));
    clearInputBuffer();

    //input valid grade
    do {
        printf("Enter student grade (0-100): ");
        if (scanf_s("%f", &newStudent->grade) != 1) {
            printf("Invalid input.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        if (newStudent->grade < 0 || newStudent->grade > 100) {
            printf("Grade must be 0-100.\n");
        }
    } while (newStudent->grade < 0 || newStudent->grade > 100);

    //insert grade into node 
    root = insertByGrade(root, newStudent);
    studentCount++;

    printf("Student added successfully! Number: %5s | Name: %5s | Grade: %.2f\n",
        newStudent->studentNumber, newStudent->name, newStudent->grade);
}

// === INSERT BY GRADE (this is very important, the nodes are arranged by GRADES) ===
// - - - - - - - - - - - -
struct BSTNode* insertByGrade(struct BSTNode* node, struct student* element) {
    if (!node) {
        struct BSTNode* newNode = (struct BSTNode*)malloc(sizeof(struct BSTNode));
    }
        if (!newNode) {
            printf("Memory allocation failed!\n");
            return NULL;
        }
        newNode->element = element;
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    
    if (element->grade > node->element->grade) {
        node->left = insertByGrade(node->right, element); //bigger goes right
    }
    else {
        node->right = insertByGrade(node->left, element); //smaller goes left
    }
    return node;
}
// - - - - - - - - - - - -


// === 2. DISPLAY IN ORDER ===
void displayInOrderByGrade(struct BSTNode* current) {
    if (!current) return;
    displayInOrderByGrade(current->left);
    printf("Number: %-10s | Name: %-10s | Grade: %.2f\n",
        current->element->studentNumber,
        current->element->name,
        current->element->grade);
    displayInOrderByGrade(current->right);
}


// === 3. SEARCH BY STUDENT NUMBER ===
struct BSTNode* searchByNumber(struct BSTNode* node, char* number) {
    if (!node) return NULL;

    // 1. find studuent number
    int cmp = strcmp(node->element->studentNumber, number);
    if (cmp == 0) return node; 

    // 2. search both subtrees since BST is organized by grade, not by number
    struct BSTNode* found = searchByNumber(node->left, number);
    if (found) return found;

    // 3. return result
    return searchByNumber(node->right, number);
}

// === MIN/MAX ===
struct BSTNode* findMin(struct BSTNode* node) {
    if (!node) return NULL;
    while (node->left) node = node->left; // left = lower grade
    return node;
}

struct BSTNode* findMax(struct BSTNode* node) {
    if (!node) return NULL;
    while (node->right) node = node->right; // right = higher grade
    return node;
}


// === 4. DELETE NODE / STUDENT ===
// - - - - - - - - - -
struct BSTNode* deleteNode(struct BSTNode* node, char* number) {
    if (!node) return NULL;

    // search entire tree for the node
    struct BSTNode* found = searchByNumber(node, number);
    if (!found) return node; // Node not found

    // compare current node to user input to see if its the one to delete
    if (strcmp(node->element->studentNumber, number) == 0) {
        if (!node->left) {
            struct BSTNode* temp = node->right;
            free(node->element);
            free(node);
            studentCount--;
            return temp;
        }
        else if (!node->right) {
            struct BSTNode* temp = node->left;
            free(node->element);
            free(node);
            studentCount--;
            return temp;
        }

        // Node with two children: get the inorder successor (smallest in left subtree)
        struct BSTNode* temp = findMin(node->left);
        // COPY the data
        struct student* tempElement = node->element;
        node->element = temp->element;
        temp->element = tempElement;
        // DELETE the inorder successor
        node->right = deleteNode(node->right, temp->element->studentNumber);
    }
    else {
        // Search in both subtrees
        node->left = deleteNode(node->left, number);
        node->right = deleteNode(node->right, number);
    }
    return node;
}
// - - - - - - - - - -



// === REMOVE STUDENT ===
void removeStudent() {
    if (!root) {
        printf("No students to remove!\n");
        return;
    }

    char number[20];
    printf("\nEnter student number to remove: ");
    scanf_s("%19s", number, (unsigned)_countof(number));
    clearInputBuffer();

    if (!searchByNumber(root, number)) {
        printf("Student %s not found!\n", number);
        return;
    }
    //call deleteNode to delete
    root = deleteNode(root, number);
    printf("Student %s removed successfully!\n", number);
}

// === STATISTICS ===
// calculate average
void calculateAverage(struct BSTNode* node, float* total, int* count) {
    if (!node) return;
    *total += node->element->grade;
    (*count)++;
    calculateAverage(node->left, total, count);
    calculateAverage(node->right, total, count);
}

// 2. display all students
void displayStatistics() {
    if (!root) {
        printf("No students!\n");
        return;
    }
    
    printf("\n=== STATISTICS ===\n");
    printf("Total students: %d\n", studentCount);

    struct BSTNode* minNode = findMin(root);
    struct BSTNode* maxNode = findMax(root);

    //display MIN and MAX
    if (minNode) {
    printf("Lowest grade: \nNumber: %s | Name: %s | Grade: %.2f\n",
        minNode->element->studentNumber,
        minNode->element->name,
        minNode->element->grade);
    }
    
    if (maxNode) {
    printf("Highest grade: \nNumber: %s | Name: %s | Grade: %.2f\n",
        maxNode->element->studentNumber,
        maxNode->element->name,
        maxNode->element->grade);
    }

    float total = 0;
    int count = 0;
    calculateAverage(root, &total, &count);
    if (count > 0) {
        printf("Average grade: %.2f\n", total / count);
    }
}

// === 6./7. REMOVE MIN/MAX ===
// remove MIN grade
void removeMinGrade() {
    struct BSTNode* minNode = findMin(root);
    if (!minNode) {
        printf("No students!\n");
        return;
    }
    // display student removed
    printf("Removing student with lowest grade: \nNumber: %s | Name: %s | Grade: %.2f\n",
        minNode->element->studentNumber,
        minNode->element->name,
        minNode->element->grade);
    root = deleteNode(root, minNode->element->studentNumber);
}
    
// remove MAX grade
void removeMaxGrade() {
    struct BSTNode* maxNode = findMax(root);
    if (!maxNode) {
        printf("No students!\n");
        return;
    }
    // display student removed
    printf("Removing student with highest grade: \nNumber: %s | Name: %s | Grade: %.2f\n",
        maxNode->element->studentNumber,
        maxNode->element->name,
        maxNode->element->grade);
    root = deleteNode(root, maxNode->element->studentNumber);
}

// === FREE TREE ===
// cleans up all dynamically allocated memoery for BST to prevent memory leaks
void freeTree(struct BSTNode* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node->element);
    free(node);
}
