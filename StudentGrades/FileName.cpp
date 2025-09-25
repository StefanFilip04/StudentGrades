#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

// function prototypes
void displayMenu();
void addStudent();
void displayAllStudents(struct BSTNode* current);
void displayInOrderByGrade(struct BSTNode* current);

void searchStudent();
void removeStudent();
void displayStatistics();

void removeMinGrade();
void removeMaxGrade();
struct BSTNode* insertByGrade(struct BSTNode* node, struct student* element);
struct BSTNode* findMin(struct BSTNode* node);
struct BSTNode* findMax(struct BSTNode* node);
struct BSTNode* deleteNode(struct BSTNode* node, char* number);
bool searchByNumber(char* number);
void freeTree(struct BSTNode* node);
void calculateAverage(struct BSTNode* node, float* total, int* count);

// ================== MAIN ==================
int main() {
    int choice;

    printf("=== STUDENT GRADE MANAGEMENT SYSTEM ===\n");

    do {
        displayMenu();
        printf("Enter your choice: ");
        if (scanf_s("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // clear buffer
            continue;
        }

        switch (choice) {
        case 1: addStudent(); break;
        case 2:
            printf("\n=== ALL STUDENTS (Sorted by Grade - Highest to Lowest) ===\n");
            displayInOrderByGrade(root);
            break;
        case 3: searchStudent(); break;
        case 4: removeStudent(); break;
        case 5: displayStatistics(); break;
        case 6: removeMinGrade(); break;
        case 7: removeMaxGrade(); break;
        case 0: printf("Exiting program. Goodbye!\n"); break;
        default: printf("Invalid choice! Please try again.\n");
        }
        printf("\n");
    } while (choice != 9);

    freeTree(root);
    return 0;
}

// ================== MENU ==================
void displayMenu() {
    printf("\n=== MAIN MENU ===\n");
    printf("1. Add New Student\n");
    printf("2. Display All Students (Sorted by Grade)\n");
    printf("3. Search Student by Number\n");
    printf("4. Remove Student\n");
    printf("5. Display Statistics\n");
    printf("6. Remove Student with Lowest Grade\n");
    printf("7. Remove Student with Highest Grade\n");
    printf("0. Exit\n");
}

// ================== INSERT NODE ==================
struct BSTNode* insertByGrade(struct BSTNode* node, struct student* element) {
    if (node == NULL) {
        struct BSTNode* newNode = (struct BSTNode*)malloc(sizeof(struct BSTNode));
        if (!newNode) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
        newNode->element = element;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    if (element->grade > node->element->grade) {
        node->left = insertByGrade(node->left, element);
    } else {
        node->right = insertByGrade(node->right, element);
    }

    return node;
}

// ================== ADD STUDENT ==================
void addStudent() {
    
    struct student* newStudent = (struct student*)malloc(sizeof(struct student));

    if (!newStudent) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("\n=== ADD NEW STUDENT ===\n");

    printf("Enter student number: ");
    if (scanf_s("%19s", newStudent->studentNumber, (unsigned)_countof(newStudent->studentNumber)) != 1) {
        printf("Input error!\n");
        free(newStudent);
        return;
    }

    if (searchByNumber(newStudent->studentNumber)) {
        printf("Error: Student number %s already exists!\n", newStudent->studentNumber);
        free(newStudent);
        return;
    }

    printf("Enter student name: ");
    if (scanf_s("%49s", newStudent->name, (unsigned)_countof(newStudent->name)) != 1) {
        printf("Input error!\n");
        free(newStudent);
        return;
    }

    do {
        printf("Enter student grade (0-100): ");
        if (scanf_s("%f", &newStudent->grade) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        if (newStudent->grade < 0 || newStudent->grade > 100) {
            printf("Invalid grade! Please enter a value between 0 and 100.\n");
        }
    } while (newStudent->grade < 0 || newStudent->grade > 100);

    root = insertByGrade(root, newStudent);
    studentCount++;

    printf("Student added successfully!\n");
    printf("Student Number: %s, Name: %s, Grade: %.2f\n",
        newStudent->studentNumber, newStudent->name, newStudent->grade);
}

//display in ascending order by grade
// ================== DISPLAY IN ORDER ==================
void displayInOrderByGrade(struct BSTNode* current) {
    if (current != NULL) {
        displayInOrderByGrade(current->left); //put biggest on left so it display on top
        printf("Student Number: %-10s | Name: %-20s | Grade: %6.2f\n",
            current->element->studentNumber,
            current->element->name,
            current->element->grade);
		displayInOrderByGrade(current->right); //put smallest on right 
    }
}

// ================== SEARCH ==================
bool searchByNumber(char* number) {
    struct BSTNode* current = root;

    while (current != NULL) {
        int cmp = strcmp(number, current->element->studentNumber);
        if (cmp == 0) return true;
        else if (cmp < 0) current = current->left;
        else current = current->right;
    }
    return false;
}


void searchStudent() {
    char number[20];
    printf("\n=== SEARCH STUDENT ===\n");
    printf("Enter student number to search: ");
    if (scanf_s("%19s", number, (unsigned)_countof(number)) != 1) return;

    struct BSTNode* current = root;
    while (current != NULL) {
        int cmp = strcmp(number, current->element->studentNumber);
        if (cmp == 0) {
            printf("Student Found:\n");
            printf("Number: %s, Name: %s, Grade: %.2f\n",
                current->element->studentNumber,
                current->element->name,
                current->element->grade);
            return;
        } else if (cmp < 0) current = current->left;
        else current = current->right;
    }
    printf("Student with number %s not found!\n", number);
}

// ================== MIN / MAX HELPERS ==================
struct BSTNode* findMin(struct BSTNode* node) {
    while (node != NULL && node->right != NULL) node = node->right;
    return node;
}
struct BSTNode* findMax(struct BSTNode* node) {
    while (node != NULL && node->left != NULL) node = node->left;
    return node;
}

// ================== DELETE NODE ==================
struct BSTNode* deleteNode(struct BSTNode* node, char* number) {
    if (node == NULL) return node;

    int cmp = strcmp(number, node->element->studentNumber);

    if (cmp < 0) node->left = deleteNode(node->left, number);
    else if (cmp > 0) node->right = deleteNode(node->right, number);
    else {
        if (node->left == NULL) {
            struct BSTNode* temp = node->right;
            free(node->element);
            free(node);
            studentCount--;
            return temp;
        } else if (node->right == NULL) {
            struct BSTNode* temp = node->left;
            free(node->element);
            free(node);
            studentCount--;
            return temp;
        }
        struct BSTNode* temp = findMin(node->right);
        strcpy_s(node->element->studentNumber, temp->element->studentNumber);
        strcpy_s(node->element->name, temp->element->name);
        node->element->grade = temp->element->grade;
        node->right = deleteNode(node->right, temp->element->studentNumber);
    }
    return node;
}

void removeStudent() {
    char number[20];
    printf("\n=== REMOVE STUDENT ===\n");
    printf("Enter student number to remove: ");
    if (scanf_s("%19s", number, (unsigned)_countof(number)) != 1) return;

    if (!searchByNumber(number)) {
        printf("Student %s not found!\n", number);
        return;
    }

    root = deleteNode(root, number);
    printf("Student %s removed successfully!\n", number);
}


// ================== STATS ==================
void calculateAverage(struct BSTNode* node, float* total, int* count) {
    if (node != NULL) {
        *total += node->element->grade;
        (*count)++;
        calculateAverage(node->left, total, count);
        calculateAverage(node->right, total, count);
    }
}

void displayStatistics() {
    if (root == NULL) {
        printf("No students!\n");
        return;
    }

    printf("\n=== GRADE STATISTICS ===\n");
    printf("Total students: %d\n", studentCount);

    struct BSTNode* minNode = findMin(root);
    struct BSTNode* maxNode = findMax(root);

    if (minNode) printf("Lowest grade: %.2f (%s - %s)\n",
        minNode->element->grade, minNode->element->studentNumber, minNode->element->name);

    if (maxNode) printf("Highest grade: %.2f (%s - %s)\n",
        maxNode->element->grade, maxNode->element->studentNumber, maxNode->element->name);

    float total = 0;
    int count = 0;
    calculateAverage(root, &total, &count);
    if (count > 0) printf("Average grade: %.2f\n", total / count);
}

// ================== REMOVE MIN/MAX ==================
void removeMinGrade() {
    if (root == NULL) {
        printf("No students!\n");
        return;
    }
    struct BSTNode* minNode = findMin(root);
    if (minNode) {
        printf("Removing lowest: %s - %s (%.2f)\n",
            minNode->element->studentNumber, minNode->element->name, minNode->element->grade);
        root = deleteNode(root, minNode->element->studentNumber);
    }
}
void removeMaxGrade() {
    if (root == NULL) {
        printf("No students!\n");
        return;
    }
    struct BSTNode* maxNode = findMax(root);
    if (maxNode) {
        printf("Removing highest: %s - %s (%.2f)\n",
            maxNode->element->studentNumber, maxNode->element->name, maxNode->element->grade);
        root = deleteNode(root, maxNode->element->studentNumber);
    }
}

// ================== FREE TREE ==================
void freeTree(struct BSTNode* node) {
    if (node != NULL) {
        freeTree(node->left);
        freeTree(node->right);
        free(node->element);
        free(node);
    }
}
