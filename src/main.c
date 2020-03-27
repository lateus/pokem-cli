#include "application/application.h"
#include "view/view.h"
#include "utils/colors.h"
#include "utils/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <errno.h>

#define PROGRAM_STRING "PokeM"
#define VERSION_STRING "0.1"

extern int printMessages;
extern struct ErrorReport errorReport;
char errorCommand[510] = {0};

void sigint_handler(int signalCode);
void sigabrt_handler(int signalCode);
void sigsegv_handler(int signalCode);

int autodetect(int argc, const char *argv[]);
void printDatabaseMenu();
int requestItem();
int requestDungeon();
int requestGame();

int main(int argc, const char *argv[])
{
    int i; /* iterations */
    const char* databaseTypesStr[] = { "pokemon", "items", "dungeons", "areas", "missions", "rewards", "mails" };
    int selection, result = 0, item = 0, dungeon = 0, game = 0;
    int autodetectResult = -1;

    /* handle signals */
    signal(SIGINT, sigint_handler);
    signal(SIGABRT, sigabrt_handler);
    signal(SIGSEGV, sigsegv_handler);

    strncpy(errorReport.applicationName, PROGRAM_STRING, 50);
    strncpy(errorReport.applicationVersion, VERSION_STRING, 50);
    strncpy(errorReport.issueUrl, "http://github.com/lateo96/pokem-cli/issues", 509);
    strncpy(errorReport.mailUrl, "thecrowporation@gmail.com", 509);
    printMessages = 1; /* enable messages */

    for (i = 0; i < argc; ++i) {
        sprintf(errorCommand, "argv[%d]: %s", i, argv[i]);
        addErrorReportCommand(errorCommand);
    }

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    system("title " PROGRAM_STRING " v" VERSION_STRING);
#endif

    /* Copyright notice */
    fputs(LIGHT PROGRAM_STRING RESET DRED " v" VERSION_STRING DGREEN "   Copyright 2018-2020 Carlos Enrique Perez Sanchez.\n"
          "..............................................................\n\n", stdout);
    fflush(stdout);

    /* Basics command line options */
    if (argc > 1 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
        showHelp(argv[0]);
        return 0;
    } else if (argc > 1 && (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))) {
        fputs(VERSION_STRING "\n", stdout);
        return 0;
    } else if (argc > 1 && (!strcmp(argv[1], "-d") || !strcmp(argv[1], "--database"))) {
        if (argc <= 2) {
            fprintf(stderr, LRED "ERROR:" RESET LIGHT " Missing argument.\n" RESET \
                       "       Expected type [pokemon|items|dungeons|areas|missions|rewards|mails]\n" \
                       "       Example: " LGREEN "%s --database pokemon\n" RESET, argv[0]);
            return InputError;
        } else {
            for (i = 0; i < 7; ++i) {
                if (!strcmp(argv[2], databaseTypesStr[i])) {
                    showDatabase(i);
                    return 0;
                }
            }
            fprintf(stderr, LRED "ERROR:" RESET LIGHT " Unknown database type " LGREEN "%s" RESET LIGHT ".\n" RESET \
                                 "       Available types: [pokemon|items|dungeons|areas|missions|rewards|mails]\n", argv[2]);
            return InputError;
        }
    }

    /* A seed to generate random numbers */
    srand((unsigned int)time(NULL));

    sprintf(errorCommand, "Autodetecting...");
    addErrorReportCommand(errorCommand);

    autodetectResult = autodetect(argc, argv);
    if (autodetectResult == -1) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        (void)result;
        for (;;) {
#endif
            selection = showSelectionScreen();
            switch (selection) {
            case 1:
                sprintf(errorCommand, "Selected: Decode WM");
                addErrorReportCommand(errorCommand);
                result = decodeWM(0, NULL);
                break;
            case 2:
                sprintf(errorCommand, "Selected: Encode WM");
                addErrorReportCommand(errorCommand);
                result = encodeWM(0, NULL);
                break;
            case 3:
                sprintf(errorCommand, "Selected: Decode SOS");
                addErrorReportCommand(errorCommand);
                result = decodeSOSM(0, NULL);
                break;
            case 4:
                sprintf(errorCommand, "Selected: Encode SOS");
                addErrorReportCommand(errorCommand);
                result = encodeSOSM(0, NULL);
                break;
            case 5:
                sprintf(errorCommand, "Selected: Convert SOS");
                addErrorReportCommand(errorCommand);
                result = convertSOS(0, NULL);
                break;
            case 6:
                dungeon = 2;
                sprintf(errorCommand, "Selected: Generate %d massive item missions in dungeon %d", 8, dungeon);
                addErrorReportCommand(errorCommand);

                sprintf(errorCommand, "Request item");
                addErrorReportCommand(errorCommand);
                item = requestItem();

                sprintf(errorCommand, "Generate %d massive item missions in dungeon %d with reward item %d", 8, dungeon, item);
                addErrorReportCommand(errorCommand);
                generateMassiveItemMissions(2, item, 8);
                break;
            case 7:
                sprintf(errorCommand, "Selected: Generate %d high-rank missions", 8);
                addErrorReportCommand(errorCommand);

                sprintf(errorCommand, "Request dungeon");
                addErrorReportCommand(errorCommand);
                dungeon = requestDungeon();

                sprintf(errorCommand, "Request Item");
                addErrorReportCommand(errorCommand);
                item = requestItem();

                sprintf(errorCommand, "Generate %d massive high-rank missions in dungeon %d with reward item %d", 8, dungeon, item);
                addErrorReportCommand(errorCommand);
                generateMassiveHighRankMissions(dungeon, item, 8);
                break;
            case 8:
                sprintf(errorCommand, "Selected: Unlock exclusive pokemon");
                addErrorReportCommand(errorCommand);

                sprintf(errorCommand, "Request game");
                addErrorReportCommand(errorCommand);
                game = requestGame();

                sprintf(errorCommand, "Unlock exclusive pokemon of game %d", game);
                addErrorReportCommand(errorCommand);
                unlockExclusivePokemon(game);
                break;
            case 9:
                sprintf(errorCommand, "Selected: Unlock dungeons");
                addErrorReportCommand(errorCommand);
                unlockDungeons();
                break;
            case 10:
                sprintf(errorCommand, "Selected: Show database");
                addErrorReportCommand(errorCommand);

                printDatabaseMenu();
                sprintf(errorCommand, "Request database topic");
                addErrorReportCommand(errorCommand);
                if (requestAndValidateIntegerInput((unsigned int*)&selection, 0, 0, "") != NoError) {
                    break;
                }

                sprintf(errorCommand, "Show database of type %d", selection - 1);
                addErrorReportCommand(errorCommand);
                showDatabase(selection - 1);
                break;
            case 11:
                sprintf(errorCommand, "Selected: Show help");
                addErrorReportCommand(errorCommand);
                showHelp(argv[0]);
                break;
            default:
                sprintf(errorCommand, "Selected: Exit");
                addErrorReportCommand(errorCommand);
                fputs("Exiting...\n", stdout);
                return 0;
            }
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
            fputc('\n', stdout);
        }
#else
        sprintf(errorCommand, "Exit with code %d", result);
        addErrorReportCommand(errorCommand);
        return result;
#endif
    } else {
        sprintf(errorCommand, "Exit with code %d", autodetectResult);
        addErrorReportCommand(errorCommand);
        return autodetectResult;
    }

    fflush(stdout);
    return 0;
}

/** signal handlers */

/* SIGINT - Interactive */
void sigint_handler(int signalCode)
{
    printMessage(stderr, InfoMessage, "User interrupt. Aborted.\n");
    exit(signalCode);
}

/* SIGABRT - Abnormal termination */
void sigabrt_handler(int signalCode)
{
    printMessage(stderr, InfoMessage, "Aborted.\n");
    exit(signalCode);
}

/* SIGSEGV - Segmentation fault */
void sigsegv_handler(int signalCode)
{
    FILE *f = NULL;
    printMessage(stderr, InfoMessage, LIGHT "The inferior stopped because it received a signal from the Operating System.\n" RESET \
                                      "Code of the signal:    " LRED "%d" RESET "\n" \
                                      "Name of the signal:    " LRED "SIGSEGV" RESET "\n" \
                                      "Meaning of the signal: " LRED "Segmentation fault" RESET "\n", signalCode);
    f = fopen("REPORT.TXT", "w");
    if (f) {
        dumpErrorReport(f);
        fclose(f);
        printMessage(stderr, InfoMessage, "A report named REPORT.TXT has been created.\n"
                                          "Open an issue in %s or send a mail to %s, and in both cases attach the above report.\n", errorReport.issueUrl, errorReport.mailUrl);
    }
    exit(signalCode);
}


int autodetect(int argc, const char *argv[])
{
    size_t lenghtArg1 = argc >= 2 ? strlen(argv[1]) : 0;
    size_t lenghtArg2 = argc >= 3 ? strlen(argv[2]) : 0;

    /* no input */
    if (argc <= 1) {
        return -1;
    }
    printMessage(stdout, InfoMessage, "Autodetected: ");
    if (lenghtArg1 >= 20 && lenghtArg1 <= 28) {
        sprintf(errorCommand, "Autodetected: Decode WM");
        addErrorReportCommand(errorCommand);

        fprintf(stdout, "Decode WM\n\n");
        return decodeWM(argc, argv);
    } else if (lenghtArg1 >= 48 && lenghtArg1 <= 56 && lenghtArg2 > 0 && lenghtArg2 <= 16) {
        sprintf(errorCommand, "Autodetected: Convert SOS");
        addErrorReportCommand(errorCommand);

        fprintf(stdout, "Convert SOS\n\n");
        return convertSOS(argc, argv);
    } else if (lenghtArg1 >= 48 && lenghtArg1 <= 56) {
        sprintf(errorCommand, "Autodetected: Decode SOS");
        addErrorReportCommand(errorCommand);

        fprintf(stdout, "Decode SOS\n\n");
        return decodeSOSM(argc, argv);
    } else if (argc == 10) {
        sprintf(errorCommand, "Autodetected: Encode WM");
        addErrorReportCommand(errorCommand);

        fprintf(stdout, "Encode WM\n\n");
        return encodeWM(argc, argv);
    } else if (argc == 7) {
        sprintf(errorCommand, "Autodetected: Encode SOS");
        addErrorReportCommand(errorCommand);

        fprintf(stdout, "Encode SOS\n\n");
        return encodeSOSM(argc, argv);
    } else {
        sprintf(errorCommand, "Autodetection failed");
        addErrorReportCommand(errorCommand);

        fprintf(stdout, "Nothing.\n\n");
        return -1; /* failed to autodetect */
    }
}

void printDatabaseMenu()
{
    fputs(LIGHT "What kind of database?\n" \
          LGREEN  "1" RESET " - Pokemon\n" \
          LGREEN  "2" RESET " - Items\n" \
          LGREEN  "3" RESET " - Dungeons\n" \
          LGREEN  "4" RESET " - Friend areas\n" \
          LGREEN  "5" RESET " - Mission types\n"
          LGREEN  "6" RESET " - Reward types\n"
          LGREEN  "7" RESET " - SOS Mail types\n"
          LGREEN  "[Other]:" LRED " Cancel\n" RESET, stdout);
    fflush(stdout);
}

int requestItem()
{
    unsigned int i = 0;
    unsigned int selection = -1; /* holds integers values */
    int errorCode = NoError;
    char stringInput[101]; /* holds strings values */
    char *stringEnd;
    int randomHolder;
    int mostSimilarIndex = 0;

    for (;;) {
        randomHolder = 1 + rand() % (itemsCount - 6);
        if (requestAndValidateStringInput(stringInput, 100, 1, itemsStr[randomHolder], LIGHT "Enter the name or room index of the " LGREEN "reward item" RESET LIGHT " (leave it blank for random).\n" RESET) != NoError) {
            continue;
        }
        selection = (unsigned int)strtol(stringInput, &stringEnd, 10);
        if (*stringEnd) { /* non-digit found */
            selection = itemsCount; /* invalid name, invalid index */
            for (i = 0; i < itemsCount; ++i) {
                if (strcmp(itemsStr[i], stringInput) == 0) {
                    selection = i;
                    break; /* item found */
                }
            }

            if (selection == itemsCount) {
                mostSimilarIndex = findMostSimilarStringInArray(stringInput, itemsStr, itemsCount);
                if (mostSimilarIndex == -1) {
                    printMessage(stderr, ErrorMessage, "Cannot find item " LRED "\"%s\"" RESET " in the database.\n", stringInput);
                    continue;
                } else {
                    selection = mostSimilarIndex;
                    printMessage(stderr, InfoMessage, LGREEN "%s" RESET " has been assumed.\n", itemsStr[mostSimilarIndex]);
                }
            }
        } else {
            printMessage(stdout, InfoMessage, "(%s" "%d" RESET ") %s\n", selection < itemsCount ? LGREEN : LRED, selection, selection < itemsCount ? itemsStr[selection] : LRED "[INVALID]" RESET);
        }

        errorCode = checkItem(selection);
        if (errorCode == NoError) {
            break; /* input is ok */
        } else if (errorCode == NoItemError) {
            printMessage(stderr, ErrorMessage, "Item " LRED "0" RESET " [" LRED "%s" RESET "] is not allowed as item to find or deliver.\n\n", itemsStr[0]);
        } else if (errorCode == ItemCannotBeObtainedError) {
            printMessage(stderr, ErrorMessage, "Item " LRED "%d" RESET " [" LRED "%s" RESET "] cannot be obtained as reward.\n\n", selection, itemsStr[selection]);
        } else if (errorCode == ItemOutOfRangeError) {
            printMessage(stderr, ErrorMessage, "Items to find or deliver must be numbers between " LGREEN "1" RESET " [" LGREEN "%s" RESET "] and " LGREEN "%d" RESET " [" LGREEN "%s" RESET "]. Current value: %u [INVALID]\n\n", itemsStr[1], itemsCount - 5, itemsStr[itemsCount - 5], selection);
        }
    } /* for (;;) */
    return selection;
}

int requestDungeon()
{
    unsigned int i = 0;
    unsigned int selection = -1; /* holds integers values */
    int errorCode = NoError;
    char stringInput[101]; /* holds strings values */
    char *stringEnd;
    int randomHolder;
    int mostSimilarIndex = 0;

    for (;;) {
        do {
            randomHolder = rand() % dungeonsCount;
        } while (checkDungeon(randomHolder, WonderMailType) != NoError);
        if (requestAndValidateStringInput(stringInput, 100, 1, dungeonsStr[randomHolder], LIGHT "Enter the name or room index of the " LGREEN "dungeon" RESET LIGHT " (leave it blank for random).\n" RESET) != NoError) {
            continue;
        }
        selection = (unsigned int)strtol(stringInput, &stringEnd, 10);
        if (*stringEnd) { /* non-digit found */
            selection = dungeonsCount; /* invalid name, invalid index */
            for (i = 0; i < dungeonsCount; ++i) {
                if (strcmp(dungeonsStr[i], stringInput) == 0) {
                    selection = i;
                    break; /* item found */
                }
            }

            if (selection == dungeonsCount) {
                mostSimilarIndex = findMostSimilarStringInArray(stringInput, dungeonsStr, dungeonsCount);
                if (mostSimilarIndex == -1) {
                    printMessage(stderr, ErrorMessage, "Cannot find dungeon " LRED "\"%s\"" RESET LIGHT " in the database.\n", stringInput);
                    continue;
                } else {
                    selection = mostSimilarIndex;
                    printMessage(stderr, InfoMessage, LGREEN "%s" RESET " has been assumed.\n", dungeonsStr[mostSimilarIndex]);
                }
            }
        } else {
            printMessage(stdout, InfoMessage, "(%s" "%d" RESET ") %s\n", selection < dungeonsCount ? LGREEN : LRED, selection, selection < dungeonsCount ? dungeonsStr[selection] : LRED "[INVALID]" RESET);
        }

        errorCode = checkDungeon(selection, WonderMailType);
        if (errorCode == NoError) {
            break; /* input is ok */
        } else if (errorCode == MissionCannotBeAcceptedInDungeonError) {
            printMessage(stderr, WarningMessage, "A mission in dungeon " LYELLOW "%u" RESET " [" LYELLOW "%s" RESET "] can be generated, but cannot be done.\n\n", selection, selection < dungeonsCount ? dungeonsStr[selection] : "INVALID");
            break; /* input is semi-ok */
        } else if (errorCode == DungeonOutOfRangeError) {
            printMessage(stderr, ErrorMessage, "The dungeon must be between " LGREEN "0" RESET " [" LGREEN "%s" RESET "] and " LGREEN "%d" RESET " [" LGREEN "%s" RESET "]. Current value: " LRED "%u" RESET " [" LGREEN "INVALID" RESET "]\n\n", dungeonsStr[0], dungeonsCount - 1, dungeonsStr[dungeonsCount - 1], selection);
        } else if (errorCode == DungeonIsInvalidError) {
            printMessage(stderr, ErrorMessage, "The dungeon " LRED "%u" RESET " [" LRED "INVALID" RESET "] is not a valid dungeon.\n\n", selection);
        }
    } /* for (;;) */
    return selection;
}

int requestGame()
{
    int selection;
    const char* gamesTypesStr[] = { "Red Rescue Team", "Blue Rescue Team" };
    int i;
    for (;;) {
        fputs(LIGHT "Select the " LGREEN "game type" RESET LIGHT ".\n" RESET, stdout);
        for (i = 0; i < 2; ++i) {
            fprintf(stdout, LGREEN "%u" RESET " - %s\n", i + 1, gamesTypesStr[i]);
        }
        if (requestAndValidateIntegerInput((unsigned int*)&selection, 0, 0, "") != NoError) {
            continue;
        }
        --selection;
        if (selection < 5) { /* `selection` is unsigned so it's always >= 0 */
            break; /* input is ok */
        }
        printMessage(stderr, ErrorMessage, "Choice " LRED "%d" RESET " is out of range.\n", selection);
    } /* forever */
    return selection;
}