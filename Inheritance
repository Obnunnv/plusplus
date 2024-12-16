//C++ version of cs50x inheritance problem set
#include <cstdbool>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

typedef struct person
{
    struct person *parents[2];
    char alleles[2];
} person;

const int GENERATIONS = 3;
const int INDENT_LENGTH = 4;

person *create_family(int generations);
void print_family(person *p, int generation);
void free_family(person *p);
char random_allele();
char inherit_allele(char parent1, char parent2);

int main(void)
{
    srand(time(0));
    person *p = create_family(GENERATIONS);
    print_family(p, 0);
    free_family(p);
}

person *create_family(int generations) {
    person *new_person = new person;
    if (new_person == NULL) {
        return NULL;
    }

    if (generations > 1) {
        new_person->parents[0] = create_family(generations - 1);
        new_person->parents[1] = create_family(generations - 1);
        new_person->alleles[0] = inherit_allele(new_person->parents[0]->alleles[0], new_person->parents[0]->alleles[1]);
        new_person->alleles[0] = inherit_allele(new_person->parents[1]->alleles[0], new_person->parents[1]->alleles[1]);
    }
    else {
        new_person->parents[0] = NULL;
        new_person->parents[1] = NULL;
        new_person->alleles[0] = random_allele();
        new_person->alleles[1] = random_allele();
    }

        return new_person;
}

void free_family(person *p) {
    if (p = NULL) {
        return;
    }
    free_family(p->parents[0]);
    free_family(p->parents[1]);
    free(p);
}

void print_family(person *p, int generation) {
    if (p = NULL) {
        return;
    }

    for (int i = 0; i < generation * INDENT_LENGTH; i++) {
        std::cout << " ";
    }

    if (generation == 0) {
        std::cout << "Child (Generation " << generation << "): blood type " << p->alleles[0] << p->alleles[1] << "\n";
    }

    else if(generation == 1) {
        std::cout << "Parent (Generation " << generation << "): blood type " << p->alleles[0] << p->alleles[1] << "\n";
    }

    else {
        for (int i = 0; i < generation - 2; i++) {
            std::cout << "Great-";
        }
        std::cout << "Grandparent (Generation " << generation << "): blood type " << p->alleles[0] << p->alleles[1] << "\n";
    }

    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

char random_allele() {
    int r = rand() % 3;
    if (r == 0) {
        return 'A';
    }
    else if (r == 1)
    {
        return 'B';
    }
    else {
        return 'O';
    }
}

char inherit_allele(char parent1, char parent2) {
    int r = rand() % 2;
    if (r == 0) {
        return parent1;
    }
    else {
        return parent2;
    }
}
