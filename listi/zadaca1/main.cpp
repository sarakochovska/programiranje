#include <iostream>

struct Item {
    int data;
    Item *nextItem = nullptr;
};

struct Chain {
    Item *first = nullptr;
};

void attachItem(Item*, int);
void insertSorted(Chain&, int);
void showChain(const Chain&);

void attachItem(Item *current, int newValue) {
    Item *newItem = new Item();
    newItem->data = newValue;
    newItem->nextItem = current->nextItem;
    current->nextItem = newItem;
}

void insertSorted(Chain &c, int newValue) {
    if (!c.first) return;

    Item *ptr = c.first;


    if (newValue < ptr->data) {
        Item *newItem = new Item();
        newItem->data = ptr->data;
        newItem->nextItem = ptr->nextItem;
        ptr->nextItem = newItem;
        ptr->data = newValue;
        return;
    }


    while (ptr->nextItem) {
        if (newValue < ptr->nextItem->data) {
            attachItem(ptr, newValue);
            return;
        }
        ptr = ptr->nextItem;
    }


    attachItem(ptr, newValue);
}

void showChain(const Chain &c) {
    if (!c.first) {
        std::cout << "Chain is empty";
        return;
    }
    Item *temp = c.first;
    while (temp) {
        std::cout << temp->data << " ";
        temp = temp->nextItem;
    }
    std::cout << '\n';
}

int main() {
    Chain listChain;
    int userInput;

    std::cout << "Enter numbers for the chain (non-number to finish):\n";
    std::cin >> userInput;

    if (userInput) {
        listChain.first = new Item();
        listChain.first->data = userInput;
    }

    while (std::cin >> userInput) {
        insertSorted(listChain, userInput);
    }

    showChain(listChain);

    return 0;
}

