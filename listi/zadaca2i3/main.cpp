
#include <iostream>

struct Item {
    int data;
    Item *nextItem = nullptr;
};

struct Chain {
    Item *first = nullptr;
};

void sortInsert(Chain &);
void sortBubble(Chain &);
void appendNode(Chain &, int);
void readChain(Chain &);
void showChain(const Chain &);
void joinChainsUnique(Chain &, Chain &, bool);

void sortInsert(Chain &c) {
    if (!c.first || !c.first->nextItem) return;

    Item *current = c.first->nextItem, *iterator;
    while (current) {
        iterator = c.first;
        while (iterator && iterator != current) {
            if (iterator->data > current->data) {
                int temp = iterator->data;
                iterator->data = current->data;
                current->data = temp;
            }
            iterator = iterator->nextItem;
        }
        current = current->nextItem;
    }
}

void sortBubble(Chain &c) {
    if (!c.first || !c.first->nextItem) return;

    Item *outer = c.first;
    while (outer->nextItem) {
        Item *inner = c.first;
        while (inner->nextItem) {
            if (inner->data > inner->nextItem->data) {
                int temp = inner->data;
                inner->data = inner->nextItem->data;
                inner->nextItem->data = temp;
            }
            inner = inner->nextItem;
        }
        outer = outer->nextItem;
    }
}

void appendNode(Chain &c, int value) {
    if (!c.first) {
        c.first = new Item();
        c.first->data = value;
        return;
    }

    Item *temp = c.first;
    while (temp->nextItem) temp = temp->nextItem;
    Item *newItem = new Item();
    newItem->data = value;
    temp->nextItem = newItem;
}

void readChain(Chain &c) {
    int input;
    std::cout << "Enter numbers for the chain (any non-number to finish):\n";
    std::cin >> input;

    if (std::cin) {
        c.first = new Item();
        c.first->data = input;
    }

    while (std::cin >> input) appendNode(c, input);
}

void showChain(const Chain &c) {
    Item *temp = c.first;
    std::cout << "Final result:\n";
    while (temp) {
        std::cout << temp->data << " ";
        temp = temp->nextItem;
    }
    std::cout << '\n';
}

void joinChainsUnique(Chain &c1, Chain &c2, bool useBubble) {
    if (!c1.first && !c2.first) {
        std::cout << "Both chains are empty\n";
        return;
    }

    Item *ptr1 = c1.first;
    while (ptr1->nextItem) ptr1 = ptr1->nextItem;
    ptr1->nextItem = c2.first;

    if (useBubble) sortBubble(c1);
    else sortInsert(c1);

    ptr1 = c1.first;
    while (ptr1->nextItem) {
        if (ptr1->data == ptr1->nextItem->data) {
            Item *dup = ptr1->nextItem->nextItem;
            delete ptr1->nextItem;
            ptr1->nextItem = dup;
        } else {
            ptr1 = ptr1->nextItem;
        }
    }

    showChain(c1);
}

int main() {
    Chain firstChain, secondChain;

    readChain(firstChain);
    std::cin.clear();
    std::cin.ignore();
    readChain(secondChain);

    joinChainsUnique(firstChain, secondChain, false); // false = insertion sort, true = bubble sort

    return 0;
}
