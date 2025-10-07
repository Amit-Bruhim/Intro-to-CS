
// including the necessary libaries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/utils.h"

// define global vars
#define RESET "\033[0m"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"

// declare the necessary structs for the mission
typedef struct person {
    char *name;
    unsigned int age;
    struct person *parent1;
    struct person *parent2;
    struct person *partner;
    struct Node *firstBorn;
    int isChecked;
} person;

typedef struct Node {
    person *data;
    struct Node *next;
} Node;

typedef struct World {
    Node *families;
    int population;
} World;

typedef enum { FALSE, TRUE } bool;

// declaring some of the functions
void choose(char choice, World *world);
void createFamilyHead(World *world);
void addToList(World *world, person newFamilyHead);
void printMenu();
void killEveryOne(Node *head);
void printlist(Node *temp);
void makeFictionalWorld(World *world);
void marry(World *world);
void initializeChecking(Node *head);
void getMarry(Node *parent1Node, Node *parent2Node);
void makeChild(World *world);
void addChild(Node *head, person offSpring);
void free3Strings(char *toFree1, char *toFree2, char *toFree3);
void printOffsprings(Node *head, int depth);
void printFamily(World *world);
void getOlder(World *world);
void getOlderHelper(Node *head, unsigned int years);
void printAmount(World *world);
void printCousins(World *world);
void bubbleSort(World *fake);
void switchData(Node *person1, Node *person2);
void printCousinsHelper1(person *current, unsigned int level, unsigned int tempLevel, World *fake);
void printCousinsHelper2(person *current, unsigned int level, unsigned int tempLevel, World *fake);
void addToListFake(World *world, person *newFamilyHead);
void printlistFake(Node *temp);
void killEveryOneFake(Node *head);
int checkValidChild(char *offspringName, char *parent2Name, char *parent1Name, World *world);
char *getString();
Node *search(Node *head, char *name);
person createChild(char *offspringName, Node *parent1Node, Node *parent2Node);

/**
 * @brief Main function
 *
 * This function asks the user for an option and calls the corresponding function.
 *
 * @return 0 on success
 */
int main() {
    // initializes the choice and the "world" (the tree that will hold all the persons)
    char choice = '1';
    World world;
    world.families = NULL;
    world.population = 0;

    // show the menu and let the user choose an option, until he finished
    while (choice != '0') {
        printMenu();
        scanf("%c%*c", &choice);
        // activate the function that activates the mission that the user chose
        choose(choice, &world);
        // initialized the check flag
        initializeChecking(world.families);
    }

    initializeChecking(world.families);
    // free all allocated memory
    Node *head = world.families;
    killEveryOne(head);
    return 0;
}

/**
 * @brief function that activate specific mission accordint to the chiose of the user
 *
 * @param choice the user's choise
 * @param world the world
 *
 * @return void
 */
void choose(char choice, World *world) {
    switch (choice) {
    case '0':
        break;
    case '1':
        createFamilyHead(world);
        break;
    case '2':
        marry(world);
        break;
    case '3':
        makeChild(world);
        break;
    case '4':
        printFamily(world);
        break;
    case '5':
        getOlder(world);
        break;
    case '6':
        printAmount(world);
        break;
    case '7':
        printCousins(world);
        break;
    default:
        // Invalid option
        printError("Invalid option\n");
        break;
    }
}

/**
 * @brief function that prints all the cousins of a person, at the degree he chooce
 *
 * @param world the world
 *
 * @return void
 */
void printCousins(World *world) {
    // asks for his name
    printf("Enter the name of the person:\n");
    char *personName = getString();
    Node *head = world->families;

    // asks for the degree of the Cousins
    unsigned int level;
    printf("Enter degree:\n");
    scanf("%u", &level);
    scanf("%*c");

    // checks that he is exist
    if (search(head, personName) == NULL) {
        free(personName);
        printError("The person does not exist\n");
        return;
    }

    // finds the person and declare on a fake world in order to sort the Cousins by name
    Node *myPerson = search(head, personName);
    person *myPerson1 = myPerson->data;
    World fake;
    fake.families = NULL;
    fake.population = 0;

    // call the function that makes the fake world
    printCousinsHelper1(myPerson1, level, 0, &fake);
    // sort the Cousins by name
    bubbleSort(&fake);
    Node *headFake = fake.families;
    printf("Cousins:\n");
    // print the Cousins by name and free the fake world
    printlistFake(headFake);
    killEveryOneFake(headFake);
    free(personName);
}

/**
 * @brief function that sort the cousins by name
 *
 * @param fake the fake world that was invented in order to sort them
 *
 * @return void
 */
void bubbleSort(World *fake) {
    // decrlare iterator and initializes the switvhed to be TRUE
    Node *head = fake->families;
    Node *i;
    bool switched = TRUE;
    if (fake->families == NULL || fake->families->next == NULL) {
        return;
    }
    // run until there wasn't any switch for a whole iteration through the list
    while (switched == TRUE) {

        // change the switched to be FALSE and initializes the iterator to the start of the list
        switched = FALSE;
        i = head;
        // run through the list
        while (i->next != NULL) {
            // check if one name is "bigger" (by alphabetic order) than his next person on the list
            if (strcmp(i->data->name, i->next->data->name) > 0) {

                // switch them and change the flag back to awitched again
                switched = TRUE;
                switchData(i, i->next);
            }

            // advance to the next node
            i = i->next;
        }
    }
}

/**
 * @brief function that switch between 2 persons on the list
 *
 * @param person1 the first person
 * @param person2 the second person
 *
 * @return void
 */
void switchData(Node *person1, Node *person2) {
    // switch the data using temp person
    person *temp = person1->data;
    person1->data = person2->data;
    person2->data = temp;
}

/**
 * @brief function that finds all the grand fathers at a Given degree
 *
 * @param current the current person that is checked
 * @param level the given degree
 * @param tempLevel the current level of the degree
 * @param fake the fake world that was invented for the cousins mission
 *
 * @return void
 */
void printCousinsHelper1(person *current, unsigned int level, unsigned int tempLevel, World *fake) {
    // stopping condition when the recoursion got the given level or someone doesnt have parents
    if (tempLevel == level) {
        // finds all the grandchildren of those grandparents
        printCousinsHelper2(current, level, 0, fake);
        return;
    }

    else if (current->parent1 == NULL) {
        // finds all the grandchildren of those grandparents
        printCousinsHelper2(current, tempLevel, 0, fake);
        return;
    }

    // recoursively finds the parents of any parent
    printCousinsHelper1(current->parent1, level, tempLevel + 1, fake);
    printCousinsHelper1(current->parent2, level, tempLevel + 1, fake);
}

/**
 * @brief function that finds all the grand children at a Given degree
 *
 * @param current the current person that is checked
 * @param level the given degree
 * @param tempLevel the current level of the degree
 * @param fake the fake world that was invented for the cousins mission
 *
 * @return void
 */
void printCousinsHelper2(person *current, unsigned int level, unsigned int tempLevel, World *fake) {
    // checks that if someone hasn't any kids but still part of the family
    // (for example an uncle that doesnt have kids, you can't print the 2 degree cousins)
    if (tempLevel < level && current->firstBorn == NULL) {
        return;
        // stopping condition when got to the given degree (and didn't checked yet)
    } else if (tempLevel == level && current->isChecked == 0) {
        // marks him as someone who was already checked
        current->isChecked = 1;
        // add him to the list
        addToListFake(fake, current);
        return;
    }

    Node *currentChild = current->firstBorn;
    // recursively calls the function with all his children
    while (currentChild != NULL) {
        printCousinsHelper2(currentChild->data, level, tempLevel + 1, fake);
        currentChild = currentChild->next;
    }
}

/**
 * @brief function that marks all the ancestors of a person
 * the function recoursively run through the tree and increases the flag of every ancestor.
 *
 * @param head the tree
 * @param parent the current parent
 *
 * @return void
 */
void markAncestor(Node *head, person *parent) {
    // increases the flag of the ancestor.
    if (parent->parent1 == NULL && parent->parent2 == NULL) {
        parent->isChecked++;
        return;
    }

    // calls the function again with his parents
    markAncestor(head, parent->parent1);
    markAncestor(head, parent->parent2);
}

/**
 * @brief function that prints the amount of people in the world
 *
 * @param world the world
 *
 * @return void
 */
void printAmount(World *world) {
    // edge case
    if (world->population == 1) {
        printf("There is one person\n");
        // print the amount of people in the world
    } else {
        printf("There are %d people\n", world->population);
    }
}

/**
 * @brief function that add the number of years passed to the age of everyone
 *
 * @param head pointer to the first node on the list
 * @param years the number of years that passed
 *
 * @return void
 */
void getOlderHelper(Node *head, unsigned int years) {
    // point the head of the list and run through all of the nodes
    Node *temp = head;
    while (temp != NULL) {
        person *tempPerson = temp->data;
        // checks that this person wasn't checked yet
        if (tempPerson->isChecked == 0) {
            // add the number of years that passed
            tempPerson->age += years;
            // marks the person as someone who was already checked
            tempPerson->isChecked++;
            // if he has kids calls the function again with his children list
            if (tempPerson->firstBorn != NULL) {
                getOlderHelper(tempPerson->firstBorn, years);
            }
        }
        // advance to the next node
        temp = temp->next;
    }
}

/**
 * @brief function that asks the user for the number of years passed and add it to everyone's age
 *
 * @param world the world
 *
 * @return void
 */
void getOlder(World *world) {
    // asks for the number of years
    printf("Enter number of years:\n");
    unsigned int years;
    scanf("%u", &years);
    scanf("%*c");

    // add the years
    Node *head = world->families;
    getOlderHelper(head, years);
}

/**
 * @brief function that prints the family of a person (wife, offsprings etc.)
 *
 * @param world the world
 *
 * @return void
 */
void printFamily(World *world) {
    // asks for the name of the person
    printf("Enter the name of the person:\n");
    char *headFamilyName = getString();
    Node *head = world->families;

    // checks that he is exist
    if (search(head, headFamilyName) == NULL) {
        free(headFamilyName);
        printError("The person does not exist\n");
        return;
    }

    // prints his family
    Node *headFamilyNode = search(head, headFamilyName);
    printOffsprings(headFamilyNode, 0);
    free(headFamilyName);
}

/**
 * @brief function that prints the family of a person (wife, offsprings etc.)
 * the function recursively prints person and then his children and so on.
 *
 * @param head the head of the current list
 * @param depth the number of recursive calls (0 for him, 1 for children...)
 *
 * @return void
 */
void printOffsprings(Node *head, int depth) {
    // print 0 tabs for himself, 1 for his children and so on.
    for (int i = 0; i < depth; i++) {
        printf("\t");
    }

    Node *current = head;
    // prints the person's details and his wife's if he is married
    printf("%s (%u)", current->data->name, current->data->age);
    if (current->data->partner != NULL) {
        printf(" - %s (%u)", current->data->partner->name, current->data->partner->age);
    }

    printf("\n");
    // checks that he has offsprings
    if (current->data->firstBorn != NULL) {
        Node *currentChild = current->data->firstBorn;
        // recursively calls the function with all his children
        while (currentChild != NULL) {
            printOffsprings(currentChild, depth + 1);
            currentChild = currentChild->next;
        }
    }
}

/**
 * @brief function that add child to the world
 *
 * @param world the world
 *
 * @return void
 */
void makeChild(World *world) {
    // asks for the parents and offspring name's
    printf("Enter the name of the first parent:\n");
    char *parent1Name = getString();
    printf("Enter the name of the second parent:\n");
    char *parent2Name = getString();
    printf("Enter offspring's name:\n");
    char *offspringName = getString();

    // check that the kid can be added to the parents
    if (checkValidChild(offspringName, parent2Name, parent1Name, world) == 1) {
        // finds the parents
        Node *head = world->families;
        initializeChecking(head);
        Node *parent1Node = search(head, parent1Name);
        initializeChecking(head);
        Node *parent2Node = search(head, parent2Name);
        initializeChecking(head);
        // create the new offspring and add him to the list
        person offSpring = createChild(offspringName, parent1Node, parent2Node);
        addChild(parent1Node, offSpring);
        // increment the population by 1, and make the other parent point to the child as well
        world->population += 1;
        parent2Node->data->firstBorn = parent1Node->data->firstBorn;
        free(parent1Name);
        free(parent2Name);
    }
}

/**
 * @brief function that creates new offspring
 *
 * @param offspringName the offspring's Name
 * @param parent1Node pointer to the first parent
 * @param parent2Node pointer to the second parent
 *
 * @return the new offspring
 */
person createChild(char *offspringName, Node *parent1Node, Node *parent2Node) {
    // sets his age to 0 and point to his parents
    person offSpring;
    offSpring.age = 0;
    offSpring.firstBorn = NULL;
    offSpring.isChecked = 0;
    offSpring.name = offspringName;
    offSpring.parent1 = parent1Node->data;
    offSpring.parent2 = parent2Node->data;
    offSpring.partner = NULL;
    return offSpring;
}

/**
 * @brief function that checks if parents are allowed to add new offspring
 *
 * @param offspringName the offspring's Name
 * @param parent1Node pointer to the first parent
 * @param parent2Node pointer to the second parent
 * @param world the world
 *
 * @return 1 if the adding is valid, 0 else.
 */
int checkValidChild(char *offspringName, char *parent2Name, char *parent1Name, World *world) {
    // finds the parents
    Node *head = world->families;
    initializeChecking(head);
    Node *parent1Node = search(head, parent1Name);
    initializeChecking(head);
    Node *parent2Node = search(head, parent2Name);
    initializeChecking(head);

    // checks that the parents exist
    if (parent1Node == NULL || parent2Node == NULL) {
        free3Strings(offspringName, parent2Name, parent1Name);
        initializeChecking(head);
        printError("One of the parents does not exist\n");
        return 0;
        // checks that the parents are married
    } else if (parent1Node->data->partner == NULL || parent2Node->data->partner == NULL) {
        free3Strings(offspringName, parent2Name, parent1Name);
        printError("The parents are not married\n");
        return 0;
    }
    // checks that the parents are married each other
    else if (strcmp(parent1Node->data->partner->name, parent2Name) != 0 ||
             strcmp(parent2Node->data->partner->name, parent1Name) != 0) {
        free3Strings(offspringName, parent2Name, parent1Name);
        printError("The parents are not married\n");
        return 0;
        // checks that the name is not taken
    } else if (search(head, offspringName) != NULL) {
        free3Strings(offspringName, parent2Name, parent1Name);
        initializeChecking(head);
        printError("The name is already taken\n");
        return 0;
        // return 1 if the adding is valid
    } else {
        return 1;
    }
}

/**
 * @brief function that free 3 strings
 *
 * @param toFree1 a string that need to be freed
 * @param toFree2 a string that need to be freed
 * @param toFree3 a string that need to be freed
 *
 * @return void
 */
void free3Strings(char *toFree1, char *toFree2, char *toFree3) {
    free(toFree1);
    free(toFree2);
    free(toFree3);
}

/**
 * @brief function that finds the node that stores specific person
 * the function recoursively run through the tree and stops when he was found.
 *
 * @param head the current list
 * @param name the name of the person that is searched
 *
 * @return the node that stores him (NULL if he wasn't found)
 */
Node *search(Node *head, char *name) {
    // edge case
    if (head == NULL) {
        return NULL;
    }

    Node *current = head;
    // stopping condition when dinds the person
    if (strcmp(name, current->data->name) == 0) {
        return current;
    }

    // if he has kids calls the function again with his children list
    if (current->data->firstBorn != NULL) {
        Node *check = search(current->data->firstBorn, name);
        if (check != NULL) {
            return check;
        }
    }

    // advance to the next node
    Node *check2 = search(current->next, name);
    return check2;
}

/**
 * @brief function that checks if 2 persons are related
 *
 * @param head the tree
 * @param parent1Node the node that store the first person
 * @param parent2Node the node that store the second person
 *
 * @return 1 if they are related, 0 else
 */
int isRelated(Node *head, Node *parent1Node, Node *parent2Node) {
    // marks the Ancestor of the bride and the broom
    markAncestor(head, parent1Node->data);
    markAncestor(head, parent2Node->data);

    // run through the ancestors and checks if someone was marked twice
    Node *temp = head;
    while (temp != NULL) {
        if (temp->data->isChecked != 0 && temp->data->isChecked != 1) {
            initializeChecking(head);
            return 1;
        }
        // advance to the next node
        temp = temp->next;
    }

    // return 0 if they aren't related
    initializeChecking(head);
    return 0;
}

/**
 * @brief function that checks if there is any reason to cancel the wedding
 *
 * @param head the tree
 * @param parent1Node the node that store the first person
 * @param parent2Node the node that store the second person
 *
 * @return 1 if there is a reason to cancel, 0 else.
 */
int objection(Node *head, Node *parent1Node, Node *parent2Node) {
    // declare the minimum age to marry
    const unsigned int LEGAL_AGE = 18;
    person *parent1 = parent1Node->data;
    person *parent2 = parent2Node->data;

    // checks that they both at the legal age
    if (parent1->age < LEGAL_AGE || parent2->age < LEGAL_AGE) {
        return 1;
        // checks if one of them is married
    } else if (parent1->partner != NULL || parent2->partner != NULL) {
        return 1;
        // checks if they are related
    } else if (isRelated(head, parent1Node, parent2Node)) {
        return 1;
    }

    // return 0 if they are not related
    return 0;
}

/**
 * @brief function that marries 2 persons
 *
 * @param world the world
 *
 * @return void
 */
void marry(World *world) {
    // asking for the names of the happy couple
    printf("Enter the name of the first person:\n");
    char *parent1Name = getString();
    printf("Enter the name of the second person:\n");
    char *parent2Name = getString();

    // checks that both persons exist
    Node *temp = world->families;
    initializeChecking(temp);
    if (search(temp, parent1Name) == NULL || (search(temp, parent2Name) == NULL)) {
        free(parent1Name);
        free(parent2Name);
        initializeChecking(temp);
        printError("One of the persons does not exist\n");
        return;
    }

    // checks that he is not married to himself
    if (strcmp(parent1Name, parent2Name) == 0) {
        free(parent1Name);
        free(parent2Name);
        printError("Invalid marriage\n");
        return;
    }

    initializeChecking(temp);
    Node *parent1Node = search(temp, parent1Name);
    initializeChecking(temp);
    Node *parent2Node = search(temp, parent2Name);
    initializeChecking(temp);

    // checks that the wedding is legal
    if (objection(temp, parent1Node, parent2Node)) {
        free(parent1Name);
        free(parent2Name);
        printError("Invalid marriage\n");
        return;
    }

    // marry them
    getMarry(parent1Node, parent2Node);
    free(parent1Name);
    free(parent2Name);
}

/**
 * @brief function that marry 2 persons
 *
 * @param head the tree
 * @param parent1Node the node that store the first person
 * @param parent2Node the node that store the second person
 *
 * @return void
 */
void getMarry(Node *parent1Node, Node *parent2Node) {
    // make them point each other and print a suitable message
    parent1Node->data->partner = parent2Node->data;
    parent2Node->data->partner = parent1Node->data;
    printf("%s and %s are now married\n", parent1Node->data->name, parent2Node->data->name);
}

/**
 * @brief function that frees the "world".
 * this function recoursively run throught the tree and it's stopping condition occurs
 * when someone doesn't have kids
 *
 * @param world is the world what needs to be freed
 *
 * @return void
 */
void killEveryOne(Node *head) {
    // stopping condition at the end of the list
    if (head == NULL) {
        return;
    }

    Node *temp = head;
    Node *nextNode = temp->next;
    person *tempPerson = temp->data;

    // call the function again with the list of the children (after checking he has children)
    if (tempPerson->firstBorn != NULL) {
        killEveryOne(tempPerson->firstBorn);
        tempPerson->firstBorn = NULL;
        // make his partner point to null (at the children field)
        if (tempPerson->partner != NULL) {
            tempPerson->partner->firstBorn = NULL;
        }
    }

    // freeing the allocated memory only if it's not null
    if (temp != NULL) {
        if (tempPerson != NULL) {
            if (tempPerson->name != NULL) {
                free(tempPerson->name);
                tempPerson->name = NULL;
            }
            free(tempPerson);
            tempPerson = NULL;
        }
        free(temp);
        temp = NULL;
    }

    // call the function again with his brother
    killEveryOne(nextNode);
}

/**
 * @brief function that frees the "world".
 * this function recoursively run throught the tree and it's stopping condition occurs
 * when someone doesn't have kids
 *
 * @param world is the world what needs to be freed
 *
 * @return void
 */
void killEveryOneFake(Node *head) {
    // point the head of the list and run through all of the nodes
    Node *temp = head;
    while (temp != NULL) {
        Node *nextNode = temp->next;
        free(temp);
        // advance to the next node
        temp = nextNode;
    }

    free(temp);
}

/**
 * @brief function that initializes the checked flag of the tree to 0
 * this function recoursively run throught the tree and it's stopping condition occurs
 * when someone doesn't have kids
 *
 * @param head the pointer to the first node of the current list
 *
 * @return void
 */
void initializeChecking(Node *head) {
    // run through the list
    Node *temp = head;
    while (temp != NULL) {
        person *tempPerson = temp->data;
        // initializes the flag
        if (tempPerson->isChecked) {
            tempPerson->isChecked = 0;
        }

        // if the person has kids calls the function again with his children list
        if (tempPerson->firstBorn != NULL) {
            initializeChecking(tempPerson->firstBorn);
        }

        // advance to the next node
        temp = temp->next;
    }
}

/**
 * @brief function that print the cousins of a person from a list that was already sorted.
 *
 * @param temp the first node at the list
 *
 * @return void
 */
void printlistFake(Node *temp) {
    // run through the list
    while (temp != NULL) {
        // print his name
        person *tempPerson = temp->data;
        printf("%s\n", tempPerson->name);
        // advance to the next node
        temp = temp->next;
    }
}

/**
 * @brief function that creates new family head
 *
 * @param world the world
 *
 * @return void
 */
void createFamilyHead(World *world) {
    // asks the user for his details and set the other details to NULL
    printf("Enter a name:\n");
    person newFamilyHead;
    newFamilyHead.name = getString();
    Node *temp = world->families;
    newFamilyHead.firstBorn = NULL;
    newFamilyHead.parent1 = NULL;
    newFamilyHead.parent2 = NULL;
    newFamilyHead.partner = NULL;
    newFamilyHead.isChecked = 0;
    printf("Enter age:\n");
    scanf("%u", &newFamilyHead.age);
    scanf("%*c");

    // checks that the name is not taken
    if (search(temp, newFamilyHead.name) != NULL) {
        printError("The name is already taken\n");
        initializeChecking(temp);
        free(newFamilyHead.name);
        return;
    }

    initializeChecking(temp);
    // add the new person to the list
    addToList(world, newFamilyHead);
}

/**
 * @brief function that add new cousin to the list
 *
 * @param world the world
 * @param newFamilyHead the new cousin
 *
 * @return void
 */
void addToListFake(World *world, person *newFamilyHead) {
    // alocate memory for the node
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        exit(1);
    }

    // initializes the next pointer of the person to NULL
    newNode->data = newFamilyHead;
    newNode->next = NULL;
    // adding it to the first place if the list is empty
    if (world->families == NULL) {
        world->families = newNode;
        // adding the person to the end of the list
    } else {
        Node *current = world->families;
        // runs through the list
        while (current->next != NULL) {
            current = current->next;
        }
        // advance to the next node
        current->next = newNode;
    }
}

/**
 * @brief function that add new family head to the world
 *
 * @param world the world
 * @param newFamilyHead the new family head
 *
 * @return void
 */
void addToList(World *world, person newFamilyHead) {
    // alocate memory for the node
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        exit(1);
    }

    // alocate memory for the person
    newNode->data = malloc(sizeof(person));
    if (newNode->data == NULL) {
        free(newNode);
        exit(1);
    }

    // initializes the next pointer of the person to NULL
    *newNode->data = newFamilyHead;
    newNode->next = NULL;
    // adding it to the first place if the list is empty
    if (world->families == NULL) {
        world->families = newNode;
        // adding the person to the end of the list
    } else {
        Node *current = world->families;
        // runs through the list
        while (current->next != NULL) {
            current = current->next;
        }
        // advance to the next node
        current->next = newNode;
    }
    // increments the population by 1
    world->population += 1;
}

/**
 * @brief function that add new offspring to the world
 *
 * @param head the list of his parent's children
 * @param newFamilyHead the new offspring
 *
 * @return void
 */
void addChild(Node *head, person offSpring) {
    // alocate memory for the node
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        exit(1);
    }

    // alocate memory for the person
    newNode->data = (person *)malloc(sizeof(person));
    if (newNode->data == NULL) {
        free(newNode);
        exit(1);
    }

    // initializes the next pointer of the person to NULL
    *newNode->data = offSpring;
    newNode->next = NULL;
    // adding it to the first place if the list is empty
    if (head->data->firstBorn == NULL) {
        head->data->firstBorn = newNode;
        // adding the person to the end of the list
    } else {
        Node *current = head->data->firstBorn;
        // runs through the list
        while (current->next != NULL) {
            current = current->next;
        }
        // advance to the next node
        current->next = newNode;
    }

    printf("%s was born\n", offSpring.name);
}

/**
 * @brief function that get a string in unknown length from the user
 *
 * @return string that the user typed
 */
char *getString() {
    char tempChar;
    int index = 1;
    // initializes memory for the first char and takes it from the user
    char *holder = (char *)malloc(sizeof(char));
    if (holder == NULL) {
        exit(1);
    }

    scanf("%c", &tempChar);
    // runs until the user press enter
    while (tempChar != '\n') {
        // add the char to the string and reallocte memory for another char
        holder[index - 1] = tempChar;
        holder = (char *)realloc(holder, (index + 1) * sizeof(char));
        if (holder == NULL) {
            free(holder);
            exit(1);
        }
        // takes the next char and increment the index
        index++;
        scanf("%c", &tempChar);
    }

    // finish the string and return it
    holder[index - 1] = '\0';
    return holder;
}

/**
 * @brief function that prints the menu.
 */
void printMenu() {
    // prints the menu
    printf(GREEN "Choose an option:\n" RESET);
    printf("0. Exit\n");
    printf("1. Add a head\n");
    printf("2. Marry two people\n");
    printf("3. New offspring\n");
    printf("4. Print family top-down\n");
    printf("5. Years pass\n");
    printf("6. Count people\n");
    printf("7. Print cousins\n");
}