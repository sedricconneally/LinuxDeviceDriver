/**************************************************************
 * Class:  CSC-415-03 Fall 2021
 * Name: Sedric Conneally
 * Student ID: 921030432
 * GitHub UserID: sedricconneally
 * Project: Assignment 6 – Device Driver
 *
 * File: conneally_sedric_HW6_test.c
 *
 * Description: This file calls all of our kernel routines from
 * the module file, and tests them to see if they work. I used
 * code from the derekmolly.ie resource (mentioned in the
 * writeup) to aid in developing my test application.
 *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_LENGTH 50
static char output[BUFFER_LENGTH];

int main()
{
   int fd;
   char userString[BUFFER_LENGTH];
   char modeInput[1];
   int userMode;

   // call my open function
   printf("Testing string reversal...\n");
   fd = open("/dev/myModule", O_RDWR);

   // call my ioctl function
   printf("Enter 1 to reverse a string, 0 to return the same string:\n");
   gets(modeInput);
   userMode = atoi(modeInput);
   ioctl(fd, userMode, 0);

   // call my write function
   printf("Type in the phrase you would like to reverse (or not if you chose 0):\n");
   scanf("%[^\n]%*c", userString);
   printf("Writing phrase to the Device Driver...\n");
   write(fd, userString, strlen(userString));

   printf("Press ENTER to read back from the Device Driver...\n");
   getchar();

   // call my read function
   printf("Reading from the Device Driver...\n");
   read(fd, output, BUFFER_LENGTH);

   //print what was read
   printf("The received message is: [%s]\n", output);
   printf("Testing Complete...\n");
   return 0;
}