int checkemail(char email[])
{   int dot=0,att=0;
    for(int i=0;i<strlen(email);i++){
        if(email[i]=='.'){
            dot=1;
        }
        if (email[i]=='@'){
            att=1;
        }
    }
    if (dot==1&&att==1){
        return 1;
    }
    else return 0;
}
void check_nostring(char string[])                                  //function that checks that the user entered anything
{


    while(strlen(string)==0)
    {
        printf("you didn't enter anything\n");                   //check if the user didn't enter anything
        printf("please try again:");
        fgets(string,1000,stdin);
        string[strlen(string)-1]='\0';
    }

    int space=1;
    while(space)
    {
        for(int i=0; i<strlen(string); i++)                      //check if the user only entered spaces or tabs
        {
            if(string[i]==' '||string[i]=='\t')
            {
                continue;
            }
            else
            {
                space=0;
            }
        }
        if(space)
        {
            printf("you didn't enter anything\n");
            printf("please try again:");
            fgets(string,1000,stdin);
            string[strlen(string)-1]='\0';
        }


    }
}
int is_date_valid(const char *input_date, const char *reserved_date) {

    int input_dd, input_mm, input_yyyy;
    int reserved_dd, reserved_mm, reserved_yyyy;


    sscanf(input_date, "%d-%d-%d", &input_dd, &input_mm, &input_yyyy);
    sscanf(reserved_date, "%d-%d-%d", &reserved_dd, &reserved_mm, &reserved_yyyy);

    if (input_yyyy < reserved_yyyy)
        return 0;
    else if (input_yyyy == reserved_yyyy && input_mm < reserved_mm)
        return 0;
    else if (input_yyyy == reserved_yyyy && input_mm == reserved_mm && input_dd < reserved_dd)
        return 0;

    return 1; // Date is valid or on the check-in�date.
}

int No_Of_Reservations() {
    int rescount = 0;
    FILE *fptr = fopen("reservations.txt", "r");
    if (!fptr) {
        printf("ERROR!! 'reservations.txt' FILE NOT FOUND!\n");
        exit(1);
    }

    while ((fscanf(fptr, "%s,%s,%s,%s,%s,%s,%s,%s,%s",
                    res[rescount].reservationid, res[rescount].roomnumber, res[rescount].reservationstat,
                    res[rescount].custname, res[rescount].custnationalid, res[rescount].noofnights,
                    res[rescount].checkindate, res[rescount].custemail, res[rescount].mobilenumber)) == 9) {
        rescount++;
        if (rescount >= MAX_RESERVATIONS) break;  // Avoid overflow of the array
    }
    fclose(fptr);
    return rescount;
}

int isLeapYear(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}
int daysInMonth(int month, int year) {
    int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) {
        return 29;
    }
    return days[--month];
}
int dateToDays(char *date) {
    int day, month, year;
    sscanf(date, "%d/%d/%d", &day, &month, &year);

    int totalDays = 0, i, m;
    for (i = 2024; i < year; i++) {
        totalDays += isLeapYear(i) ? 366 : 365;
    }

    for (m = 1; m < month; m++) {
        totalDays += daysInMonth(m, year);
    }

    totalDays += day - 1;  // Subtract 1 because we start at day 0

    return totalDays;
}


