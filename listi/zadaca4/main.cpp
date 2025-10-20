#include <iostream>

struct Element {
    int data;
    Element *prev = nullptr, *next = nullptr; // default pointer values
};

struct DoublyList {
    Element *start = nullptr;
    int count = 0;
};

void addElement(Element*, int);
void insertElement(DoublyList&, int, int = -1);
void removeElement(DoublyList&, int);
Element *findElement(const DoublyList&, int);
void displayList(const DoublyList&);
void createList(DoublyList&);

void addElement(Element *node, int val) {
    Element *newElem = new Element();
    newElem->data = val;
    newElem->prev = node;
    if (node->next) {
        newElem->next = node->next;
        newElem->next->prev = newElem;
    }
    node->next = newElem;
}

void insertElement(DoublyList &lst, int val, int pos) {
    if (!lst.start) {
        lst.start = new Element();
        lst.start->data = val;
    } else {
        Element *ptr = lst.start;
        if (pos == 0) {
            Element *temp = new Element();
            temp->data = ptr->data;
            temp->next = ptr->next;
            temp->prev = ptr;
            ptr->next = temp;
            ptr->data = val;
        } else if (pos == -1) {
            while (ptr->next) ptr = ptr->next;
            addElement(ptr, val);
        } else {
            for (int i = 0; i < pos - 1; i++) ptr = ptr->next;
            addElement(ptr, val);
        }
    }
    lst.count++;
}

void createList(DoublyList &lst) {
    int num;

    std::cout << "Enter a series of numbers:\n";
    std::cin >> num;

    if (num) {
        lst.start = new Element();
        lst.start->data = num;
        lst.count++;
    }

    while (std::cin >> num) {
        insertElement(lst, num);
    }

    std::cin.clear();
    std::cin.ignore();
}

void removeElement(DoublyList &lst, int pos) {
    if (!lst.start) {
        std::cout << "The list is empty.\n";
        return;
    }

    Element *ptr = lst.start;
    if (pos == -1) {
        while (ptr->next) ptr = ptr->next;
        if (ptr->prev) ptr->prev->next = nullptr;
        delete ptr;
    } else if (pos == 0) {
        if (ptr->next) {
            lst.start = ptr->next;
            ptr->next->prev = nullptr;
            delete ptr;
        } else lst.start = nullptr;
    } else {
        for (int i = 0; i < pos; i++) ptr = ptr->next;
        if (ptr->prev) ptr->prev->next = ptr->next;
        if (ptr->next) ptr->next->prev = ptr->prev;
        delete ptr;
    }
    lst.count--;
}

Element *findElement(const DoublyList &lst, int val) {
    if (!lst.start) {
        std::cout << "List is empty, cannot search.\n";
        return nullptr;
    }
    Element *ptr = lst.start;
    while (ptr) {
        if (ptr->data == val) {
            std::cout << "Value found in list.\n";
            return ptr;
        }
        ptr = ptr->next;
    }
    std::cout << "Value not found in list.\n";
    return nullptr;
}

void displayList(const DoublyList &lst) {
    if (!lst.start) {
        std::cout << "The list is empty.\n";
        return;
    }
    Element *ptr = lst.start;
    while (ptr) {
        std::cout << ptr->data << " ";
        ptr = ptr->next;
    }
    std::cout << '\n';
}

int main() {
    DoublyList myList;
    createList(myList);

    int num, index;

    std::cout << "Enter a value and a position to add it (-1 to add at the end):\n";
    std::cin >> num >> index;
    if (num) {
        if (index > myList.count) index = -1;
        insertElement(myList, num, index);
        std::cout << "Updated list: ";
        displayList(myList);
    }

    std::cin.clear();
    std::cin.ignore();

    std::cout << "Enter a position to delete (-1 to delete last element):\n";
    std::cin >> index;
    if (index > myList.count) index = myList.count - 1;
    removeElement(myList, index);
    std::cout << "Updated list: ";
    displayList(myList);

    std::cin.clear();
    std::cin.ignore();

    std::cout << "Enter a value to search for:\n";
    std::cin >> num;
    if (num) findElement(myList, num);

    return 0;
}