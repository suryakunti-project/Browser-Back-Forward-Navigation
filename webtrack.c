/*
 * ============================================================================
 * Project Title : WebTrack - Browser History Management System Using Two Stacks
 * Language      : C (ISO C99)
 * Description   : Terminal-based browser navigation simulator that models
 *                 Back and Forward buttons using exactly two array-based stacks,
 *                 alongside a separate browsing history log.
 * ============================================================================
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Capacity constants for stacks, browsing history, and URL strings */
#define MAX_STACK_SIZE 50
#define MAX_HISTORY_SIZE 200
#define MAX_URL_LEN 256

/* Return codes for safe line reading */
#define INPUT_OK 1
#define INPUT_TOO_LONG 0
#define INPUT_EOF -1

/*
 * Structure: Stack
 * Purpose  : Represents a fixed-capacity LIFO stack of website URLs.
 *            Used separately for Back Stack and Forward Stack.
 */
typedef struct {
    char items[MAX_STACK_SIZE][MAX_URL_LEN];
    int top; /* Index of the top element; -1 means the stack is empty */
} Stack;

/*
 * Structure: BrowsingHistory
 * Purpose  : Stores every visited website URL in chronological order.
 *            Kept separate from the Back and Forward navigation stacks.
 */
typedef struct {
    char urls[MAX_HISTORY_SIZE][MAX_URL_LEN];
    int count; /* Number of entries currently stored in history */
} BrowsingHistory;

/*
 * Structure: NavStats
 * Purpose  : Tracks navigation statistics for the current session.
 */
typedef struct {
    int totalVisits;  /* Total new websites visited */
    int backCount;    /* Total successful Go Back operations */
    int forwardCount; /* Total successful Go Forward operations */
} NavStats;

/* -------------------- Stack Function Prototypes -------------------- */
void initStack(Stack *s);
int isEmpty(const Stack *s);
int isFull(const Stack *s);
int push(Stack *s, const char *url);
int pop(Stack *s, char *outUrl);
void displayStack(const Stack *s, const char *stackName);
void clearStack(Stack *s);

/* -------------------- UI & Input Helper Prototypes ----------------- */
void displayWelcomeBanner(void);
void displayMenu(void);
int readLine(char *buffer, int size);
void trimSpaces(char *str);
int readMenuChoice(int *choice);
int isValidUrl(const char *url);
int containsIgnoreCase(const char *text, const char *pattern);

/* -------------------- Navigation & History Prototypes -------------- */
void initHistory(BrowsingHistory *history);
void initStats(NavStats *stats);
void addHistoryEntry(BrowsingHistory *history, const char *url);
void visitWebsite(Stack *backStack, Stack *forwardStack, char *currentWebsite,
                  BrowsingHistory *history, NavStats *stats);
void goBack(Stack *backStack, Stack *forwardStack, char *currentWebsite,
            NavStats *stats);
void goForward(Stack *backStack, Stack *forwardStack, char *currentWebsite,
               NavStats *stats);
void displayCurrentWebsite(const char *currentWebsite);
void viewBrowsingHistory(const BrowsingHistory *history);
void searchBrowsingHistory(const BrowsingHistory *history);
void displayStatistics(const Stack *backStack, const Stack *forwardStack,
                       const char *currentWebsite,
                       const BrowsingHistory *history, const NavStats *stats);
void clearBrowsingHistory(BrowsingHistory *history);

/* ============================================================================
 * Main Function
 * Coordinates program initialization and the menu-driven loop.
 * ============================================================================
 */
int main(void) {
    Stack backStack;
    Stack forwardStack;
    BrowsingHistory history;
    NavStats stats;
    char currentWebsite[MAX_URL_LEN] = "";
    int choice = -1;
    int running = 1;

    /* Initialize both stacks, browsing history, and session statistics */
    initStack(&backStack);
    initStack(&forwardStack);
    initHistory(&history);
    initStats(&stats);

    displayWelcomeBanner();

    while (running) {
        int inputStatus;

        displayMenu();
        printf("Enter your choice (0-10): ");

        inputStatus = readMenuChoice(&choice);
        if (inputStatus == INPUT_EOF) {
            printf("\nEnd of input detected. Exiting WebTrack...\n");
            break;
        }
        if (inputStatus == 0) {
            printf("\n[Error] Invalid menu choice. Please enter a number from 0 to 10.\n");
            continue;
        }

        switch (choice) {
            case 1:
                visitWebsite(&backStack, &forwardStack, currentWebsite,
                             &history, &stats);
                break;
            case 2:
                goBack(&backStack, &forwardStack, currentWebsite, &stats);
                break;
            case 3:
                goForward(&backStack, &forwardStack, currentWebsite, &stats);
                break;
            case 4:
                displayCurrentWebsite(currentWebsite);
                break;
            case 5:
                displayStack(&backStack, "Back Stack");
                break;
            case 6:
                displayStack(&forwardStack, "Forward Stack");
                break;
            case 7:
                viewBrowsingHistory(&history);
                break;
            case 8:
                searchBrowsingHistory(&history);
                break;
            case 9:
                displayStatistics(&backStack, &forwardStack, currentWebsite,
                                  &history, &stats);
                break;
            case 10:
                clearBrowsingHistory(&history);
                break;
            case 0:
                printf("\nThank you for using WebTrack! Goodbye.\n");
                running = 0;
                break;
            default:
                printf("\n[Error] Invalid menu choice. Please enter a number from 0 to 10.\n");
                break;
        }
    }

    return 0;
}

/* ============================================================================
 * Stack Operations (Array-Based LIFO Implementation)
 * ============================================================================
 */

/*
 * Function: initStack
 * Purpose : Prepares a stack for use by setting top to -1 (empty state).
 */
void initStack(Stack *s) {
    s->top = -1;
}

/*
 * Function: isEmpty
 * Purpose : Checks whether the stack has zero elements.
 * Returns : 1 if empty, 0 otherwise.
 */
int isEmpty(const Stack *s) {
    return s->top == -1;
}

/*
 * Function: isFull
 * Purpose : Checks whether the stack has reached MAX_STACK_SIZE.
 * Returns : 1 if full, 0 otherwise.
 */
int isFull(const Stack *s) {
    return s->top == MAX_STACK_SIZE - 1;
}

/*
 * Function: push
 * Purpose : Inserts a URL onto the top of the stack (LIFO).
 * Returns : 1 on success, 0 if the stack is full (overflow prevented).
 */
int push(Stack *s, const char *url) {
    if (isFull(s)) {
        return 0;
    }
    s->top++;
    strncpy(s->items[s->top], url, MAX_URL_LEN - 1);
    s->items[s->top][MAX_URL_LEN - 1] = '\0';
    return 1;
}

/*
 * Function: pop
 * Purpose : Removes the top URL from the stack and copies it into outUrl.
 * Returns : 1 on success, 0 if the stack is empty (underflow prevented).
 */
int pop(Stack *s, char *outUrl) {
    if (isEmpty(s)) {
        return 0;
    }
    strncpy(outUrl, s->items[s->top], MAX_URL_LEN - 1);
    outUrl[MAX_URL_LEN - 1] = '\0';
    s->top--;
    return 1;
}

/*
 * Function: displayStack
 * Purpose : Prints all URLs in the stack from top to bottom so LIFO order
 *           is easy to visualize.
 */
void displayStack(const Stack *s, const char *stackName) {
    int i;

    printf("\n--- %s ---\n", stackName);
    if (isEmpty(s)) {
        printf("%s is empty.\n", stackName);
        return;
    }

    printf("Total items: %d / %d\n", s->top + 1, MAX_STACK_SIZE);
    for (i = s->top; i >= 0; i--) {
        if (i == s->top) {
            printf("  [TOP] %s\n", s->items[i]);
        } else {
            printf("  [%2d]  %s\n", i + 1, s->items[i]);
        }
    }
}

/*
 * Function: clearStack
 * Purpose : Removes all elements from the stack by resetting top to -1.
 */
void clearStack(Stack *s) {
    s->top = -1;
}

/* ============================================================================
 * UI & Input Helper Functions
 * ============================================================================
 */

/*
 * Function: displayWelcomeBanner
 * Purpose : Prints the application title and brief introduction at startup.
 */
void displayWelcomeBanner(void) {
    printf("============================================================\n");
    printf("      WebTrack - Browser History Management System (C)      \n");
    printf("         Simulating Back & Forward Using Two Stacks         \n");
    printf("============================================================\n");
    printf("  Stack Capacity : %d URLs per stack\n", MAX_STACK_SIZE);
    printf("  History Limit  : %d entries\n", MAX_HISTORY_SIZE);
    printf("  Accepted URLs  : Must start with http:// or https://\n");
    printf("============================================================\n");
}

/*
 * Function: displayMenu
 * Purpose : Displays all available menu options to the user.
 */
void displayMenu(void) {
    printf("\n-------------------- WEBTRACK MENU -------------------------\n");
    printf("  1. Visit a New Website\n");
    printf("  2. Go Back\n");
    printf("  3. Go Forward\n");
    printf("  4. Display Current Website\n");
    printf("  5. Display Back Stack\n");
    printf("  6. Display Forward Stack\n");
    printf("  7. View Browsing History\n");
    printf("  8. Search Browsing History\n");
    printf("  9. Display Navigation Statistics\n");
    printf(" 10. Clear Browsing History\n");
    printf("  0. Exit\n");
    printf("------------------------------------------------------------\n");
}

/*
 * Function: readLine
 * Purpose : Safely reads a line of text using fgets(), removes the trailing
 *           newline, and discards leftover characters if the line is too long.
 * Returns : INPUT_OK (1) on success, INPUT_TOO_LONG (0) if input exceeded size,
 *           or INPUT_EOF (-1) if end-of-file occurred.
 */
int readLine(char *buffer, int size) {
    size_t len;

    if (fgets(buffer, size, stdin) == NULL) {
        return INPUT_EOF;
    }

    len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        return INPUT_OK;
    }

    /* Check if there are still unread characters before newline or EOF */
    if ((int)len == size - 1) {
        int ch = getchar();
        if (ch != '\n' && ch != EOF) {
            /* Flush the remaining characters on this line */
            while (ch != '\n' && ch != EOF) {
                ch = getchar();
            }
            return INPUT_TOO_LONG;
        }
    }

    return INPUT_OK;
}

/*
 * Function: trimSpaces
 * Purpose : Removes leading and trailing whitespace characters from a string.
 */
void trimSpaces(char *str) {
    size_t start = 0;
    size_t len = strlen(str);
    size_t i;

    while (start < len && isspace((unsigned char)str[start])) {
        start++;
    }

    while (len > start && isspace((unsigned char)str[len - 1])) {
        len--;
    }

    if (start > 0) {
        for (i = 0; i < len - start; i++) {
            str[i] = str[start + i];
        }
    }
    str[len - start] = '\0';
}

/*
 * Function: readMenuChoice
 * Purpose : Reads and validates an integer menu selection (0-10).
 * Returns : 1 if valid integer in range, 0 if invalid, INPUT_EOF on EOF.
 */
int readMenuChoice(int *choice) {
    char line[64];
    int status = readLine(line, (int)sizeof(line));
    size_t i;
    long val;

    if (status == INPUT_EOF) {
        return INPUT_EOF;
    }
    if (status == INPUT_TOO_LONG) {
        return 0;
    }

    trimSpaces(line);
    if (line[0] == '\0') {
        return 0;
    }

    /* Reject non-digit characters (such as letters, symbols, or minus signs) */
    for (i = 0; line[i] != '\0'; i++) {
        if (!isdigit((unsigned char)line[i])) {
            return 0;
        }
    }

    val = strtol(line, NULL, 10);
    if (val < 0 || val > 10) {
        return 0;
    }

    *choice = (int)val;
    return 1;
}

/*
 * Function: isValidUrl
 * Purpose : Performs beginner-friendly URL validation:
 *           - Cannot be empty
 *           - Cannot contain spaces
 *           - Must start with "http://" or "https://"
 *           - Must contain at least one character after the protocol prefix
 * Returns : 1 if valid, 0 if invalid (with a descriptive error message).
 */
int isValidUrl(const char *url) {
    size_t prefixLen = 0;
    size_t i;

    if (url[0] == '\0') {
        printf("\n[Error] Website URL cannot be empty.\n");
        return 0;
    }

    for (i = 0; url[i] != '\0'; i++) {
        if (isspace((unsigned char)url[i])) {
            printf("\n[Error] Website URL cannot contain spaces.\n");
            return 0;
        }
    }

    if (strncmp(url, "http://", 7) == 0) {
        prefixLen = 7;
    } else if (strncmp(url, "https://", 8) == 0) {
        prefixLen = 8;
    } else {
        printf("\n[Error] Invalid URL! URL must start with http:// or https://\n");
        return 0;
    }

    if (url[prefixLen] == '\0') {
        printf("\n[Error] Incomplete URL! Please include a website name after the protocol.\n");
        return 0;
    }

    return 1;
}

/*
 * Function: containsIgnoreCase
 * Purpose : Checks whether 'pattern' appears inside 'text', ignoring letter case.
 * Returns : 1 if found, 0 otherwise.
 */
int containsIgnoreCase(const char *text, const char *pattern) {
    size_t i;
    size_t j;

    if (pattern[0] == '\0') {
        return 1;
    }

    for (i = 0; text[i] != '\0'; i++) {
        j = 0;
        while (pattern[j] != '\0' && text[i + j] != '\0' &&
               tolower((unsigned char)text[i + j]) ==
                   tolower((unsigned char)pattern[j])) {
            j++;
        }
        if (pattern[j] == '\0') {
            return 1;
        }
    }

    return 0;
}

/* ============================================================================
 * Navigation & Browsing History Functions
 * ============================================================================
 */

/*
 * Function: initHistory
 * Purpose : Initializes browsing history count to 0.
 */
void initHistory(BrowsingHistory *history) {
    history->count = 0;
}

/*
 * Function: initStats
 * Purpose : Resets all navigation statistics counters to 0.
 */
void initStats(NavStats *stats) {
    stats->totalVisits = 0;
    stats->backCount = 0;
    stats->forwardCount = 0;
}

/*
 * Function: addHistoryEntry
 * Purpose : Appends a visited URL to browsing history.
 *           If the 200-entry capacity is reached, shows a clear warning and
 *           does not overwrite existing entries.
 */
void addHistoryEntry(BrowsingHistory *history, const char *url) {
    if (history->count >= MAX_HISTORY_SIZE) {
        printf("[Warning] Browsing history is full (%d/%d entries). "
               "Old entries were kept and this visit was not added to history.\n",
               history->count, MAX_HISTORY_SIZE);
        return;
    }

    strncpy(history->urls[history->count], url, MAX_URL_LEN - 1);
    history->urls[history->count][MAX_URL_LEN - 1] = '\0';
    history->count++;
}

/*
 * Function: visitWebsite
 * Purpose : Visits a new website URL, pushes any existing current website onto
 *           the Back Stack, clears the Forward Stack, and updates history.
 */
void visitWebsite(Stack *backStack, Stack *forwardStack, char *currentWebsite,
                  BrowsingHistory *history, NavStats *stats) {
    char newUrl[MAX_URL_LEN];
    int status;
    int hadForwardItems;

    printf("\nEnter website URL (starting with http:// or https://): ");
    status = readLine(newUrl, MAX_URL_LEN);

    if (status == INPUT_EOF) {
        printf("\n[Error] Input ended unexpectedly.\n");
        return;
    }
    if (status == INPUT_TOO_LONG) {
        printf("\n[Error] Website URL exceeds maximum allowed length (%d characters).\n",
               MAX_URL_LEN - 1);
        return;
    }

    trimSpaces(newUrl);
    if (!isValidUrl(newUrl)) {
        return;
    }

    /*
     * Safety check before changing any state:
     * If a current website already exists, we must push it onto Back Stack.
     * If Back Stack is full, abort safely so navigation state stays consistent.
     */
    if (currentWebsite[0] != '\0') {
        if (isFull(backStack)) {
            printf("\n[Error] Back Stack is full (limit: %d URLs). "
                   "Cannot push \"%s\" onto Back Stack.\n",
                   MAX_STACK_SIZE, currentWebsite);
            printf("Navigation state was preserved.\n");
            return;
        }
        push(backStack, currentWebsite);
    }

    /* Update current website to the newly visited URL */
    strncpy(currentWebsite, newUrl, MAX_URL_LEN - 1);
    currentWebsite[MAX_URL_LEN - 1] = '\0';

    /* Visiting a new website always clears the Forward Stack */
    hadForwardItems = !isEmpty(forwardStack);
    clearStack(forwardStack);

    /* Record visit in history and statistics */
    stats->totalVisits++;
    addHistoryEntry(history, currentWebsite);

    printf("\n[Success] Now visiting: %s\n", currentWebsite);
    if (hadForwardItems) {
        printf("[Info] Forward Stack was cleared because a new website was visited.\n");
    }
}

/*
 * Function: goBack
 * Purpose : Moves from the current website to the previous website on Back Stack.
 */
void goBack(Stack *backStack, Stack *forwardStack, char *currentWebsite,
            NavStats *stats) {
    char previousUrl[MAX_URL_LEN];

    if (isEmpty(backStack)) {
        printf("\nNo previous website available.\n");
        return;
    }

    /* Check Forward Stack capacity before changing state */
    if (isFull(forwardStack)) {
        printf("\n[Error] Forward Stack is full (limit: %d URLs). "
               "Cannot go back without losing current page.\n",
               MAX_STACK_SIZE);
        return;
    }

    push(forwardStack, currentWebsite);
    pop(backStack, previousUrl);

    strncpy(currentWebsite, previousUrl, MAX_URL_LEN - 1);
    currentWebsite[MAX_URL_LEN - 1] = '\0';
    stats->backCount++;

    printf("\n[Back] Current website: %s\n", currentWebsite);
}

/*
 * Function: goForward
 * Purpose : Moves from the current website to the next website on Forward Stack.
 */
void goForward(Stack *backStack, Stack *forwardStack, char *currentWebsite,
               NavStats *stats) {
    char nextUrl[MAX_URL_LEN];

    if (isEmpty(forwardStack)) {
        printf("\nNo forward website available.\n");
        return;
    }

    /* Check Back Stack capacity before changing state */
    if (isFull(backStack)) {
        printf("\n[Error] Back Stack is full (limit: %d URLs). "
               "Cannot go forward without losing current page.\n",
               MAX_STACK_SIZE);
        return;
    }

    push(backStack, currentWebsite);
    pop(forwardStack, nextUrl);

    strncpy(currentWebsite, nextUrl, MAX_URL_LEN - 1);
    currentWebsite[MAX_URL_LEN - 1] = '\0';
    stats->forwardCount++;

    printf("\n[Forward] Current website: %s\n", currentWebsite);
}

/*
 * Function: displayCurrentWebsite
 * Purpose : Displays the website currently open in the browser simulator.
 */
void displayCurrentWebsite(const char *currentWebsite) {
    printf("\n--- Current Website ---\n");
    if (currentWebsite[0] == '\0') {
        printf("No website is currently open. Visit a website first.\n");
    } else {
        printf("Current Website: %s\n", currentWebsite);
    }
}

/*
 * Function: viewBrowsingHistory
 * Purpose : Displays every recorded website visit in chronological order.
 */
void viewBrowsingHistory(const BrowsingHistory *history) {
    int i;

    printf("\n--- Browsing History ---\n");
    if (history->count == 0) {
        printf("Browsing history is empty.\n");
        return;
    }

    printf("Stored entries: %d / %d\n", history->count, MAX_HISTORY_SIZE);
    for (i = 0; i < history->count; i++) {
        printf("  %3d. %s\n", i + 1, history->urls[i]);
    }
}

/*
 * Function: searchBrowsingHistory
 * Purpose : Searches browsing history for URLs matching the user's keyword
 *           or full URL (case-insensitive).
 */
void searchBrowsingHistory(const BrowsingHistory *history) {
    char query[MAX_URL_LEN];
    int status;
    int i;
    int matches = 0;

    printf("\n--- Search Browsing History ---\n");
    if (history->count == 0) {
        printf("Browsing history is empty. Nothing to search.\n");
        return;
    }

    printf("Enter URL or keyword to search: ");
    status = readLine(query, MAX_URL_LEN);
    if (status == INPUT_EOF) {
        printf("\n[Error] Input ended unexpectedly.\n");
        return;
    }
    if (status == INPUT_TOO_LONG) {
        printf("\n[Error] Search text exceeds maximum length (%d characters).\n",
               MAX_URL_LEN - 1);
        return;
    }

    trimSpaces(query);
    if (query[0] == '\0') {
        printf("\n[Error] Search query cannot be empty.\n");
        return;
    }

    printf("\nSearch results for \"%s\":\n", query);
    for (i = 0; i < history->count; i++) {
        if (containsIgnoreCase(history->urls[i], query)) {
            printf("  %3d. %s\n", i + 1, history->urls[i]);
            matches++;
        }
    }

    if (matches == 0) {
        printf("No matching website found in browsing history.\n");
    } else {
        printf("Total matches found: %d\n", matches);
    }
}

/*
 * Function: displayStatistics
 * Purpose : Displays summary statistics of visits, stack usage, and history.
 */
void displayStatistics(const Stack *backStack, const Stack *forwardStack,
                       const char *currentWebsite,
                       const BrowsingHistory *history, const NavStats *stats) {
    printf("\n--- Navigation Statistics ---\n");
    printf("  Current Website        : %s\n",
           (currentWebsite[0] == '\0') ? "None" : currentWebsite);
    printf("  Total Website Visits   : %d\n", stats->totalVisits);
    printf("  Go Back Operations     : %d\n", stats->backCount);
    printf("  Go Forward Operations  : %d\n", stats->forwardCount);
    printf("  Back Stack Size        : %d / %d\n", backStack->top + 1,
           MAX_STACK_SIZE);
    printf("  Forward Stack Size     : %d / %d\n", forwardStack->top + 1,
           MAX_STACK_SIZE);
    printf("  Stored History Entries : %d / %d\n", history->count,
           MAX_HISTORY_SIZE);
}

/*
 * Function: clearBrowsingHistory
 * Purpose : Clears the browsing history log after user confirmation.
 *           Keeps navigation stacks untouched because history is a log,
 *           whereas stacks manage active Back/Forward tab navigation.
 */
void clearBrowsingHistory(BrowsingHistory *history) {
    char confirm[32];
    int status;

    printf("\n--- Clear Browsing History ---\n");
    if (history->count == 0) {
        printf("Browsing history is already empty.\n");
        return;
    }

    printf("Are you sure you want to clear all %d history entries? (y/n): ",
           history->count);
    status = readLine(confirm, (int)sizeof(confirm));
    if (status == INPUT_EOF) {
        printf("\n[Error] Input ended unexpectedly.\n");
        return;
    }
    if (status == INPUT_TOO_LONG) {
        printf("\n[Info] Unrecognized confirmation. Clear history cancelled.\n");
        return;
    }

    trimSpaces(confirm);
    if (strcmp(confirm, "y") == 0 || strcmp(confirm, "Y") == 0 ||
        strcmp(confirm, "yes") == 0 || strcmp(confirm, "YES") == 0) {
        history->count = 0;
        printf("\n[Success] Browsing history cleared.\n");
        printf("[Info] Active Back and Forward stacks were kept intact.\n");
    } else {
        printf("\n[Info] Clear history cancelled.\n");
    }
}
