#ifndef VALIDATIONS_H_INCLUDED
#define VALIDATIONS_H_INCLUDED

int checkemail(char email[]);
void check_nostring(char string[]);
int is_date_valid(const char *input_date, const char *reserved_date);
int No_Of_Reservations();
int isLeapYear(int year);
int daysInMonth(int month, int year);
int dateToDays(char *date);


#endif // VALIDATIONS_H_INCLUDED
