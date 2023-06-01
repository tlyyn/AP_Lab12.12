#include <iostream>
#include <string>
#include <algorithm> // Для функції std::sort()

using namespace std;


// Структура даних для запису
struct Note {
    string firstName;
    string lastName;
    string phoneNumber;
    int birthDate[3]; // [0] - day, [1] - month, [2] - year
};

// Структура елемента списку
struct Elem {
    Note data;
    Elem* next;
};

// Структура вузла бінарного дерева
struct Node {
    Note data;
    Node* left;
    Node* right;
};

// Функція для введення даних про особу з клавіатури
Note enterPersonData() {
    Note person;
    cout << "Enter first name: ";
    cin >> person.firstName;
    cout << "Enter last name: ";
    cin >> person.lastName;
    cout << "Enter phone number: ";
    cin >> person.phoneNumber;
    cout << "Enter birth date (day month year): ";
    cin >> person.birthDate[0] >> person.birthDate[1] >> person.birthDate[2];
    return person;
}

// Функція для створення нового елемента списку
Elem* createListElement(Note data) {
    Elem* element = new Elem;
    element->data = data;
    element->next = NULL;
    return element;
}

// Функція для додавання елемента в кінець списку
void addToList(Elem*& head, Note data) {
    Elem* element = createListElement(data);
    if (head == NULL) {
        head = element;
    }
    else {
        Elem* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = element;
    }
}

// Функція для виведення інформації про особу
void displayPersonInfo(Note person) {
    cout << "First Name: " << person.firstName << endl;
    cout << "Last Name: " << person.lastName << endl;
    cout << "Phone Number: " << person.phoneNumber << endl;
    cout << "Birth Date: " << person.birthDate[0] << "." << person.birthDate[1] << "." << person.birthDate[2] << endl;
    cout << endl;
}

// Функція для виведення списку
void displayList(Elem* head) {
    if (head == NULL) {
        cout << "List is empty" << endl;
        return;
    }

    // Створюємо тимчасовий масив для сортування
    int size = 0;
    Elem* current = head;
    while (current != NULL) {
        size++;
        current = current->next;
    }

    Note* array = new Note[size];
    current = head;
    int index = 0;
    while (current != NULL) {
        array[index] = current->data;
        current = current->next;
        index++;
    }

    // Сортуємо масив за прізвищем в алфавітному порядку
    sort(array, array + size, [](const Note& a, const Note& b) {
        return a.lastName < b.lastName;
        });

    // Виводимо відсортований список
    for (int i = 0; i < size; i++) {
        displayPersonInfo(array[i]);
    }

    // Звільняємо пам'ять, що була виділена для масиву
    delete[] array;
}

// Функція для видалення елемента зі списку за номером телефону
void deleteFromList(Elem*& head, string phoneNumber) {
    if (head == NULL) {
        return;
    }

    if (head->data.phoneNumber == phoneNumber) {
        Elem* tmp = head->next;
        delete head;
        head = tmp;
        return;
    }

    Elem* current = head;
    while (current->next != NULL && current->next->data.phoneNumber != phoneNumber) {
        current = current->next;
    }

    if (current->next != NULL) {
        Elem* tmp = current->next->next;
        delete current->next;
        current->next = tmp;
    }
}

// Функція для видалення всього списку
void deleteList(Elem*& head) {
    while (head != NULL) {
        Elem* tmp = head->next;
        delete head;
        head = tmp;
    }
}

// Функція для видалення елемента з бінарного дерева за номером телефону
Node* deleteFromBinaryTree(Node* root, string phoneNumber) {
    if (root == NULL) {
        return NULL;
    }

    if (root->data.phoneNumber == phoneNumber) {
        if (root->left == NULL && root->right == NULL) {
            delete root;
            return NULL;
        }
        else if (root->left == NULL) {
            Node* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == NULL) {
            Node* temp = root->left;
            delete root;
            return temp;
        }
        else {
            Node* minValueNode = root->right;
            while (minValueNode->left != NULL) {
                minValueNode = minValueNode->left;
            }
            root->data = minValueNode->data;
            root->right = deleteFromBinaryTree(root->right, minValueNode->data.phoneNumber);
        }
    }
    else if (phoneNumber < root->data.phoneNumber) {
        root->left = deleteFromBinaryTree(root->left, phoneNumber);
    }
    else {
        root->right = deleteFromBinaryTree(root->right, phoneNumber);
    }

    return root;
}

// Функція для видалення бінарного дерева
void deleteBinaryTree(Node*& root) {
    if (root == NULL) {
        return;
    }

    deleteBinaryTree(root->left);
    deleteBinaryTree(root->right);

    delete root;
    root = NULL;
}

// Функція для додавання елемента в бінарне дерево
void addToBinaryTree(Node*& root, Note data) {
    if (root == NULL) {
        root = new Node;
        root->data = data;
        root->left = NULL;
        root->right = NULL;
    }
    else if (data.lastName < root->data.lastName) {
        addToBinaryTree(root->left, data);
    }
    else {
        addToBinaryTree(root->right, data);
    }
}

// Функція для виведення бінарного дерева в порядку "вліво-центр-вправо"
void inOrderTraversal(Node* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        displayPersonInfo(root->data);
        inOrderTraversal(root->right);
    }
}

// Функція для виведення осіб, які народилися в заданому місяці (для списку)
void displayPeopleByMonthList(Elem* head, int month) {
    bool found = false;

    Elem* current = head;
    while (current != NULL) {
        if (current->data.birthDate[1] == month) {
            displayPersonInfo(current->data);
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        cout << "No people found with birth month " << month << endl;
    }
}

// Функція для виведення осіб, які народилися в заданому місяці (для бінарного дерева)
void displayPeopleByMonthTree(Node* root, int month) {
    bool found = false;

    if (root != NULL) {
        displayPeopleByMonthTree(root->left, month);
        if (root->data.birthDate[1] == month) {
            displayPersonInfo(root->data);
            found = true;
        }
        displayPeopleByMonthTree(root->right, month);
    }

    if (!found) {
        cout << "There is no person with such a month of birth" << endl;
    }
}

int main() {
    Elem* list = NULL;
    Node* binaryTree = NULL;
    int choice;

    do {
        cout << "MENU" << endl;
        cout << "1. Add person to the list" << endl;
        cout << "2. Display list" << endl;
        cout << "3. Delete person from the list" << endl;
        cout << "4. Delete entire list" << endl;
        cout << "5. Add person to the binary tree" << endl;
        cout << "6. Display binary tree" << endl;
        cout << "7. Display people by birth month (List)" << endl;
        cout << "8. Display people by birth month (Binary Tree)" << endl;
        cout << "9. Delete person form binary tree" << endl;
        cout << "10. Delete binary tree" << endl;
        cout << "11. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            Note person = enterPersonData();
            addToList(list, person);
            break;
        }
        case 2: {
            displayList(list);
            break;
        }
        case 3: {
            string phoneNumber;
            cout << "Enter the phone number of the person to delete: ";
            cin >> phoneNumber;
            deleteFromList(list, phoneNumber);
            break;
        }
        case 4: {
            deleteList(list);
            cout << "List deleted" << endl;
            break;
        }
        case 5: {
            Note person = enterPersonData();
            addToBinaryTree(binaryTree, person);
            break;
        }
        case 6: {
            inOrderTraversal(binaryTree);
            break;
        }
        case 7: {
            int month;
            cout << "Enter the birth month: ";
            cin >> month;
            displayPeopleByMonthList(list, month);
            break;
        }
        case 8: {
            int month;
            cout << "Enter the birth month: ";
            cin >> month;
            displayPeopleByMonthTree(binaryTree, month);
            break;
        }
        case 9: {
            string phoneNumber;
            cout << "Enter the phone number of the person to delete: ";
            cin >> phoneNumber;
            binaryTree = deleteFromBinaryTree(binaryTree, phoneNumber);
            break;
        }
        case 10: {
            deleteBinaryTree(binaryTree);
            cout << "Binary tree deleted" << endl;
            break;
        }
        case 11:
            break;
        default:
            cout << "Invalid choice" << endl;
        }

        cout << endl;
    } while (choice != 11);

    deleteList(list);
    deleteBinaryTree(binaryTree);

    return 0;
}
