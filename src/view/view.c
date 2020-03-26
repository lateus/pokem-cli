#include "view.h"
#include "../utils/colors.h"
#include "../utils/utils.h"
#include "../../lib/pokem.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define forever for(;;)
#define DISCLAIMER \
    "This program comes with ABSOLUTELY NO WARRANTY.\n" \
    "This is free software, and you are welcome to modify and/or redistribute it under the\n" \
    "terms of the GNU GENERAL PUBLIC LICENSE v2. Please visit http://www.gnu.org/licenses/\n\n"
#define GRATITUDE \
    "Special thanks to Peter O., since this work couldn't be possible without his research.\n\n"


void showHelp(const char *programName)
{
    fputs(DISCLAIMER GRATITUDE, stdout);

    fprintf(stdout, LIGHT "Usage: " LGREEN "%s [ARGUMENTS]...\n" \
                    LYELLOW "-h, --help              " RESET "display this help and exit\n" \
                    LYELLOW "-v, --version           " RESET "display version information and exit\n" \
                    LYELLOW "-d, --database <type>   " RESET "display a database of the selected type,\n" \
                            "                        which can be one of [pokemon|items|dungeons|areas|missions|rewards|mails]\n\n", programName);

    fprintf(stdout, LIGHT "Generic: ==================================================================\n" RESET \
                    "This program will try to autodetect what do you want to do based\n" \
                    "on your input. If you enter a Wonder Mail or SOS Mail, this program\n" \
                    "will try to decode it. If you enter an SOS Mail and an item, this\n" \
                    "program will try to convert it in a A-OK Mail and then in a Thank-You\n" \
                    "Mail. If you enter exactly 9 or 6 arguments, this program will try\n" \
                    "to encode a Wonder Mail or a SOS Mail respectively.\n\n");

    fprintf(stdout, LIGHT "Conventions: ==============================================================\n" RESET \
                    LIGHT "1. " RESET "Symbols conventions:\n" \
                    "    \'Female\' -> \'" LGREEN "*" RESET "\'\n" \
                    "    \'Male\'   -> \'" LGREEN "/" RESET "\'\n" \
                    "    \'...\'    -> \'" LGREEN "." RESET "\'\n" \
                    LIGHT "2. " RESET "It's recommended that you quote passwords arguments and arguments that\n" \
                    "   contains spaces by using \"double quotes\" on Windows and \'single quotes\' on\n" \
                    "   UNIX systems.\n\n");

    fprintf(stdout, LIGHT "Decode Wonder Mail: =======================================================\n" RESET \
                    LIGHT "Usage: " LGREEN "%s [password1] [password2]...\n" RESET \
                    "Decode all entered passwords. Invalid ones are skipped.\n" \
                    "Examples of valid entries:\n" \
                    LGREEN "%s \'4?6F7M+?4JNRJ*??K??0+9??\' \'16J.8!0?1PS35-?06N?RP8?W\' \'F?N.?QY?8RNYYN?4.J75N+?W\'\n" \
                    LGREEN "%s \'??J9XJX?2P??8??F6H?0P??W\'\n\n", programName, programName, programName);

    fprintf(stdout, LIGHT "Encode Wonder Mail: =======================================================\n" RESET \
                    LIGHT "Usage: " LGREEN "%s [Mission type] [Pkmn client] [Pkmn target] [Item to find/deliver] [Dungeon] [Floor] [Reward type] [Item reward] [Friend area reward]\n" RESET \
                    "You must enter exactly 9 arguments.\n" \
                    "You can use both numeric or text values for pokemon, items, dungeons and friend areas.\n" \
                    "For the remaining fields, only numbers are accepted.\n" \
                    "Check the database to know the available mission types, pokemon, items, dungeons, etc.\n", programName);

    fprintf(stdout, "Example of valid entries:\n" \
                    LGREEN "%s 0 Milotic 1 0 \"Fantasy Strait\" 2 6 \"Wish Stone\" 0\n" RESET \
                    LIGHT "NOTE: " RESET "It's a Help-type mission from Milotic, the target pkmn and the item\n" \
                    "      to find/deliver are ignored. The rescue is in Fantasy Strait at 2nd floor.\n" \
                    "      The reward is a lot of money and a Wish Stone, and the Friend Area reward is ignored.\n", programName);
    fprintf(stdout, "--This is very useful, as wou can get any item, including hard to find ones like Beauty Scarf.\n" \
                    "--You can also unlock game-exclusive pokemon like Porygon, Mantine, Plusle, Roselia and Feebas in\n"
                    "  Red Rescue Team (GBA), and Lapras, Minum, Aipom and Magikarp in Blue Rescue Team (DS).\n" \
                    "--And, of course, you can also create high rescue points rewarding missions to improve your rank.\n\n");

    fprintf(stdout, LIGHT "Decode SOS Mail: ==========================================================\n" RESET \
                    LIGHT "Usage: " LGREEN "%s [password1] [password2]...\n" RESET \
                    "Decode all entered passwords. Invalid ones are skipped.\n" \
                    "Examples of valid entries:\n" \
                    LGREEN "%s \'?M???.R066???2FC?!?R????3HCP?-??32H???Y?M4C??1J??NQ04?\' \'S6???.RF?6F??NWH*5KC???RH1!9?8?JK7P0??SNMJRPSKJ??7QJ4N\'\n" RESET \
                    LGREEN "%s \'????6+7SHX???1?4???H??4?NP???4???TR?????X25??PJ??07?C?\'\n\n", programName, programName, programName);

    fprintf(stdout, LIGHT "Encode SOS Mail: ==========================================================\n" RESET \
                    LIGHT "Usage: " LGREEN "%s [Mail type] [Pkmn client] [Pkmn nickname] [Dungeon] [Floor] [Rescue chances]\n" RESET \
                    "You must enter exactly 6 arguments.\n" \
                    "You can use both numeric or text values for pokemon, items and dungeons.\n" \
                    "For the remaining fields, only numeric values are accepted.\n" \
                    "Check the database to know the available pokemon, items, dungeons, etc.\n", programName);
    fprintf(stdout, "Example of valid entries:\n" \
                    LGREEN "%s 0 Chansey Nurcy \'Joyous Tower\' 50 30\n" RESET \
                    LIGHT "NOTE: " RESET "Rescue (SOS Mail) a Chansey named \"Nurcy\" at Joyous Tower floor 50.\n" \
                    "      You can try 30 times.\n" \
                    "--It is very unlikely that you ever need to encode a SOS Mail, but, still, I want to support it.\n\n", programName);

    fprintf(stdout, LIGHT "Convert SOS Mail in A-OK and Thank-You Mail: ==============================\n" RESET \
                    LIGHT "Usage: " LGREEN "%s [password1] [item1] [password2] [item2]...\n" RESET \
                    "Convert all entered SOS passwords in A-OK and Thank-You Mail passwords. Invalid ones are skipped.\n" \
                    "The input is not restricted to an SOS Mail. If you enter an A-OK Mail, it will be converted to Thank-You Mail,\n" \
                    "and if you enter a Thank-You Mail, its reward item will be replaced with the received one.\n", programName);
    fprintf(stdout, "The item specified is set as reward in the Thank-You Mail password.\n" \
                    "If the item specified does not exists, the most similar is assumed.\n" \
                    "Check the database to know the available items.\n" \
                    "Examples of valid entries:\n" \
                    LGREEN "%s \'?M???.R066???2FC?!?R????3HCP?-??32H???Y?M4C??1J??NQ04?\' \"Joy Seed\" \'S6???.RF?6F??NWH*5KC???RH1!9?8?JK7P0??SNMJRPSKJ??7QJ4N\' Nothing\n" RESET \
                    "--This is very useful, as wou can rescue yourself by converting your own SOS Mail.\n", programName);
    fflush(stdout);
}



void showDatabase(enum DatabaseType type)
{
    unsigned int i;
    switch (type) {
    case PokemonDB:
        fprintf(stdout, LIGHT "Pokemon database\n" RESET);
        for (i = 0; i < pkmnSpeciesCount; ++i) {
            fprintf(stdout, "(" LGREEN "%d" RESET ") %s\n", i, pkmnSpeciesStr[i]);
        }
        break;
    case ItemsDB:
        fprintf(stdout, LIGHT "Items database\n" RESET);
        for (i = 0; i < itemsCount; ++i) {
            fprintf(stdout, "(" LGREEN "%d" RESET ") %s\n", i, itemsStr[i]);
        }
        break;
    case DungeonDB:
        fprintf(stdout, LIGHT "Dungeons database\n" RESET);
        for (i = 0; i < dungeonsCount; ++i) {
            fprintf(stdout, "(" LGREEN "%d" RESET ") %s\n", i, dungeonsStr[i]);
        }
        break;
    case FriendAreaDB:
        fprintf(stdout, LIGHT "Friend areas database\n" RESET);
        for (i = 0; i < friendAreasCount; ++i) {
            fprintf(stdout, "(" LGREEN "%d" RESET ") %s\n", i, friendAreasStr[i]);
        }
        break;
    case MissionDB:
        fprintf(stdout, LIGHT "Mission types database\n" RESET);
        for (i = 0; i < missionTypeObjectiveCount; ++i) {
            fprintf(stdout, "(" LGREEN "%d" RESET ") %s\n", i, missionTypeObjectiveStr[i]);
        }
        break;
    case RewardTypeDB:
        fputs(LIGHT "Reward types database\n" RESET \
              "(" LGREEN "0" RESET ") Money\n" \
              "(" LGREEN "1" RESET ") Money + ?\n" \
              "(" LGREEN "2" RESET ") Item\n" \
              "(" LGREEN "3" RESET ") Item + ?\n" \
              "(" LGREEN "4" RESET ") Money\n" \
              "(" LGREEN "5" RESET ") Money + ?\n" \
              "(" LGREEN "6" RESET ") Item\n" \
              "(" LGREEN "7" RESET ") Item + ?\n" \
              "(" LGREEN "8" RESET ") Friend area\n", stdout);
        break;
    case SosMailTypeDB:
        fputs(LIGHT "Mail types database\n" RESET \
              "(" LGREEN "0" RESET ") SOS Mail\n" \
              "(" LGREEN "1" RESET ") A-Ok Mail\n" \
              "(" LGREEN "2" RESET ") Thank-You Mail\n", stdout);
        break;
    default:
        printMessage(stderr, ErrorMessage, "Invalid request\n"); /* this should never happen */
        break;
    }
}



int showSelectionScreen()
{
    unsigned int selection;
    fputs(LIGHT "What do you want to do?\n" \
          DGRAY   "----------------------- Main Services ------------------------\n" \
          LGREEN  "1" RESET "  - Decode a Wonder Mail\n" \
          LGREEN  "2" RESET "  - Encode a Wonder Mail\n" \
          LGREEN  "3" RESET "  - Decode a SOS Mail\n" \
          LGREEN  "4" RESET "  - Encode a SOS Mail\n" \
          LGREEN  "5" RESET "  - Convert a SOS Mail -> A-OK Mail -> Thank-You Mail\n" \
          DGRAY   "--------------------- Massive Generation ---------------------\n" \
          LGREEN  "6" RESET "  - Generate 8 easy \"Help Me!\" missions\n" \
          LGREEN  "7" RESET "  - Generate a maximum of 8 high-rank \"Help Me!\" missions\n", stdout);
    fputs(DGRAY   "--------------------------- Unlock ---------------------------\n" \
          LGREEN  "8" RESET "  - Unlock pokemon\n" \
          LGREEN  "9" RESET "  - Unlock dungeons\n" \
          DGRAY   "---------------------------- Help ----------------------------\n" \
          LGREEN  "10" RESET " - Show Database\n" \
          LGREEN  "11" RESET " - Show Help\n" \
          LGREEN  "[Other]:" LRED " Exit\n" RESET, stdout);
    if (requestAndValidateIntegerInput(&selection, 0, 0, "") != NoError) {
        return 0; /* exit */
    } else {
        return selection;
    }
}



int requestWonderMailPassword(char *password)
{
    return requestAndValidateStringInput(password, 24, 0, NULL, LIGHT "Enter the Wonder Mail's password\n" RESET);
}



int requestAndParseWonderMailData(struct WonderMail *wm)
{
    unsigned int i = 0;
    unsigned int selection = -1; /* holds integers values */
    int errorCode = NoError;
    char stringInput[101]; /* holds strings values */
    char *stringEnd;
    int randomHolder;
    int mostSimilarIndex = 0;

    const enum RewardType rewardTypes[] = { MoneyMoney, Item2, MoneyMoneyItem, ItemItem2, FriendArea };
    const char* rewardTypesStr[] = { "Money", "Item", "Money + (?)", "Item + (?)", "Friend Area" };
    const int availableFriendAreasIndexes[] = { 9, 10, 15, 37 };

    /* mission type */
    forever {
        fputs(LIGHT "Select the " LGREEN "type of mission" RESET LIGHT " (leave it blank for \"Help Me!\").\n" RESET, stdout);
        for (i = 0; i < 5; ++i) {
            fprintf(stdout, LGREEN "%u" RESET " - ", i + 1);
            fprintf(stdout, missionTypeObjectiveStr[i], i == FindItem || i == DeliverItem ? "item" : "pokemon");
            fputc('\n', stdout);
        }
        if (requestAndValidateIntegerInput(&selection, 1, 1, "") != NoError) {
            continue;
        }
        --selection;
        if (selection < 5) { /* `selection` is unsigned so it's always >= 0 */
            break; /* input is ok */
        }
        printMessage(stderr, ErrorMessage, "Choice " LRED "%d" RESET " is out of range.\n", selection);
    }
    wm->missionType = selection;

    /* pokemon client */
    forever {
        do {
            randomHolder = rand() % pkmnSpeciesCount;
        } while (checkPokemon(randomHolder, WonderMailType) != NoError);
        if (requestAndValidateStringInput(stringInput, 100, 1, pkmnSpeciesStr[randomHolder], LIGHT "Enter the name or room index of the " LGREEN "client pokemon" RESET LIGHT " (leave it blank for random).\n" RESET) != NoError) {
            continue;
        }
        selection = (unsigned int)strtol(stringInput, &stringEnd, 10);
        if (*stringEnd) { /* non-digit found */
            selection = pkmnSpeciesCount; /* invalid name, invalid index */
            for (i = 0; i < pkmnSpeciesCount; ++i) {
                if (strcmp(pkmnSpeciesStr[i], stringInput) == 0) {
                    selection = i;
                    break; /* pkmn found */
                }
            }

            if (selection == pkmnSpeciesCount) {
                mostSimilarIndex = findMostSimilarStringInArray(stringInput, pkmnSpeciesStr, pkmnSpeciesCount);
                if (mostSimilarIndex == -1) {
                    printMessage(stderr, ErrorMessage, "Cannot find pokemon " LRED "\"%s\"" RESET " in the database.\n", stringInput);
                    continue;
                } else {
                    selection = mostSimilarIndex;
                    printMessage(stderr, InfoMessage, LGREEN "%s" RESET " has been assumed.\n", pkmnSpeciesStr[mostSimilarIndex]);
                }
            }
        } else {
            printMessage(stdout, InfoMessage, "(%s" "%d" RESET ") %s\n", selection < pkmnSpeciesCount ? LGREEN : LRED, selection, selection < pkmnSpeciesCount ? pkmnSpeciesStr[selection] : LRED "[INVALID]" RESET);
        }

        errorCode = checkPokemon(selection, WonderMailType);
        if (errorCode == NoError) {
            break; /* input is ok */
        } else if (errorCode == PokemonOutOfRangeError) {
            printMessage(stderr, ErrorMessage, "Pokemon must be between " LGREEN "1" RESET " [" LGREEN "%s" RESET "] and " LGREEN "%d" RESET " [" LGREEN "%s" RESET "]. Current value: " LRED "%u" RESET " [" LRED "INVALID" RESET "]\n\n", pkmnSpeciesStr[1], pkmnSpeciesCount - 1, pkmnSpeciesStr[pkmnSpeciesCount - 1], selection);
        } else if (errorCode == PokemonNotAllowedError) {
            printMessage(stderr, ErrorMessage, LIGHT "Legendaries" RESET " are not allowed in Wonder Mails. Current value: " LRED "%u" RESET " [" LRED "%s" RESET "]\n\n", wm->pkmnClient, pkmnSpeciesStr[selection]);
        }
    } /* forever */
    wm->pkmnClient = selection;

    /* pokemon target */
    if (wm->missionType == Find || wm->missionType == Escort) {
        forever {
            do {
                randomHolder = rand() % pkmnSpeciesCount;
            } while (checkPokemon(randomHolder, WonderMailType) != NoError);
            if (requestAndValidateStringInput(stringInput, 100, 1, pkmnSpeciesStr[randomHolder], LIGHT "Enter the name or room index of the " LGREEN "target pokemon" RESET LIGHT " (leave it blank for random).\n" RESET) != NoError) {
                continue;
            }
            selection = (unsigned int)strtol(stringInput, &stringEnd, 10);
            if (*stringEnd) { /* non-digit found */
                selection = pkmnSpeciesCount; /* invalid name, invalid index */
                for (i = 0; i < pkmnSpeciesCount; ++i) {
                    if (strcmp(pkmnSpeciesStr[i], stringInput) == 0) {
                        selection = i;
                        break; /* pkmn found */
                    }
                }

                if (selection == pkmnSpeciesCount) {
                    mostSimilarIndex = findMostSimilarStringInArray(stringInput, pkmnSpeciesStr, pkmnSpeciesCount);
                    if (mostSimilarIndex == -1) {
                        printMessage(stderr, ErrorMessage, "Cannot find pokemon " LRED "\"%s\"" RESET " in the database.\n", stringInput);
                        continue;
                    } else {
                        selection = mostSimilarIndex;
                        printMessage(stderr, InfoMessage, LGREEN "%s" RESET " has been assumed.\n", pkmnSpeciesStr[mostSimilarIndex]);
                    }
                }
            } else {
                printMessage(stderr, ErrorMessage, LIGHT "Legendaries" RESET " are not allowed in Wonder Mails. Current value: " LRED "%u" RESET " [" LRED "%s" RESET "]\n\n", wm->pkmnTarget, pkmnSpeciesStr[selection]);
            }

            errorCode = checkPokemon(selection, WonderMailType);
            if (errorCode == NoError) {
                break; /* input is ok */
            } else if (errorCode == PokemonOutOfRangeError) {
                printMessage(stderr, ErrorMessage, "Pokemon must be between " LGREEN "1" RESET " [" LGREEN "%s" RESET "] and " LGREEN "%d" RESET " [" LGREEN "%s" RESET "]. Current value: " LRED "%u" RESET " [" LRED "INVALID" RESET "]\n\n", pkmnSpeciesStr[1], pkmnSpeciesCount - 1, pkmnSpeciesStr[pkmnSpeciesCount - 1], selection);
            } else if (errorCode == PokemonNotAllowedError) {
                printMessage(stderr, ErrorMessage, LIGHT "Legendaries" RESET " are not allowed in Wonder Mails. Current value: " LRED "%u" RESET " [" LRED "%s" RESET "]\n\n", wm->pkmnClient, pkmnSpeciesStr[selection]);
            }
        } /* forever */
        wm->pkmnTarget = selection;
    } else {
        wm->pkmnTarget = wm->pkmnClient;
    }

    /* dungeon */
    forever {
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
                    break; /* dungeon found */
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
    } /* forever */
    wm->dungeon = selection;

    /* floor */
    forever {
        if (requestAndValidateIntegerInput(&selection, 1, 1 + rand() % difficulties[wm->dungeon][0], LIGHT "Enter the " LGREEN "floor" RESET LIGHT " (leave it blank for random).\n" RESET) == NoError) {
            errorCode = checkFloor(selection, wm->dungeon);
            if (errorCode == NoError) {
                break; /* input is ok */
            } else if (errorCode == FloorOutOfRangeError) {
                printMessage(stderr, ErrorMessage, "The dungeon " LIGHT "%u" LIGHT " [" LIGHT "%s" LIGHT "] has floors in the range " LGREEN "1" RESET "-" LGREEN "%d" RESET " floors. Current value: " LRED "%u" RESET "\n\n", wm->dungeon, wm->dungeon < dungeonsCount ? dungeonsStr[wm->dungeon] : "INVALID", difficulties[wm->dungeon][0], selection);
            } else if (errorCode == FloorInvalidInDungeonError) {
                printMessage(stderr, ErrorMessage, "A mission cannot be made in floor " LRED "%d" RESET " of dungeon " LIGHT "%u" RESET " [" LIGHT "%s" RESET "].\n\n",
                            selection, wm->dungeon, wm->dungeon < dungeonsCount ? dungeonsStr[wm->dungeon] : "INVALID");
            }
        }
    } /* forever */
    wm->floor = selection;

    /* item to find/deliver */
    if (wm->missionType == FindItem || wm->missionType == DeliverItem) {
        forever {
            randomHolder = wm->missionType == FindItem ? (unsigned int)itemsInDungeons[wm->dungeon][1 + rand() % (itemsInDungeons[wm->dungeon][0] - 1)] : 1 + rand() % (itemsCount - 6);
            fprintf(stdout, LIGHT "Enter the name or room index of the " LGREEN "item to %s" RESET LIGHT " (leave it blank for random).\n" RESET, wm->missionType == FindItem ? "find" : "deliver");
            if (requestAndValidateStringInput(stringInput, 100, 1, itemsStr[randomHolder], "") != NoError) {
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
                        printMessage(stderr, ErrorMessage, "Cannot find item " LRED "\"%s\"" RESET LIGHT " in the database.\n", stringInput);
                        continue;
                    } else {
                        selection = mostSimilarIndex;
                        printMessage(stderr, InfoMessage, LGREEN "%s" RESET " has been assumed.\n", dungeonsStr[mostSimilarIndex]);
                    }
                }
            } else {
                printMessage(stdout, InfoMessage, "(%s" "%d" RESET ") %s\n", selection < itemsCount ? LGREEN : LRED, selection, selection < itemsCount ? itemsStr[selection] : LRED "[INVALID]" RESET);
            }

            errorCode = checkItem(selection);
            if (errorCode == NoError) {
                errorCode = checkItemExistenceInDungeon(selection, wm->dungeon);
                if (wm->missionType == DeliverItem) {
                    break; /* input is ok */
                } else if (errorCode == NoError) {
                    break; /* input is ok */
                } else if (errorCode == ItemNotExistsInDungeonError) {
                    printMessage(stderr, ErrorMessage, "The item " LRED "%u" RESET " [" LRED "%s" RESET "] can't be found in the dungeon " LIGHT "%u" RESET " [" LIGHT "%s" RESET "] To accept a job about finding an item inside a dungeon, the item must exist on that dungeon. The items that can be found in that dungeon are listed bellow:\n",
                                                        selection, itemsStr[selection], wm->dungeon, dungeonsStr[wm->dungeon]);
                    for (i = 1; (int)i <= itemsInDungeons[wm->dungeon][0]; ++i) {
                        fprintf(stderr, RESET "(" LGREEN "%u" RESET ") %s\n", itemsInDungeons[wm->dungeon][i], itemsStr[itemsInDungeons[wm->dungeon][i]]);
                    }
                    fprintf(stderr, "\n\n");
                }
            } else if (errorCode == NoItemError) {
                printMessage(stderr, ErrorMessage, "Item " LRED "0" RESET " [" LRED "%s" RESET "] is not allowed as item to find or deliver.\n\n", itemsStr[0]);
            } else if (errorCode == ItemCannotBeObtainedError) {
                printMessage(stderr, ErrorMessage, "Item " LRED "%d" RESET " [" LRED "%s" RESET "] cannot be obtained as reward.\n\n", selection, itemsStr[selection]);
            } else if (errorCode == ItemOutOfRangeError) {
                printMessage(stderr, ErrorMessage, "Items to find or deliver must be numbers between " LGREEN "1" RESET " [" LGREEN "%s" RESET "] and " LGREEN "%d" RESET " [" LGREEN "%s" RESET "]. Current value: %u [INVALID]\n\n", itemsStr[1], itemsCount - 5, itemsStr[itemsCount - 5], selection);
            }
        } /* forever */
        wm->itemDeliverFind = selection;
    } else {
        wm->itemDeliverFind = 9; /* the default value must be 9 */
    }

    /* reward type */
    forever {
        fputs(LIGHT "Select the " LGREEN "reward type" RESET LIGHT " (leave it blank for \"Money + (?)\").\n" RESET, stdout);
        for (i = 0; i < 5; ++i) {
            fprintf(stdout, LGREEN "%u" RESET " - ", i + 1);
            fputs(rewardTypesStr[i], stdout);
            fputc('\n', stdout);
        }
        if (requestAndValidateIntegerInput(&selection, 1, 3, "") != NoError) {
            continue;
        }
        --selection;
        if (selection < 5) { /* `selection` is unsigned so it's always >= 0 */
            break; /* input is ok */
        }
        printMessage(stderr, ErrorMessage, "Choice " LRED "%d" RESET " is out of range.\n", selection);
    } /* forever */
    wm->rewardType = rewardTypes[selection];

    /* reward item */
    if (wm->rewardType == Item || wm->rewardType == ItemItem || wm->rewardType == Item2 || wm->rewardType == ItemItem2 || wm->rewardType == MoneyItem || wm->rewardType == MoneyMoneyItem) {
        forever {
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
        } /* forever */
        wm->itemReward = selection;
    } else { /* no item reward */
        wm->itemReward = 0;
    }

    /* friend area reward */
    if (wm->rewardType == FriendArea) {
        forever {
            fputs(LIGHT "Select the " LGREEN "friend area" RESET LIGHT " reward (leave it blank for random).\n" RESET, stdout);
            for (i = 0; i < 4; ++i) {
                fprintf(stdout, LGREEN "%u" RESET " - ", i + 1);
                fputs(friendAreasStr[availableFriendAreasIndexes[i]], stdout);
                fputc('\n', stdout);
            }
            if (requestAndValidateIntegerInput(&selection, 1, 1 + rand() % 4, "") != NoError) {
                continue;
            }
            --selection;
            if (selection < 4) { /* `selection` is unsigned so it's always >= 0 */
                break; /* input is ok */
            }
            printMessage(stderr, ErrorMessage, "Choice " LRED "%d" RESET " is out of range.\n", selection);
        }
        wm->friendAreaReward = availableFriendAreasIndexes[selection];
    } else {
        wm->friendAreaReward = 0;
    }

    return NoError;
}



int requestSOSMailPassword(char *password)
{
    return requestAndValidateStringInput(password, 54, 0, NULL, LIGHT "Enter the SOS Mail's password\n" RESET);
}



int requestAndParseSosMailData(struct SosMail *sos)
{
    unsigned int i = 0;
    unsigned int selection = -1;
    int errorCode = NoError;
    char stringInput[101];
    int randomHolder;
    char *stringEnd;
    int mostSimilarIndex = 0;

    /* mail type */
    const enum MailType mailTypes[] = { SosMailType, AOkMailType, ThankYouMailType };
    const char* mailTypesStr[] = { "SOS Mail", "A-Ok Mail", "Thank-You Mail" };

    /* chances left */
    unsigned int minChancesLeft;
    unsigned int maxChancesLeft;

    forever {
        fputs(LIGHT "Select the " LGREEN "type of mail" RESET LIGHT " (leave it blank for SOS Mail).\n" RESET, stdout);
        for (i = 0; i < 3; ++i) {
            fprintf(stdout, LGREEN "%u" RESET " - ", i + 1);
            fputs(mailTypesStr[i], stdout);
            fputc('\n', stdout);
        }
        if (requestAndValidateIntegerInput(&selection, 1, 1, "") == NoError) {
            --selection;
            if (selection < 3) { /* as `selection` is unsigned, it is always >= 0 */
                break; /* input is ok */
            }
        }
        printMessage(stderr, ErrorMessage, "Choice " LRED "%d" RESET " is out of range.\n", selection);
    }
    sos->mailType = mailTypes[selection];

    /* pokemon to rescue */
    forever {
        if (requestAndValidateStringInput(stringInput, 100, 1, pkmnSpeciesStr[rand() % pkmnSpeciesCount], LIGHT "Enter the name or room index of the " LGREEN "pokemon to rescue" RESET LIGHT " (leave it blank for random).\n" RESET) != NoError) {
            continue;
        }
        selection = (unsigned int)strtol(stringInput, &stringEnd, 10);
        if (*stringEnd) { /* non-digit found */
            selection = pkmnSpeciesCount; /* invalid name, invalid index */
            for (i = 0; i < pkmnSpeciesCount; ++i) {
                if (strcmp(pkmnSpeciesStr[i], stringInput) == 0) {
                    selection = i;
                    break; /* pkmn found */
                }
            }

            if (selection == pkmnSpeciesCount) {
                mostSimilarIndex = findMostSimilarStringInArray(stringInput, pkmnSpeciesStr, pkmnSpeciesCount);
                if (mostSimilarIndex == -1) {
                    printMessage(stderr, ErrorMessage, "Cannot find pokemon " LRED "\"%s\"" RESET " in the database.\n", stringInput);
                    continue;
                } else {
                    selection = mostSimilarIndex;
                    printMessage(stderr, InfoMessage, LGREEN "%s" RESET " has been assumed.\n", pkmnSpeciesStr[mostSimilarIndex]);
                }
            }
        } else {
            printMessage(stdout, InfoMessage, "(%s" "%d" RESET ") %s\n", selection < pkmnSpeciesCount ? LGREEN : LRED, selection, selection < pkmnSpeciesCount ? pkmnSpeciesStr[selection] : LRED "[INVALID]" RESET);
        }

        errorCode = checkPokemon(selection, SosMailType);
        if (errorCode == NoError || errorCode == PokemonNotAllowedError) {
            break; /* input is ok */
        } else if (errorCode == PokemonOutOfRangeError) {
            printMessage(stderr, ErrorMessage, "Pokemon must be between " LGREEN "1" RESET " [" LGREEN "%s" RESET "] and " LGREEN "%d" RESET " [" LGREEN "%s" RESET "]. Current value: " LRED "%u" RESET " [" LRED "INVALID" RESET "]\n\n", pkmnSpeciesStr[1], pkmnSpeciesCount - 1, pkmnSpeciesStr[pkmnSpeciesCount - 1], selection);
        }
    } /* forever */
    sos->pkmnToRescue = selection;

    /* nickname */
    forever {
        if (requestAndValidateStringInput(stringInput, 10, 1, pkmnSpeciesStr[sos->pkmnToRescue], LIGHT "Enter the " LGREEN "nickname" RESET LIGHT " of that pokemon (leave it blank for no nickname).\n" RESET) == NoError) {
            break;
        }        
    } /* forever */
    strncpy(sos->pkmnNick, stringInput, 10);

    /* dungeon */
    forever {
        do {
            randomHolder = rand() % dungeonsCount;
        } while (checkDungeon(randomHolder, SosMailType) != NoError);
        if (requestAndValidateStringInput(stringInput, 100, 1, dungeonsStr[randomHolder], LIGHT "Enter the name or room index of the " LGREEN "dungeon" RESET LIGHT " (leave it blank for random).\n" RESET) != NoError) {
            continue;
        }
        selection = (unsigned int)strtol(stringInput, &stringEnd, 10);
        if (*stringEnd) { /* non-digit found */
            selection = dungeonsCount; /* invalid name, invalid index */
            for (i = 0; i < dungeonsCount; ++i) {
                if (strcmp(dungeonsStr[i], stringInput) == 0) {
                    selection = i;
                    break; /* dungeon found */
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

        errorCode = checkDungeon(selection, SosMailType);
        if (errorCode == NoError) {
            break; /* input is ok */
        } else if (errorCode == DungeonOutOfRangeError) {
            printMessage(stderr, ErrorMessage, "The dungeon must be between " LGREEN "0" RESET " [" LGREEN "%s" RESET "] and " LGREEN "%d" RESET " [" LGREEN "%s" RESET "]. Current value: " LRED "%u" RESET " [" LGREEN "INVALID" RESET "]\n\n", dungeonsStr[0], dungeonsCount - 1, dungeonsStr[dungeonsCount - 1], selection);
        } else if (errorCode == DungeonIsInvalidError) {
            printMessage(stderr, ErrorMessage, "The dungeon " LRED "%u" RESET " [" LRED "INVALID" RESET "] is not a valid dungeon.\n\n", selection);
        }
    } /* forever */
    sos->dungeon = selection;

    /* floor */
    forever {
        if (requestAndValidateIntegerInput(&selection, 1, 1 + rand() % difficulties[sos->dungeon][0], LIGHT "Enter the " LGREEN "floor" RESET LIGHT " (leave it blank for random).\n" RESET) == NoError) {
            errorCode = checkFloor(selection, sos->dungeon);
            if (errorCode == NoError) {
                break; /* input is ok */
            } else if (errorCode == FloorOutOfRangeError) {
                printMessage(stderr, ErrorMessage, "The dungeon " LIGHT "%u" LIGHT " [" LIGHT "%s" LIGHT "] has floors in the range " LGREEN "1" RESET "-" LGREEN "%d" RESET " floors. Current value: " LRED "%u" RESET "\n\n", sos->dungeon, sos->dungeon < dungeonsCount ? dungeonsStr[sos->dungeon] : "INVALID", difficulties[sos->dungeon][0], selection);
            }
        }
    }
    sos->floor = selection;

    /* reward item */
    if (sos->mailType == ThankYouMailType) {
        forever {
            randomHolder = 1 + rand() % (itemsCount - 6);
            if (requestAndValidateStringInput(stringInput, 100, 1, itemsStr[randomHolder], LIGHT "Enter the name or room index of the " LGREEN "reward item" RESET LIGHT " (type \"" LGREEN "0" RESET LIGHT "\" or \"" LGREEN "Nothing" RESET LIGHT "\" for no reward, or leave it blank for random).\n" RESET) != NoError) {
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
            if (errorCode == NoError || errorCode == NoItemError) {
                break; /* input is ok */
            } else if (errorCode == ItemCannotBeObtainedError) {
                printMessage(stderr, ErrorMessage, "Item " LRED "%d" RESET " [" LRED "%s" RESET "] cannot be obtained as reward.\n\n", selection, itemsStr[selection]);
            } else if (errorCode == ItemOutOfRangeError) {
                printMessage(stderr, ErrorMessage, "Items to find or deliver must be numbers between " LGREEN "1" RESET " [" LGREEN "%s" RESET "] and " LGREEN "%d" RESET " [" LGREEN "%s" RESET "]. Current value: %u [INVALID]\n\n", itemsStr[1], itemsCount - 5, itemsStr[itemsCount - 5], selection);
            }
        } /* forever */
        sos->itemReward = selection;
    } else {
        sos->itemReward = 1 + rand() % (itemsCount - 6); /* ignored if not thank-u mail */
    }

    /* Mail ID */
    sos->mailID = rand() % 10000;

    /* Chances left */
    minChancesLeft = sos->mailType == SosMailType ?   1 :  0;
    maxChancesLeft = sos->mailType == SosMailType ? 100 : 99;
    forever {
        fprintf(stdout, LIGHT "Enter the number of " LGREEN "chances left" RESET LIGHT " (%d to %d, leave it blank for %d).\n" RESET, minChancesLeft, maxChancesLeft, maxChancesLeft);
        if (requestAndValidateIntegerInput(&selection, 1, maxChancesLeft, "") == NoError && selection >= minChancesLeft && selection <= maxChancesLeft) {
            if (selection >= minChancesLeft && selection <= maxChancesLeft) {
                break; /* input is ok */
            } else {
                printMessage(stderr, ErrorMessage, LRED "%d" RESET " is out of range.\n", selection);
            }
            
        }
    }
    sos->chancesLeft = selection;

    return NoError;
}



int requestAndParseSOSMailConversion(char *password, int *item)
{
    unsigned int i;
    unsigned int selection = -1;
    int errorCode = NoError;
    char stringInput[101];
    int randomHolder;
    char *stringEnd;
    int mostSimilarIndex = 0;

    if (requestAndValidateStringInput(password, 54, 0, NULL, LIGHT "Enter the SOS, A-OK or Thank-You Mail's " LGREEN "password" RESET LIGHT ".\n" RESET) != NoError) {
        return InputError;
    } else {
        for (i = 0; i < strlen(password); ++i) {
            password[i] = toupper(password[i]);
        }
    }

    /* reward item */
    forever {
        randomHolder = 1 + rand() % (itemsCount - 6);
        if (requestAndValidateStringInput(stringInput, 100, 1, itemsStr[randomHolder], LIGHT "Enter the name or room index of the " LGREEN "reward item" RESET LIGHT " for the Thank-You mail" RESET LIGHT " (type \"" LGREEN "0" RESET LIGHT "\" or \"" LGREEN "Nothing" RESET LIGHT "\" for no reward, or leave it blank for random).\n" RESET) != NoError) {
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
        if (errorCode == NoError || errorCode == NoItemError) {
            break; /* input is ok */
        } else if (errorCode == ItemCannotBeObtainedError) {
            printMessage(stderr, ErrorMessage, "Item " LRED "%d" RESET " [" LRED "%s" RESET "] cannot be obtained as reward.\n\n", selection, itemsStr[selection]);
        } else if (errorCode == ItemOutOfRangeError) {
            printMessage(stderr, ErrorMessage, "Items to find or deliver must be numbers between " LGREEN "1" RESET " [" LGREEN "%s" RESET "] and " LGREEN "%d" RESET " [" LGREEN "%s" RESET "]. Current value: %u [INVALID]\n\n", itemsStr[1], itemsCount - 5, itemsStr[itemsCount - 5], selection);
        }
    } /* forever */
    *item = selection;
    return NoError;
}



void printWonderMailData(const struct WonderMailInfo *mailInfo, const struct WonderMail *mail)
{
    char newHead[76] = {0};
    char newBody1[150] = {0};
    char newBody2[150] = {0};
    char newObjective[72] = {0};
    char newPlace[76] = {0};
    char newFloor[57] = {0};
    char newReward[107] = {0};

    char* pkmnHead  = NULL;
    char* pkmnBody1 = NULL;
    char* pkmnBody2 = NULL;
    char* pkmnObjective = NULL;
    char* dungeonPlace  = NULL;
    char floorStr[3] = {0};

    char* dungeonFloor  = NULL;
    char* itemHead      = NULL;
    char* itemBody1     = NULL;
    char* itemBody2     = NULL;
    char* itemObjective = NULL;
    char* itemReward    = NULL;
    char* friendAreaReward = NULL;

    char hold[200] = {0};

    char placeAndFloor[120] = {0};
    char diffColor[50] = {0};

    int i, j;
    char temp[30] = {0};

    strcpy(newHead, mailInfo->head);
    strcpy(newBody1, mailInfo->body1);
    strcpy(newBody2, mailInfo->body2);
    strcpy(newObjective, mailInfo->objective);
    strcpy(newPlace, mailInfo->place);
    strcpy(newFloor, mailInfo->floor);
    strcpy(newReward, mailInfo->reward);
    pkmnHead = strstr(newHead, pkmnSpeciesStr[mail->pkmnTarget]);
    pkmnBody1 = strstr(newBody1, pkmnSpeciesStr[mail->pkmnTarget]);
    pkmnBody2 = strstr(newBody2, pkmnSpeciesStr[mail->pkmnTarget]);
    pkmnObjective = strstr(newObjective, pkmnSpeciesStr[mail->pkmnTarget]);
    dungeonPlace = strstr(newPlace, dungeonsStr[mail->dungeon]);
    sprintf(floorStr, "%d", mail->floor % 100);
    dungeonFloor = strstr(newFloor, floorStr);
    itemHead = strstr(newHead, itemsStr[mail->itemDeliverFind]);
    itemBody1 = strstr(newBody1, itemsStr[mail->itemDeliverFind]);
    itemBody2 = strstr(newBody2, itemsStr[mail->itemDeliverFind]);
    itemObjective = strstr(newObjective, itemsStr[mail->itemDeliverFind]);
    itemReward = strstr(newReward, itemsStr[mail->itemReward]);
    friendAreaReward = strstr(newReward, friendAreasStr[mail->friendAreaReward]);

    if (pkmnHead) {
        strcpy(hold, pkmnHead);
        strcpy(pkmnHead, LYELLOW);
        strncat(newHead, hold, strlen(pkmnSpeciesStr[mail->pkmnTarget]));
        strcat(newHead, WHITE);
        strcat(newHead, hold + strlen(pkmnSpeciesStr[mail->pkmnTarget]));
    }

    if (pkmnBody1) {
        strcpy(hold, pkmnBody1);
        strcpy(pkmnBody1, COLOR_YELLOW);
        strncat(newBody1, hold, strlen(pkmnSpeciesStr[mail->pkmnTarget]));
        strcat(newBody1, RESET COLOR_BACKGROUND);
        strcat(newBody1, hold + strlen(pkmnSpeciesStr[mail->pkmnTarget]));
    }

    if (pkmnBody2) {
        strcpy(hold, pkmnBody2);
        strcpy(pkmnBody2, COLOR_YELLOW);
        strncat(newBody2, hold, strlen(pkmnSpeciesStr[mail->pkmnTarget]));
        strcat(newBody2, RESET COLOR_BACKGROUND);
        strcat(newBody2, hold + strlen(pkmnSpeciesStr[mail->pkmnTarget]));
    }

    if (pkmnObjective) {
        strcpy(hold, pkmnObjective);
        strcpy(pkmnObjective, COLOR_YELLOW);
        strncat(newObjective, hold, strlen(pkmnSpeciesStr[mail->pkmnTarget]));
        strcat(newObjective, RESET COLOR_BACKGROUND);
        strcat(newObjective, hold + strlen(pkmnSpeciesStr[mail->pkmnTarget]));
    }

    if (dungeonPlace) {
        strcpy(hold, dungeonPlace);
        strcpy(dungeonPlace, COLOR_YELLOW);
        strncat(newPlace, hold, strlen(dungeonsStr[mail->dungeon]));
        strcat(newPlace, RESET COLOR_BACKGROUND);
        strcat(newPlace, hold + strlen(dungeonsStr[mail->dungeon]));
    } else {
        strcat(newPlace, COLOR_YELLOW RESET COLOR_BACKGROUND);
    }

    if (dungeonFloor) {
        --dungeonFloor;
        strcpy(hold, dungeonFloor);
        strcpy(dungeonFloor, COLOR_CYAN);
        strncat(newFloor, hold, strlen(floorStr) + 1);
        strcat(newFloor, RESET COLOR_BACKGROUND);
        strcat(newFloor, hold + strlen(floorStr) + 1);
    } else {
        strcat(newFloor, COLOR_CYAN RESET COLOR_BACKGROUND);
    }

    if (itemHead) {
        strcpy(hold, itemHead);
        strcpy(itemHead, LGREEN);
        strncat(newHead, hold, strlen(itemsStr[mail->itemDeliverFind]));
        strcat(newHead, WHITE);
        strcat(newHead, hold + strlen(itemsStr[mail->itemDeliverFind]));
    }

    if (itemBody1) {
        strcpy(hold, itemBody1);
        strcpy(itemBody1, COLOR_GREEN);
        strncat(newBody1, hold, strlen(itemsStr[mail->itemDeliverFind]));
        strcat(newBody1, RESET COLOR_BACKGROUND);
        strcat(newBody1, hold + strlen(itemsStr[mail->itemDeliverFind]));
    }

    if (itemBody2) {
        strcpy(hold, itemBody2);
        strcpy(itemBody2, COLOR_GREEN);
        strncat(newBody2, hold, strlen(itemsStr[mail->itemDeliverFind]));
        strcat(newBody2, RESET COLOR_BACKGROUND);
        strcat(newBody2, hold + strlen(itemsStr[mail->itemDeliverFind]));
    }

    if (itemObjective) {
        strcpy(hold, itemObjective);
        strcpy(itemObjective, COLOR_GREEN);
        strncat(newObjective, hold, strlen(itemsStr[mail->itemDeliverFind]));
        strcat(newObjective, RESET COLOR_BACKGROUND);
        strcat(newObjective, hold + strlen(itemsStr[mail->itemDeliverFind]));
    }
    
    if (itemReward) {
        strcpy(hold, itemReward);
        strcpy(itemReward, COLOR_GREEN);
        strncat(newReward, hold, strlen(itemsStr[mail->itemReward]));
        strcat(newReward, RESET COLOR_BACKGROUND);
        strcat(newReward, hold + strlen(itemsStr[mail->itemReward]));
    } else {
        strcat(newReward, COLOR_GREEN RESET COLOR_BACKGROUND);
    }
    
    if (friendAreaReward) {
        strcpy(hold, friendAreaReward);
        strcpy(friendAreaReward, COLOR_GREEN);
        strncat(newReward, hold, strlen(friendAreasStr[mail->friendAreaReward]));
        strcat(newReward, RESET COLOR_BACKGROUND);
        strcat(newReward, hold + strlen(friendAreasStr[mail->friendAreaReward]));
    } else {
        strcat(newReward, COLOR_GREEN RESET COLOR_BACKGROUND);
    }

    if (!pkmnHead && !itemHead) {
        strcat(newHead, LYELLOW WHITE);
    }
    if (!pkmnBody1 && !itemBody1) {
        strcat(newBody1, COLOR_YELLOW RESET COLOR_BACKGROUND);
    }
    if (!pkmnBody2 && !itemBody2) {
        strcat(newBody2, COLOR_YELLOW RESET COLOR_BACKGROUND);
    }
    if (!pkmnObjective && !itemObjective) {
        strcat(newObjective, COLOR_YELLOW RESET COLOR_BACKGROUND);        
    }

    sprintf(placeAndFloor, "%s  %s", newPlace, newFloor);
    strcpy(diffColor, mailInfo->difficulty == 'E' ? RESET COLOR_BACKGROUND : mailInfo->difficulty == 'D' || mailInfo->difficulty == 'C' ? COLOR_GREEN : mailInfo->difficulty == 'B' || mailInfo->difficulty == 'A' ? COLOR_CYAN : mailInfo->difficulty == 'S' ? COLOR_RED : LYELLOW);
    

#ifndef NO_USE_COLORS

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define EndOfLineString "\xFF\n"
#else
#define EndOfLineString "\n"
#endif

    for (i = j = 0; i < 24; ++i) {
        if (i && i % 4 == 0) {
            temp[i + j++] = '\0';
        }
        temp[i + j] = mailInfo->password[i];
    }

    fprintf(stdout, COLOR_BORDER COLOR_BACKGROUND "************************************************" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "* " RESET WHITE COLOR_BACKGROUND UNDERLINE "%-58s" RESET COLOR_BACKGROUND COLOR_BORDER " *" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "* " COLOR_BORDER "%-45s*" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "* " RESET COLOR_BACKGROUND "%-83s" COLOR_BORDER COLOR_BACKGROUND "*" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "* " RESET COLOR_BACKGROUND "%-83s" COLOR_BORDER COLOR_BACKGROUND "*" RESET EndOfLineString, newHead, "", newBody1, newBody2);
    fprintf(stdout, COLOR_BORDER COLOR_BACKGROUND "* " COLOR_BORDER "%-45s*" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "* " WHITE COLOR_BACKGROUND "Client:     " RESET COLOR_BACKGROUND "%-33s" COLOR_BORDER "*" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "* " WHITE COLOR_BACKGROUND "Objective:  " RESET COLOR_BACKGROUND "%-71s" COLOR_BORDER "*" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "* " WHITE COLOR_BACKGROUND "Place:      " RESET COLOR_BACKGROUND "%-109s" COLOR_BORDER "*" RESET EndOfLineString, "", mailInfo->client, newObjective, placeAndFloor);
    fprintf(stdout, COLOR_BORDER COLOR_BACKGROUND "* " WHITE COLOR_BACKGROUND "Difficulty: " RESET COLOR_BACKGROUND "%s%c%-32s" COLOR_BORDER "*" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "* " WHITE COLOR_BACKGROUND "Reward:     " RESET COLOR_BACKGROUND "%-109s" COLOR_BORDER "*" RESET EndOfLineString, diffColor, mailInfo->difficulty, "", newReward);
    fprintf(stdout, COLOR_BORDER COLOR_BACKGROUND "* " WHITE COLOR_BACKGROUND "Password:   " RESET COLOR_BACKGROUND "%s" COLOR_YELLOW "%s" RESET COLOR_BACKGROUND "%-25s" COLOR_BORDER COLOR_BACKGROUND "*" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "* " RESET COLOR_BACKGROUND "            %s" COLOR_YELLOW "%s" RESET COLOR_BACKGROUND "%-25s" COLOR_BORDER COLOR_BACKGROUND "*" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "************************************************" RESET EndOfLineString,
                    temp, temp + 5, temp + 10, temp + 15, temp + 20, temp + 25);
#else
    printWonderMailDataToFile(mailInfo, stdout);
#endif
}


void printSOSData(const struct SosMailInfo *mailInfo, const struct SosMail *mail)
{
    char newClient[70]  = {0};
    char newPlace[76]   = {0};
    char newFloor[57]   = {0};
    char newReward[106] = {0};

    char* dungeonPlace = NULL;
    char floorStr[3] = {0};
    char* dungeonFloor = NULL;
    char* itemReward = NULL;

    char hold[200] = {0};

    char placeAndFloor[120] = {0};
    char diffColor[50] = {0};

    int i, j;
    char temp[70] = {0};

    sprintf(newClient, "%s " COLOR_GREEN "(%s)" RESET COLOR_BACKGROUND, mailInfo->nickname, mailInfo->client);
    strcpy(newPlace, mailInfo->place);
    strcpy(newFloor, mailInfo->floor);
    strcpy(newReward, mailInfo->reward);
    dungeonPlace = strstr(newPlace, dungeonsStr[mail->dungeon]);
    sprintf(floorStr, "%d", mail->floor % 100);
    dungeonFloor = strstr(newFloor, floorStr);
    itemReward = strstr(newReward, itemsStr[mail->itemReward]);

    if (dungeonPlace) {
        strcpy(hold, dungeonPlace);
        strcpy(dungeonPlace, COLOR_YELLOW);
        strncat(newPlace, hold, strlen(dungeonsStr[mail->dungeon]));
        strcat(newPlace, RESET COLOR_BACKGROUND);
        strcat(newPlace, hold + strlen(dungeonsStr[mail->dungeon]));
    } else {
        strcat(newPlace, COLOR_YELLOW RESET COLOR_BACKGROUND);
    }

    if (dungeonFloor) {
        --dungeonFloor;
        strcpy(hold, dungeonFloor);
        strcpy(dungeonFloor, COLOR_CYAN);
        strncat(newFloor, hold, strlen(floorStr) + 1);
        strcat(newFloor, RESET COLOR_BACKGROUND);
        strcat(newFloor, hold + strlen(floorStr) + 1);
    } else {
        strcat(newFloor, COLOR_CYAN RESET COLOR_BACKGROUND);
    }
    
    if (itemReward) {
        strcpy(hold, itemReward);
        strcpy(itemReward, COLOR_GREEN);
        strncat(newReward, hold, strlen(itemsStr[mail->itemReward]));
        strcat(newReward, RESET COLOR_BACKGROUND);
        strcat(newReward, hold + strlen(itemsStr[mail->itemReward]));
    } else {
        strcat(newReward, COLOR_GREEN RESET COLOR_BACKGROUND);
    }

    sprintf(placeAndFloor, "%s  %s", newPlace, newFloor);
    strcpy(diffColor, mailInfo->difficulty == 'E' ? RESET COLOR_BACKGROUND : mailInfo->difficulty == 'D' || mailInfo->difficulty == 'C' ? COLOR_GREEN : mailInfo->difficulty == 'B' || mailInfo->difficulty == 'A' ? COLOR_CYAN : mailInfo->difficulty == 'S' ? COLOR_RED : LYELLOW);

#ifndef NO_USE_COLORS

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define EndOfLineString "\xFF\n"
#else
#define EndOfLineString "\n"
#endif

    for (i = j = 0; i < 54; ++i) {
        if (i ==  5 || i == 13 || i == 18 ||
            i == 23 || i == 31 || i == 36 ||
            i == 41 || i == 49) {
            temp[i + j++] = '\0';
        }
        temp[i + j] = mailInfo->password[i];
    }

    fprintf(stdout, COLOR_BORDER COLOR_BACKGROUND "**************************************************" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "* " RESET WHITE COLOR_BACKGROUND UNDERLINE "%-46s" RESET COLOR_BACKGROUND COLOR_BORDER " *" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "* " COLOR_BORDER "%-47s*" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "* " RESET COLOR_BACKGROUND "%-47s" COLOR_BORDER COLOR_BACKGROUND "*" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "* " COLOR_BORDER "%-47s*" RESET EndOfLineString, mailInfo->head, "", mailInfo->body, "");
    fprintf(stdout, COLOR_BORDER COLOR_BACKGROUND "* " WHITE COLOR_BACKGROUND "Client:       " RESET COLOR_BACKGROUND "%-71s" COLOR_BORDER COLOR_BACKGROUND "*" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "* " WHITE COLOR_BACKGROUND "Objective:    " RESET COLOR_BACKGROUND "%-33s" COLOR_BORDER "*" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "* " WHITE COLOR_BACKGROUND "Place:        " RESET COLOR_BACKGROUND "%-109s" COLOR_BORDER "*" RESET EndOfLineString, newClient, mailInfo->objective, placeAndFloor);
    fprintf(stdout, COLOR_BORDER COLOR_BACKGROUND "* " WHITE COLOR_BACKGROUND "Difficulty:   " RESET COLOR_BACKGROUND "%s%c%-32s" COLOR_BORDER "*" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "* " WHITE COLOR_BACKGROUND "Reward:       " RESET COLOR_BACKGROUND "%-71s" COLOR_BORDER "*" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "* " WHITE COLOR_BACKGROUND "ID:           " RESET COLOR_BACKGROUND "%-33s" COLOR_BORDER "*" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "%s" WHITE COLOR_BACKGROUND "%s"             RESET COLOR_BACKGROUND "%-33s" COLOR_BORDER "%s" RESET "%s", diffColor, mailInfo->difficulty, "", newReward, mailInfo->id, mail->mailType == SosMailType ? "* " : "\r", mail->mailType == SosMailType ? "Chances left: " : "\r", mail->mailType == SosMailType ? mailInfo->chancesLeft : "\r", mail->mailType == SosMailType ? "*" : "\r", mail->mailType == SosMailType ? "\n" : "\r");
    fprintf(stdout, COLOR_BORDER COLOR_BACKGROUND "* " WHITE COLOR_BACKGROUND "Password:     " RESET COLOR_BACKGROUND "%s" COLOR_YELLOW "%s" RESET COLOR_BACKGROUND "%-20s" COLOR_BORDER COLOR_BACKGROUND "*" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "* " RESET COLOR_BACKGROUND "              %s" COLOR_YELLOW "%s" RESET COLOR_BACKGROUND "%-20s" COLOR_BORDER COLOR_BACKGROUND "*" RESET EndOfLineString
                    COLOR_BORDER COLOR_BACKGROUND "* " RESET COLOR_BACKGROUND "              %s" COLOR_YELLOW "%s" RESET COLOR_BACKGROUND "%-20s" COLOR_BORDER COLOR_BACKGROUND "*" RESET EndOfLineString, temp, temp + 6, temp + 15, temp + 21, temp + 27, temp + 36, temp + 42, temp + 48, temp + 57);
    fprintf(stdout, COLOR_BORDER COLOR_BACKGROUND "**************************************************" RESET EndOfLineString);
#else
    printSOSDataToFile(mailInfo, mail->mailType, stdout);
#endif
}



void printWonderMailDataToFile(const struct WonderMailInfo *mailInfo, FILE *f)
{
    char placeAndFloor[51] = {0};

    char passwordHalf1[13] = {0};
    char passwordHalf2[13] = {0};

    sprintf(placeAndFloor, "%s  %s", mailInfo->place, mailInfo->floor);
    strncpy(passwordHalf1, mailInfo->password, 12);
    strncpy(passwordHalf2, mailInfo->password + 12, 12);

    fprintf(f, "************************************************\n"
                    "* %-44s *\n"
                    "* %-44s *\n"
                    "* %-44s *\n"
                    "* %-44s *\n"
                    "* %-44s *\n"
                    "* Client:     %-32s *\n"
                    "* Objective:  %-32s *\n"
                    "* Place:      %-32s *\n"
                    "* Difficulty: %s%c%-31s *\n"
                    "* Reward:     %-32s *\n"
                    "* Password:   %-32s *\n"
                    "*             %-32s *\n"
                    "************************************************\n",
            mailInfo->head, "", mailInfo->body1, mailInfo->body2, "", mailInfo->client, mailInfo->objective, placeAndFloor, "", mailInfo->difficulty, "", mailInfo->reward, passwordHalf1, passwordHalf2);
}



void printSOSDataToFile(const struct SosMailInfo *mailInfo, enum MailType mailType, FILE *f)
{
    char placeAndFloor[51] = {0};

    char passwordThird1[19] = {0};
    char passwordThird2[19] = {0};
    char passwordThird3[19] = {0};

    sprintf(placeAndFloor, "%s  %s", mailInfo->place, mailInfo->floor);
    strncpy(passwordThird1, mailInfo->password, 18);
    strncpy(passwordThird2, mailInfo->password + 18, 18);
    strncpy(passwordThird3, mailInfo->password + 36, 18);

    fprintf(f, "**************************************************\n"
                    "* %-46s *\n"
                    "* %-46s *\n"
                    "* %-46s *\n"
                    "* %-46s *\n"
                    "* Client:       %-32s *\n"
                    "* Objective:    %-32s *\n"
                    "* Place:        %-32s *\n"
                    "* Difficulty:   %s%c%-31s *\n"
                    "* Reward:       %-32s *\n"
                    "* ID:           %-32s *\n"
                    "%s%s"          "%-33s%s%s"
                    "* Password:     %-32s *\n"
                    "*               %-32s *\n"
                    "*               %-32s *\n"
                    "**************************************************\n",
            mailInfo->head, "", mailInfo->body, "", mailInfo->client, mailInfo->objective, placeAndFloor, "", mailInfo->difficulty, "", mailInfo->reward, mailInfo->id, mailType == SosMailType ? "* " : "\r", mailType == SosMailType ? "Chances left: " : "\r", mailType == SosMailType ? mailInfo->chancesLeft : "\r", mailType == SosMailType ? "*" : "\r", mailType == SosMailType ? "\n" : "\r", passwordThird1, passwordThird2, passwordThird3);
}



int requestAndValidateIntegerInput(unsigned int *n, int allowEmptyValue, int valueIfEmpty, const char* message)
{
#define MAX_LENGTH_INPUT 20
    char *stringEnd;
    char stringInput[MAX_LENGTH_INPUT + 1];

    fputs(message, stdout);
    fputs(">>> " LGREEN, stdout);
    fflush(stdout);
    (void)!fgets(stringInput, MAX_LENGTH_INPUT, stdin);
    if (stringInput[strlen(stringInput) - 1] == '\n') {
        stringInput[strlen(stringInput) - 1] = '\0';
    }
    if (strlen(stringInput) == 0) {
        if (allowEmptyValue) {
            *n = valueIfEmpty;
            fprintf(stdout, "%u\n", valueIfEmpty);
            return NoError;
        } else {
            return InputError;
        }
    } else {
        errno = 0; /* strtol modifies `errno` if the input is too big to be stored in a `long int` */
        *n = (unsigned int)strtol(stringInput, &stringEnd, 10);
        if (*stringEnd || errno) {
            printMessage(stderr, ErrorMessage, "Invalid input: %s.\n\n" RESET, errno ? strerror(errno) : "Only positive numbers are allowed");
            return InputError;
        }
        return NoError;
    }
#undef MAX_LENGTH_INPUT
}

int requestAndValidateStringInput(char* str, unsigned int maxLength, int allowEmptyValue, const char* valueIfEmpty, const char* message)
{
#define MAX_LENGTH_INPUT 100 /* if `maxLength` is bigger the behavior is undefined */
    char stringInput[MAX_LENGTH_INPUT + 1];

    fputs(message, stdout);
    fputs(">>> " LGREEN, stdout);
    fflush(stdout);
    (void)!fgets(stringInput, MAX_LENGTH_INPUT, stdin);
    if (stringInput[strlen(stringInput) - 1] == '\n') {
        stringInput[strlen(stringInput) - 1] = '\0';
    }
    strncpy(str, stringInput, maxLength);
    if (strlen(stringInput) > maxLength) {
        printMessage(stderr, WarningMessage, "The input (" LRED "%s" RESET ") is bigger than %d characters, so it has been truncated to " LGREEN "%s" RESET ".\n" LGREEN, stringInput, maxLength, str);
    }
    
    if (strlen(stringInput) == 0 && !allowEmptyValue) {
        return InputError;
    } else if (strlen(stringInput) == 0) {
        strncpy(str, valueIfEmpty, maxLength);
        fprintf(stdout, "%s\n", str);
    }

    return NoError;
#undef MAX_LENGTH_INPUT
}

#undef forever
#undef DISCLAIMER
