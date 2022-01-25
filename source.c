#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Player{
    char name[25];
    int hp;
    char gunname[25];
    int gundamage;
    int exp;
}Player;

//lisää hahmo. Komento A
Player* addCharacter(Player* arr, const char *name, int hp, const char *weapon, int damage, int exp, int size){
    arr = realloc(arr, sizeof(Player)*(size+1));
    Player uusi;
    strcpy(uusi.name, name);
    uusi.hp = hp;
    strcpy(uusi.gunname, weapon);
    uusi.gundamage = damage;
    uusi.exp = exp;
    arr[size] = uusi;
    return arr;
}
//Tarkista onko saman nimistä hahmoa olemassa. Käytetään komennon A yhteydessä.
int checkIfNameAlreadyInUse(Player* arr, const char *newPlayer, int size){
    int tulos=0;
    int laskuri=0;
    while(laskuri<size){
        if(strcmp(arr[laskuri].name,newPlayer)==0){
            tulos++;
            return tulos;
        }
        else{
            laskuri++;
        }
    }
    return tulos;
}
//Hyökkäys funktio, komento H. Jokainen hyökkääjän vastustajalta viemä elämä lisää hyökkääjälle yhden kokemuspisteen.
//Täten jos hyökkääjän ase tekee 20 vahinkoa ja vastustajalla on 30 elämää. Saa hyökkääjä 20 kokemuspistettä ja vastustajalle jää 10 elämää.
void attack(Player* arr, char* attacker, char* target, int size){
    int life;
    int laskuri=0;
    //hyökkääjä
    int indeksi1=-1;
    //kohde
    int indeksi2=-1;
    //Etsitään aluksi millä indekseillä halutut hahmot ovat
    while(laskuri<size){
    if(strcmp(attacker, arr[laskuri].name)==0){
        indeksi1 = laskuri;
    }
    else if(strcmp(target, arr[laskuri].name)==0){
        indeksi2 = laskuri;
    }
    laskuri++;
    }
    //Jos jompaakumpaa hahmoa ei löydy, keskeytä funktion suoritus ja ilmoita siitä
    if(indeksi1<0){
        printf("Attacker wasn't found. Check your input\n");
    }
    else if(indeksi2<0){
        printf("Target wasn't found. Check your input\n");
    }
    //Jos molemmat löytyvät niin tarkistetaan ettei hyökkääjä ole kuollut. Estetään myös jo kuolleen hahmon vahingoittaminen.
    else{
    if(arr[indeksi1].hp <= 0){
        printf("%s is dead and therefore can't attack\n", arr[indeksi1].name);
    }
    else if(arr[indeksi2].hp <= 0 && arr[indeksi1].gundamage>=0){
        printf("%s is already dead. %s gained 0 experience\n", arr[indeksi2].name, arr[indeksi1].name);
    }
    else{
    //Jos hyökkääjän ase tekee negatiivista vahinkoa lasketaan se parantamiseksi. Jokainen parannettu elämä tuo yhden kokemuspisteen    
    if(arr[indeksi1].gundamage<=0){
        int healed = -arr[indeksi1].gundamage;
        arr[indeksi1].exp+=healed;
        arr[indeksi2].hp += healed;
        printf("%s healed %s. %s gained %d experience. %s health is now %d\n", arr[indeksi1].name, arr[indeksi2].name, arr[indeksi1].name, healed, arr[indeksi2].name, arr[indeksi2].hp);
    }
    else{
    life = arr[indeksi2].hp - arr[indeksi1].gundamage;
    //Jos hyökkääjän ase tekee enemmän vahinkoa kuin mitä kohteella on elämiä. Tulee kohteen elämäksi 0 eli kuollut. Hyökkääjä saa kokemuspisteitä
    //tekemänsä vahingon verran.
    if(life<=0){
        arr[indeksi1].exp += arr[indeksi2].hp;
        arr[indeksi2].hp = 0;
    }
    else{
        arr[indeksi1].exp += arr[indeksi1].gundamage;
        arr[indeksi2].hp = arr[indeksi2].hp - arr[indeksi1].gundamage;
    }
    //Eri tulostukset riippuen siitä kuoliko kohde vai ei
    if(arr[indeksi2].hp <= 0){
     printf("%s attacked %s. %s gained %d experience. %s is now dead\n", attacker, target, attacker, arr[indeksi1].exp, target);   
    }
    else{
    printf("%s attacked %s. %s gained %d experience. %s health is now %d\n", attacker, target, attacker, arr[indeksi1].exp, target, arr[indeksi2].hp);
    }
  }
}
}
}

//Apufunktio kahden hahmon paikkojen vaihtamiseen taulukossa
Player* switchIndex(Player* array, int index1, int index2)
{
    Player aux;
    aux = array[index1];
    array[index1] = array[index2];
    array[index2] = aux;
    return array;
}
//Apufunktio hahmojen järjestämiseen kokemuspisteiden perusteella
Player* sortCharacters(Player* arr, int size){
    Player *sorted = arr;
    int i;
    int j;
    
    for(i=0; i<size; i++){
        for(j=i+1; j<size; j++){
            if(arr[i].exp<arr[j].exp){
                switchIndex(sorted, i, j);
            }
        }
    }
    return sorted;
}
//W komento. Tallentaa tiedostoon pelissä olevat hahmot ja niiden tiedot
int savePlayers(Player* arr, const char* tiedosto, int size){
    int laskuri = 0;
    FILE* f = fopen(tiedosto,"w");
    if(!f){
        return 1;
    }
    while(laskuri<size){
        int ret = fprintf(f, "%s %d %s %d %d\n", arr[laskuri].name, arr[laskuri].hp, arr[laskuri].gunname, arr[laskuri].gundamage, arr[laskuri].exp);
        laskuri++;
        if(ret<0){
            return 1;
        }
    }

    fclose(f);
    return 0;
}

int main(void){
//varataan aluksi aivan vähän muistia
Player *array = malloc(1);
//Size muuttuja pitää kirjaa siitä monta alkiota taulukossa on
int size = 0;
//komennon maksimipituus on 80 merkkiä
char komento[80];
char eka;
//nimien maksimi pituus on 25 merkkiä
char name[25];
char target[25];
int hp;
char weapon[25];
char tiedosto[25];
int damage;
int exp=0;

    printf("The commands are as follows:\n");
    printf("A adds a character. Parameters are name of character, health, name of the weapon and the weapon damage.\nThe maximum character length is 25 characters. The maximum health and damage is 1 billion.\n\n");
    printf("H makes one character attack some other character. The parameters are name of the attacker and name of the target\n");
    printf("L print the characters\n");
    printf("W save the game to a file\n");
    printf("O open saved game file\n");
    printf("Q exit the program\n");

while(69) {
    printf("\nGive a command. The max length is 80 characters\n\n");
    printf("Available commands are A, H, L, W, O and Q\n\n");

    //Lue käyttäjän syöte
    fgets(komento, sizeof(komento), stdin);
    
    //Jos syötteessä on pelkkä rivin vaihto, ei käyttäjä syöttänyt mitään
    if(strlen(komento)==1){
        printf("You didn't input anything.\n");
    }
    else if(strlen(komento)>79){
        printf("The maximum character limit is 80 characters\n");
    }
    else{
    sscanf(komento, "%c", &eka);
    //lisää hahmo
    if (eka == 'A') {
    int ret = sscanf(komento, "%c %s %d %s %d", &eka, name, &hp, weapon, &damage);
    if(ret!=5){
        printf("You didn't give enough parameters or they were the wrong type. Your parameters could have also been too long.\n The maximum name length is 25 characters\n");
    }
    else{
    if(checkIfNameAlreadyInUse(array, name, size)==1){
        printf("Name already in use\n");
    }
    if(damage>1000000000){
        printf("The maximum damage is 1 000 000 000\n");
    }
    if(hp>1000000000){
        printf("The maximum health is 1 000 000 000\n");
    }
    else{
    array = addCharacter(array, name, hp ,weapon, damage, exp, size);
    printf("Creating character %s succeeded\n", array[size].name);
    size++;
    }
    }
    }
    //hyökkää
    else if (eka == 'H') {
    int ret = sscanf(komento, "%c %s %s", &eka, name, target);
        if(ret != 3){
        printf("You didn't give enough parameters or they were the wrong type. Your parameters could have also been too long.\n The maximum name length is 25 characters\n");
    }
    //Hahmo ei voi hyökätä itseensä
    else if(strcmp(name, target)==0){
        printf("Character can't attack itself\n");
    }
    else{
    //Jos pelissä on vain yksi hahmo ei ole ketään kehen hyökätä
    if(size<2){
        printf("Not enough characters\n");
    }
    else{
    attack(array, name, target, size);
    }
    }
    }
    //tulosta
    else if (eka == 'L') {
    //Jos ei ole luotu hahmoja ei ole mitään mitä tulostaa
    if(size==0){
        printf("No characters to print\n");
    }

    else{
    int laskuri = 0;
    //Järjestetään hahmot aluksi kokemuspisteiden mukaan
    Player *print = sortCharacters(array, size);
    //Tulostetaan jokaisen hahmon tiedot kerrallaan
    while(laskuri<size){
        if(print[laskuri].hp>0){
        printf("%s has %d health %d experience and has a gun named %s which makes %d damage.\n", print[laskuri].name, print[laskuri].hp, print[laskuri].exp, print[laskuri].gunname, print[laskuri].gundamage);
        }
        laskuri++;
        }
        laskuri=0;
        while(laskuri<size){
        if(print[laskuri].hp==0){
        printf("%s is dead, has %d experience and has a gun named %s which makes %d damage.\n", print[laskuri].name, print[laskuri].exp, print[laskuri].gunname, print[laskuri].gundamage);
        }
        laskuri++;
        }
    }

    }
    
    //tallenna tiedostoon
    else if (eka == 'W') {
    int ret = sscanf(komento, "%c %s", &eka, tiedosto);
    if(ret != 2){
        printf("You didn't give enough parameters or they were the wrong type. Your parameters could have also been too long.\n The maximum name length is 25 characters\n");
    }
    else{
    if(size==0){
        printf("No players to save\n");
    }
    else{
    if(savePlayers(array, tiedosto, size)==0){
        printf("Saving the game was successful\n");
    }
    else{
        printf("Saving the game was unsuccessful\n");
    }
    }
    }
    }
    //avaa tiedostosta
    else if (eka == 'O') {
    int ret = sscanf(komento, "%c %s", &eka, tiedosto);
    if(ret != 2){
        printf("You didn't give enough parameters or they were the wrong type. Your parameters could have also been too long.\n The maximum name length is 25 characters\n");
    }
    else{
        FILE* f = fopen(tiedosto, "r");
        if(f==NULL){
            printf("Opening the file failed\n");
        }
        else{
    int laskuri = 0;
    int ret = fscanf(f, "%s %d %s %d %d\n", name, &hp, weapon, &damage, &exp);
    //Jos pelissä on jo joitain hahmoja poistetaan ensin ne
    free(array);
    array = malloc(1);
    while(ret == 5) {
        array = addCharacter(array, name, hp, weapon, damage, exp, laskuri);
        ret = fscanf(f, "%s %d %s %d %d\n", name, &hp, weapon, &damage, &exp);
        laskuri++;
    }
    fclose(f);
    size = laskuri;
    if(array==NULL){
        printf("Opening the file failed\n");
    }
    else{
        printf("Opening the file was successful\n");
    }
    }
    }
    }

    //poistu ohjelmasta
    else if (eka == 'Q') {
    free(array);
    printf("Exited the program\n");
    exit(0);
    }
    else {
        printf("invalid command\n");
    }
    }
    
}
return 0;
}