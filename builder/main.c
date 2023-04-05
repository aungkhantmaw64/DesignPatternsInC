#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 50

typedef struct Person
{
    char name[BUFF_SIZE];
    char address[BUFF_SIZE];
    char email[BUFF_SIZE];
} Person;

typedef struct PersonBuilder
{
    Person profile;
    struct PersonBuilder *(*WithName)(const char *name);
    struct PersonBuilder *(*WithAddress)(const char *address);
    struct PersonBuilder *(*WithEmail)(const char *email);
    Person *(*build)(void);
} PersonBuilder;

static PersonBuilder *_builder = NULL;

Person *Person_Create(const char *name, const char *address,
                      const char *email)
{
    Person *self = (Person *)malloc(sizeof(Person));
    strcpy(self->name, name);
    strcpy(self->address, address);
    strcpy(self->email, email);
    return self;
}

void Person_Print(Person *self)
{
    printf("\n{");
    if (strlen(self->name))
        printf("%s", self->name);
    if (strlen(self->address))
        printf(",%s", self->address);
    if (strlen(self->email))
        printf(",%s", self->email);
    printf("}");
}

void Person_Destroy(Person *self)
{
    free(self);
}

PersonBuilder *PersonBuilder_SetName(const char *name)
{
    strcpy(_builder->profile.name, name);
    return _builder;
}

PersonBuilder *PersonBuilder_SetAddress(const char *address)
{
    strcpy(_builder->profile.address, address);
    return _builder;
}

PersonBuilder *PersonBuilder_SetEmail(const char *email)
{
    strcpy(_builder->profile.email, email);
    return _builder;
}

Person *PersonBuilder_Build(void)
{
    Person *person = Person_Create(
        _builder->profile.name,
        _builder->profile.address,
        _builder->profile.email);
    memset(_builder->profile.name, 0, BUFF_SIZE);
    memset(_builder->profile.address, 0, BUFF_SIZE);
    memset(_builder->profile.email, 0, BUFF_SIZE);
    return person;
}

PersonBuilder *PersonBuilder_Create()
{
    if (!_builder)
    {
        _builder = (PersonBuilder *)malloc(sizeof(PersonBuilder));
    }
    memset(_builder->profile.name, 0, BUFF_SIZE);
    memset(_builder->profile.address, 0, BUFF_SIZE);
    memset(_builder->profile.email, 0, BUFF_SIZE);
    _builder->WithName = &PersonBuilder_SetName;
    _builder->WithAddress = &PersonBuilder_SetAddress;
    _builder->WithEmail = &PersonBuilder_SetEmail;
    _builder->build = &PersonBuilder_Build;
    return _builder;
}
int main(int argc, char **argv)
{
    PersonBuilder *builder = PersonBuilder_Create();
    builder->WithName("John")
        ->WithAddress("Chicago")
        ->WithEmail("john@gmail.com");
    Person *john = builder->build();
    builder->WithName("Mary")
        ->WithAddress("New York");
    Person *mary = builder->build();
    Person_Print(john);
    Person_Print(mary);
    return 0;
}