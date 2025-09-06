#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_RESERVATIONS 20
#define MAX_ROOMS 20

// ANSI color codes
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"
#define RESET   "\033[0m"
#define BOLD    "\033[1m"

typedef struct {
    char reservationid[100];
    char roomnumber[100];
    char reservationstat[100];
    char custname[100];
    char custnationalid[100];
    int noofnights;
    char checkindate[100];
    char custemail[100];
    char mobilenumber[100];
} reservation;

typedef struct {
    char roomnumber[100];
    char roomstatus[100];
    char roomcategory[100];
    int pricepernight;
} room;

room rooms[MAX_ROOMS];
reservation res[MAX_RESERVATIONS];

void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
    }
}

int checkemail(char email[]) {
    int dot = 0, att = 0;
    for (int i = 0; i < strlen(email); i++) {
        if (email[i] == '.') dot = 1;
        if (email[i] == '@') att = 1;
    }
    return (dot == 1 && att == 1) ? 1 : 0;
}

void check_nostring(char string[]) {
    while (strlen(string) == 0) {
        printf("you didn't enter anything\n");
        printf("please try again:");
        fgets(string, 1000, stdin);
        string[strlen(string) - 1] = '\0';
    }

    int space = 1;
    while (space) {
        for (int i = 0; i < strlen(string); i++) {
            if (string[i] != ' ' && string[i] != '\t') {
                space = 0;
                break;
            }
        }
        if (space) {
            printf("you didn't enter anything\n");
            printf("please try again:");
            fgets(string, 1000, stdin);
            string[strlen(string) - 1] = '\0';
        }
    }
}

void menueorexit();
void displayMenu();

void login() {
    int flag = 0;
    char username[1000];
    char password[1000];
    int valid = 0;
    FILE *fptr;
    fptr = fopen("users.txt", "r");
    if (fptr == NULL) {
        printf("Error couldn't find the file!");
        exit(1);
    }

    while (!valid) {
        printf("Enter your username:");
        fgets(username, 1000, stdin);
        username[strlen(username) - 1] = '\0';
        check_nostring(username);

        printf("Enter your password:");
        fgets(password, 1000, stdin);
        password[strlen(password) - 1] = '\0';
        check_nostring(password);

        char line[1000];
        rewind(fptr);
        flag = 0;
        while (fgets(line, sizeof(line), fptr)) {
            char *token1 = strtok(line, " ");
            char *token2 = strtok(NULL, "\n");
            if (token1 != NULL && token2 != NULL) {
                trim_newline(token1);
                trim_newline(token2);
                if (strcmp(username, token1) == 0 && strcmp(password, token2) == 0) {
                    flag = 1;
                    break;
                }
            }
        }

        if (flag == 0) {
            printf("\nInvalid information!\n");
            printf("Please try again.\n");
        } else {
            printf("\nLogin successful!!\n");
            valid = 1;
            displayMenu();
        }
    }
    fclose(fptr);
}

void load() {
    char line[1000];
    FILE *fptr;
    fptr = fopen("reservations.txt", "r");
    if (fptr == NULL) {
        printf("Error couldn't find the 'reservations.txt' file!");
        exit(1);
    }

    printf("\nLoading reserved rooms data....\n");
    int count = 0;
    while (fgets(line, sizeof(line), fptr)) {
        line[strcspn(line, "\n")] = '\0';
        char *Reservation_ID = strtok(line, ",");
        char *Roomnumber = strtok(NULL, ",");
        char *roomreservstatus = strtok(NULL, ",");
        char *custname = strtok(NULL, ",");
        char *customernationalid = strtok(NULL, ",");
        char *noofnights = strtok(NULL, ",");
        char *checkindate = strtok(NULL, ",");
        char *email = strtok(NULL, ",");
        char *mobilenumber = strtok(NULL, ",");

        if (Reservation_ID && Roomnumber && roomreservstatus && custname &&
            customernationalid && noofnights && checkindate && email && mobilenumber) {
            printf("Reservation ID: %s\n", Reservation_ID);
            printf("Room number: %s\n", Roomnumber);
            printf("Room reservation status: %s\n", roomreservstatus);
            printf("Customer name: %s\n", custname);
            printf("Customer national ID: %s\n", customernationalid);
            printf("Number of nights: %s\n", noofnights);
            printf("Check-in date: %s\n", checkindate);
            printf("Email: %s\n", email);
            printf("Mobile number: %s\n\n", mobilenumber);
            count++;
        }
    }

    printf("---------Data loaded---------\n");
    fclose(fptr);
    menueorexit();
}

void roomss() {
    FILE *fptr;
    fptr = fopen("Room.txt", "r");
    if (fptr == NULL) {
        printf("Error couldn't find the 'Room.txt' file!");
        exit(1);
    }

    char line[1000];
    printf("\nRoom Data:\n");
    while (fgets(line, sizeof(line), fptr) != NULL) {
        char *roomnumber = strtok(line, " ");
        char *roomstatus = strtok(NULL, " ");
        char *roomcategory = strtok(NULL, " ");
        char *price = strtok(NULL, "\n");

        if (roomnumber && roomstatus && roomcategory && price) {
            printf("Room Number: %s\n", roomnumber);
            printf("Room Status: %s\n", roomstatus);
            printf("Room Category: %s\n", roomcategory);
            printf("Price: %s\n\n", price);
        }
    }
    fclose(fptr);
    menueorexit();
}

void updateroomstatus(char *Roomnumber, char *newstat) {
    char line[1000];
    FILE *proom = fopen("Room.txt", "r");
    if (proom == NULL) {
        printf("Error can't find the 'Room.txt' file.");
        return;
    }

    FILE *ptemproom = fopen("temproom.txt", "w");
    if (ptemproom == NULL) {
        printf("Error can't create temporary file");
        fclose(proom);
        return;
    }

    while (fgets(line, sizeof(line), proom) != NULL) {
        char temp[1000];
        strcpy(temp, line);
        char *roomnumber = strtok(line, " ");
        char *roomstatus = strtok(NULL, " ");
        char *roomcategory = strtok(NULL, " ");
        char *price = strtok(NULL, "\n");

        if (roomnumber && strcmp(roomnumber, Roomnumber) == 0) {
            fprintf(ptemproom, "%s %s %s %s\n", roomnumber, newstat, roomcategory, price);
        } else {
            fprintf(ptemproom, "%s", temp);
        }
    }

    fclose(proom);
    fclose(ptemproom);
    remove("Room.txt");
    rename("temproom.txt", "Room.txt");
}

void addinformation() {
    reservation reserv2;
    char line1[1000], avroom[1000], roomcategory[100];
    FILE *fptr1 = fopen("Room.txt", "r");
    if (fptr1 == NULL) {
        printf("ERROR couldn't find 'Room.txt' file\n");
        return;
    }

    FILE *fptr2 = fopen("reservations.txt", "a");
    if (fptr2 == NULL) {
        printf("ERROR couldn't find 'reservations.txt' file\n");
        fclose(fptr1);
        return;
    }

    printf("\nThe required information to reserve a room:\n");
    printf("Enter customer name:");
    fgets(reserv2.custname, sizeof(reserv2.custname), stdin);
    reserv2.custname[strlen(reserv2.custname) - 1] = '\0';
    check_nostring(reserv2.custname);

    printf("Enter customer national id:\n");
    fgets(reserv2.custnationalid, sizeof(reserv2.custnationalid), stdin);
    reserv2.custnationalid[strlen(reserv2.custnationalid) - 1] = '\0';
    check_nostring(reserv2.custnationalid);

    while (strlen(reserv2.custnationalid) != 14) {
        printf("The national id must contain 14 numbers!\n");
        printf("Enter customer national id:\n");
        fgets(reserv2.custnationalid, sizeof(reserv2.custnationalid), stdin);
        reserv2.custnationalid[strlen(reserv2.custnationalid) - 1] = '\0';
    }

    printf("Enter email:\n");
    fgets(reserv2.custemail, sizeof(reserv2.custemail), stdin);
    reserv2.custemail[strlen(reserv2.custemail) - 1] = '\0';
    check_nostring(reserv2.custemail);

    while (checkemail(reserv2.custemail) == 0) {
        printf("Incorrect email format! please try again!\n");
        printf("Enter email:\n");
        fgets(reserv2.custemail, sizeof(reserv2.custemail), stdin);
        reserv2.custemail[strlen(reserv2.custemail) - 1] = '\0';
        check_nostring(reserv2.custemail);
    }

    printf("Enter mobile number:\n");
    fgets(reserv2.mobilenumber, sizeof(reserv2.mobilenumber), stdin);
    reserv2.mobilenumber[strlen(reserv2.mobilenumber) - 1] = '\0';
    check_nostring(reserv2.mobilenumber);

    printf("Enter check in date (DD-MM-YYYY):\n");
    fgets(reserv2.checkindate, sizeof(reserv2.checkindate), stdin);
    reserv2.checkindate[strlen(reserv2.checkindate) - 1] = '\0';
    check_nostring(reserv2.checkindate);

    printf("Enter the number of nights:\n");
    scanf("%d", &reserv2.noofnights);
    getchar();

    printf("Enter the room category (SeaView, LakeView, or GardenView):");
    fgets(roomcategory, sizeof(roomcategory), stdin);
    roomcategory[strlen(roomcategory) - 1] = '\0';
    check_nostring(roomcategory);

    roomcategory[0] = toupper(roomcategory[0]);
    for (int i = 1; i < strlen(roomcategory); i++) {
        if (roomcategory[i] != 'V' && roomcategory[i] != 'v') {
            roomcategory[i] = tolower(roomcategory[i]);
        } else {
            roomcategory[i] = toupper(roomcategory[i]);
        }
    }

    srand(time(0));
    sprintf(reserv2.reservationid, "%d", rand() % 100000 + 1);

    int found = 0;
    rewind(fptr1);
    while (fgets(line1, sizeof(line1), fptr1)) {
        char *roomid = strtok(line1, " ");
        char *roomstat = strtok(NULL, " ");
        char *category = strtok(NULL, " ");
        char *price = strtok(NULL, "\n");

        if (roomid && roomstat && category && price &&
            strcmp(category, roomcategory) == 0 &&
            strcmp(roomstat, "Available") == 0) {
            found = 1;
            strcpy(avroom, roomid);
            break;
        }
    }

    if (!found) {
        printf("Can't find a room with this category.\n");
        fclose(fptr1);
        fclose(fptr2);
        return;
    }

    strcpy(reserv2.roomnumber, avroom);
    strcpy(reserv2.reservationstat, "unconfirmed");

    fprintf(fptr2, "%s,%s,%s,%s,%s,%d,%s,%s,%s\n",
            reserv2.reservationid,
            reserv2.roomnumber,
            reserv2.reservationstat,
            reserv2.custname,
            reserv2.custnationalid,
            reserv2.noofnights,
            reserv2.checkindate,
            reserv2.custemail,
            reserv2.mobilenumber);

    fclose(fptr1);
    fclose(fptr2);
    printf("----------Reservation created successfully----------\n");
    menueorexit();
}

int is_date_valid(const char *input_date, const char *reserved_date) {
    int input_dd, input_mm, input_yyyy;
    int reserved_dd, reserved_mm, reserved_yyyy;

    sscanf(input_date, "%d-%d-%d", &input_dd, &input_mm, &input_yyyy);
    sscanf(reserved_date, "%d-%d-%d", &reserved_dd, &reserved_mm, &reserved_yyyy);

    if (input_yyyy < reserved_yyyy) return 0;
    if (input_yyyy == reserved_yyyy && input_mm < reserved_mm) return 0;
    if (input_yyyy == reserved_yyyy && input_mm == reserved_mm && input_dd < reserved_dd) return 0;

    return 1;
}

void checkin() {
    char reservationid[150];
    char line[1000];
    int found = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char currentDate[20];
    sprintf(currentDate, "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    FILE *fptr1 = fopen("reservations.txt", "r");
    if (fptr1 == NULL) {
        printf("ERROR! Couldn't find the 'reservations.txt' file.\n");
        return;
    }

    FILE *fptr2 = fopen("Temporaryreservation.txt", "w");
    if (fptr2 == NULL) {
        printf("ERROR! Couldn't create temporary file.\n");
        fclose(fptr1);
        return;
    }

    printf("Enter your reservation ID please: ");
    fgets(reservationid, sizeof(reservationid), stdin);
    reservationid[strcspn(reservationid, "\n")] = '\0';

    while (fgets(line, sizeof(line), fptr1) != NULL) {
        char temp[1000];
        strcpy(temp, line);

        char *Reservation_ID = strtok(line, ",");
        char *Roomnumber = strtok(NULL, ",");
        char *roomreservstatus = strtok(NULL, ",");
        char *custname = strtok(NULL, ",");
        char *customernationalid = strtok(NULL, ",");
        char *noofnights = strtok(NULL, ",");
        char *checkindate = strtok(NULL, ",");
        char *email = strtok(NULL, ",");
        char *mobilenumber = strtok(NULL, "\n");

        if (Reservation_ID && strcmp(reservationid, Reservation_ID) == 0) {
            found = 1;

            if (strcmp(roomreservstatus, "unconfirmed") != 0) {
                printf("This reservation is already confirmed!\n");
                fprintf(fptr2, "%s", temp);
                continue;
            }

            if (!is_date_valid(currentDate, checkindate)) {
                printf("Error: Cannot check-in before the reserved date!\n");
                fprintf(fptr2, "%s", temp);
                continue;
            }

            fprintf(fptr2, "%s,%s,confirmed,%s,%s,%s,%s,%s,%s\n",
                    Reservation_ID, Roomnumber, custname, customernationalid,
                    noofnights, checkindate, email, mobilenumber);
            printf("Check-in successful!\n");
            updateroomstatus(Roomnumber, "Reserved");
        } else {
            fprintf(fptr2, "%s", temp);
        }
    }

    fclose(fptr1);
    fclose(fptr2);

    if (!found) {
        printf("Reservation ID not found!\n");
        remove("Temporaryreservation.txt");
    } else {
        char saveOption;
        printf("Do you want to save changes? (Y/N): ");
        scanf(" %c", &saveOption);
        getchar();

        if (saveOption == 'Y' || saveOption == 'y') {
            remove("reservations.txt");
            rename("Temporaryreservation.txt", "reservations.txt");
            printf("Changes saved successfully!\n");
        } else {
            remove("Temporaryreservation.txt");
            printf("Changes discarded.\n");
        }
    }
    menueorexit();
}

void cancelreserv() {
    char input[100];
    char line[1000];
    int found = 0;

    FILE *fptr1 = fopen("reservations.txt", "r");
    if (fptr1 == NULL) {
        printf("ERROR: Cannot find the 'reservations.txt' file!\n");
        return;
    }

    FILE *temprr = fopen("tempreservations.txt", "w");
    if (temprr == NULL) {
        printf("ERROR: Cannot create temporary file!\n");
        fclose(fptr1);
        return;
    }

    printf("Enter the Reservation ID or Room Number to cancel: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    while (fgets(line, sizeof(line), fptr1)) {
        line[strcspn(line, "\n")] = '\0';

        char *Reservation_ID = strtok(line, ",");
        char *Roomnumber = strtok(NULL, ",");
        char *roomreservstatus = strtok(NULL, ",");
        char *custname = strtok(NULL, ",");
        char *customernationalid = strtok(NULL, ",");
        char *noofnights = strtok(NULL, ",");
        char *checkindate = strtok(NULL, ",");
        char *email = strtok(NULL, ",");
        char *mobilenumber = strtok(NULL, "\n");

        if (Reservation_ID && Roomnumber && roomreservstatus &&
            (strcmp(input, Reservation_ID) == 0 || strcmp(input, Roomnumber) == 0)) {
            found = 1;

            if (strcmp(roomreservstatus, "confirmed") == 0) {
                printf("Cannot cancel confirmed reservations.\n");
                fprintf(temprr, "%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
                        Reservation_ID, Roomnumber, roomreservstatus, custname,
                        customernationalid, noofnights, checkindate, email, mobilenumber);
            } else {
                printf("Reservation cancelled successfully.\n");
                updateroomstatus(Roomnumber, "Available");
            }
        } else {
            fprintf(temprr, "%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
                    Reservation_ID, Roomnumber, roomreservstatus, custname,
                    customernationalid, noofnights, checkindate, email, mobilenumber);
        }
    }

    fclose(fptr1);
    fclose(temprr);

    if (found) {
        remove("reservations.txt");
        rename("tempreservations.txt", "reservations.txt");
    } else {
        printf("No reservation found with the given ID or room number.\n");
        remove("tempreservations.txt");
    }
    menueorexit();
}

void editReservation() {
    char reservationid[100];
    char line[1000];
    int found = 0;

    FILE *fptr1 = fopen("reservations.txt", "r");
    if (fptr1 == NULL) {
        printf("ERROR: Cannot find the 'reservations.txt' file!\n");
        return;
    }

    FILE *tempfile = fopen("temp_reservations.txt", "w");
    if (tempfile == NULL) {
        printf("ERROR: Cannot create temporary file!\n");
        fclose(fptr1);
        return;
    }

    printf("Enter the Reservation ID to edit: ");
    fgets(reservationid, sizeof(reservationid), stdin);
    reservationid[strcspn(reservationid, "\n")] = '\0';

    while (fgets(line, sizeof(line), fptr1)) {
        char temp[1000];
        strcpy(temp, line);

        char *Reservation_ID = strtok(line, ",");
        char *Roomnumber = strtok(NULL, ",");
        char *roomreservstatus = strtok(NULL, ",");
        char *custname = strtok(NULL, ",");
        char *customernationalid = strtok(NULL, ",");
        char *noofnights = strtok(NULL, ",");
        char *checkindate = strtok(NULL, ",");
        char *email = strtok(NULL, ",");
        char *mobilenumber = strtok(NULL, "\n");

        if (Reservation_ID && strcmp(reservationid, Reservation_ID) == 0) {
            found = 1;

            if (strcmp(roomreservstatus, "confirmed") == 0) {
                printf("Cannot edit confirmed reservations.\n");
                fprintf(tempfile, "%s", temp);
                continue;
            }

            printf("\nEditing Reservation ID: %s\n", Reservation_ID);
            printf("Current Room Number: %s\n", Roomnumber);
            printf("Current Customer Name: %s\n", custname);
            printf("Current Check-in Date: %s\n", checkindate);
            printf("Current Nights: %s\n", noofnights);
            printf("Current Email: %s\n", email);
            printf("Current Mobile: %s\n", mobilenumber);
            printf("\n");

            reservation edited;
            strcpy(edited.reservationid, Reservation_ID);
            strcpy(edited.roomnumber, Roomnumber);
            strcpy(edited.reservationstat, roomreservstatus);
            strcpy(edited.custname, custname);
            strcpy(edited.custnationalid, customernationalid);
            edited.noofnights = atoi(noofnights);
            strcpy(edited.checkindate, checkindate);
            strcpy(edited.custemail, email);
            strcpy(edited.mobilenumber, mobilenumber);

            printf("Enter new customer name (current: %s): ", custname);
            fgets(edited.custname, sizeof(edited.custname), stdin);
            edited.custname[strcspn(edited.custname, "\n")] = '\0';
            if (strlen(edited.custname) == 0) {
                strcpy(edited.custname, custname);
            }

            printf("Enter new check-in date DD-MM-YYYY (current: %s): ", checkindate);
            fgets(edited.checkindate, sizeof(edited.checkindate), stdin);
            edited.checkindate[strcspn(edited.checkindate, "\n")] = '\0';
            if (strlen(edited.checkindate) == 0) {
                strcpy(edited.checkindate, checkindate);
            }

            printf("Enter new number of nights (current: %s): ", noofnights);
            char nights_input[10];
            fgets(nights_input, sizeof(nights_input), stdin);
            nights_input[strcspn(nights_input, "\n")] = '\0';
            if (strlen(nights_input) > 0) {
                edited.noofnights = atoi(nights_input);
            }

            printf("Enter new email (current: %s): ", email);
            fgets(edited.custemail, sizeof(edited.custemail), stdin);
            edited.custemail[strcspn(edited.custemail, "\n")] = '\0';
            if (strlen(edited.custemail) == 0) {
                strcpy(edited.custemail, email);
            }

            printf("Enter new mobile number (current: %s): ", mobilenumber);
            fgets(edited.mobilenumber, sizeof(edited.mobilenumber), stdin);
            edited.mobilenumber[strcspn(edited.mobilenumber, "\n")] = '\0';
            if (strlen(edited.mobilenumber) == 0) {
                strcpy(edited.mobilenumber, mobilenumber);
            }

            fprintf(tempfile, "%s,%s,%s,%s,%s,%d,%s,%s,%s\n",
                    edited.reservationid,
                    edited.roomnumber,
                    edited.reservationstat,
                    edited.custname,
                    edited.custnationalid,
                    edited.noofnights,
                    edited.checkindate,
                    edited.custemail,
                    edited.mobilenumber);

            printf("Reservation updated successfully!\n");
        } else {
            fprintf(tempfile, "%s", temp);
        }
    }

    fclose(fptr1);
    fclose(tempfile);

    if (found) {
        remove("reservations.txt");
        rename("temp_reservations.txt", "reservations.txt");
    } else {
        printf("No reservation found with ID: %s\n", reservationid);
        remove("temp_reservations.txt");
    }
    menueorexit();
}

void viewcustdetails() {
    char search_term[100];
    int found = 0;

    printf("Enter customer name, national ID, or reservation ID to search: ");
    fgets(search_term, sizeof(search_term), stdin);
    search_term[strcspn(search_term, "\n")] = '\0';

    FILE *fptr = fopen("reservations.txt", "r");
    if (fptr == NULL) {
        printf("ERROR: Cannot find reservations file!\n");
        return;
    }

    char line[1000];
    printf("\nCustomer Details:\n");
    printf("-----------------\n");

    while (fgets(line, sizeof(line), fptr)) {
        char *Reservation_ID = strtok(line, ",");
        char *Roomnumber = strtok(NULL, ",");
        char *roomreservstatus = strtok(NULL, ",");
        char *custname = strtok(NULL, ",");
        char *customernationalid = strtok(NULL, ",");
        char *noofnights = strtok(NULL, ",");
        char *checkindate = strtok(NULL, ",");
        char *email = strtok(NULL, ",");
        char *mobilenumber = strtok(NULL, "\n");

        if (Reservation_ID && custname && customernationalid) {
            if (strcmp(search_term, custname) == 0 ||
                strcmp(search_term, customernationalid) == 0 ||
                strcmp(search_term, Reservation_ID) == 0) {
                found = 1;
                printf("Reservation ID: %s\n", Reservation_ID);
                printf("Room Number: %s\n", Roomnumber);
                printf("Status: %s\n", roomreservstatus);
                printf("Customer Name: %s\n", custname);
                printf("National ID: %s\n", customernationalid);
                printf("Nights: %s\n", noofnights);
                printf("Check-in Date: %s\n", checkindate);
                printf("Email: %s\n", email);
                printf("Mobile: %s\n", mobilenumber);
                printf("-----------------\n");
            }
        }
    }

    fclose(fptr);

    if (!found) {
        printf("No customer found with search term: %s\n", search_term);
    }
    menueorexit();
}

void checkOut() {
    char room_number[100];
    int found = 0;

    printf("Enter room number to check-out: ");
    fgets(room_number, sizeof(room_number), stdin);
    room_number[strcspn(room_number, "\n")] = '\0';

    FILE *fptr1 = fopen("reservations.txt", "r");
    if (fptr1 == NULL) {
        printf("ERROR: Cannot find reservations file!\n");
        return;
    }

    FILE *tempfile = fopen("temp_checkout.txt", "w");
    if (tempfile == NULL) {
        printf("ERROR: Cannot create temporary file!\n");
        fclose(fptr1);
        return;
    }

    char line[1000];
    while (fgets(line, sizeof(line), fptr1)) {
        char *Reservation_ID = strtok(line, ",");
        char *Roomnumber = strtok(NULL, ",");
        char *roomreservstatus = strtok(NULL, ",");
        char *custname = strtok(NULL, ",");
        char *customernationalid = strtok(NULL, ",");
        char *noofnights = strtok(NULL, ",");
        char *checkindate = strtok(NULL, ",");
        char *email = strtok(NULL, ",");
        char *mobilenumber = strtok(NULL, "\n");

        if (Roomnumber && strcmp(Roomnumber, room_number) == 0 &&
            roomreservstatus && strcmp(roomreservstatus, "confirmed") == 0) {
            found = 1;
            printf("\nChecking out room %s\n", room_number);
            printf("Customer: %s\n", custname);
            printf("Reservation ID: %s\n", Reservation_ID);
            printf("Check-out completed successfully!\n");

            updateroomstatus(room_number, "Available");
        } else {
            fprintf(tempfile, "%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
                    Reservation_ID, Roomnumber, roomreservstatus, custname,
                    customernationalid, noofnights, checkindate, email, mobilenumber);
        }
    }

    fclose(fptr1);
    fclose(tempfile);

    if (found) {
        remove("reservations.txt");
        rename("temp_checkout.txt", "reservations.txt");
    } else {
        printf("No confirmed reservation found for room %s\n", room_number);
        remove("temp_checkout.txt");
    }
    menueorexit();
}

void report() {
    int total_reservations = 0;
    int confirmed = 0;
    int unconfirmed = 0;
    int sea_view = 0;
    int lake_view = 0;
    int garden_view = 0;

    FILE *fptr = fopen("reservations.txt", "r");
    if (fptr == NULL) {
        printf("ERROR: Cannot find reservations file!\n");
        return;
    }

    char line[1000];
    while (fgets(line, sizeof(line), fptr)) {
        total_reservations++;

        char *Reservation_ID = strtok(line, ",");
        char *Roomnumber = strtok(NULL, ",");
        char *roomreservstatus = strtok(NULL, ",");
        char *custname = strtok(NULL, ",");
        char *customernationalid = strtok(NULL, ",");
        char *noofnights = strtok(NULL, ",");
        char *checkindate = strtok(NULL, ",");
        char *email = strtok(NULL, ",");
        char *mobilenumber = strtok(NULL, "\n");

        if (roomreservstatus) {
            if (strcmp(roomreservstatus, "confirmed") == 0) {
                confirmed++;
            } else if (strcmp(roomreservstatus, "unconfirmed") == 0) {
                unconfirmed++;
            }
        }

        if (Roomnumber) {
            if (Roomnumber[0] == '1') sea_view++;
            else if (Roomnumber[0] == '2') lake_view++;
            else if (Roomnumber[0] == '3') garden_view++;
        }
    }
    fclose(fptr);

    printf("\nReservation Report:\n");
    printf("===================\n");
    printf("Total Reservations: %d\n", total_reservations);
    printf("Confirmed: %d\n", confirmed);
    printf("Unconfirmed: %d\n", unconfirmed);
    printf("Sea View Rooms: %d\n", sea_view);
    printf("Lake View Rooms: %d\n", lake_view);
    printf("Garden View Rooms: %d\n", garden_view);
    printf("===================\n");
    menueorexit();
}

void search() {
    char search_type[10];
    char search_term[100];
    int found = 0;

    printf("Search by (id/room/name/email): ");
    fgets(search_type, sizeof(search_type), stdin);
    search_type[strcspn(search_type, "\n")] = '\0';

    printf("Enter search term: ");
    fgets(search_term, sizeof(search_term), stdin);
    search_term[strcspn(search_term, "\n")] = '\0';

    FILE *fptr = fopen("reservations.txt", "r");
    if (fptr == NULL) {
        printf("ERROR: Cannot find reservations file!\n");
        return;
    }

    char line[1000];
    printf("\nSearch Results:\n");
    printf("===============\n");

    while (fgets(line, sizeof(line), fptr)) {
        char *Reservation_ID = strtok(line, ",");
        char *Roomnumber = strtok(NULL, ",");
        char *roomreservstatus = strtok(NULL, ",");
        char *custname = strtok(NULL, ",");
        char *customernationalid = strtok(NULL, ",");
        char *noofnights = strtok(NULL, ",");
        char *checkindate = strtok(NULL, ",");
        char *email = strtok(NULL, ",");
        char *mobilenumber = strtok(NULL, "\n");

        int match = 0;
        if (strcmp(search_type, "id") == 0 && Reservation_ID && strstr(Reservation_ID, search_term)) {
            match = 1;
        } else if (strcmp(search_type, "room") == 0 && Roomnumber && strstr(Roomnumber, search_term)) {
            match = 1;
        } else if (strcmp(search_type, "name") == 0 && custname && strstr(custname, search_term)) {
            match = 1;
        } else if (strcmp(search_type, "email") == 0 && email && strstr(email, search_term)) {
            match = 1;
        }

        if (match) {
            found = 1;
            printf("Reservation ID: %s\n", Reservation_ID);
            printf("Room Number: %s\n", Roomnumber);
            printf("Status: %s\n", roomreservstatus);
            printf("Customer Name: %s\n", custname);
            printf("National ID: %s\n", customernationalid);
            printf("Nights: %s\n", noofnights);
            printf("Check-in Date: %s\n", checkindate);
            printf("Email: %s\n", email);
            printf("Mobile: %s\n", mobilenumber);
            printf("===============\n");
        }
    }

    fclose(fptr);

    if (!found) {
        printf("No results found for %s: %s\n", search_type, search_term);
    }
    menueorexit();
}

void generateBill() {
    char reservationid[100];
    int found = 0;
    int room_price = 0;
    int total_amount = 0;

    printf("Enter Reservation ID for billing: ");
    fgets(reservationid, sizeof(reservationid), stdin);
    reservationid[strcspn(reservationid, "\n")] = '\0';

    FILE *fptr_res = fopen("reservations.txt", "r");
    if (fptr_res == NULL) {
        printf("ERROR: Cannot find reservations file!\n");
        return;
    }

    FILE *fptr_room = fopen("Room.txt", "r");
    if (fptr_room == NULL) {
        printf("ERROR: Cannot find room file!\n");
        fclose(fptr_res);
        return;
    }

    char line[1000];
    reservation res_data;

    while (fgets(line, sizeof(line), fptr_res)) {
        char *Reservation_ID = strtok(line, ",");
        char *Roomnumber = strtok(NULL, ",");
        char *roomreservstatus = strtok(NULL, ",");
        char *custname = strtok(NULL, ",");
        char *customernationalid = strtok(NULL, ",");
        char *noofnights = strtok(NULL, ",");
        char *checkindate = strtok(NULL, ",");
        char *email = strtok(NULL, ",");
        char *mobilenumber = strtok(NULL, "\n");

        if (Reservation_ID && strcmp(reservationid, Reservation_ID) == 0) {
            found = 1;
            strcpy(res_data.reservationid, Reservation_ID);
            strcpy(res_data.roomnumber, Roomnumber);
            strcpy(res_data.custname, custname);
            strcpy(res_data.custnationalid, customernationalid);
            res_data.noofnights = atoi(noofnights);
            strcpy(res_data.checkindate, checkindate);
            break;
        }
    }
    fclose(fptr_res);

    if (!found) {
        printf("Reservation ID not found!\n");
        fclose(fptr_room);
        return;
    }

    rewind(fptr_room);
    while (fgets(line, sizeof(line), fptr_room)) {
        char *roomnumber = strtok(line, " ");
        char *roomstatus = strtok(NULL, " ");
        char *roomcategory = strtok(NULL, " ");
        char *price = strtok(NULL, "\n");

        if (roomnumber && strcmp(roomnumber, res_data.roomnumber) == 0 && price) {
            room_price = atoi(price);
            break;
        }
    }
    fclose(fptr_room);

    if (room_price == 0) {
        printf("ERROR: Could not find room price!\n");
        return;
    }

    total_amount = room_price * res_data.noofnights;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    printf("\n");
    printf("============================================\n");
    printf("               HOTEL BILL INVOICE           \n");
    printf("============================================\n");
    printf("Invoice Date: %02d-%02d-%04d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    printf("Reservation ID: %s\n", res_data.reservationid);
    printf("Customer Name: %s\n", res_data.custname);
    printf("National ID: %s\n", res_data.custnationalid);
    printf("Room Number: %s\n", res_data.roomnumber);
    printf("Check-in Date: %s\n", res_data.checkindate);
    printf("Number of Nights: %d\n", res_data.noofnights);
    printf("Room Price per Night: $%d\n", room_price);
    printf("--------------------------------------------\n");
    printf("Subtotal: $%d\n", total_amount);

    float tax = total_amount * 0.14;
    float service_charge = total_amount * 0.10;
    float grand_total = total_amount + tax + service_charge;

    printf("Tax (14%%): $%.2f\n", tax);
    printf("Service Charge (10%%): $%.2f\n", service_charge);
    printf("--------------------------------------------\n");
    printf("GRAND TOTAL: $%.2f\n", grand_total);
    printf("============================================\n");

    FILE *bill_file = fopen("bills.txt", "a");
    if (bill_file != NULL) {
        fprintf(bill_file, "Reservation ID: %s | Customer: %s | Room: %s | Nights: %d | Total: $%.2f | Date: %02d-%02d-%04d\n",
                res_data.reservationid, res_data.custname, res_data.roomnumber,
                res_data.noofnights, grand_total, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
        fclose(bill_file);
    }

    menueorexit();
}

void exitPROG() {
    printf(RED BOLD "Exiting the system. Goodbye!\n" RESET);
    exit(0);
}

void menueorexit() {
    int choice;
    printf("\n" MAGENTA "Choose (1,2):\n" RESET);
    printf(MAGENTA "1. Return to main menu\n" RESET);
    printf(MAGENTA "2. EXIT\n" RESET);

    do {
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                displayMenu();
                return;
            case 2:
                exitPROG();
                break;
            default:
                printf(RED "Invalid choice please try again!\n" RESET);
                break;
        }
    } while (choice != 2);
}

void logindisplay() {
    printf(MAGENTA BOLD "                                 ---------------------LOGIN PAGE---------------------\n" RESET);
    printf(MAGENTA BOLD "Choose (1,2):\n" RESET);
    printf(MAGENTA "1. LOGIN\n" RESET);
    printf(MAGENTA "2. EXIT\n" RESET);

    int choicee;
    do {
        printf("Enter your choice: ");
        scanf("%d", &choicee);
        getchar();

        switch (choicee) {
            case 1:
                login();
                return;
            case 2:
                exitPROG();
                break;
            default:
                printf(RED "Invalid choice please try again!\n" RESET);
                break;
        }
    } while (choicee != 2);
}

void displayMenu() {
    int choice;
    do {
        printf(CYAN BOLD "\n                                         --- Hotel Reservation System Menu ---\n" RESET);
        printf(CYAN "1. Load Reservation Data\n" RESET);
        printf(CYAN "2. Load Room Data\n" RESET);
        printf(CYAN "3. Reserve A Room\n" RESET);
        printf(CYAN "4. Check-in\n" RESET);
        printf(CYAN "5. Cancel Reservation\n" RESET);
        printf(CYAN "6. Edit Reservation\n" RESET);
        printf(CYAN "7. View Customer Information\n" RESET);
        printf(CYAN "8. CHECK-OUT\n" RESET);
        printf(CYAN "9. Reservation Report\n" RESET);
        printf(CYAN "10. Search\n" RESET);
        printf(CYAN "11. Generate Bill\n" RESET);  // Add this line
        printf(CYAN "12. Exit\n" RESET);           // Change this from 11 to 12
        printf(CYAN "-------------------------------------\n" RESET);
        printf(CYAN "Enter your choice: " RESET);

        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                load();
                break;
            case 2:
                roomss();
                break;
            case 3:
                addinformation();
                break;
            case 4:
                checkin();
                break;
            case 5:
                cancelreserv();
                break;
            case 6:
                editReservation();
                break;
            case 7:
                viewcustdetails();
                break;
            case 8:
                checkOut();
                break;
            case 9:
                report();
                break;
            case 10:
                search();
                break;
            case 11:
                generateBill();  // Add this line
                break;
            case 12:
                printf("Exiting the system. Goodbye!\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 12);
}

int main() {
    logindisplay();
    return 0;
}
